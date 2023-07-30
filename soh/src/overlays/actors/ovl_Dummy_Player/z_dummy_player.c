#include <libultraship/libultra.h>
#include "global.h"

#include "z_dummy_player.h"
#include "overlays/actors/ovl_Bg_Heavy_Block/z_bg_heavy_block.h"
#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "overlays/actors/ovl_En_Boom/z_en_boom.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "overlays/actors/ovl_En_Box/z_en_box.h"
#include "overlays/actors/ovl_En_Door/z_en_door.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "overlays/actors/ovl_En_Fish/z_en_fish.h"
#include "overlays/actors/ovl_En_Horse/z_en_horse.h"
#include "overlays/effects/ovl_Effect_Ss_Fhg_Flash/z_eff_ss_fhg_flash.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_link_child/object_link_child.h"
#include "textures/icon_item_24_static/icon_item_24_static.h"
#include <soh/Enhancements/custom-message/CustomMessageTypes.h>
#include "soh/Enhancements/item-tables/ItemTableTypes.h"
#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/Enhancements/randomizer/randomizer_entrance.h"
#include <overlays/actors/ovl_En_Partner/z_en_partner.h>
#include "soh/Enhancements/enhancementTypes.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"
#include "soh/Enhancements/randomizer/randomizer_grotto.h"
#include "soh/frame_interpolation.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*
    Player Info to share
    Scene
    PosRot
    Shape PosRot
    Age
    SkelAnime->animation
    SkelAnimeUpper->animation
    Collider
    StateFlags2
    User Colour
*/

//todo figure out height draw issues

static ColliderCylinderInit sDummyColliderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_PLAYER,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 12, 60, 0, { 0, 0, 0 } },
};

static Vec3s sBaseTransl = { -57, 3377, 0 };

void DummyPlayer_Init(Actor* thisx, PlayState* play) {
    DummyPlayer* this = (DummyPlayer*)thisx;
    this->linkAge = /*GameInteractor_GetCoopPlayerAge(this->actor.params)*/ 1;

    SkelAnime_InitLink(play, &this->skelAnime, gPlayerSkelHeaders[((void)0, 1/*this->linkAge*/)], &gPlayerAnim_link_normal_wait_free, 9, this->jointTable, this->morphTable, PLAYER_LIMB_MAX);
    this->skelAnime.baseTransl = sBaseTransl;
    SkelAnime_InitLink(play, &this->skelAnimeUpper, gPlayerSkelHeaders[((void)0, 1/*this->linkAge*/)], &gPlayerAnim_link_normal_wait_free, 9, this->jointTable2, this->morphTable2, PLAYER_LIMB_MAX);
    this->skelAnimeUpper.baseTransl = sBaseTransl;
    strcpy(this->anim , gPlayerAnim_link_normal_wait_free);
    strcpy(this->animUpper , gPlayerAnim_link_normal_wait_free);
    this->moveFlags = this->skelAnime.moveFlags;

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sDummyColliderInit);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFeet, GET_PLAYER(play)->ageProperties->unk_04); //-- lazy - get shadow properties from other player maybe
}

void DummyPlayer_Update(Actor* thisx, PlayState* play) {
    DummyPlayer* this = (DummyPlayer*)thisx;
    bool updateLimb = false;

    if (GameInteractor_GetCoopPlayerScene(this->actor.params) == play->sceneNum) {
        PosRot coopPlayerPos = GameInteractor_GetCoopPlayerPosition(this->actor.params);
         if (this->actor.world.pos.x != coopPlayerPos.pos.x || this->actor.world.pos.y != coopPlayerPos.pos.y || this->actor.world.pos.z != coopPlayerPos.pos.z) {
            for (u16 limbIndex = 0; limbIndex < PLAYER_LIMB_BUF_COUNT; ++limbIndex) {
                this->skelAnime.jointTable[limbIndex] = GameInteractor_GetCoopPlayerLimb(this->actor.params, limbIndex);
                this->skelAnimeUpper.jointTable[limbIndex] = GameInteractor_GetCoopPlayerLimbUpper(this->actor.params, limbIndex);
            }
            updateLimb = true;
         }
        // if hidden, immediately update position
        if (this->actor.world.pos.y == -9999.0f) { 
            this->actor.world = coopPlayerPos;
        // Otherwise smoothly update position
        } else {
            Math_SmoothStepToF(&this->actor.world.pos.x, coopPlayerPos.pos.x, 0.5f, 1000.0f, 0.0f);
            Math_SmoothStepToF(&this->actor.world.pos.y, coopPlayerPos.pos.y, 0.5f, 1000.0f, 0.0f);
            Math_SmoothStepToF(&this->actor.world.pos.z, coopPlayerPos.pos.z, 0.5f, 1000.0f, 0.0f);
            this->actor.world.rot = coopPlayerPos.rot;
        }
        this->actor.shape.rot = GameInteractor_GetCoopPlayerShapeRotation(this->actor.params);
        //this->actor.scale = GameInteractor_GetCoopPlayerScale(this->actor.params);
        this->collider.dim = GameInteractor_GetCoopPlayerCylinder(this->actor.params);
        this->moveFlags = GameInteractor_GetCoopPlayerMoveFlags(this->actor.params);
        this->skelAnime.moveFlags = this->moveFlags;
        this->shapePitchOffset = GameInteractor_GetCoopPlayerPitchOffset(this->actor.params);
        this->shapeOffsetY = GameInteractor_GetCoopPlayerOffsetY(this->actor.params);
        

        Collider_UpdateCylinder(&this->actor, &this->collider);
        
        char anim[128];
        char animUpper[128];
        GameInteractor_GetCoopPlayerAnim(this->actor.params, anim);
        GameInteractor_GetCoopPlayerAnimUpper(this->actor.params, animUpper);

        f32 playSpeed = GameInteractor_GetCoopPlayerPlaySpeed(this->actor.params, false);
        f32 startFrame = GameInteractor_GetCoopPlayerStartFrame(this->actor.params, false);
        f32 curFrame = GameInteractor_GetCoopPlayerCurFrame(this->actor.params,false);
        f32 endFrame = GameInteractor_GetCoopPlayerEndFrame(this->actor.params, false);
        u8 mode = GameInteractor_GetCoopPlayerMode(this->actor.params, false);
        
        
        if (strcmp(this->anim, anim)) {
            f32 morphFrames = 0.0f;//GameInteractor_GetCoopPlayerMorphFrames(this->actor.params, false);
            strcpy(this->anim, anim);
            LinkAnimation_Change(play, &this->skelAnime, &this->anim, playSpeed, startFrame, endFrame, mode, morphFrames);
        } else {
            this->skelAnime.playSpeed = playSpeed;
            this->skelAnime.startFrame = startFrame;
            //this->skelAnime.curFrame = curFrame;
            this->skelAnime.endFrame = endFrame;
            this->skelAnime.mode = mode;
        }

        playSpeed = GameInteractor_GetCoopPlayerPlaySpeed(this->actor.params, true);
        startFrame = GameInteractor_GetCoopPlayerStartFrame(this->actor.params, true);
        curFrame = GameInteractor_GetCoopPlayerCurFrame(this->actor.params,true);
        endFrame = GameInteractor_GetCoopPlayerEndFrame(this->actor.params, true);
        mode = GameInteractor_GetCoopPlayerMode(this->actor.params, true);

        if (strcmp(this->animUpper, animUpper)) {
            strcpy(this->animUpper, animUpper);
            LinkAnimation_Change(play, &this->skelAnimeUpper, &this->animUpper, playSpeed, startFrame, endFrame, mode, 0.0f);
        } else {
            this->skelAnimeUpper.playSpeed = playSpeed;
            this->skelAnimeUpper.startFrame = startFrame;
            //this->skelAnimeUpper.curFrame = curFrame;
            this->skelAnimeUpper.endFrame = endFrame;
            this->skelAnimeUpper.mode = mode;
        }

        
        LinkAnimation_Update(play, &this->skelAnime);
        LinkAnimation_Update(play, &this->skelAnimeUpper);
        // if (updateLimb) {
        //     for (u16 limbIndex = 0; limbIndex < PLAYER_LIMB_BUF_COUNT; ++limbIndex) {
        //         this->skelAnime.jointTable[limbIndex] = GameInteractor_GetCoopPlayerLimb(this->actor.params, limbIndex);
        //         this->skelAnimeUpper.jointTable[limbIndex] = GameInteractor_GetCoopPlayerLimbUpper(this->actor.params, limbIndex);
        //     }
        // }
    } else {
        this->actor.world.pos.x = -9999.0f;
        this->actor.world.pos.y = -9999.0f;
        this->actor.world.pos.z = -9999.0f;
    }
}

void DummyPlayer_DrawGameplay(PlayState* play, DummyPlayer* this, s32 lod, Gfx* cullDList,
                         OverrideLimbDrawOpa overrideLimbDraw) {
    static s32 D_8085486C = 255;

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x0C, cullDList);
    gSPSegment(POLY_XLU_DISP++, 0x0C, cullDList);

    func_8008F470(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, lod,
                  PLAYER_TUNIC_DUMMY + this->actor.params /*custom tunic colour*/, 0 /*todo boots*/, this->actor.shape.face /*todo face*/, overrideLimbDraw, Dummy_PostLimbDrawGameplay,
                  &this);

    //todo mask
    /*if ((overrideLimbDraw == func_80090014) && (this->currentMask != PLAYER_MASK_NONE)) {
        // Fixes a bug in vanilla where ice traps are rendered extremely large while wearing a bunny hood
        if (CVarGetInteger("gFixIceTrapWithBunnyHood", 1)) Matrix_Push();
        Mtx* sp70 = Graph_Alloc(play->state.gfxCtx, 2 * sizeof(Mtx));

        if (this->currentMask == PLAYER_MASK_BUNNY) {
            Vec3s sp68;

            FrameInterpolation_RecordActorPosRotMatrix();
            gSPSegment(POLY_OPA_DISP++, 0x0B, sp70);

            sp68.x = D_80858AC8.unk_02 + 0x3E2;
            sp68.y = D_80858AC8.unk_04 + 0xDBE;
            sp68.z = D_80858AC8.unk_00 - 0x348A;
            Matrix_SetTranslateRotateYXZ(97.0f, -1203.0f - CVarGetFloat("gCosmetics.BunnyHood_EarLength", 0.0f), -240.0f - CVarGetFloat("gCosmetics.BunnyHood_EarSpread", 0.0f), &sp68);
            MATRIX_TOMTX(sp70++);

            sp68.x = D_80858AC8.unk_02 - 0x3E2;
            sp68.y = -0xDBE - D_80858AC8.unk_04;
            sp68.z = D_80858AC8.unk_00 - 0x348A;
            Matrix_SetTranslateRotateYXZ(97.0f, -1203.0f - CVarGetFloat("gCosmetics.BunnyHood_EarLength", 0.0f), 240.0f + CVarGetFloat("gCosmetics.BunnyHood_EarSpread", 0.0f), &sp68);
            MATRIX_TOMTX(sp70);
        }

        gSPDisplayList(POLY_OPA_DISP++, sMaskDlists[this->currentMask - 1]);
        if (CVarGetInteger("gFixIceTrapWithBunnyHood", 1)) Matrix_Pop();
    }*/

    //todo hoverboots
    /*if ((this->currentBoots == PLAYER_BOOTS_HOVER ||
         (CVarGetInteger("gIvanCoopModeEnabled", 0) && this->ivanFloating)) &&
        !(this->actor.bgCheckFlags & 1) &&
        !(this->stateFlags1 & PLAYER_STATE1_ON_HORSE) && (this->hoverBootsTimer != 0)) {
        s32 sp5C;
        s32 hoverBootsTimer = this->hoverBootsTimer;

        if (this->hoverBootsTimer < 19) {
            if (hoverBootsTimer >= 15) {
                D_8085486C = (19 - hoverBootsTimer) * 51.0f;
            } else if (hoverBootsTimer < 19) {
                sp5C = hoverBootsTimer;

                if (sp5C > 9) {
                    sp5C = 9;
                }

                D_8085486C = (-sp5C * 4) + 36;
                D_8085486C = D_8085486C * D_8085486C;
                D_8085486C = (s32)((Math_CosS(D_8085486C) * 100.0f) + 100.0f) + 55.0f;
                D_8085486C = D_8085486C * (sp5C * (1.0f / 9.0f));
            }

            FrameInterpolation_RecordActorPosRotMatrix();
            Matrix_SetTranslateRotateYXZ(this->actor.world.pos.x, this->actor.world.pos.y + 2.0f,
                                         this->actor.world.pos.z, &D_80854864);
            Matrix_Scale(4.0f, 4.0f, 4.0f, MTXMODE_APPLY);

            gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(POLY_XLU_DISP++, 0x08,
                       Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, 0, 16, 32, 1, 0,
                                        (play->gameplayFrames * -15) % 128, 16, 32));
            gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 255, D_8085486C);
            gDPSetEnvColor(POLY_XLU_DISP++, 120, 90, 30, 128);
            gSPDisplayList(POLY_XLU_DISP++, gHoverBootsCircleDL);
        }
    }*/

    CLOSE_DISPS(play->state.gfxCtx);
}

void DummyPlayer_Draw(Actor* thisx, PlayState* play) {
    //PlayState* play = play2;
    /*
     For Reference
        overridelimbdrawopa: func_80090014 - default | func_80090440 - crawling
        postlimbdrawopa: func_80090D20
    */
    DummyPlayer* this = (DummyPlayer*)thisx;

    if (GameInteractor_GetCoopPlayerScene(this->actor.params) == play->sceneNum) {

        OPEN_DISPS(play->state.gfxCtx);

        // only draw if not hidden
        if (this->actor.world.pos.y != -9999.0f) {
            OverrideLimbDrawOpa overrideLimbDraw = Dummy_OverrideLimbDrawGameplayDefault;
            s32 lod;
            s32 pad;

            lod = 0;

            func_80093C80(play);
            Gfx_SetupDL_25Xlu(play->state.gfxCtx);

            func_8002EBCC(&this->actor, play, 0);
            func_8002ED80(&this->actor, play, 0);

            if (GameInteractor_GetCoopPlayerStateFlags2(this->actor.params) & PLAYER_STATE2_CRAWLING) {
                if (this->actor.projectedPos.z < 0.0f) {
                    overrideLimbDraw = Dummy_OverrideLimbDrawGameplayCrawling;
                }
            }

            if (GameInteractor_GetCoopPlayerStateFlags2(this->actor.params) & PLAYER_STATE2_REFLECTION) {
                f32 sp78 = ((u16)(play->gameplayFrames * 600) * M_PI) / 0x8000;
                f32 sp74 = ((u16)(play->gameplayFrames * 1000) * M_PI) / 0x8000;

                Matrix_Push();
                this->actor.scale.y = -this->actor.scale.y;
                Matrix_SetTranslateRotateYXZ(
                    this->actor.world.pos.x,
                    (this->actor.floorHeight + (this->actor.floorHeight - this->actor.world.pos.y)) +
                        (this->actor.shape.yOffset * this->actor.scale.y),
                    this->actor.world.pos.z, &this->actor.shape.rot);
                Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
                Matrix_RotateX(sp78, MTXMODE_APPLY);
                Matrix_RotateY(sp74, MTXMODE_APPLY);
                Matrix_Scale(1.1f, 0.95f, 1.05f, MTXMODE_APPLY);
                Matrix_RotateY(-sp74, MTXMODE_APPLY);
                Matrix_RotateX(-sp78, MTXMODE_APPLY);
                DummyPlayer_DrawGameplay(play, this, lod, gCullFrontDList, overrideLimbDraw);
                this->actor.scale.y = -this->actor.scale.y;
                Matrix_Pop();
            }

            gSPClearGeometryMode(POLY_OPA_DISP++, G_CULL_BOTH);
            gSPClearGeometryMode(POLY_XLU_DISP++, G_CULL_BOTH);

            DummyPlayer_DrawGameplay(play, this, lod, gCullBackDList, overrideLimbDraw);

            if (GameInteractor_GetCoopPlayerStateFlags2(this->actor.params) & PLAYER_STATE2_FROZEN) {
                f32 scale = (0 >> 1) * 22.0f;

                gSPSegment(POLY_XLU_DISP++, 0x08,
                        Gfx_TwoTexScroll(play->state.gfxCtx, 0, 0, (0 - play->gameplayFrames) % 128, 32, 32, 1,
                                            0, (play->gameplayFrames * -2) % 128, 32, 32));

                Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, MATRIX_NEWMTX(play->state.gfxCtx),
                        G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gDPSetEnvColor(POLY_XLU_DISP++, 0, 50, 100, 255);
                gSPDisplayList(POLY_XLU_DISP++, gEffIceFragment3DL);
            }
        }

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void DummyPlayer_Destroy(Actor* thisx, PlayState* play) {
    DummyPlayer* this = (DummyPlayer*)thisx;
    Collider_DestroyCylinder(play, &this->collider);

    ResourceMgr_UnregisterSkeleton(&this->skelAnime);
    ResourceMgr_UnregisterSkeleton(&this->skelAnimeUpper);
}