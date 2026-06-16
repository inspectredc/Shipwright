#include "item_priority.h"
#include "soh/Enhancements/randomizer/SeedContext.h"

namespace Rando {

ItemPriority GetItemPriority(RandomizerGet item) {
    auto ctx = Rando::Context::GetInstance();

    // Check if the overall item progression is enabled
    if (ctx->GetOption(RSK_KEY_ITEM_PRIORITY).Is(RO_KEY_PRIORITY_OFF)) {
        return ItemPriority::Default;
    }

    RandomizerSettingKey itemRsk = RSK_NONE;

    switch (item) {
        case RG_PROGRESSIVE_SLINGSHOT:
            itemRsk = RSK_KEY_PRIORITY_SLINGSHOT;
            break;
        case RG_PROGRESSIVE_BOMB_BAG:
            itemRsk = RSK_KEY_PRIORITY_BOMB_BAG;
            break;
        // case RG_BOMBCHU_5:
        // case RG_BOMBCHU_10:
        // case RG_BOMBCHU_20:
        case RG_PROGRESSIVE_BOMBCHU_BAG:
            itemRsk = RSK_KEY_PRIORITY_BOMBCHUS;
            break;
        case RG_BOOMERANG:
            itemRsk = RSK_KEY_PRIORITY_BOOMERANG;
            break;
        case RG_PROGRESSIVE_BOW:
            itemRsk = RSK_KEY_PRIORITY_BOW;
            break;
        case RG_PROGRESSIVE_HOOKSHOT:
            itemRsk = RSK_KEY_PRIORITY_HOOKSHOT;
            break;
        case RG_LENS_OF_TRUTH:
            itemRsk = RSK_KEY_PRIORITY_LENS_OF_TRUTH;
            break;
        case RG_FAIRY_OCARINA:
        case RG_OCARINA_OF_TIME:
            itemRsk = RSK_KEY_PRIORITY_OCARINA;
            break;
        case RG_MEGATON_HAMMER:
            itemRsk = RSK_KEY_PRIORITY_HAMMER;
            break;
        case RG_FIRE_ARROWS:
            itemRsk = RSK_KEY_PRIORITY_FIRE_ARROWS;
            break;
        case RG_ICE_ARROWS:
            itemRsk = RSK_KEY_PRIORITY_ICE_ARROWS;
            break;
        case RG_LIGHT_ARROWS:
            itemRsk = RSK_KEY_PRIORITY_LIGHT_ARROWS;
            break;
        case RG_DINS_FIRE:
            itemRsk = RSK_KEY_PRIORITY_DINS_FIRE;
            break;
        case RG_FARORES_WIND:
            itemRsk = RSK_KEY_PRIORITY_FARORES_WIND;
            break;
        case RG_NAYRUS_LOVE:
            itemRsk = RSK_KEY_PRIORITY_NAYRUS_LOVE;
            break;
        case RG_IRON_BOOTS:
            itemRsk = RSK_KEY_PRIORITY_IRON_BOOTS;
            break;
        case RG_HOVER_BOOTS:
            itemRsk = RSK_KEY_PRIORITY_HOVER_BOOTS;
            break;
        case RG_PROGRESSIVE_MAGIC_METER:
            itemRsk = RSK_KEY_PRIORITY_MAGIC_METER;
            break;
        case RG_MIRROR_SHIELD:
            itemRsk = RSK_KEY_PRIORITY_MIRROR_SHIELD;
            break;
        case RG_GORON_TUNIC:
            itemRsk = RSK_KEY_PRIORITY_GORON_TUNIC;
            break;
        case RG_ZORA_TUNIC:
            itemRsk = RSK_KEY_PRIORITY_ZORA_TUNIC;
            break;
        case RG_PROGRESSIVE_STRENGTH:
            itemRsk = RSK_KEY_PRIORITY_STRENGTH_UPGRADES;
            break;
        case RG_PROGRESSIVE_SCALE:
            itemRsk = RSK_KEY_PRIORITY_SCALE_UPGRADES;
            break;
        default:
            break;
    }

    if (ctx->GetOption(RSK_KEY_ITEM_PRIORITY).Is(RO_KEY_PRIORITY_CUSTOM)) {
        if (itemRsk != RSK_NONE) {
            uint8_t customPriority = ctx->GetOption(itemRsk).Get();
            if (customPriority == RO_ITEM_PRIORITY_EARLY)
                return ItemPriority::Early;
            if (customPriority == RO_ITEM_PRIORITY_LATE)
                return ItemPriority::Late;
        }
        // If it's a song, we default to early if in custom mode (or default)?
        // Let's just leave it default unless specifically overridden.
    } else if (ctx->GetOption(RSK_KEY_ITEM_PRIORITY).Is(RO_KEY_PRIORITY_TRICKY)) {
        // Tricky defaults
        switch (item) {
            // Early
            case RG_PROGRESSIVE_SLINGSHOT:
            case RG_BOOMERANG:
            case RG_PROGRESSIVE_BOW:
            case RG_PROGRESSIVE_HOOKSHOT:
            case RG_LENS_OF_TRUTH:
            case RG_FAIRY_OCARINA:
            case RG_OCARINA_OF_TIME:
            // Songs
            case RG_ZELDAS_LULLABY:
            case RG_EPONAS_SONG:
            case RG_SARIAS_SONG:
            case RG_SUNS_SONG:
            case RG_SONG_OF_TIME:
            case RG_SONG_OF_STORMS:
            case RG_MINUET_OF_FOREST:
            case RG_BOLERO_OF_FIRE:
            case RG_SERENADE_OF_WATER:
            case RG_REQUIEM_OF_SPIRIT:
            case RG_NOCTURNE_OF_SHADOW:
            case RG_PRELUDE_OF_LIGHT:
                return ItemPriority::Early;

            // Late (incorporating user feedback for Bomb Bag/Bombchus)
            case RG_PROGRESSIVE_BOMB_BAG:
            // case RG_BOMBCHU_5:
            // case RG_BOMBCHU_10:
            // case RG_BOMBCHU_20:
            case RG_PROGRESSIVE_BOMBCHU_BAG:
            case RG_MIRROR_SHIELD:
            case RG_GORON_TUNIC:
            case RG_ZORA_TUNIC:
            case RG_PROGRESSIVE_STRENGTH:
                return ItemPriority::Late;

            // Default
            default:
                break;
        }
    }

    return ItemPriority::Default;
}

} // namespace Rando
