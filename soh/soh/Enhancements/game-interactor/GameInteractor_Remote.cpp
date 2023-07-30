#ifdef ENABLE_REMOTE_CONTROL

#include "GameInteractor.h"
#include <spdlog/spdlog.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <unordered_map>
#include <tuple>
#include <type_traits>
#include <libultraship/libultraship.h>

#include "soh/Enhancements/mods.h"

using json = nlohmann::json;

// MARK: - Declarations

template <class DstType, class SrcType>
bool IsType(const SrcType* src) {
  return dynamic_cast<const DstType*>(src) != nullptr;
}

template <class TypeA>
bool TakesParameter() {
    return std::is_base_of_v<ParameterizedGameInteractionEffect, TypeA>;
}

template <class TypeA>
bool IsRemovable() {
    return std::is_base_of_v<RemovableGameInteractionEffect, TypeA>;
}

/// Map of string name to enum value and flag whether it takes in a param or not
std::unordered_map<std::string, std::tuple<GameInteractionEffect::Values, bool, bool>> nameToEnum = {
    {"modify_heart_container", {
        GameInteractionEffect::Values::modifyHeartContainers,
        IsRemovable<GameInteractionEffect::ModifyHeartContainers>(),
        TakesParameter<GameInteractionEffect::ModifyHeartContainers>()
    }},
    { "fill_magic", {
        GameInteractionEffect::Values::fillMagic,
        IsRemovable<GameInteractionEffect::ModifyHeartContainers>(),
        TakesParameter<GameInteractionEffect::FillMagic>()
    }},
    { "empty_magic", {
        GameInteractionEffect::Values::emptyMagic,
        IsRemovable<GameInteractionEffect::EmptyMagic>(),
        TakesParameter<GameInteractionEffect::EmptyMagic>()
    }},
    { "modify_rupees", {
        GameInteractionEffect::Values::modifyRupees,
        IsRemovable<GameInteractionEffect::ModifyRupees>(),
        TakesParameter<GameInteractionEffect::ModifyRupees>()
    }},
    { "no_ui", {
        GameInteractionEffect::Values::noUI,
        IsRemovable<GameInteractionEffect::NoUI>(),
        TakesParameter<GameInteractionEffect::NoUI>()
    }},
    { "modify_gravity", {
        GameInteractionEffect::Values::modifyGravity,
        IsRemovable<GameInteractionEffect::ModifyGravity>(),
        TakesParameter<GameInteractionEffect::ModifyGravity>()
    }},
    { "modify_health", {
        GameInteractionEffect::Values::modifyHealth,
        IsRemovable<GameInteractionEffect::ModifyHealth>(),
        TakesParameter<GameInteractionEffect::ModifyHealth>()
    }},
    { "set_player_health", {
        GameInteractionEffect::Values::setPlayerHealth,
        IsRemovable<GameInteractionEffect::SetPlayerHealth>(),
        TakesParameter<GameInteractionEffect::SetPlayerHealth>()
    }},
    { "freeze_player", {
        GameInteractionEffect::Values::freezePlayer,
        IsRemovable<GameInteractionEffect::FreezePlayer>(),
        TakesParameter<GameInteractionEffect::FreezePlayer>()
    }},
    { "burn_player", {
        GameInteractionEffect::Values::burnPlayer,
        IsRemovable<GameInteractionEffect::BurnPlayer>(),
        TakesParameter<GameInteractionEffect::BurnPlayer>()
    }},
    { "electrocute_player", {
        GameInteractionEffect::Values::electrocutePlayer,
        IsRemovable<GameInteractionEffect::ElectrocutePlayer>(),
        TakesParameter<GameInteractionEffect::ElectrocutePlayer>()
    }},
    { "knockback_player", {
        GameInteractionEffect::Values::knockbackPlayer,
        IsRemovable<GameInteractionEffect::KnockbackPlayer>(),
        TakesParameter<GameInteractionEffect::KnockbackPlayer>()
    }},
    { "modify_link_size", {
        GameInteractionEffect::Values::modifyLinkSize,
        IsRemovable<GameInteractionEffect::ModifyLinkSize>(),
        TakesParameter<GameInteractionEffect::ModifyLinkSize>()
    }},
    { "invisible_link", {
        GameInteractionEffect::Values::invisibleLink,
        IsRemovable<GameInteractionEffect::InvisibleLink>(),
        TakesParameter<GameInteractionEffect::InvisibleLink>()
    }},
    { "pacifist_mode", {
        GameInteractionEffect::Values::pacifistMode,
        IsRemovable<GameInteractionEffect::PacifistMode>(),
        TakesParameter<GameInteractionEffect::PacifistMode>()
    }},
    { "disable_z_targeting", {
        GameInteractionEffect::Values::disableZTargeting,
        IsRemovable<GameInteractionEffect::DisableZTargeting>(),
        TakesParameter<GameInteractionEffect::DisableZTargeting>()
    }},
    { "weather_rainstorm", {
        GameInteractionEffect::Values::weatherRainstorm,
        IsRemovable<GameInteractionEffect::WeatherRainstorm>(),
        TakesParameter<GameInteractionEffect::WeatherRainstorm>()
    }},
    { "reverse_controls", {
        GameInteractionEffect::Values::reverseControls,
        IsRemovable<GameInteractionEffect::ReverseControls>(),
        TakesParameter<GameInteractionEffect::ReverseControls>()
    }},
    { "force_equip_boots", {
        GameInteractionEffect::Values::forceEquipBoots,
        IsRemovable<GameInteractionEffect::ForceEquipBoots>(),
        TakesParameter<GameInteractionEffect::ForceEquipBoots>()
    }},
    { "modify_run_speed_modifier", {
        GameInteractionEffect::Values::modifyRunSpeedModifier,
        IsRemovable<GameInteractionEffect::ModifyRunSpeedModifier>(),
        TakesParameter<GameInteractionEffect::ModifyRunSpeedModifier>()
    }},
    { "one_hit_ko", {
        GameInteractionEffect::Values::oneHitKO,
        IsRemovable<GameInteractionEffect::OneHitKO>(),
        TakesParameter<GameInteractionEffect::OneHitKO>()
    }},
    { "modify_defense_modifier", {
        GameInteractionEffect::Values::modifyDefenseModifier,
        IsRemovable<GameInteractionEffect::ModifyDefenseModifier>(),
        TakesParameter<GameInteractionEffect::ModifyDefenseModifier>()
    }},
    { "give_or_take_shield", {
        GameInteractionEffect::Values::giveOrTakeShield,
        IsRemovable<GameInteractionEffect::GiveOrTakeShield>(),
        TakesParameter<GameInteractionEffect::GiveOrTakeShield>()
    }},
};

// MARK: - Remote

void GameInteractor::EnableRemoteInteractor() {
    if (isRemoteInteractorEnabled) {
        return;
    }

    Uint16 port;
    ImGui::DataTypeApplyFromText(GameInteractor::Instance->remotePortStr, ImGuiDataType_U16, &port, "%u");

    if (SDLNet_ResolveHost(&remoteIP, remoteIPStr, port) == -1) {
        SPDLOG_ERROR("[GameInteractor] SDLNet_ResolveHost: {}", SDLNet_GetError());
    }

    isRemoteInteractorEnabled = true;
    remoteThreadReceive = std::thread(&GameInteractor::ReceiveFromServer, this);
}

void GameInteractor::RegisterRemoteForwarder(std::function<void(nlohmann::json)> method) {
    remoteForwarder = method;
}

void GameInteractor::DisableRemoteInteractor() {
    if (!isRemoteInteractorEnabled) {
        return;
    }

    isRemoteInteractorEnabled = false;
    remoteThreadReceive.join();
    remoteForwarder = nullptr;

    if (CVarGetInteger("gRemoteGIScheme", 0) == 2) {
        GameInteractor::State::CoopPlayerIds.clear();
        if (IsSaveLoaded()) {
            //GameInteractor_SpawnCoopFairies();
            GameInteractor_SpawnCoopDummyPlayers();
        }
    }
}

void GameInteractor::TransmitMessageToRemote(nlohmann::json payload) {
    std::string jsonPayload = payload.dump();
    if (payload["type"] != "PlayerData") {
        SPDLOG_INFO("Sending payload: \n{}\n", jsonPayload);
    }
    jsonPayload += "\n";
    SDLNet_TCP_Send(remoteSocket, (jsonPayload).c_str(), jsonPayload.size());
}

// MARK: - Private

std::string receivedData;

void GameInteractor::ReceiveFromServer() {
    while (isRemoteInteractorEnabled) {
        while (!isRemoteInteractorConnected && isRemoteInteractorEnabled) {
            SPDLOG_TRACE("[GameInteractor] Attempting to make connection to server...");
            remoteSocket = SDLNet_TCP_Open(&remoteIP);

            if (remoteSocket) {
                isRemoteInteractorConnected = true;
                SPDLOG_INFO("[GameInteractor] Connection to server established!");

                nlohmann::json payload;
                payload["roomId"] = CVarGetString("gAnchorRoomId", "");
                payload["type"] = "Ping";
                TransmitMessageToRemote(payload);
                break;
            }
        }

        SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
        if (remoteSocket) {
            SDLNet_TCP_AddSocket(socketSet, remoteSocket);
        }

        while (isRemoteInteractorConnected && remoteSocket && isRemoteInteractorEnabled) {
            int socketsReady = SDLNet_CheckSockets(socketSet, 0);

            if (socketsReady == -1) {
                SPDLOG_ERROR("[GameInteractor] SDLNet_CheckSockets: {}", SDLNet_GetError());
                break;
            }

            if (socketsReady == 0) {
                continue;
            }

            char remoteDataReceived[512];
            memset(remoteDataReceived, 0, sizeof(remoteDataReceived));
            int len = SDLNet_TCP_Recv(remoteSocket, remoteDataReceived, sizeof(remoteDataReceived));
            if (len <= 0) {
                // Either an error occurred or the connection was closed
                SPDLOG_ERROR("[GameInteractor] SDLNet_TCP_Recv: {}", SDLNet_GetError());
                break;
            }

            receivedData.append(remoteDataReceived, len);

            // Proess all complete packets
            size_t delimiterPos = receivedData.find("\n");
            while (delimiterPos != std::string::npos) {
                // Extract the complete packet until the delimiter
                std::string packet = receivedData.substr(0, delimiterPos);
                // Remove the packet (including the delimiter) from the received data
                receivedData.erase(0, delimiterPos + 1);
                HandleRemoteMessage(packet);
                // Find the next delimiter
                delimiterPos = receivedData.find("\n");
            }
        }

        if (isRemoteInteractorConnected) {
            SDLNet_TCP_Close(remoteSocket);
            isRemoteInteractorConnected = false;
            SPDLOG_INFO("[GameInteractor] Ending receiving thread...");
        }
    }
}

// making it available as it's defined below
GameInteractionEffectBase* EffectFromJson(std::string name, nlohmann::json payload);

void from_json(const json& j, Vec3f& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
}

void from_json(const json& j, Vec3s& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
}

void from_json(const json& j, PosRot& posRot) {
    j.at("pos").get_to(posRot.pos);
    j.at("rot").get_to(posRot.rot);
}

void from_json(const json& j, Cylinder16& cylinder) {
    j.at("radius").get_to(cylinder.radius);
    j.at("height").get_to(cylinder.height);
    j.at("yShift").get_to(cylinder.yShift);
    j.at("pos").get_to(cylinder.pos);
}

void from_json(const json& j, Color_RGB8& color) {
    j.at("r").get_to(color.r);
    j.at("g").get_to(color.g);
    j.at("b").get_to(color.b);
}

void from_json(const json& j, AnimationHeaderCommon& common) {
    j.at("frameCount").get_to(common.frameCount);
}

void GameInteractor::HandleRemoteMessage(std::string message) {
    nlohmann::json payload;
    try {
        payload = nlohmann::json::parse(message);
        if (payload["type"] != "PlayerData") {
            SPDLOG_INFO("Received payload: \n{}\n", message);
        }
    } catch (const std::exception& e) {
        SPDLOG_ERROR("[GameInteractor] Failed to parse message: \n{}\n{}\n", message, e.what());
        return;
    }

    if (remoteForwarder) {
        remoteForwarder(payload);
        return;
    }

    // { action: "apply_effect, effect: { "name: "value", "payload": { "parameter": "value" } }
    // { action: "remove_effect, effect: { "name: "value" }
    // if action contains effect then it's an effect
    if (payload["action"] == "apply_effect" || payload["action"] == "remove_effect") {
        nlohmann::json effect = payload["effect"];
        GameInteractionEffectBase* giEffect = EffectFromJson(effect["name"].get<std::string>(), effect["payload"]);
        if (giEffect) {
            if (payload["action"] == "apply_effect") {
                giEffect->Apply();
            } else if (IsType<RemovableGameInteractionEffect>(giEffect)) {
                dynamic_cast<RemovableGameInteractionEffect*>(giEffect)->Remove();
            }
        }
    }

    if (payload["type"] == "GiveItem") {
        auto effect = new GameInteractionEffect::GiveItem();
        effect->parameters[0] = payload["modId"].get<uint16_t>();
        effect->parameters[1] = payload["getItemId"].get<uint16_t>();
        effect->Apply();
    }
    if (payload["type"] == "SetSceneFlag") {
        auto effect = new GameInteractionEffect::SetSceneFlag();
        effect->parameters[0] = payload["sceneNum"].get<int16_t>();
        effect->parameters[1] = payload["flagType"].get<int16_t>();
        effect->parameters[2] = payload["flag"].get<int16_t>();
        effect->Apply();
    }
    if (payload["type"] == "SetFlag") {
        auto effect = new GameInteractionEffect::SetFlag();
        effect->parameters[0] = payload["flagType"].get<int16_t>();
        effect->parameters[1] = payload["flag"].get<int16_t>();
        effect->Apply();
    }
    if (payload["type"] == "PlayerData") {
        GameInteractor::State::CoopPlayerPositions[payload["clientId"].get<uint32_t>()] = { payload["sceneNum"].get<int16_t>(), payload["posRot"].get<PosRot>() };
        GameInteractor::State::CoopPlayerShapeRotations[payload["clientId"].get<uint32_t>()] = payload["rot"].get<Vec3s>();
        GameInteractor::State::CoopPlayerAges[payload["clientId"].get<uint32_t>()] = payload["linkAge"].get<int32_t>();
        GameInteractor::State::CoopPlayerCylinders[payload["clientId"].get<uint32_t>()] = payload["cylinder"].get<Cylinder16>();
        GameInteractor::State::CoopPlayerColors[payload["clientId"].get<uint32_t>()] = payload["color"].get<Color_RGB8>();
        GameInteractor::State::CoopPlayerStateFlags2[payload["clientId"].get<uint32_t>()] = payload["stateFlags2"].get<uint32_t>();
        GameInteractor::State::CoopPlayerMoveFlags[payload["clientId"].get<uint32_t>()] = payload["moveFlags"].get<uint8_t>();
        GameInteractor::State::CoopPlayerPitchOffset[payload["clientId"].get<uint32_t>()] = payload["shapePitchOffset"].get<int16_t>();
        GameInteractor::State::CoopPlayerOffsetY[payload["clientId"].get<uint32_t>()] = payload["shapeOffsetY"].get<float>();
        GameInteractor::State::CoopPlayerLimbs[payload["clientId"].get<uint32_t>()] = payload["jointTable"].get<std::vector<Vec3s>>();
        GameInteractor::State::CoopPlayerLimbsUpper[payload["clientId"].get<uint32_t>()] = payload["jointTableUpper"].get<std::vector<Vec3s>>();
    }
    if (payload["type"] == "PlayerAnim") {
        GameInteractor::State::CoopPlayerAnim[payload["clientId"].get<uint32_t>()] = payload["anim"].get<std::string>();
        GameInteractor::State::CoopPlayerPlaySpeeds[payload["clientId"].get<uint32_t>()] = payload["playSpeed"].get<float>();
        GameInteractor::State::CoopPlayerStartFrames[payload["clientId"].get<uint32_t>()] = payload["startFrame"].get<float>();
        GameInteractor::State::CoopPlayerCurFrames[payload["clientId"].get<uint32_t>()] = payload["curFrame"].get<float>();
        GameInteractor::State::CoopPlayerEndFrames[payload["clientId"].get<uint32_t>()] = payload["endFrame"].get<float>();
        GameInteractor::State::CoopPlayerModes[payload["clientId"].get<uint32_t>()] = payload["mode"].get<uint8_t>();
    }
    if (payload["type"] == "PlayerAnimUpper") {
        GameInteractor::State::CoopPlayerAnimUpper[payload["clientId"].get<uint32_t>()] = payload["animUpper"].get<std::string>();
        GameInteractor::State::CoopPlayerPlaySpeeds2[payload["clientId"].get<uint32_t>()] = payload["playSpeed2"].get<float>();
        GameInteractor::State::CoopPlayerStartFrames2[payload["clientId"].get<uint32_t>()] = payload["startFrame2"].get<float>();
        GameInteractor::State::CoopPlayerCurFrames2[payload["clientId"].get<uint32_t>()] = payload["curFrame2"].get<float>();
        GameInteractor::State::CoopPlayerEndFrames2[payload["clientId"].get<uint32_t>()] = payload["endFrame2"].get<float>();
        GameInteractor::State::CoopPlayerModes2[payload["clientId"].get<uint32_t>()] = payload["mode2"].get<uint8_t>();
    } 
    if (payload["type"] == "UnsetSceneFlag") {
        auto effect = new GameInteractionEffect::UnsetSceneFlag();
        effect->parameters[0] = payload["sceneNum"].get<int16_t>();
        effect->parameters[1] = payload["flagType"].get<int16_t>();
        effect->parameters[2] = payload["flag"].get<int16_t>();
        effect->Apply();
    }
    if (payload["type"] == "UnsetFlag") {
        auto effect = new GameInteractionEffect::UnsetFlag();
        effect->parameters[0] = payload["flagType"].get<int16_t>();
        effect->parameters[1] = payload["flag"].get<int16_t>();
        effect->Apply();
    }
    if (payload["type"] == "PushSaveState" && IsSaveLoaded()) {
        ParseSaveStateFromRemote(payload);
    }
    if (payload["type"] == "RequestSaveState" && IsSaveLoaded()) {
        PushSaveStateToRemote();
    }
    if (payload["type"] == "RoomClientIds") {
        GameInteractor::State::CoopPlayerIds = payload["clientIds"].get<std::vector<uint32_t>>();
        if (IsSaveLoaded()) {
            //GameInteractor_SpawnCoopFairies();
            GameInteractor_SpawnCoopDummyPlayers();
        }
    }
}

// MARK: - Effect Helpers

GameInteractionEffectBase* EffectFromJson(std::string name, nlohmann::json payload) {
    if (nameToEnum.find(name) == nameToEnum.end()) {
        return nullptr;
    }

    switch (std::get<0>(nameToEnum[name])) {
        case GameInteractionEffect::Values::modifyHeartContainers: {
            auto effect = new GameInteractionEffect::ModifyHeartContainers();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::fillMagic:
            return new GameInteractionEffect::FillMagic();
        case GameInteractionEffect::Values::emptyMagic:
            return new GameInteractionEffect::EmptyMagic();
        case GameInteractionEffect::Values::modifyRupees: {
            auto effect = new GameInteractionEffect::ModifyRupees();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::noUI:
            return new GameInteractionEffect::NoUI();
        case GameInteractionEffect::Values::modifyGravity: {
            auto effect = new GameInteractionEffect::ModifyGravity();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::modifyHealth: {
            auto effect = new GameInteractionEffect::ModifyHealth();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::setPlayerHealth: {
            auto effect = new GameInteractionEffect::SetPlayerHealth();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::freezePlayer:
            return new GameInteractionEffect::FreezePlayer();
        case GameInteractionEffect::Values::burnPlayer:
            return new GameInteractionEffect::BurnPlayer();
        case GameInteractionEffect::Values::electrocutePlayer:
            return new GameInteractionEffect::ElectrocutePlayer();
        case GameInteractionEffect::Values::knockbackPlayer: {
            auto effect = new GameInteractionEffect::KnockbackPlayer();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::modifyLinkSize: {
            auto effect = new GameInteractionEffect::ModifyLinkSize();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::invisibleLink:
            return new GameInteractionEffect::InvisibleLink();
        case GameInteractionEffect::Values::pacifistMode:
            return new GameInteractionEffect::PacifistMode();
        case GameInteractionEffect::Values::disableZTargeting:
            return new GameInteractionEffect::DisableZTargeting();
        case GameInteractionEffect::Values::weatherRainstorm:
            return new GameInteractionEffect::WeatherRainstorm();
        case GameInteractionEffect::Values::reverseControls:
            return new GameInteractionEffect::ReverseControls();
        case GameInteractionEffect::Values::forceEquipBoots: {
            auto effect = new GameInteractionEffect::ForceEquipBoots();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::modifyRunSpeedModifier: {
            auto effect = new GameInteractionEffect::ModifyRunSpeedModifier();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::oneHitKO:
            return new GameInteractionEffect::OneHitKO();
        case GameInteractionEffect::Values::modifyDefenseModifier: {
            auto effect = new GameInteractionEffect::ModifyDefenseModifier();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        case GameInteractionEffect::Values::giveOrTakeShield: {
            auto effect = new GameInteractionEffect::GiveOrTakeShield();
            effect->parameters[0] = payload["parameter"].get<int32_t>();
            return effect;
        }
        default:
            return nullptr;
    }
}

#endif