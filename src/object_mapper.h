#ifndef ZETAMACHINE_OBJECT_MAPPER_H
#define ZETAMACHINE_OBJECT_MAPPER_H

#define OBJECT_V5_SIZE 14

#define DEFAULT_PROPERTY_TABLE_SIZE 126

#include <cstdint>

namespace zm {
    class Memory;

    struct ObjectV3 {
        uint32_t attributes;
        uint8_t parent;
        uint8_t sibling;
        uint8_t child;
        uint16_t properties;
    };

    struct ObjectV5 {
        uint32_t attributes_top;
        uint16_t attributes_bottom;
        uint16_t parent;
        uint16_t sibling;
        uint16_t child;
        uint16_t properties;
        uint32_t address;
    };

    class ObjectMapper {
    public:
        explicit ObjectMapper(Memory &memory);

        bool test_attribute(uint16_t object, uint8_t attribute);
        void set_attribute(uint16_t object, uint8_t attribute);
        void clear_attribute(uint16_t object, uint8_t attribute);

        void insert_object(uint16_t source_object, uint16_t destination_object);

        uint16_t get_property(uint16_t object, uint16_t property);
        uint16_t get_property_address(uint16_t object, uint16_t property);
        uint16_t  get_next_property(uint16_t object, uint16_t property);

        uint16_t get_property_length(uint16_t property_address);

        uint16_t get_parent(uint16_t object);
        uint16_t get_sibling(uint16_t object);
        uint16_t get_child(uint16_t object);

        void remove_object(uint16_t object);

        void print_object_table();

    private:
        uint32_t base_address;
        Memory &memory;

        ObjectV5 map_object(uint16_t number);
    };
}


#endif //ZETAMACHINE_OBJECT_MAPPER_H
