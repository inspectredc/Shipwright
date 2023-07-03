#include "GameInteractor.h"

extern "C" {
#include "z64actor.h"
#include "functions.h"
extern PlayState* gPlayState;
extern "C" s16 gEnPartnerId;
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
