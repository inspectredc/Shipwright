#include "GameInteractor.h"

extern "C" {
#include "z64actor.h"
#include "functions.h"
extern PlayState* gPlayState;
extern "C" s16 gEnPartnerId;
extern "C" s16 gDummyPlayerId;
}

// MARK: - State Definitions

bool GameInteractor::State::NoUIActive = 0;
GILinkSize GameInteractor::State::LinkSize = GI_LINK_SIZE_NORMAL;
bool GameInteractor::State::InvisibleLinkActive = 0;
bool GameInteractor::State::OneHitKOActive = 0;
bool GameInteractor::State::PacifistModeActive = 0;
bool GameInteractor::State::DisableZTargetingActive = 0;
bool GameInteractor::State::ReverseControlsActive = 0;
int32_t GameInteractor::State::DefenseModifier = 0;
int32_t GameInteractor::State::RunSpeedModifier = 0;
GIGravityLevel GameInteractor::State::GravityLevel = GI_GRAVITY_LEVEL_NORMAL;
uint32_t GameInteractor::State::EmulatedButtons = 0;
uint8_t GameInteractor::State::RandomBombFuseTimerActive = 0;
uint8_t GameInteractor::State::DisableLedgeGrabsActive = 0;
uint8_t GameInteractor::State::RandomWindActive = 0;
uint8_t GameInteractor::State::RandomWindSecondsSinceLastDirectionChange = 0;
uint8_t GameInteractor::State::RandomBonksActive = 0;
uint8_t GameInteractor::State::SlipperyFloorActive = 0;
uint8_t GameInteractor::State::SecondCollisionUpdate = 0;
std::vector<uint32_t> GameInteractor::State::CoopPlayerIds = {};
std::map<uint32_t, std::pair<uint8_t, PosRot>> GameInteractor::State::CoopPlayerPositions = {};
std::unordered_map<uint32_t, Vec3s> GameInteractor::State::CoopPlayerShapeRotations = {};
std::unordered_map<uint32_t, int32_t> GameInteractor::State::CoopPlayerAges = {};
std::unordered_map<uint32_t, Cylinder16> GameInteractor::State::CoopPlayerCylinders = {};
std::unordered_map<uint32_t, Color_RGB8> GameInteractor::State::CoopPlayerColors = {};
std::unordered_map<uint32_t, std::string> GameInteractor::State::CoopPlayerAnim = {};
std::unordered_map<uint32_t, std::string> GameInteractor::State::CoopPlayerAnimUpper = {};
std::unordered_map<uint32_t, uint32_t> GameInteractor::State::CoopPlayerStateFlags2 = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerPlaySpeeds = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerStartFrames = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerCurFrames = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerEndFrames = {};
std::unordered_map<uint32_t, uint8_t> GameInteractor::State::CoopPlayerModes = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerPlaySpeeds2 = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerStartFrames2 = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerCurFrames2 = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerEndFrames2 = {};
std::unordered_map<uint32_t, uint8_t> GameInteractor::State::CoopPlayerModes2 = {};
std::unordered_map<uint32_t, uint8_t> GameInteractor::State::CoopPlayerMoveFlags = {};
std::unordered_map<uint32_t, int16_t> GameInteractor::State::CoopPlayerPitchOffset = {};
std::unordered_map<uint32_t, float> GameInteractor::State::CoopPlayerOffsetY = {};
std::unordered_map<uint32_t, std::vector<Vec3s>> GameInteractor::State::CoopPlayerLimbs = {};
std::unordered_map<uint32_t, std::vector<Vec3s>> GameInteractor::State::CoopPlayerLimbsUpper = {};


void GameInteractor::State::SetPacifistMode(bool active) {
    PacifistModeActive = active;
    // Force interface update to update the button's transparency.
    GameInteractor::RawAction::ForceInterfaceUpdate();
}

// MARK: C - Bridge

// MARK: - GameInteractor::State::NoUIActive
uint8_t GameInteractor_NoUIActive() {
    return GameInteractor::State::NoUIActive;
}

// MARK: - GameInteractor::State::LinkSize
GILinkSize GameInteractor_GetLinkSize() {
    return GameInteractor::State::LinkSize;
}

// MARK: - GameInteractor::State::LinkSize
void GameInteractor_SetLinkSize(GILinkSize size) {
    GameInteractor::State::LinkSize = size;
}

// MARK: - GameInteractor::State::InvisibleLinkActive
uint8_t GameInteractor_InvisibleLinkActive() {
    return GameInteractor::State::InvisibleLinkActive;
}

void GameInteractor_SetInvisibleLinkActive(uint8_t active) {
    GameInteractor::State::InvisibleLinkActive = active;
}

// MARK: - GameInteractor::State::OneHitKOActive
uint8_t GameInteractor_OneHitKOActive() {
    return GameInteractor::State::OneHitKOActive;
}

// MARK: - GameInteractor::State::PacifistModeActive
uint8_t GameInteractor_PacifistModeActive() {
    return GameInteractor::State::PacifistModeActive;
}

// MARK: - GameInteractor::State::DisableZTargetingActive
uint8_t GameInteractor_DisableZTargetingActive() {
    return GameInteractor::State::DisableZTargetingActive;
}

// MARK: - GameInteractor::State::DisableCameraRotationActive
uint8_t GameInteractor_ReverseControlsActive() {
    return GameInteractor::State::ReverseControlsActive;
}

// MARK: - GameInteractor::State::DisableCameraRotationActive
int32_t GameInteractor_DefenseModifier() {
    return GameInteractor::State::DefenseModifier;
}

// MARK: - GameInteractor::State::DisableCameraRotationActive
int32_t GameInteractor_RunSpeedModifier() {
    return GameInteractor::State::RunSpeedModifier;
}

// MARK: - GameInteractor::State::DisableCameraRotationActive
GIGravityLevel GameInteractor_GravityLevel() {
    return GameInteractor::State::GravityLevel;
}

// MARK: - GameInteractor::State::GetEmulatedButtons
uint32_t GameInteractor_GetEmulatedButtons() {
    return GameInteractor::State::EmulatedButtons;
}

// MARK: - GameInteractor::State::SetEmulatedButtons
void GameInteractor_SetEmulatedButtons(uint32_t buttons) {
    GameInteractor::State::EmulatedButtons = buttons;
}

// MARK: - GameInteractor::State::GetRandomBombFuseTimerActive
uint8_t GameInteractor_GetRandomBombFuseTimerActive() {
    return GameInteractor::State::RandomBombFuseTimerActive;
}

// MARK: - GameInteractor::State::GetDisableLedgeGrabsActive
uint8_t GameInteractor_GetDisableLedgeGrabsActive() {
    return GameInteractor::State::DisableLedgeGrabsActive;
}

// MARK: - GameInteractor::State::GetRandomWindActive
uint8_t GameInteractor_GetRandomWindActive() {
    return GameInteractor::State::RandomWindActive;
}

// MARK: - GameInteractor::State::GetRandomBonksActive
uint8_t GameInteractor_GetRandomBonksActive() {
    return GameInteractor::State::RandomBonksActive;
}

// MARK: - GameInteractor::State::GetSlipperyFloorActive
uint8_t GameInteractor_GetSlipperyFloorActive() {
    return GameInteractor::State::SlipperyFloorActive;
}

// MARK: - GameInteractor::State::SecondCollisionUpdate
uint8_t GameInteractor_SecondCollisionUpdate() {
    return GameInteractor::State::SecondCollisionUpdate;
}

uint8_t GameInteractor_GetCoopPlayerScene(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerPositions.find(playerId) == GameInteractor::State::CoopPlayerPositions.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0;
    }

    return GameInteractor::State::CoopPlayerPositions[playerId].first;
}

PosRot GameInteractor_GetCoopPlayerPosition(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerPositions.find(playerId) == GameInteractor::State::CoopPlayerPositions.end() ||
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return {-9999.0, -9999.0, -9999.0, 0, 0, 0};
    }

    return GameInteractor::State::CoopPlayerPositions[playerId].second;
}

Vec3s GameInteractor_GetCoopPlayerShapeRotation(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerShapeRotations.find(playerId) == GameInteractor::State::CoopPlayerShapeRotations.end() ||
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return { 0, 0, 0 };
    }

    return GameInteractor::State::CoopPlayerShapeRotations[playerId];
}

int32_t GameInteractor_GetCoopPlayerAge(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerAges.find(playerId) == GameInteractor::State::CoopPlayerAges.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 1;
    }

    return GameInteractor::State::CoopPlayerAges[playerId];
}

Cylinder16 GameInteractor_GetCoopPlayerCylinder(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerCylinders.find(playerId) == GameInteractor::State::CoopPlayerCylinders.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return { 12, 60, 0, { 0, 0, 0 } };
    }

    return GameInteractor::State::CoopPlayerCylinders[playerId];
}

Color_RGB8 GameInteractor_GetCoopPlayerColor(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerColors.find(playerId) == GameInteractor::State::CoopPlayerColors.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return {255, 255, 255};
    }

    return GameInteractor::State::CoopPlayerColors[playerId];
}

char* GameInteractor_GetCoopPlayerAnim(uint32_t playerId, char* anim) {
    if (
        GameInteractor::State::CoopPlayerAnim.find(playerId) == GameInteractor::State::CoopPlayerAnim.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        strcpy(anim, gPlayerAnim_link_normal_wait_free);
        return anim;
    }

    strcpy(anim, GameInteractor::State::CoopPlayerAnim[playerId].c_str());
    return anim;
}

char* GameInteractor_GetCoopPlayerAnimUpper(uint32_t playerId, char* anim) {
    if (
        GameInteractor::State::CoopPlayerAnimUpper.find(playerId) == GameInteractor::State::CoopPlayerAnimUpper.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        strcpy(anim, gPlayerAnim_link_normal_wait_free);
        return anim;
    }

    strcpy(anim, GameInteractor::State::CoopPlayerAnimUpper[playerId].c_str());
    return anim;
}

uint32_t GameInteractor_GetCoopPlayerStateFlags2(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerStateFlags2.find(playerId) == GameInteractor::State::CoopPlayerStateFlags2.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0;
    }

    return GameInteractor::State::CoopPlayerStateFlags2[playerId];
}

uint8_t GameInteractor_GetCoopPlayerMoveFlags(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerMoveFlags.find(playerId) == GameInteractor::State::CoopPlayerMoveFlags.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0;
    }

    return GameInteractor::State::CoopPlayerMoveFlags[playerId];
}

int16_t GameInteractor_GetCoopPlayerPitchOffset(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerPitchOffset.find(playerId) == GameInteractor::State::CoopPlayerPitchOffset.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0;
    }

    return GameInteractor::State::CoopPlayerPitchOffset[playerId];
}

float GameInteractor_GetCoopPlayerOffsetY(uint32_t playerId) {
    if (
        GameInteractor::State::CoopPlayerOffsetY.find(playerId) == GameInteractor::State::CoopPlayerOffsetY.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0.0f;
    }

    return GameInteractor::State::CoopPlayerOffsetY[playerId];
}

float GameInteractor_GetCoopPlayerPlaySpeed(uint32_t playerId, bool isUpper) {
    if (
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 1.0f;
    }
    if (!isUpper) {
        if (
            GameInteractor::State::CoopPlayerPlaySpeeds.find(playerId) == GameInteractor::State::CoopPlayerPlaySpeeds.end()
        ) {
            return 1.0f;
        }
        return GameInteractor::State::CoopPlayerPlaySpeeds[playerId];
    } else {
        if (
            GameInteractor::State::CoopPlayerPlaySpeeds2.find(playerId) == GameInteractor::State::CoopPlayerPlaySpeeds2.end()
        ) {
            return 1.0f;
        }
        return GameInteractor::State::CoopPlayerPlaySpeeds2[playerId];
    }
}

float GameInteractor_GetCoopPlayerStartFrame(uint32_t playerId, bool isUpper) {
    if (
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0.0f;
    }
    if (!isUpper) {
        if (
            GameInteractor::State::CoopPlayerStartFrames.find(playerId) == GameInteractor::State::CoopPlayerStartFrames.end()
        ) {
            return 0.0f;
        }
        return GameInteractor::State::CoopPlayerStartFrames[playerId];
    } else {
        if (
            GameInteractor::State::CoopPlayerStartFrames2.find(playerId) == GameInteractor::State::CoopPlayerStartFrames2.end()
        ) {
            return 0.0f;
        }
        return GameInteractor::State::CoopPlayerStartFrames2[playerId];
    }
}

float GameInteractor_GetCoopPlayerCurFrame(uint32_t playerId, bool isUpper) {
    if (
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0.0f;
    }
    if (!isUpper) {
        if (
            GameInteractor::State::CoopPlayerCurFrames.find(playerId) == GameInteractor::State::CoopPlayerCurFrames.end()
        ) {
            return 0.0f;
        }
        return GameInteractor::State::CoopPlayerCurFrames[playerId];
    } else {
        if (
            GameInteractor::State::CoopPlayerCurFrames2.find(playerId) == GameInteractor::State::CoopPlayerCurFrames2.end()
        ) {
            return 0.0f;
        }
        return GameInteractor::State::CoopPlayerCurFrames2[playerId];
    }
}

float GameInteractor_GetCoopPlayerEndFrame(uint32_t playerId, bool isUpper) {
    if (
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 1.0f;
    }
    if (!isUpper) {
        if (
            GameInteractor::State::CoopPlayerEndFrames.find(playerId) == GameInteractor::State::CoopPlayerEndFrames.end()
        ) {
            return 1.0f;
        }
        return GameInteractor::State::CoopPlayerEndFrames[playerId];
    } else {
        if (
            GameInteractor::State::CoopPlayerEndFrames2.find(playerId) == GameInteractor::State::CoopPlayerEndFrames2.end()
        ) {
            return 1.0f;
        }
        return GameInteractor::State::CoopPlayerEndFrames2[playerId];
    }
}

uint8_t GameInteractor_GetCoopPlayerMode(uint32_t playerId, bool isUpper) {
    if (
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return 0;
    }
    if (!isUpper) {
        if (
            GameInteractor::State::CoopPlayerModes.find(playerId) == GameInteractor::State::CoopPlayerModes.end()
        ) {
            return 0;
        }
        return GameInteractor::State::CoopPlayerModes[playerId];
    } else {
        if (
            GameInteractor::State::CoopPlayerModes2.find(playerId) == GameInteractor::State::CoopPlayerModes2.end()
        ) {
            return 0;
        }
        return GameInteractor::State::CoopPlayerModes2[playerId];
    }
}

Vec3s GameInteractor_GetCoopPlayerLimb(uint32_t playerId, uint16_t limbIndex) {
    if (
        GameInteractor::State::CoopPlayerLimbs.find(playerId) == GameInteractor::State::CoopPlayerLimbs.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return {0,0,0};
    }

    return GameInteractor::State::CoopPlayerLimbs[playerId][limbIndex];
}

Vec3s GameInteractor_GetCoopPlayerLimbUpper(uint32_t playerId, uint16_t limbIndex) {
    if (
        GameInteractor::State::CoopPlayerLimbsUpper.find(playerId) == GameInteractor::State::CoopPlayerLimbsUpper.end() || 
        std::find(GameInteractor::State::CoopPlayerIds.begin(), GameInteractor::State::CoopPlayerIds.end(), playerId) == GameInteractor::State::CoopPlayerIds.end()
    ) {
        return {0,0,0};
    }

    return GameInteractor::State::CoopPlayerLimbsUpper[playerId][limbIndex];
}

void GameInteractor_SpawnCoopFairies() {
    if (gPlayState == NULL) return;
    Actor* actor = gPlayState->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;

    while (actor != NULL) {
        if (gEnPartnerId == actor->id) {
            Actor_Kill(actor);
        }
        actor = actor->next;
    }

    if (GameInteractor::State::CoopPlayerIds.size() == 0) {
        return;
    }

    for (uint32_t playerId : GameInteractor::State::CoopPlayerIds) {
        Actor_Spawn(&gPlayState->actorCtx, gPlayState, gEnPartnerId, -9999.0, -9999.0, -9999.0, 0, 0, 0, 3 + playerId, false);
    }
}

void GameInteractor_SpawnCoopDummyPlayers() {
    if (gPlayState == NULL) return;
    Actor* actor = gPlayState->actorCtx.actorLists[ACTORCAT_ITEMACTION].head;

    while (actor != NULL) {
        if (gDummyPlayerId == actor->id) {
            Actor_Kill(actor);
        }
        actor = actor->next;
    }

    if (GameInteractor::State::CoopPlayerIds.size() == 0) {
        return;
    }

    for (uint32_t playerId : GameInteractor::State::CoopPlayerIds) {
        Actor_Spawn(&gPlayState->actorCtx, gPlayState, gDummyPlayerId, -9999.0, -9999.0, -9999.0, 0, 0, 0, playerId, false);
    }
}
