#ifndef ZETAMACHINE_VIDEO_H
#define ZETAMACHINE_VIDEO_H


#include <cstdint>

namespace zm {
    class Video {
    public:
        uint8_t line_height();
        uint8_t character_width();

        uint16_t picture_height();
        uint16_t picture_width();

        bool has_color();
    };
}

#endif //ZETAMACHINE_VIDEO_H
