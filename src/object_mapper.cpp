#include "object_mapper.h"
#include "memory.h"

#include <iostream>

zm::ObjectV5 zm::ObjectMapper::map_object(uint16_t number) {
    uint32_t object_address = base_address + DEFAULT_PROPERTY_TABLE_SIZE + (number * OBJECT_V5_SIZE);

    return {
        memory.read_double_word(object_address),
        memory.read_word(object_address + 4),
        memory.read_word(object_address + 6),
        memory.read_word(object_address + 8),
        memory.read_word(object_address + 10),
        memory.read_word(object_address + 12),
        object_address
    };
}

zm::ObjectMapper::ObjectMapper(zm::Memory &memory) : memory(memory) {
    base_address = memory.read_word(0x0A);
}

bool zm::ObjectMapper::test_attribute(uint16_t object, uint8_t attribute) {
    auto obj = map_object(object - 1);

    /*
     * The general rule for retrieving a numbered attribute is to
     * shift attributes "array" to the right so that the correct
     * attribute is at bit 0. Since attributes are numbered from
     * 0 to MAX starting at the top bit (e.g. bit 31), attribute
     * N can be retrieved using `attribute >> (31 - N)`.
     */
    if (attribute > 31) {
        /*
         * Use bottom attributes, which are numbered 32 to 47
         * This means that there are now only 16 possible attributes
         * and we must reset the attribute number to the range of
         * 0 to 15 by taking 32 positions from it.
         */
        return (obj.attributes_bottom >> (15 - (attribute - 32))) & 1;
    } else {
        return (obj.attributes_top >> (31 - attribute)) & 1;
    }
}

void zm::ObjectMapper::set_attribute(uint16_t object, uint8_t attribute) {
    auto obj = map_object(object - 1);

    if (attribute > 31) {
        obj.attributes_bottom |= (0x8000 >> (attribute - 32));
        memory.write_word(obj.address + 4, obj.attributes_bottom);
    } else {
        obj.attributes_top |= (0x80000000 >> attribute);
        memory.write_double_word(obj.address, obj.attributes_top);
    }
}

void zm::ObjectMapper::clear_attribute(uint16_t object, uint8_t attribute) {
    auto obj = map_object(object - 1);

    if (attribute > 31) {
        obj.attributes_bottom &= (~(0x8000 >> (attribute - 32)));
        memory.write_word(obj.address + 4, obj.attributes_bottom);
    } else {
        obj.attributes_top &= (~(0x80000000 >> attribute));
        memory.write_double_word(obj.address, obj.attributes_top);
    }
}

void zm::ObjectMapper::insert_object(uint16_t source_object, uint16_t destination_object) {
    auto source = map_object(source_object - 1);
    auto destination = map_object(destination_object - 1);

    if (source.parent != 0) {
        auto parent = map_object(source.parent);

        if (parent.child == source_object) {
            // Promote the sibling to the first child
            parent.child = source.sibling;
            memory.write_double_word(parent.address + 10, parent.child);
        } else {
            // Search for left sibling and connect to this object's sibling
            auto obj = map_object(parent.child);

            while (obj.sibling != source_object && obj.sibling != 0) {
                obj = map_object(obj.sibling);
            }

            obj.sibling = source.sibling;
            memory.write_double_word(obj.address + 8, obj.sibling);
        }
    }

    source.parent = destination_object;
    memory.write_double_word(source.address + 6, source.parent);

    source.sibling = destination.child;
    memory.write_double_word(source.address + 8, source.sibling);

    destination.child = source_object;
    memory.write_double_word(destination.address + 10, destination.child);
}

struct PropertyPointer {
    uint8_t size;
    uint8_t number;
    bool listed;
    uint32_t address;
    uint32_t data_address;
};

PropertyPointer compute_property_location(uint32_t property_address, uint16_t property, zm::Memory &memory) {
    // Read property size and number
    uint8_t property_size_first_byte = memory.read_byte(property_address);
    uint8_t property_number = property_size_first_byte & 0x3F;

    auto property_data_address = property_address + 1;
    auto property_size = 0;

    if (property_size_first_byte & 0x80) {
        // Size is on the second byte, bits 0 to 5
        property_data_address++;
        property_size = memory.read_byte(property_address + 1) & 0x3F;
    } else {
        property_size = property_size_first_byte & 0x20 ? 2 : 1;
    }

    if (property > property_number) {
        // Requested property is outside bounds, get the default property
        return { 2, static_cast<uint8_t>(property), false,static_cast<uint32_t>(memory.read_word(0x0A) + property), static_cast<uint32_t>(memory.read_word(0x0A) + property) };

    } else if (property == property_number) {
        return { static_cast<uint8_t>(property_size), property_number, true, property_address, property_data_address };
    } else {
        return compute_property_location(property_data_address + property_size, property, memory);
    }
}

uint16_t zm::ObjectMapper::get_property(uint16_t object, uint16_t property) {
    auto obj = map_object(object - 1);

    // Jump to property table
    uint16_t object_description_length = memory.read_byte(obj.properties);
    auto prop = compute_property_location(obj.properties + ((object_description_length << 1) + 1), property, memory);

    if (prop.size == 1) {
        return memory.read_byte(prop.data_address);
    } else {
        return memory.read_word(prop.data_address);
    }
}

uint16_t zm::ObjectMapper::get_property_address(uint16_t object, uint16_t property) {
    auto obj = map_object(object - 1);

    // Jump to property table
    uint16_t object_description_length = memory.read_byte(obj.properties);
    auto prop = compute_property_location(obj.properties + ((object_description_length << 1) + 1), property, memory);

    return prop.data_address;
}

uint16_t zm::ObjectMapper::get_next_property(uint16_t object, uint16_t property) {
    auto obj = map_object(object - 1);

    uint16_t object_description_length = memory.read_byte(obj.properties);
    auto property_list_address = obj.properties + ((object_description_length << 1) + 1);

    if (property == 0) {
        uint8_t property_size_first_byte = memory.read_byte(property_list_address);
        return property_size_first_byte & 0x3F;
    } else {
        auto prop = compute_property_location(property_list_address, property, memory);

        if (prop.listed) {
            return memory.read_byte(prop.data_address + prop.size) & 0x3F;
        } else {
            return prop.number - 1;
        }
    }
}

uint16_t zm::ObjectMapper::get_property_length(uint16_t property_address) {
    auto size_byte = memory.read_byte(property_address - 1);

    if (size_byte & 0x80) {
        // 7th bit is set, which means the property length is on bits 0 to 5
        return size_byte & 0x3F;
    } else {
        return size_byte & 0x20 ? 2 : 1;
    }
}

uint16_t zm::ObjectMapper::get_parent(uint16_t object) {
    auto obj = map_object(object - 1);

    return obj.parent;
}

uint16_t zm::ObjectMapper::get_sibling(uint16_t object) {
    auto obj = map_object(object - 1);

    return obj.sibling;
}

uint16_t zm::ObjectMapper::get_child(uint16_t object) {
    auto obj = map_object(object - 1);

    return obj.child;
}

void zm::ObjectMapper::remove_object(uint16_t object) {
    auto obj = map_object(object - 1);

    if (obj.parent != 0) {
        auto parent = map_object(obj.parent);

        if (parent.child == object) {
            // The sibling must be promoted as the parent's first child
            parent.child = obj.sibling;
            memory.write_word(parent.address + 10, parent.child);
        }
    }

    obj.parent = 0;
    memory.write_word(obj.address + 6, obj.parent);
}

void zm::ObjectMapper::print_object_table() {
    // Print property defaults
    std::cout << "Property defaults:" << std::endl;
    for (int i = 0; i < 63; i++) {
        std::cout << "\t[" << i + 1 << "] = " << std::hex << memory.read_word(base_address + i) << std::dec << std::endl;
    }

    // Compute number of objects
    int object_count = 99999999;

    for (int object_id = 0; object_id < object_count; object_id++) {
        auto object = map_object(object_id);

        /*
         * Estimate the number of objects by getting the address
         * of the object's properties minus the start of the object
         * table divided by the size of an object
         */
        uint16_t properties_address = object.properties;
        uint16_t offset = properties_address - (memory.read_word(base_address) + DEFAULT_PROPERTY_TABLE_SIZE);

        auto pseudo_count = offset / OBJECT_V5_SIZE;

        if (pseudo_count < object_count) {
            std::cout << "Object count estimated at " << pseudo_count << std::endl;
            object_count = pseudo_count;
        }

        // Print object ID
        std::cout << object_id + 1 << ".";

        // Print attributes
        std::cout << "\tAttributes: ";

        if (object.attributes_top == 0x00000000 && object.attributes_bottom == 0x0000) {
            std::cout << "None";
        } else {
            for (int attribute = 0; attribute < 48; attribute++) {
                if (test_attribute(object_id + 1, attribute)) {
                    std::cout << attribute << ", ";
                }
            }
        }

        std::cout << std::endl;

        // Print relationships
        std::cout << "\tParent object:\t" << object.parent << "\tSibling object:\t" << object.sibling << "\tChild object:\t" << object.child << std::endl;

        // Print property address
        std::cout << "\tProperty address: " << std::hex << object.properties << std::dec << std::endl;

        // Print each property on the list, until \0 is found
        uint16_t property = 0;

        std::cout << "\t\tProperties:" << std::endl;
        while((property = get_next_property(object_id + 1, property)) != 0) {
            std::cout << "\t\t\t[" << property << "]";

            uint16_t property_address = get_property_address(object_id + 1, property);
            uint16_t property_length = get_property_length(property_address);

            for (int i = 0; i < property_length; i++) {
                std::cout << " " << std::hex << +memory.read_byte(property_address + i) << std::dec;
            }

            std::cout << std::endl;
        }
    }
}
