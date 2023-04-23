#ifndef Z_ARROW_BOMB_H
#define Z_ARROW_BOMB_H

#include <libultraship/libultra.h>
#include "global.h"

struct ArrowBomb;

typedef void (*ArrowBombActionFunc)(struct ArrowBomb*, PlayState*);

typedef struct ArrowBomb {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ Vec3f unkPos;
    /* 0x0158 */ f32 unk_158;
    /* 0x015C */ f32 unk_15C;
    /* 0x0160 */ ArrowBombActionFunc actionFunc;
    /* 0x0164 */ s16 radius;
    /* 0x0166 */ u16 timer;
    /* 0x0168 */ u8 alpha;
                 f32 flashIntensity;
                 u16 flashTimer;
                 u8 exploded;
                 u8 startBombCount;
} ArrowBomb; // size = 0x016C

#endif
