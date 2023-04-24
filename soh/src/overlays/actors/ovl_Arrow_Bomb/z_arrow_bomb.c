/*
 * File: z_arrow_bomb.c
 * Overlay: ovl_Arrow_Bomb
 * Description: Bomb Arrow. Spawned as a child of a normal arrow.
 */

#include "z_arrow_bomb.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "overlays/actors/ovl_En_Bom/z_en_bom.h"
#include "objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_25)

void ArrowBomb_Init(Actor* thisx, PlayState* play);
void ArrowBomb_Destroy(Actor* thisx, PlayState* play);
void ArrowBomb_Update(Actor* thisx, PlayState* play);
void ArrowBomb_Draw(Actor* thisx, PlayState* play);

void ArrowBomb_Charge(ArrowBomb* this, PlayState* play);
void ArrowBomb_Fly(ArrowBomb* this, PlayState* play);
void ArrowBomb_Hit(ArrowBomb* this, PlayState* play);

#include "overlays/ovl_Arrow_Fire/ovl_Arrow_Fire.h"

const ActorInit Arrow_Bomb_InitVars = {
    ACTOR_ARROW_BOMB,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ArrowBomb),
    (ActorFunc)ArrowBomb_Init,
    (ActorFunc)ArrowBomb_Destroy,
    (ActorFunc)ArrowBomb_Update,
    (ActorFunc)ArrowBomb_Draw,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_STOP),
};

void ArrowBomb_SetupAction(ArrowBomb* this, ArrowBombActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void ArrowBomb_Init(Actor* thisx, PlayState* play) {
    ArrowBomb* this = (ArrowBomb*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    this->radius = 0;
    this->unk_158 = 1.0f;
    ArrowBomb_SetupAction(this, ArrowBomb_Charge);
    Actor_SetScale(&this->actor, 0.01f);
    this->alpha = 160;
    this->timer = 0;
    this->unk_15C = 0.0f;
    this->exploded = false;
    this->startBombCount = AMMO(ITEM_BOMB);
}

void ArrowBomb_Destroy(Actor* thisx, PlayState* play) {
    func_800876C8(play);
    LOG_STRING("消滅"); // "Disappearance"
}

void ArrowBomb_Charge(ArrowBomb* this, PlayState* play) {
    EnArrow* arrow;

    arrow = (EnArrow*)this->actor.parent;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        Actor_Kill(&this->actor);
        return;
    }

    if (this->radius < 10) {
        this->radius += 1;
    }
    // copy position and rotation from arrow
    this->actor.world.pos = arrow->actor.world.pos;
    this->actor.shape.rot = arrow->actor.shape.rot;

    func_8002F974(&this->actor, NA_SE_IT_BOMB_IGNIT - SFX_FLAG);

    // if arrow has no parent, player has fired the arrow
    if (arrow->actor.parent == NULL) {
        this->unkPos = this->actor.world.pos;
        this->radius = 10;
        ArrowBomb_SetupAction(this, ArrowBomb_Fly);
        this->alpha = 255;
    }
}

void func_80865ECC_ArrowBomb(Vec3f* unkPos, Vec3f* bombPos, f32 scale) {
    unkPos->x += ((bombPos->x - unkPos->x) * scale);
    unkPos->y += ((bombPos->y - unkPos->y) * scale);
    unkPos->z += ((bombPos->z - unkPos->z) * scale);
}

void ArrowBomb_Hit(ArrowBomb* this, PlayState* play) {
    f32 scale;
    f32 offset;
    u16 timer;

    if(!this->exploded) {
        EnBom* bomb = (EnBom*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BOM, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0, 0);
        bomb->timer = 0;
        this->exploded = true;
    }

    if (this->actor.projectedW < 50.0f) {
        scale = 10.0f;
    } else {
        if (950.0f < this->actor.projectedW) {
            scale = 310.0f;
        } else {
            scale = this->actor.projectedW;
            scale = ((scale - 50.0f) * (1.0f / 3.0f)) + 10.0f;
        }
    }

    timer = this->timer;
    if (timer != 0) {
        this->timer -= 1;

        if (this->timer >= 8) {
            offset = ((this->timer - 8) * (1.0f / 24.0f));
            offset = SQ(offset);
            this->radius = (((1.0f - offset) * scale) + 10.0f);
            this->unk_158 += ((2.0f - this->unk_158) * 0.1f);
            if (this->timer < 16) {
                this->alpha = ((this->timer * 0x23) - 0x118);
            }
        }
    }

    if (this->timer >= 9) {
        if (this->unk_15C < 1.0f) {
            this->unk_15C += 0.25f;
        }
    } else {
        if (this->unk_15C > 0.0f) {
            this->unk_15C -= 0.125f;
        }
    }

    if (this->timer < 8) {
        this->alpha = 0;
    }

    if (this->timer == 0) {
        this->timer = 255;
        Actor_Kill(&this->actor);
    }
}

void ArrowBomb_Fly(ArrowBomb* this, PlayState* play) {
    EnArrow* arrow;
    f32 distanceScaled;
    s32 pad;

    arrow = (EnArrow*)this->actor.parent;
    if ((arrow == NULL) || (arrow->actor.update == NULL)) {
        Actor_Kill(&this->actor);
        return;
    }
    if (this->startBombCount == AMMO(ITEM_BOMB)) {
        --AMMO(ITEM_BOMB);
    }
    // copy position and rotation from arrow
    this->actor.world.pos = arrow->actor.world.pos;
    this->actor.shape.rot = arrow->actor.shape.rot;
    distanceScaled = Math_Vec3f_DistXYZ(&this->unkPos, &this->actor.world.pos) * (1.0f / 24.0f);
    this->unk_158 = distanceScaled;
    if (distanceScaled < 1.0f) {
        this->unk_158 = 1.0f;
    }
    func_80865ECC_ArrowBomb(&this->unkPos, &this->actor.world.pos, 0.05f);

    if (arrow->hitFlags & 1) {
        Audio_PlayActorSound2(&this->actor, NA_SE_IT_EXPLOSION_FRAME);
        ArrowBomb_SetupAction(this, ArrowBomb_Hit);
        this->timer = 32;
        this->alpha = 255;
    }
}

void ArrowBomb_Update(Actor* thisx, PlayState* play) {
    ArrowBomb* this = (ArrowBomb*)thisx;

    static Vec3f moveEff = { 0.0f, 0.0f, 32.0f };
    static Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };
    static Vec3f dustAccel = { 0.0f, 0.6f, 0.0f };
    static Color_RGBA8 sparkEnv = { 255, 255, 150, 255 };
    static Color_RGBA8 sparkPrim = { 255, 0, 0, 0 };
    static Color_RGBA8 dustColor = { 255, 255, 255, 255 };
    Vec3f effPos;

    this->flashTimer++;

    Matrix_Translate(thisx->world.pos.x, thisx->world.pos.y, thisx->world.pos.z, MTXMODE_NEW);
    Matrix_RotateY(thisx->shape.rot.y * (360.0f / 65535.0f) * (M_PI / 180.0f), MTXMODE_APPLY);
    Matrix_RotateX(thisx->shape.rot.x * (360.0f / 65535.0f) * (M_PI / 180.0f), MTXMODE_APPLY);
    Matrix_MultVec3f(&moveEff, &effPos);
    effPos.y += 3.0f;

    /*if ((play->gameplayFrames % 2) == 0) {
        EffectSsGSpk_SpawnAccel(play, thisx, &effPos, &zeroVec, &zeroVec, &sparkEnv, &sparkPrim, 25, 5);
    }*/

    func_8002829C(play, &effPos, &zeroVec, &dustAccel, &dustColor, &dustColor, 25, 5);

    if (play->msgCtx.msgMode == MSGMODE_OCARINA_CORRECT_PLAYBACK ||
        play->msgCtx.msgMode == MSGMODE_SONG_PLAYED) {
        Actor_Kill(&this->actor);
    } else {
        this->actionFunc(this, play);
    }

    if ((this->flashTimer & 8) != 0) {
        Math_SmoothStepToF(&this->flashIntensity, 140.0f, 1.0f, 140.0f / 7, 0.0f);
    } else {
        Math_SmoothStepToF(&this->flashIntensity, 0.0f, 1.0f, 140.0f / 7, 0.0f);
    }
}

void ArrowBomb_Draw(Actor* thisx, PlayState* play2) {
    ArrowBomb* this = (ArrowBomb*)thisx;
    PlayState* play = play2;
    u32 stateFrames;
    
    EnArrow* arrow;
    Actor* tranform;

    Color_RGB8 primaryColor = {100, 20, 20};
    
    Color_RGB8 secondaryColor = {20, 20, 20};

    stateFrames = play->state.frames;
    arrow = (EnArrow*)this->actor.parent;
    
    if (this->actor.parent != NULL && (this->timer < 255)) {
        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL_25Opa(play->state.gfxCtx);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        Matrix_Scale(0.35f, 0.35f, 0.35f, MTXMODE_APPLY);
        Matrix_Translate(50.0f, 50.0f, -2500.0f, MTXMODE_APPLY);

        // gold cap
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_en_bom.c", 928),G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(POLY_OPA_DISP++, gBombCapDL);

        // bomb body
        Matrix_RotateZYX(0x4000, 0, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_en_bom.c", 934),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetEnvColor(POLY_OPA_DISP++, this->flashIntensity, 0, 40, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, this->flashIntensity, 0, 40, 255);
        gSPDisplayList(POLY_OPA_DISP++, gBombBodyDL);

        // draw dark effect on screen when arrow hits
        if (this->unk_15C > 0) {
            POLY_XLU_DISP = Gfx_SetupDL_57(POLY_XLU_DISP);
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 
                (s32)((secondaryColor.r / 6) * this->unk_15C) & 0xFF,
                (s32)((secondaryColor.g / 6) * this->unk_15C) & 0xFF, 
                (s32)((secondaryColor.b / 6) * this->unk_15C) & 0xFF,
                (s32)(150.0f * this->unk_15C) & 0xFF);
            gDPSetAlphaDither(POLY_XLU_DISP++, G_AD_DISABLE);
            gDPSetColorDither(POLY_XLU_DISP++, G_CD_DISABLE);
            gDPFillRectangle(POLY_XLU_DISP++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        }

        tranform = (arrow->hitFlags & 2) ? &this->actor : &arrow->actor;

        Matrix_Translate(tranform->world.pos.x, tranform->world.pos.y, tranform->world.pos.z, MTXMODE_NEW);
        Matrix_RotateY(tranform->shape.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_RotateX(tranform->shape.rot.x * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_RotateZ(tranform->shape.rot.z * (M_PI / 0x8000), MTXMODE_APPLY);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

        // Draw dark aura on the arrow

        Gfx_SetupDL_25Xlu(play->state.gfxCtx);
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, primaryColor.r, primaryColor.g, primaryColor.b, this->alpha);
        gDPSetEnvColor(POLY_XLU_DISP++, secondaryColor.r, secondaryColor.g, secondaryColor.b, 128);
        Matrix_RotateZYX(0x4000, 0x0, 0x0, MTXMODE_APPLY);
        if (this->timer != 0) {
            Matrix_Translate(0.0f, 0.0f, 0.0f, MTXMODE_APPLY);
        } else {
            Matrix_Translate(0.0f, 1500.0f, 0.0f, MTXMODE_APPLY);
        }
        Matrix_Scale(this->radius * 0.2f, this->unk_158 * 4.0f, this->radius * 0.2f, MTXMODE_APPLY);
        Matrix_Translate(0.0f, -700.0f, 0.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, sMaterialDL);
        gSPDisplayList(POLY_XLU_DISP++,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, 255 - (stateFrames * 2) % 256, 0, 64, 32, 1,
                                        255 - stateFrames % 256, 511 - (stateFrames * 10) % 512, 64, 64));
        gSPDisplayList(POLY_XLU_DISP++, sModelDL);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}
