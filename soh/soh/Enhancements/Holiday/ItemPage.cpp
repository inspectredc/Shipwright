#include "Holiday.hpp"
#include "soh_assets.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"
#include "textures/parameter_static/parameter_static.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/icon_item_nes_static/icon_item_nes_static.h"
#include "textures/icon_item_ger_static/icon_item_ger_static.h"
#include "textures/icon_item_fra_static/icon_item_fra_static.h"
#include "soh/frame_interpolation.h"
#include "soh/util.h"

extern "C" {
#include "z64.h"
#include "macros.h"
#include "functions.h"
#include "variables.h"
extern PlayState* gPlayState;
extern s16 func_80823A0C(PlayState* play, Vtx* vtx, s16 pageIndex, s16 arg3);
extern Gfx* KaleidoScope_DrawPageSections(Gfx* gfx, Vtx* vertices, void** textures);
}

#define AUTHOR "inspectredc"
#define CVAR(v) "gHoliday." AUTHOR "." v

#define RX_DIFF 45.0f
#define TY_DIFF 127.0f
#define TZ_DIFF 40.0f
#define PDOWN_OFFSET 93
#define PUP_OFFSET -86

typedef enum PageIndex {
    PAGE_ORIGINAL,
    PAGE_NEW_1
} PageIndex;

typedef enum NewPageState {
    NEW_PAGE_STATE_READY,
    NEW_PAGE_STATE_TRANSITION
} NewPageState;

static const char* sSelectItemENGTexs[] = {
    gPauseSelectItem00ENGTex, gPauseSelectItem01Tex,    gPauseSelectItem02Tex,    gPauseSelectItem03Tex,
    gPauseSelectItem04Tex,    gPauseSelectItem10ENGTex, gPauseSelectItem11Tex,    gPauseSelectItem12Tex,
    gPauseSelectItem13Tex,    gPauseSelectItem14Tex,    gPauseSelectItem20ENGTex, gPauseSelectItem21Tex,
    gPauseSelectItem22Tex,    gPauseSelectItem23Tex,    gPauseSelectItem24Tex,
};
static const char* sSelectItemGERTexs[] = {
    gPauseSelectItem00GERTex, gPauseSelectItem01Tex,    gPauseSelectItem02Tex,    gPauseSelectItem03Tex,
    gPauseSelectItem04Tex,    gPauseSelectItem10GERTex, gPauseSelectItem11Tex,    gPauseSelectItem12Tex,
    gPauseSelectItem13Tex,    gPauseSelectItem14Tex,    gPauseSelectItem20GERTex, gPauseSelectItem21Tex,
    gPauseSelectItem22Tex,    gPauseSelectItem23Tex,    gPauseSelectItem24Tex,
};
static const char* sSelectItemFRATexs[] = {
    gPauseSelectItem00FRATex, gPauseSelectItem01Tex,    gPauseSelectItem02Tex,    gPauseSelectItem03Tex,
    gPauseSelectItem04Tex,    gPauseSelectItem10FRATex, gPauseSelectItem11Tex,    gPauseSelectItem12Tex,
    gPauseSelectItem13Tex,    gPauseSelectItem14Tex,    gPauseSelectItem20FRATex, gPauseSelectItem21Tex,
    gPauseSelectItem22Tex,    gPauseSelectItem23Tex,    gPauseSelectItem24Tex,
};
static const char** sSelectItemTexs[] = {
    sSelectItemENGTexs,
    sSelectItemGERTexs,
    sSelectItemFRATexs,
};

static uint32_t sNewPageIndex = PAGE_ORIGINAL;
static uint32_t sNewPageState = NEW_PAGE_STATE_READY;
static Vtx* sNewItemPageVtx = NULL;
static bool sCursorOnPageDown = false;
static bool sCursorOnPageUp = false;
static uint32_t sItemPageCount = 2;

static void ClearItemPagePosition() {
    // TODO use real variables
    CVarSetFloat(CVAR("ItemPageRotateX"), 0.0f);
    CVarSetFloat(CVAR("ItemPageRotateY"), 0.0f);
    CVarSetFloat(CVAR("ItemPageRotateZ"), 0.0f);
    CVarSetFloat(CVAR("ItemPageScale"), 0.78f);
    CVarSetFloat(CVAR("ItemPagePositionX"), 0.0f);
    CVarSetFloat(CVAR("ItemPagePositionY"), 0.0f);
    CVarSetFloat(CVAR("ItemPagePositionZ"), 0.0f);
}

/*
 * Performs linear interpoloation between `cur` and `target`.  If `cur` is within
 * `minDiff` units, The result is rounded up to `target`
 */
static f32 ItemPage_LERPCeilF(f32 target, f32 cur, f32 stepScale, f32 minDiff) {
    f32 diff = target - cur;
    f32 step;
    f32 ret;

    if (fabsf(diff) >= minDiff) {
        step = diff * stepScale;
        ret = cur + step;
    } else {
        ret = target;
    }

    return ret;
}

static void DrawNewItemMenus(PauseContext* pauseCtx) {

}

static void InitPageVertices(PlayState* play, GraphicsContext* gfxCtx) {
    sNewItemPageVtx = (Vtx*)Graph_Alloc(gfxCtx, 60 * sizeof(Vtx));
    func_80823A0C(play, sNewItemPageVtx, 0, 0);
}

static void DrawNewItemMenuPage(PlayState* play, GraphicsContext* gfxCtx) {

    InitPageVertices(play, gfxCtx);

    OPEN_DISPS(gfxCtx);

    // Matrix_Translate(0.0f, -6240 / 100.0f, -9355 / 100.0f, MTXMODE_NEW);
    Matrix_Translate(CVarGetFloat(CVAR("ItemPagePositionX"), 0.0f), CVarGetFloat(CVAR("ItemPagePositionY"), 0.0f) - TY_DIFF + (f32)WREG(2) / 100.0f, fabsf(CVarGetFloat(CVAR("ItemPagePositionZ"), 0.0f) - TZ_DIFF) - (f32)WREG(3) / 100.0f, MTXMODE_NEW);

    // Matrix_Scale(0.78f, 0.78f, 0.78f, MTXMODE_APPLY);
    Matrix_Scale(CVarGetFloat(CVAR("ItemPageScale"), 0.78f), CVarGetFloat(CVAR("ItemPageScale"), 0.78f), CVarGetFloat(CVAR("ItemPageScale"), 0.78f), MTXMODE_APPLY);

    // Matrix_RotateX(-pauseCtx->unk_1F4 / 100.0f, MTXMODE_APPLY);
    Matrix_RotateZ(DEG_TO_RAD(CVarGetFloat(CVAR("ItemPageRotateZ"), 0.0f)), MTXMODE_APPLY);
    Matrix_RotateY(DEG_TO_RAD(CVarGetFloat(CVAR("ItemPageRotateY"), 0.0f)), MTXMODE_APPLY);
    Matrix_RotateX(DEG_TO_RAD(CVarGetFloat(CVAR("ItemPageRotateX"), 0.0f) - RX_DIFF), MTXMODE_APPLY);

    gSPMatrix(POLY_KAL_DISP++, Matrix_NewMtx(gfxCtx, (char*)__FILE__, __LINE__),
            G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    POLY_KAL_DISP = KaleidoScope_DrawPageSections(POLY_KAL_DISP, sNewItemPageVtx,
                                                                (void**)sSelectItemTexs[gSaveContext.language]);

    CLOSE_DISPS(gfxCtx);
}

static void EditOriginalItemPagePosition(PauseContext* pauseCtx, GraphicsContext* gfxCtx) {

    if (CVarGetInteger(CVAR("ItemPageUp"), false)) {
        f32 trx = RX_DIFF;
        f32 tty = TY_DIFF;
        f32 ttz = TZ_DIFF;
        f32 crx = CVarGetFloat(CVAR("ItemPageRotateX"), 0.0f);
        f32 cty = CVarGetFloat(CVAR("ItemPagePositionY"), 0.0f);
        f32 ctz = CVarGetFloat(CVAR("ItemPagePositionZ"), 0.0f);
        crx = ItemPage_LERPCeilF(trx, crx, 1.0f / 5.0f, 1.0f);
        cty = ItemPage_LERPCeilF(tty, cty, 1.0f / 5.0f, 1.0f);
        ctz = ItemPage_LERPCeilF(ttz, ctz, 1.0f / 5.0f, 1.0f);
        CVarSetFloat(CVAR("ItemPageRotateX"), crx);
        CVarSetFloat(CVAR("ItemPagePositionY"), cty);
        CVarSetFloat(CVAR("ItemPagePositionZ"), ctz);
        if (trx == crx && tty == cty && ttz == ctz) {
            CVarSetInteger(CVAR("ItemPageUp"), false);
            sNewPageIndex = PAGE_NEW_1;
            sNewPageState = NEW_PAGE_STATE_READY;
        }
    } else if (CVarGetInteger(CVAR("ItemPageDown"), false)) {
        f32 trx = 0.0f;
        f32 tty = 0.0f;
        f32 ttz = 0.0f;
        f32 crx = CVarGetFloat(CVAR("ItemPageRotateX"), 0.0f);
        f32 cty = CVarGetFloat(CVAR("ItemPagePositionY"), 0.0f);
        f32 ctz = CVarGetFloat(CVAR("ItemPagePositionZ"), 0.0f);
        crx = ItemPage_LERPCeilF(trx, crx, 1.0f / 5.0f, 1.0f);
        cty = ItemPage_LERPCeilF(tty, cty, 1.0f / 5.0f, 1.0f);
        ctz = ItemPage_LERPCeilF(ttz, ctz, 1.0f / 5.0f, 1.0f);
        CVarSetFloat(CVAR("ItemPageRotateX"), crx);
        CVarSetFloat(CVAR("ItemPagePositionY"), cty);
        CVarSetFloat(CVAR("ItemPagePositionZ"), ctz);
        if (trx == crx && tty == cty && ttz == ctz) {
            CVarSetInteger(CVAR("ItemPageDown"), false);
            sNewPageIndex = PAGE_ORIGINAL;
            sNewPageState = NEW_PAGE_STATE_READY;
        }
    }


    OPEN_DISPS(gfxCtx);

    // Matrix_Translate(0.0f, -6240 / 100.0f, -9355 / 100.0f, MTXMODE_NEW);
    Matrix_Translate(CVarGetFloat(CVAR("ItemPagePositionX"), 0.0f), CVarGetFloat(CVAR("ItemPagePositionY"), 0.0f) + (f32)WREG(2) / 100.0f, CVarGetFloat(CVAR("ItemPagePositionZ"), 0.0f) - (f32)WREG(3) / 100.0f, MTXMODE_NEW);

    // Matrix_Scale(0.78f, 0.78f, 0.78f, MTXMODE_APPLY);
    Matrix_Scale(CVarGetFloat(CVAR("ItemPageScale"), 0.78f), CVarGetFloat(CVAR("ItemPageScale"), 0.78f), CVarGetFloat(CVAR("ItemPageScale"), 0.78f), MTXMODE_APPLY);

    // Matrix_RotateX(-pauseCtx->unk_1F4 / 100.0f, MTXMODE_APPLY);
    Matrix_RotateZ(DEG_TO_RAD(CVarGetFloat(CVAR("ItemPageRotateZ"), 0.0f)), MTXMODE_APPLY);
    Matrix_RotateY(DEG_TO_RAD(CVarGetFloat(CVAR("ItemPageRotateY"), 0.0f)), MTXMODE_APPLY);
    Matrix_RotateX(DEG_TO_RAD(CVarGetFloat(CVAR("ItemPageRotateX"), 0.0f)), MTXMODE_APPLY);

    gSPMatrix(POLY_KAL_DISP++, Matrix_NewMtx(gfxCtx, (char*)__FILE__, __LINE__),
            G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    CLOSE_DISPS(gfxCtx);
}

static void DrawPageDownMarker(GraphicsContext* gfxCtx) {
    static s32 alpha = 255;
    const s32 textureWidth = 32;
    const s32 textureHeight = 32;
    Vtx* vtx = (Vtx*)Graph_Alloc(gfxCtx, 4 * sizeof(Vtx));

    if (sNewPageState == NEW_PAGE_STATE_READY) {
        alpha = ItemPage_LERPCeilF(255, alpha, 0.3f, 0.5f);
    } else if (sNewPageState == NEW_PAGE_STATE_TRANSITION) {
        alpha = ItemPage_LERPCeilF(0, alpha, 0.3f, 0.5f);
    }

    vtx[0].v.ob[0] = vtx[2].v.ob[0] = -72;

    vtx[1].v.ob[0] = vtx[3].v.ob[0] = 0;

    vtx[0].v.ob[1] = vtx[1].v.ob[1] = -40;

    vtx[2].v.ob[1] = vtx[3].v.ob[1] = -40 - (textureWidth * 2 / 3);

    vtx[0].v.ob[2] = vtx[1].v.ob[2] =
        vtx[2].v.ob[2] = vtx[3].v.ob[2] = 0;

    vtx[0].v.flag = vtx[1].v.flag =
        vtx[2].v.flag = vtx[3].v.flag = 0;

    vtx[0].v.tc[0] = vtx[0].v.tc[1] =
        vtx[1].v.tc[1] = vtx[2].v.tc[0] = 0;

    vtx[1].v.tc[0] = vtx[3].v.tc[0] = 0x900;

    vtx[2].v.tc[1] = vtx[3].v.tc[1] = 0x300;

    vtx[0].v.cn[0] = vtx[2].v.cn[0] =
        vtx[0].v.cn[1] = vtx[2].v.cn[1] =
            vtx[0].v.cn[2] = vtx[2].v.cn[2] =
                vtx[1].v.cn[0] = vtx[3].v.cn[0] =
                    vtx[1].v.cn[1] = vtx[3].v.cn[1] =
                        vtx[1].v.cn[2] = vtx[3].v.cn[2] = 200;

    vtx[0].v.cn[3] = vtx[2].v.cn[3] =
        vtx[1].v.cn[3] = vtx[3].v.cn[3] = 255;


    if (sCursorOnPageDown) {
        vtx[0].v.ob[0] = vtx[2].v.ob[0] = (WREG(16) + WREG(17) - 12) / 2;

        vtx[1].v.ob[0] = vtx[3].v.ob[0] = vtx[0].v.ob[0] + textureWidth * 3;

        vtx[0].v.ob[1] = vtx[1].v.ob[1] = WREG(18) - PDOWN_OFFSET;

        vtx[2].v.ob[1] = vtx[3].v.ob[1] =
            vtx[0].v.ob[1] - textureHeight;
    } else {
        vtx[0].v.ob[0] = vtx[2].v.ob[0] = (WREG(16) + WREG(17) - 12) / 2 + 3;

        vtx[1].v.ob[0] = vtx[3].v.ob[0] = vtx[0].v.ob[0] + (textureWidth * 3 * 2 / 3);

        vtx[0].v.ob[1] = vtx[1].v.ob[1] = WREG(18) - 3 - PDOWN_OFFSET;

        vtx[2].v.ob[1] = vtx[3].v.ob[1] =
            vtx[0].v.ob[1] - (textureHeight * 2 / 3);
    }



    OPEN_DISPS(gfxCtx);
    Gfx_SetupDL_39Overlay(gfxCtx);

    gSPVertex(POLY_KAL_DISP++, (uintptr_t)vtx, 4, 0);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, 180, 210, 255, alpha);
    gDPSetEnvColor(POLY_KAL_DISP++, 255, 255, 255, alpha);

    gDPLoadTextureBlock(POLY_KAL_DISP++, gEmptyCDownArrowTex, G_IM_FMT_IA, G_IM_SIZ_8b, textureWidth, textureHeight, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gSP1Quadrangle(POLY_KAL_DISP++, 0, 2, 3, 1, 0);

    gDPPipeSync(POLY_KAL_DISP++);

    CLOSE_DISPS(gfxCtx);
}

static void DrawPageUpMarker(GraphicsContext* gfxCtx) {
    static s32 alpha = 255;
    const s32 textureWidth = 32;
    const s32 textureHeight = 32;
    Vtx* vtx = (Vtx*)Graph_Alloc(gfxCtx, 4 * sizeof(Vtx));

    if (sNewPageState == NEW_PAGE_STATE_READY) {
        alpha = ItemPage_LERPCeilF(255, alpha, 0.3f, 0.5f);
    } else if (sNewPageState == NEW_PAGE_STATE_TRANSITION) {
        alpha = ItemPage_LERPCeilF(0, alpha, 0.3f, 0.5f);
    }

    vtx[0].v.ob[0] = vtx[2].v.ob[0] = -72;

    vtx[1].v.ob[0] = vtx[3].v.ob[0] = 0;

    vtx[2].v.ob[1] = vtx[3].v.ob[1] = -40;

    vtx[0].v.ob[1] = vtx[1].v.ob[1] = -40 - (textureWidth * 2 / 3);

    vtx[0].v.ob[2] = vtx[1].v.ob[2] =
        vtx[2].v.ob[2] = vtx[3].v.ob[2] = 0;

    vtx[0].v.flag = vtx[1].v.flag =
        vtx[2].v.flag = vtx[3].v.flag = 0;

    vtx[0].v.tc[0] = vtx[0].v.tc[1] =
        vtx[1].v.tc[1] = vtx[2].v.tc[0] = 0;

    vtx[1].v.tc[0] = vtx[3].v.tc[0] = 0x900;

    vtx[2].v.tc[1] = vtx[3].v.tc[1] = 0x300;

    vtx[0].v.cn[0] = vtx[2].v.cn[0] =
        vtx[0].v.cn[1] = vtx[2].v.cn[1] =
            vtx[0].v.cn[2] = vtx[2].v.cn[2] =
                vtx[1].v.cn[0] = vtx[3].v.cn[0] =
                    vtx[1].v.cn[1] = vtx[3].v.cn[1] =
                        vtx[1].v.cn[2] = vtx[3].v.cn[2] = 200;

    vtx[0].v.cn[3] = vtx[2].v.cn[3] =
        vtx[1].v.cn[3] = vtx[3].v.cn[3] = 255;


    if (sCursorOnPageUp) {
        vtx[0].v.ob[0] = vtx[2].v.ob[0] = (WREG(16) + WREG(17) - 12) / 2;

        vtx[1].v.ob[0] = vtx[3].v.ob[0] = vtx[0].v.ob[0] + textureWidth * 3;

        vtx[2].v.ob[1] = vtx[3].v.ob[1] = WREG(18) - PUP_OFFSET;

        vtx[0].v.ob[1] = vtx[1].v.ob[1] =
            vtx[2].v.ob[1] - textureHeight;
    } else {
        vtx[0].v.ob[0] = vtx[2].v.ob[0] = (WREG(16) + WREG(17) - 12) / 2 + 3;

        vtx[1].v.ob[0] = vtx[3].v.ob[0] = vtx[0].v.ob[0] + (textureWidth * 3 * 2 / 3);

        vtx[2].v.ob[1] = vtx[3].v.ob[1] = WREG(18) - 3 - PUP_OFFSET;

        vtx[0].v.ob[1] = vtx[1].v.ob[1] =
            vtx[2].v.ob[1] - (textureHeight * 2 / 3);
    }

    OPEN_DISPS(gfxCtx);
    Gfx_SetupDL_39Overlay(gfxCtx);

    gSPVertex(POLY_KAL_DISP++, (uintptr_t)vtx, 4, 0);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, 180, 210, 255, alpha);
    gDPSetEnvColor(POLY_KAL_DISP++, 255, 255, 255, alpha);

    gDPLoadTextureBlock(POLY_KAL_DISP++, gEmptyCDownArrowTex, G_IM_FMT_IA, G_IM_SIZ_8b, textureWidth, textureHeight, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gSP1Quadrangle(POLY_KAL_DISP++, 2, 0, 1, 3, 0);

    gDPPipeSync(POLY_KAL_DISP++);

    CLOSE_DISPS(gfxCtx);
}

static void DrawPageMarkers() {
    PlayState* play = gPlayState;
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    if (sNewPageIndex != sItemPageCount - 1) {
        DrawPageDownMarker(gfxCtx);
    }

    if (sNewPageIndex != PAGE_ORIGINAL) {
        DrawPageUpMarker(gfxCtx);
    }
}

static void DrawNewItemPages() {
    PlayState* play = gPlayState;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    PauseContext* pauseCtx = &play->pauseCtx;

    // TODO: Setup loop between i - 1 and i + 1 in range 0 to numPages - 1 and pass as parameter

    if (pauseCtx->state == 6 /* PAUSE_STATE_MAIN */ || (pauseCtx->state >= 8 && pauseCtx->state <= 17)) {
        DrawNewItemMenuPage(play, gfxCtx);
    }
}

static void ResetNewPage() {
    sNewPageIndex = PAGE_ORIGINAL;
    sNewPageState = NEW_PAGE_STATE_READY;
    sCursorOnPageDown = false;
    sCursorOnPageUp = false;
    ClearItemPagePosition();
}

static void OnConfigurationChanged() {
    ResetNewPage();

    COND_HOOK(OnDrawItemSelectEnd, CVarGetInteger(CVAR("NewItems"), false), DrawNewItemPages);
    COND_HOOK(OnDrawCursorSpecialPos, CVarGetInteger(CVAR("NewItems"), false), DrawPageMarkers);

    // Should Draw Icons and Cursor on Page
    COND_VB_SHOULD(VB_DRAW_ITEM_SELECT, CVarGetInteger(CVAR("NewItems"), false), {
        PauseContext* pauseCtx = va_arg(args, PauseContext*);
        DrawNewItemMenus(pauseCtx);
        if (sNewPageIndex > PAGE_NEW_1) {
            *should = false;
        }
    });

    // Should Draw Page
    COND_VB_SHOULD(VB_DRAW_ITEM_SELECT_PAGE, CVarGetInteger(CVAR("NewItems"), false), {
        PlayState* play = va_arg(args, PlayState*);
        GraphicsContext* gfxCtx = va_arg(args, GraphicsContext*);
        if (sNewPageIndex > PAGE_NEW_1) {
            *should = false;
        }
    });

    COND_VB_SHOULD(VB_SETUP_ITEM_PAGE_POSITION, CVarGetInteger(CVAR("NewItems"), false), {
        PlayState* play = va_arg(args, PlayState*);
        GraphicsContext* gfxCtx = va_arg(args, GraphicsContext*);
        PauseContext* pauseCtx = &play->pauseCtx;
        // If pause menu active or cycling between pages
        if (pauseCtx->state == 6 /* PAUSE_STATE_MAIN */ || (pauseCtx->state >= 8 && pauseCtx->state <= 17)) {
            EditOriginalItemPagePosition(pauseCtx, gfxCtx);
            *should = false;
        }
    })
}

static void DrawMenu() {
    ImGui::SeparatorText(AUTHOR);
    if (UIWidgets::EnhancementCheckbox("New Items", CVAR("NewItems"))) {
        OnConfigurationChanged();
    }

    UIWidgets::EnhancementSliderFloat("ItemPageRotateX %.2f", "##IPRX", CVAR("ItemPageRotateX"), -180.0f, 180.0f, "", 0.0f, false, true, !CVarGetInteger(CVAR("NewItems"), false));
    UIWidgets::EnhancementSliderFloat("ItemPageRotateY %.2f", "##IPRY", CVAR("ItemPageRotateY"), -180.0f, 180.0f, "", 0.0f, false, true, !CVarGetInteger(CVAR("NewItems"), false));
    UIWidgets::EnhancementSliderFloat("ItemPageRotateZ %.2f", "##IPRZ", CVAR("ItemPageRotateZ"), -180.0f, 180.0f, "", 0.0f, false, true, !CVarGetInteger(CVAR("NewItems"), false));
    UIWidgets::EnhancementSliderFloat("ItemPageScale %.2f", "##IPS", CVAR("ItemPageScale"), 0.1f, 2.0f, "", 0.78f, false, true, !CVarGetInteger(CVAR("NewItems"), false));
    UIWidgets::EnhancementSliderFloat("ItemPagePositionX %.2f", "##IPPX", CVAR("ItemPagePositionX"), -15.0f, 15.0f, "", 0.0f, false, true, !CVarGetInteger(CVAR("NewItems"), false));
    UIWidgets::EnhancementSliderFloat("ItemPagePositionY %.2f", "##IPPY", CVAR("ItemPagePositionY"), -200.0f, 200.0f, "", 0.0f, false, true, !CVarGetInteger(CVAR("NewItems"), false));
    UIWidgets::EnhancementSliderFloat("ItemPagePositionZ %.2f", "##IPPZ", CVAR("ItemPagePositionZ"), -100.0f, 100.0f, "", 0.0f, false, true, !CVarGetInteger(CVAR("NewItems"), false));

    if (ImGui::Button("Reset Item Page Position")) {
        ClearItemPagePosition();
    }

    if (ImGui::Button("Page Up")) {
        sNewPageState = NEW_PAGE_STATE_TRANSITION;
        CVarSetInteger(CVAR("ItemPageUp"), true);
    }
    
    if (ImGui::Button("Page Down")) {
        sNewPageState = NEW_PAGE_STATE_TRANSITION;
        CVarSetInteger(CVAR("ItemPageDown"), true);
    }
}

static void RegisterMod() {
    // #region Leave this alone unless you know what you are doing
    OnConfigurationChanged();
    // #endregion

    // TODO: Anything you want to run once on startup
}

// TODO: Uncomment this line to enable the mod
static Holiday holiday(DrawMenu, RegisterMod);
