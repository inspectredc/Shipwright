#ifdef ENABLE_REMOTE_CONTROL
#ifndef Z_PROP_PUPPET_H
#define Z_PROP_PUPPET_H

#include <libultraship/libultra.h>
#include "soh/Enhancements/nametag.h"
#include "global.h"

/* 
    This Actor acts as a wrapper around the Link Puppet while also giving access to a set of prop drawing and control
 */

struct PropPuppet;

typedef enum PropType {
    /* 0x00 */ PROP_LINK,
    /* 0x01 */ PROP_STALFOS,
    /* 0x02 */ PROP_DOOR,
    /* 0x03 */ PROP_CHEST,
    /* 0x04 */ PROP_FIREWALL,
    /* 0x05 */ PROP_POE,
    /* 0x06 */ PROP_OCTOROK,
    /* 0x07 */ PROP_WEBS,
    /* 0x08 */ PROP_BOMB,
    /* 0x09 */ PROP_WALLMASTER,
    /* 0x0A */ PROP_DODONGO,
    /* 0x0B */ PROP_KEESE,
    /* 0x0C */ PROP_HORSE,
    /* 0x0D */ PROP_RUPEE_GREEN,
    /* 0x0E */ PROP_RUPEE_BLUE,
    /* 0x0F */ PROP_RUPEE_RED,
    /* 0x10 */ PROP_RUPEE_PURPLE,
    /* 0x11 */ PROP_RUPEE_PINK,
    /* 0x12 */ PROP_RUPEE_ORANGE,
    /* 0x13 */ PROP_HEART,
    /* 0x14 */ PROP_FAIRY,
    /* 0x15 */ PROP_CUCCO,
    /* 0x16 */ PROP_TEKTITE,
    /* 0x17 */ PROP_LEEVER,
    /* 0x18 */ PROP_PEEHAT,
    /* 0x19 */ PROP_PEEHAT_LARVA,
    /* 0x1A */ PROP_LIZALFOS,
    /* 0x1B */ PROP_DINALFOS,
    /* 0x1C */ PROP_FLAGPOLE,
    /* 0x1D */ PROP_GOMA_LARVA,
    /* 0x1E */ PROP_PUSH_CUBE,
    /* 0x1F */ PROP_SHABOM,
    /* 0x20 */ PROP_DODONGO_BABY,
    /* 0x21 */ PROP_DARK_LINK,
    /* 0x22 */ PROP_BIRI,
    /* 0x23 */ PROP_TAILPASARAN,
    /* 0x24 */ PROP_SKULLTULA,
    /* 0x25 */ PROP_SKULLTULA_SMALL,
    /* 0x26 */ PROP_SKULLTULA_LARGE,
    /* 0x27 */ PROP_SKULLTULA_GOLD,
    /* 0x28 */ PROP_WALLTULA,
    /* 0x29 */ PROP_TORCH_SLUG,
    /* 0x2A */ PROP_BLOCK_SMALL,
    /* 0x2B */ PROP_BLOCK_LARGE,
    /* 0x2C */ PROP_BLOCK_SMALL_ROT,
    /* 0x2D */ PROP_BLOCK_LARGE_ROT,
    /* 0x2E */ PROP_GRASS_CLUMP,
    /* 0x2F */ PROP_TREE_STUMP,
    /* 0x30 */ PROP_SIGNPOST_OBLONG,
    /* 0x31 */ PROP_SIGNPOST_ARROW,
    /* 0x32 */ PROP_BOULDER_FRAGMENT,
    /* 0x33 */ PROP_STINGER,
    /* 0x34 */ PROP_SHOPKEEPER,
    /* 0x35 */ PROP_HAMMER_STATUE,
    /* 0x36 */ PROP_STONE_BLOCK,
    /* 0x37 */ PROP_FLAMETHROWER_SPINNING,
    /* 0x38 */ PROP_FLAMETHROWER_STATIONARY,
    /* 0x39 */ PROP_SHEIK,
    /* 0x3A */ PROP_FLAME_CIRCLE,
    /* 0x3B */ PROP_MOBLINS,
    /* 0x3C */ PROP_BOMBFLOWER,
    /* 0x3D */ PROP_SPIKED_LOG,
    /* 0x3E */ PROP_ARMOS,
    /* 0x3F */ PROP_DEKU_BABA,
    /* 0x40 */ PROP_BOMBABLE_WALL,
    /* 0x41 */ PROP_TORCH,
    /* 0x42 */ PROP_HEART_CONTAINER,
    /* 0x43 */ PROP_MAD_SCRUB,
    /* 0x44 */ PROP_BARI,
    /* 0x45 */ PROP_BUBBLE,
    /* 0x46 */ PROP_MASTER_SWORD,
    /* TODO: CONTINUE FROM ACTOR LIST */
} PropType;

typedef struct PropPuppet {
    LinkPuppet link;
    PropType propId;
    SkelAnime skelAnime;
    SkelAnime upperSkelanime;
} PropPuppet;

#ifdef __cplusplus
extern "C" {
#endif
void PropPuppet_Init(Actor* thisx, PlayState* play);
void PropPuppet_Destroy(Actor* thisx, PlayState* play);
void PropPuppet_Update(Actor* thisx, PlayState* play);
void PropPuppet_Draw(Actor* thisx, PlayState* play);
#ifdef __cplusplus
}
#endif

#endif
#endif
