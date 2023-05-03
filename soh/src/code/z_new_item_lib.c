#include "global.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"
#include "overlays/actors/ovl_Demo_Effect/z_demo_effect.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"

#include "soh/Enhancements/game-interactor/GameInteractor.h"

//Fill in as more new bottled items
static bool sCanSellNewBottle[] = {
    false,          //PLAYER_IA_BOTTLE_START
    false           //PLAYER_IA_BOTTLE_END
};

static bool sCanDrinkNewBottle[] = {
    false,          //PLAYER_IA_BOTTLE_START
    false           //PLAYER_IA_BOTTLE_END
};

static s32 sNewBottleExchangeItem[] = { //Fill with exchange item ids, -1 if not an exchange item
    -1,             //PLAYER_IA_BOTTLE_START
    -1,             //PLAYER_IA_BOTTLE_END
};

static s32 sNewTradeExchangeItem[] = { //Fill with exchange item ids, -1 if not an exchange item
    -1,             //PLAYER_IA_TI_START
    -1,             //PLAYER_IA_TI_END
};

static u8 sMagicArrowCosts[] = {
    4,
    4,
    8,
};

static u8 sNewMagicArrowCosts[] = {
    4,              //PLAYER_IA_BOW_START
    4,              //PLAYER_IA_BOW_BOMB
    4,              //PLAYER_IA_BOW_END
};

static u32 sNewArrowDmgFlags[] = {
    0x00000020,     //PLAYER_IA_BOW_START
    0x00000020,     //PLAYER_IA_BOW_BOMB
    0x00000020,     //PLAYER_IA_BOW_END
};

static u8 sBottleDrinkEffects[] = {
    0x01, 0x03, 0x02, 0x04, 0x04,
};

static u8 sNewBottleDrinkEffects[] = { //drink effects are 1 -> strong heal, 2 -> fill magic, 4 -> weak heal. effects can be combined through addition
    0x00,           //PLAYER_IA_BOTTLE_START
    0x00,           //PLAYER_IA_BOTTLE_END
};

static s16 sNewItemActionParams[] = {
    PLAYER_IA_NONE,                               // NEW_ITEM_1
    PLAYER_IA_NONE,                               // NEW_ITEM_2
    PLAYER_IA_NONE,                               // NEW_ITEM_3
    PLAYER_IA_BOW_BOMB,                           // NEW_ITEM_4
    PLAYER_IA_NONE,                               // NEW_ITEM_5
    PLAYER_IA_NONE,                               // NEW_ITEM_6
    PLAYER_IA_NONE,                               // NEW_ITEM_7
    PLAYER_IA_NONE,                               // NEW_ITEM_8
    PLAYER_IA_NONE,                               // NEW_ITEM_9
    PLAYER_IA_NONE,                               // NEW_ITEM_10
    PLAYER_IA_NONE,                               // NEW_ITEM_11
    PLAYER_IA_NONE,                               // NEW_ITEM_12
    PLAYER_IA_NONE,                               // NEW_ITEM_13
    PLAYER_IA_NONE,                               // NEW_ITEM_14
    PLAYER_IA_NONE,                               // NEW_ITEM_15
    PLAYER_IA_NONE,                               // NEW_ITEM_16
    PLAYER_IA_NONE,                               // NEW_ITEM_17
    PLAYER_IA_NONE,                               // NEW_ITEM_18
    PLAYER_IA_NONE,                               // NEW_ITEM_19
    PLAYER_IA_NONE,                               // NEW_ITEM_20
    PLAYER_IA_NONE,                               // NEW_ITEM_21
    PLAYER_IA_NONE,                               // NEW_ITEM_22
    PLAYER_IA_NONE,                               // NEW_ITEM_23
    PLAYER_IA_NONE,                               // NEW_ITEM_24
};

bool NewItem_IsActionParamBow(s16 actionParam) {
    return ((actionParam >= PLAYER_IA_BOW && actionParam <= PLAYER_IA_BOW_0E) || (actionParam >= PLAYER_IA_BOW_START && actionParam <= PLAYER_IA_BOW_END));
}

bool NewItem_IsActionParamBoots(s16 actionParam) { //for potential future boots
    return (actionParam >= PLAYER_IA_BOOTS_KOKIRI && actionParam <= PLAYER_IA_BOOTS_HOVER);
}

bool NewItem_IsActionParamTunic(s16 actionParam) { //for potential future tunics
    return (actionParam >= PLAYER_IA_TUNIC_KOKIRI && actionParam <= PLAYER_IA_TUNIC_GORON);
}

bool NewItem_IsActionParamShield(s16 actionParam) { //for potential future shields
    return (actionParam >= PLAYER_IA_SHIELD_DEKU && actionParam <= PLAYER_IA_SHIELD_MIRROR);
}

bool NewItem_IsActionParamEquipment(s16 actionParam) {
    return NewItem_IsActionParamBoots(actionParam) || NewItem_IsActionParamTunic(actionParam) || NewItem_IsActionParamShield(actionParam);
}

bool NewItem_IsActionParamMagicSpell(s16 actionParam) {
    return ((actionParam >= PLAYER_IA_MAGIC_SPELL_15 && actionParam <= PLAYER_IA_DINS_FIRE) || (actionParam >= PLAYER_IA_MAGIC_SPELL_START && actionParam <= PLAYER_IA_MAGIC_SPELL_END));
}

bool NewItem_IsActionParamOcarina(s16 actionParam) {
    return (actionParam == PLAYER_IA_OCARINA_FAIRY || actionParam == PLAYER_IA_OCARINA_TIME || (actionParam >= PLAYER_IA_OCARINA_START && actionParam <= PLAYER_IA_OCARINA_END));
}

bool NewItem_IsActionParamBottledItem(s16 actionParam) {
    return ((actionParam >= PLAYER_IA_BOTTLE_FISH && actionParam <= PLAYER_IA_BOTTLE_FAIRY) || (actionParam >= PLAYER_IA_BOTTLE_START && actionParam <= PLAYER_IA_BOTTLE_END));
}

bool NewItem_IsActionParamBottledSellable(s16 actionParam) {
    return ((actionParam >= PLAYER_IA_BOTTLE_FISH && actionParam <= PLAYER_IA_BOTTLE_LETTER) || sCanSellNewBottle[actionParam - PLAYER_IA_BOTTLE_START]);
}

bool NewItem_IsActionParamBottledConsumable(s16 actionParam) {
    return ((actionParam >= PLAYER_IA_BOTTLE_POTION_RED && actionParam <= PLAYER_IA_BOTTLE_FAIRY) || sCanDrinkNewBottle[actionParam - PLAYER_IA_BOTTLE_START]);
}

bool NewItem_IsActionParamBottledExchange(s16 actionParam) {
    if (actionParam >= PLAYER_IA_BOTTLE_START && actionParam <= PLAYER_IA_BOTTLE_END) {
        return sNewBottleExchangeItem[actionParam - PLAYER_IA_BOTTLE_START] >= 0;
    } else {
        return actionParam == PLAYER_IA_BOTTLE_BIG_POE || actionParam == PLAYER_IA_BOTTLE_LETTER;
    }
}

bool NewItem_IsActionParamTradeItem(s16 actionParam) {
    return ((actionParam >= PLAYER_IA_LETTER_ZELDA && actionParam <= PLAYER_IA_CLAIM_CHECK) || (actionParam >= PLAYER_IA_TI_START && actionParam <= PLAYER_IA_TI_END));
}

bool NewItem_IsActionParamMask(s16 actionParam) {
    return ((actionParam >= PLAYER_IA_MASK_KEATON && actionParam <= PLAYER_IA_MASK_TRUTH) || (actionParam >= PLAYER_IA_MASK_START && actionParam <= PLAYER_IA_MASK_END));
}



s32 NewItem_ActionParamToArrowType(s16 actionParam) {
    if (actionParam >= PLAYER_IA_BOW && actionParam <= PLAYER_IA_BOW_0E) {
        return ARROW_NORMAL + (actionParam - PLAYER_IA_BOW);
    } else {
        return ARROW_START + (actionParam - PLAYER_IA_BOW_START);
    }
}

u16 NewItem_ActionParamToBoots(s16 actionParam) { //todo
    return actionParam - PLAYER_IA_BOOTS_KOKIRI + 1;
}

u16 NewItem_ActionParamToTunic(s16 actionParam) { //todo
    return actionParam - PLAYER_IA_TUNIC_KOKIRI + 1;
}

u8 NewItem_ActionParamToMask(s16 actionParam) { // 8 vanilla masks
    if (actionParam >= PLAYER_IA_MASK_KEATON && actionParam <= PLAYER_IA_MASK_TRUTH) {
        return actionParam - PLAYER_IA_MASK_KEATON + 1;
    } else { //assume mask start not selectable for now (or it will default to Mask of Truth)
        return actionParam - PLAYER_IA_MASK_START + 8;
    } 
}

u8 NewItem_GetMagicArrowCost(s16 arrowType) {
    if (arrowType >= ARROW_FIRE && arrowType <= ARROW_LIGHT) {
        return sMagicArrowCosts[arrowType - ARROW_FIRE];
    } else if (arrowType >= ARROW_START && arrowType <= ARROW_END) {
        return sNewMagicArrowCosts[arrowType - ARROW_START];
    } else { //shouldnt be reached
        return 0;
    }
}

void *NewItem_GetBlureArrow(s16 arrowType) { //todo add to cosmetics
    static EffectBlureInit2 blureNormal = {
        0, 4, 0, { 0, 255, 200, 255 },   { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 255, 170, 255 }, { 0, 150, 0, 0 }, 0,
    };
    static EffectBlureInit2 blureBomb = {
        0, 4, 0, { 0, 255, 200, 255 }, { 0, 255, 255, 255 }, { 0, 255, 200, 0 }, { 0, 255, 255, 0 }, 16,
        0, 1, 0, { 255, 20, 20, 255 }, { 20, 20, 20, 0 }, 0,
    };

    switch(arrowType) {
        case ARROW_BOMB:
            return &blureBomb;
        default:
            return &blureNormal;
    }

}

s16 NewItem_GetArrowActorId(s16 arrowType) {
    switch (arrowType) {
        case ARROW_BOMB:
            return ACTOR_ARROW_BOMB;
        default:
            return ACTOR_ARROW_FIRE;
    }
}

u32 NewItem_GetArrowDamageFlag(s16 arrowType) {
    return sNewArrowDmgFlags[arrowType - ARROW_START];
}

s32 NewItem_GetCutsceneItem(s16 actionParam) { // 15 vanilla cutscene items todo:figure out bean
    if (actionParam >= PLAYER_IA_LETTER_ZELDA && actionParam <= PLAYER_IA_CLAIM_CHECK) {
        return actionParam - PLAYER_IA_LETTER_ZELDA;
    } else if (actionParam >= PLAYER_IA_TI_START && actionParam <= PLAYER_IA_TI_END) {
        return actionParam - PLAYER_IA_TI_START + 15;
    } else {
        return -1;
    }
}

s32 NewItem_GetExchangeItem(s32 tradeItem, s32 bottle) { 
    // todo figure out way to allow for easy adding of new exchange items 
    s32 exchangeItem = tradeItem;
    if (tradeItem >= 0 && tradeItem < 15) {
        exchangeItem = tradeItem + 1;
    } else if (bottle >= 0 && bottle < 13) {
        exchangeItem = bottle + EXCH_ITEM_FISH;
    } else { //new exchange item behaviour
        if (sNewTradeExchangeItem[tradeItem - PLAYER_IA_TI_START] >= 0) {
            exchangeItem = sNewTradeExchangeItem[tradeItem - PLAYER_IA_TI_START];
        } else if (sNewBottleExchangeItem[tradeItem - PLAYER_IA_TI_START] >= 0) {
            exchangeItem = sNewBottleExchangeItem[bottle - PLAYER_IA_TI_START];
        } else { //shouldnt happen
            exchangeItem = bottle + EXCH_ITEM_FISH;
        }
    }
    return exchangeItem;
}

s32 NewItem_GetBottleDrinkEffect(s16 actionParam) {
    if (actionParam >= PLAYER_IA_BOTTLE_POTION_RED && actionParam <= PLAYER_IA_BOTTLE_MILK_HALF) {
        return sBottleDrinkEffects[actionParam - PLAYER_IA_BOTTLE_POTION_RED];
    } else {
        return sNewBottleDrinkEffects[actionParam - PLAYER_IA_BOTTLE_START];
    }
}

s16 NewItem_ItemToActionParam(s32 newItem) {
    return sNewItemActionParams[newItem];
}

//functions todo:
/*
NewItem_GetBottleDropInfo
*/