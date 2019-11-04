#ifndef ZETAMACHINE_RANDOM_NUMBER_GENERATOR_H
#define ZETAMACHINE_RANDOM_NUMBER_GENERATOR_H

#include <cstdint>

namespace zm {
    class RandomNumberGenerator {
    public:
        void seed(uint16_t seed);

        uint16_t random();
    };
}

#endif //ZETAMACHINE_RANDOM_NUMBER_GENERATOR_H
