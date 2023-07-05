#ifndef Z_DUMMY_PLAYER_H
#define Z_DUMMY_PLAYER_H

#include <libultraship/libultra.h>
#include "global.h"

struct DummyPlayer;

typedef struct DummyPlayer {
    Actor                actor;
    SkelAnime            skelAnime;
    Vec3s                jointTable[PLAYER_LIMB_BUF_COUNT];
    Vec3s                morphTable[PLAYER_LIMB_BUF_COUNT];
    Vec3s                blendTable[PLAYER_LIMB_BUF_COUNT];
    ColliderCylinder     collider;
    SkelAnime            skelAnimeUpper;
    Vec3s                jointTable2[PLAYER_LIMB_BUF_COUNT];
    Vec3s                morphTable2[PLAYER_LIMB_BUF_COUNT];
    char                 anim[128];          //todo
    char                 animUpper[128];     //todo
    u16                  animGroup;
    u16                  animGroupUpper;
    u8                   modelAnimType;
    //todo held actor maybe?
} DummyPlayer;

#ifdef __cplusplus
extern "C" {
#endif
void DummyPlayer_Init(Actor* thisx, PlayState* play);
void DummyPlayer_Destroy(Actor* thisx, PlayState* play);
void DummyPlayer_Update(Actor* thisx, PlayState* play);
void DummyPlayer_Draw(Actor* thisx, PlayState* play);
#ifdef __cplusplus
}
#endif

#endif