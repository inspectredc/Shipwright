#ifdef ENABLE_REMOTE_CONTROL

#include "vt.h"
#include "z_link_puppet.h"
#include "z_prop_puppet.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Anchor.h"
#include <string.h>

#include "overlays/actors/ovl_En_Test/z_en_test.h"

#include <objects/gameplay_dangeon_keep/gameplay_dangeon_keep.h>
#include <objects/object_gi_boomerang/object_gi_boomerang.h>
#include <objects/object_horse_link_child/object_horse_link_child.h>
#include <objects/object_oF1s/object_oF1s.h>
#include <objects/gameplay_field_keep/gameplay_field_keep.h>
#include <objects/object_gi_boots_2/object_gi_boots_2.h>
#include <objects/object_horse_normal/object_horse_normal.h>
#include <objects/object_o_anime/object_o_anime.h>
#include <objects/gameplay_keep/gameplay_keep.h>
#include <objects/object_gi_bosskey/object_gi_bosskey.h>
#include <objects/object_horse_zelda/object_horse_zelda.h>
#include <objects/object_okuta/object_okuta.h>
#include <objects/object_Bb/object_Bb.h>
#include <objects/object_gi_bottle/object_gi_bottle.h>
#include <objects/object_hs/object_hs.h>
#include <objects/object_opening_demo1/object_opening_demo1.h>
#include <objects/object_ahg/object_ahg.h>
#include <objects/object_gi_bottle_letter/object_gi_bottle_letter.h>
#include <objects/object_human/object_human.h>
#include <objects/object_os/object_os.h>
#include <objects/object_am/object_am.h>
#include <objects/object_gi_bow/object_gi_bow.h>
#include <objects/object_ice_objects/object_ice_objects.h>
#include <objects/object_os_anime/object_os_anime.h>
#include <objects/object_ane/object_ane.h>
#include <objects/object_gi_bracelet/object_gi_bracelet.h>
#include <objects/object_ik/object_ik.h>
#include <objects/object_ossan/object_ossan.h>
#include <objects/object_ani/object_ani.h>
#include <objects/object_gi_brokensword/object_gi_brokensword.h>
#include <objects/object_im/object_im.h>
#include <objects/object_ouke_haka/object_ouke_haka.h>
#include <objects/object_anubice/object_anubice.h>
#include <objects/object_gi_butterfly/object_gi_butterfly.h>
#include <objects/object_in/object_in.h>
#include <objects/object_owl/object_owl.h>
#include <objects/object_aob/object_aob.h>
#include <objects/object_gi_clothes/object_gi_clothes.h>
#include <objects/object_ingate/object_ingate.h>
#include <objects/object_peehat/object_peehat.h>
#include <objects/object_b_heart/object_b_heart.h>
#include <objects/object_gi_coin/object_gi_coin.h>
#include <objects/object_jj/object_jj.h>
#include <objects/object_po_composer/object_po_composer.h>
#include <objects/object_bba/object_bba.h>
#include <objects/object_gi_compass/object_gi_compass.h>
#include <objects/object_js/object_js.h>
#include <objects/object_po_field/object_po_field.h>
#include <objects/object_bdan_objects/object_bdan_objects.h>
#include <objects/object_gi_dekupouch/object_gi_dekupouch.h>
#include <objects/object_jya_door/object_jya_door.h>
#include <objects/object_po_sisters/object_po_sisters.h>
#include <objects/object_bdoor/object_bdoor.h>
#include <objects/object_gi_egg/object_gi_egg.h>
#include <objects/object_jya_iron/object_jya_iron.h>
#include <objects/object_poh/object_poh.h>
#include <objects/object_bg/object_bg.h>
#include <objects/object_gi_eye_lotion/object_gi_eye_lotion.h>
#include <objects/object_jya_obj/object_jya_obj.h>
#include <objects/object_ps/object_ps.h>
#include <objects/object_bigokuta/object_bigokuta.h>
#include <objects/object_gi_fire/object_gi_fire.h>
#include <objects/object_ka/object_ka.h>
#include <objects/object_pu_box/object_pu_box.h>
#include <objects/object_bird/object_bird.h>
#include <objects/object_gi_fish/object_gi_fish.h>
#include <objects/object_kanban/object_kanban.h>
#include <objects/object_rd/object_rd.h>
#include <objects/object_bji/object_bji.h>
#include <objects/object_gi_frog/object_gi_frog.h>
#include <objects/object_kibako2/object_kibako2.h>
#include <objects/object_reeba/object_reeba.h>
#include <objects/object_bl/object_bl.h>
#include <objects/object_gi_gerudo/object_gi_gerudo.h>
#include <objects/object_kingdodongo/object_kingdodongo.h>
#include <objects/object_relay_objects/object_relay_objects.h>
#include <objects/object_blkobj/object_blkobj.h>
#include <objects/object_gi_gerudomask/object_gi_gerudomask.h>
#include <objects/object_km1/object_km1.h>
#include <objects/object_rl/object_rl.h>
#include <objects/object_bob/object_bob.h>
#include <objects/object_gi_ghost/object_gi_ghost.h>
#include <objects/object_kusa/object_kusa.h>
#include <objects/object_rr/object_rr.h>
#include <objects/object_boj/object_boj.h>
#include <objects/object_gi_glasses/object_gi_glasses.h>
#include <objects/object_kw1/object_kw1.h>
#include <objects/object_rs/object_rs.h>
#include <objects/object_bombf/object_bombf.h>
#include <objects/object_gi_gloves/object_gi_gloves.h>
#include <objects/object_kz/object_kz.h>
#include <objects/object_ru1/object_ru1.h>
#include <objects/object_bombiwa/object_bombiwa.h>
#include <objects/object_gi_goddess/object_gi_goddess.h>
#include <objects/object_light_ring/object_light_ring.h>
#include <objects/object_ru2/object_ru2.h>
#include <objects/object_bowl/object_bowl.h>
#include <objects/object_gi_golonmask/object_gi_golonmask.h>
#include <objects/object_lightbox/object_lightbox.h>
#include <objects/object_sa/object_sa.h>
#include <objects/object_box/object_box.h>
#include <objects/object_gi_grass/object_gi_grass.h>
#include <objects/object_lightswitch/object_lightswitch.h>
#include <objects/object_sb/object_sb.h>
#include <objects/object_brob/object_brob.h>
#include <objects/object_gi_hammer/object_gi_hammer.h>
#include <objects/object_link_boy/object_link_boy.h>
#include <objects/object_sd/object_sd.h>
#include <objects/object_bubble/object_bubble.h>
#include <objects/object_gi_heart/object_gi_heart.h>
#include <objects/object_link_child/object_link_child.h>
#include <objects/object_shop_dungen/object_shop_dungen.h>
#include <objects/object_bv/object_bv.h>
#include <objects/object_gi_hearts/object_gi_hearts.h>
#include <objects/object_ma1/object_ma1.h>
#include <objects/object_shopnuts/object_shopnuts.h>
#include <objects/object_bw/object_bw.h>
#include <objects/object_gi_hookshot/object_gi_hookshot.h>
#include <objects/object_ma2/object_ma2.h>
#include <objects/object_siofuki/object_siofuki.h>
#include <objects/object_bwall/object_bwall.h>
#include <objects/object_gi_hoverboots/object_gi_hoverboots.h>
#include <objects/object_mag/object_mag.h>
#include <objects/object_sk2/object_sk2.h>
#include <objects/object_bxa/object_bxa.h>
#include <objects/object_gi_insect/object_gi_insect.h>
#include <objects/object_mamenoki/object_mamenoki.h>
#include <objects/object_skb/object_skb.h>
#include <objects/object_cne/object_cne.h>
#include <objects/object_gi_jewel/object_gi_jewel.h>
#include <objects/object_mastergolon/object_mastergolon.h>
#include <objects/object_skj/object_skj.h>
#include <objects/object_cob/object_cob.h>
#include <objects/object_gi_key/object_gi_key.h>
#include <objects/object_masterkokiri/object_masterkokiri.h>
#include <objects/object_spot00_break/object_spot00_break.h>
#include <objects/object_cow/object_cow.h>
#include <objects/object_gi_ki_tan_mask/object_gi_ki_tan_mask.h>
#include <objects/object_masterkokirihead/object_masterkokirihead.h>
#include <objects/object_spot00_objects/object_spot00_objects.h>
#include <objects/object_crow/object_crow.h>
#include <objects/object_gi_letter/object_gi_letter.h>
#include <objects/object_masterzoora/object_masterzoora.h>
#include <objects/object_spot01_matoya/object_spot01_matoya.h>
#include <objects/object_cs/object_cs.h>
#include <objects/object_gi_liquid/object_gi_liquid.h>
#include <objects/object_mb/object_mb.h>
#include <objects/object_spot01_matoyab/object_spot01_matoyab.h>
#include <objects/object_d_elevator/object_d_elevator.h>
#include <objects/object_gi_longsword/object_gi_longsword.h>
#include <objects/object_md/object_md.h>
#include <objects/object_spot01_objects/object_spot01_objects.h>
#include <objects/object_d_hsblock/object_d_hsblock.h>
#include <objects/object_gi_m_arrow/object_gi_m_arrow.h>
#include <objects/object_medal/object_medal.h>
#include <objects/object_spot01_objects2/object_spot01_objects2.h>
#include <objects/object_d_lift/object_d_lift.h>
#include <objects/object_gi_magicpot/object_gi_magicpot.h>
#include <objects/object_menkuri_objects/object_menkuri_objects.h>
#include <objects/object_spot02_objects/object_spot02_objects.h>
#include <objects/object_daiku/object_daiku.h>
#include <objects/object_gi_map/object_gi_map.h>
#include <objects/object_mir_ray/object_mir_ray.h>
#include <objects/object_spot03_object/object_spot03_object.h>
#include <objects/object_ddan_objects/object_ddan_objects.h>
#include <objects/object_gi_medal/object_gi_medal.h>
#include <objects/object_mizu_objects/object_mizu_objects.h>
#include <objects/object_spot04_objects/object_spot04_objects.h>
#include <objects/object_dekubaba/object_dekubaba.h>
#include <objects/object_gi_melody/object_gi_melody.h>
#include <objects/object_mjin/object_mjin.h>
#include <objects/object_spot05_objects/object_spot05_objects.h>
#include <objects/object_dekujr/object_dekujr.h>
#include <objects/object_gi_milk/object_gi_milk.h>
#include <objects/object_mjin_dark/object_mjin_dark.h>
#include <objects/object_spot06_objects/object_spot06_objects.h>
#include <objects/object_dekunuts/object_dekunuts.h>
#include <objects/object_gi_mushroom/object_gi_mushroom.h>
#include <objects/object_mjin_flame/object_mjin_flame.h>
#include <objects/object_spot07_object/object_spot07_object.h>
#include <objects/object_demo_6k/object_demo_6k.h>
#include <objects/object_gi_niwatori/object_gi_niwatori.h>
#include <objects/object_mjin_flash/object_mjin_flash.h>
#include <objects/object_spot08_obj/object_spot08_obj.h>
#include <objects/object_demo_kekkai/object_demo_kekkai.h>
#include <objects/object_gi_nuts/object_gi_nuts.h>
#include <objects/object_mjin_ice/object_mjin_ice.h>
#include <objects/object_spot09_obj/object_spot09_obj.h>
#include <objects/object_demo_tre_lgt/object_demo_tre_lgt.h>
#include <objects/object_gi_ocarina/object_gi_ocarina.h>
#include <objects/object_mjin_oka/object_mjin_oka.h>
#include <objects/object_spot11_obj/object_spot11_obj.h>
#include <objects/object_dh/object_dh.h>
#include <objects/object_gi_ocarina_0/object_gi_ocarina_0.h>
#include <objects/object_mjin_soul/object_mjin_soul.h>
#include <objects/object_spot12_obj/object_spot12_obj.h>
#include <objects/object_dnk/object_dnk.h>
#include <objects/object_gi_pachinko/object_gi_pachinko.h>
#include <objects/object_mjin_wind/object_mjin_wind.h>
#include <objects/object_spot15_obj/object_spot15_obj.h>
#include <objects/object_dns/object_dns.h>
#include <objects/object_gi_powder/object_gi_powder.h>
#include <objects/object_mk/object_mk.h>
#include <objects/object_spot16_obj/object_spot16_obj.h>
#include <objects/object_dodojr/object_dodojr.h>
#include <objects/object_gi_prescription/object_gi_prescription.h>
#include <objects/object_mm/object_mm.h>
#include <objects/object_spot17_obj/object_spot17_obj.h>
#include <objects/object_dodongo/object_dodongo.h>
#include <objects/object_gi_purse/object_gi_purse.h>
#include <objects/object_mo/object_mo.h>
#include <objects/object_spot18_obj/object_spot18_obj.h>
#include <objects/object_dog/object_dog.h>
#include <objects/object_gi_rabit_mask/object_gi_rabit_mask.h>
#include <objects/object_mori_hineri1/object_mori_hineri1.h>
#include <objects/object_ssh/object_ssh.h>
#include <objects/object_door_gerudo/object_door_gerudo.h>
#include <objects/object_gi_redead_mask/object_gi_redead_mask.h>
#include <objects/object_mori_hineri1a/object_mori_hineri1a.h>
#include <objects/object_sst/object_sst.h>
#include <objects/object_door_killer/object_door_killer.h>
#include <objects/object_gi_rupy/object_gi_rupy.h>
#include <objects/object_mori_hineri2/object_mori_hineri2.h>
#include <objects/object_st/object_st.h>
#include <objects/object_ds/object_ds.h>
#include <objects/object_gi_saw/object_gi_saw.h>
#include <objects/object_mori_hineri2a/object_mori_hineri2a.h>
#include <objects/object_stream/object_stream.h>
#include <objects/object_ds2/object_ds2.h>
#include <objects/object_gi_scale/object_gi_scale.h>
#include <objects/object_mori_objects/object_mori_objects.h>
#include <objects/object_syokudai/object_syokudai.h>
#include <objects/object_du/object_du.h>
#include <objects/object_gi_seed/object_gi_seed.h>
#include <objects/object_mori_tex/object_mori_tex.h>
#include <objects/object_ta/object_ta.h>
#include <objects/object_dy_obj/object_dy_obj.h>
#include <objects/object_gi_shield_1/object_gi_shield_1.h>
#include <objects/object_ms/object_ms.h>
#include <objects/object_timeblock/object_timeblock.h>
#include <objects/object_ec/object_ec.h>
#include <objects/object_gi_shield_2/object_gi_shield_2.h>
#include <objects/object_mu/object_mu.h>
#include <objects/object_tite/object_tite.h>
#include <objects/object_efc_crystal_light/object_efc_crystal_light.h>
#include <objects/object_gi_shield_3/object_gi_shield_3.h>
#include <objects/object_nb/object_nb.h>
#include <objects/object_tk/object_tk.h>
#include <objects/object_efc_doughnut/object_efc_doughnut.h>
#include <objects/object_gi_skj_mask/object_gi_skj_mask.h>
#include <objects/object_niw/object_niw.h>
#include <objects/object_toki_objects/object_toki_objects.h>
#include <objects/object_efc_erupc/object_efc_erupc.h>
#include <objects/object_gi_soldout/object_gi_soldout.h>
#include <objects/object_nwc/object_nwc.h>
#include <objects/object_torch2/object_torch2.h>
#include <objects/object_efc_fire_ball/object_efc_fire_ball.h>
#include <objects/object_gi_soul/object_gi_soul.h>
#include <objects/object_ny/object_ny.h>
#include <objects/object_toryo/object_toryo.h>
#include <objects/object_efc_flash/object_efc_flash.h>
#include <objects/object_gi_stick/object_gi_stick.h>
#include <objects/object_oA1/object_oA1.h>
#include <objects/object_tp/object_tp.h>
#include <objects/object_efc_lgt_shower/object_efc_lgt_shower.h>
#include <objects/object_gi_sutaru/object_gi_sutaru.h>
#include <objects/object_oA10/object_oA10.h>
#include <objects/object_tr/object_tr.h>
#include <objects/object_efc_star_field/object_efc_star_field.h>
#include <objects/object_gi_sword_1/object_gi_sword_1.h>
#include <objects/object_oA11/object_oA11.h>
#include <objects/object_trap/object_trap.h>
#include <objects/object_efc_tw/object_efc_tw.h>
#include <objects/object_gi_ticketstone/object_gi_ticketstone.h>
#include <objects/object_oA2/object_oA2.h>
#include <objects/object_triforce_spot/object_triforce_spot.h>
#include <objects/object_ei/object_ei.h>
#include <objects/object_gi_truth_mask/object_gi_truth_mask.h>
#include <objects/object_oA3/object_oA3.h>
#include <objects/object_ts/object_ts.h>
#include <objects/object_fa/object_fa.h>
#include <objects/object_gi_zoramask/object_gi_zoramask.h>
#include <objects/object_oA4/object_oA4.h>
#include <objects/object_tsubo/object_tsubo.h>
#include <objects/object_fd/object_fd.h>
#include <objects/object_gj/object_gj.h>
#include <objects/object_oA5/object_oA5.h>
#include <objects/object_tw/object_tw.h>
#include <objects/object_fd2/object_fd2.h>
#include <objects/object_gjyo_objects/object_gjyo_objects.h>
#include <objects/object_oA6/object_oA6.h>
#include <objects/object_umajump/object_umajump.h>
#include <objects/object_fhg/object_fhg.h>
#include <objects/object_gla/object_gla.h>
#include <objects/object_oA7/object_oA7.h>
#include <objects/object_vali/object_vali.h>
#include <objects/object_fire/object_fire.h>
#include <objects/object_gm/object_gm.h>
#include <objects/object_oA8/object_oA8.h>
#include <objects/object_vase/object_vase.h>
#include <objects/object_firefly/object_firefly.h>
#include <objects/object_gnd/object_gnd.h>
#include <objects/object_oA9/object_oA9.h>
#include <objects/object_vm/object_vm.h>
#include <objects/object_fish/object_fish.h>
#include <objects/object_gnd_magic/object_gnd_magic.h>
#include <objects/object_oB1/object_oB1.h>
#include <objects/object_wallmaster/object_wallmaster.h>
#include <objects/object_fr/object_fr.h>
#include <objects/object_gndd/object_gndd.h>
#include <objects/object_oB2/object_oB2.h>
#include <objects/object_warp1/object_warp1.h>
#include <objects/object_fu/object_fu.h>
#include <objects/object_god_lgt/object_god_lgt.h>
#include <objects/object_oB3/object_oB3.h>
#include <objects/object_warp2/object_warp2.h>
#include <objects/object_fw/object_fw.h>
#include <objects/object_gol/object_gol.h>
#include <objects/object_oB4/object_oB4.h>
#include <objects/object_wf/object_wf.h>
#include <objects/object_fz/object_fz.h>
#include <objects/object_goma/object_goma.h>
#include <objects/object_oE1/object_oE1.h>
#include <objects/object_wood02/object_wood02.h>
#include <objects/object_ganon/object_ganon.h>
#include <objects/object_goroiwa/object_goroiwa.h>
#include <objects/object_oE10/object_oE10.h>
#include <objects/object_xc/object_xc.h>
#include <objects/object_ganon2/object_ganon2.h>
#include <objects/object_gr/object_gr.h>
#include <objects/object_oE11/object_oE11.h>
#include <objects/object_yabusame_point/object_yabusame_point.h>
#include <objects/object_ganon_anime1/object_ganon_anime1.h>
#include <objects/object_gs/object_gs.h>
#include <objects/object_oE12/object_oE12.h>
#include <objects/object_ydan_objects/object_ydan_objects.h>
#include <objects/object_ganon_anime2/object_ganon_anime2.h>
#include <objects/object_gt/object_gt.h>
#include <objects/object_oE1s/object_oE1s.h>
#include <objects/object_yukabyun/object_yukabyun.h>
#include <objects/object_ganon_anime3/object_ganon_anime3.h>
#include <objects/object_haka/object_haka.h>
#include <objects/object_oE2/object_oE2.h>
#include <objects/object_zf/object_zf.h>
#include <objects/object_ganon_objects/object_ganon_objects.h>
#include <objects/object_haka_door/object_haka_door.h>
#include <objects/object_oE3/object_oE3.h>
#include <objects/object_zg/object_zg.h>
#include <objects/object_ge1/object_ge1.h>
#include <objects/object_haka_objects/object_haka_objects.h>
#include <objects/object_oE4/object_oE4.h>
#include <objects/object_zl1/object_zl1.h>
#include <objects/object_geff/object_geff.h>
#include <objects/object_hakach_objects/object_hakach_objects.h>
#include <objects/object_oE4s/object_oE4s.h>
#include <objects/object_zl2/object_zl2.h>
#include <objects/object_geldb/object_geldb.h>
#include <objects/object_hata/object_hata.h>
#include <objects/object_oE5/object_oE5.h>
#include <objects/object_zl2_anime1/object_zl2_anime1.h>
#include <objects/object_gi_arrow/object_gi_arrow.h>
#include <objects/object_heavy_object/object_heavy_object.h>
#include <objects/object_oE6/object_oE6.h>
#include <objects/object_zl2_anime2/object_zl2_anime2.h>
#include <objects/object_gi_arrowcase/object_gi_arrowcase.h>
#include <objects/object_hidan_objects/object_hidan_objects.h>
#include <objects/object_oE7/object_oE7.h>
#include <objects/object_zl4/object_zl4.h>
#include <objects/object_gi_bean/object_gi_bean.h>
#include <objects/object_hintnuts/object_hintnuts.h>
#include <objects/object_oE8/object_oE8.h>
#include <objects/object_zo/object_zo.h>
#include <objects/object_gi_bomb_1/object_gi_bomb_1.h>
#include <objects/object_hni/object_hni.h>
#include <objects/object_oE9/object_oE9.h>
#include <objects/object_gi_bomb_2/object_gi_bomb_2.h>
#include <objects/object_horse/object_horse.h>
#include <objects/object_oE_anime/object_oE_anime.h>
#include <objects/object_gi_bombpouch/object_gi_bombpouch.h>
#include <objects/object_horse_ganon/object_horse_ganon.h>
#include <objects/object_oF1d_map/object_oF1d_map.h>

#define FLAGS (ACTOR_FLAG_UPDATE_WHILE_CULLED | ACTOR_FLAG_DRAW_WHILE_CULLED)


static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_HIT5,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK1,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 15, 50, 0, { 0, 0, 0 } },
};

static DamageTable sDamageTable[] = {
    /* Deku nut      */ DMG_ENTRY(0, PUPPET_DMGEFF_STUN),
    /* Deku stick    */ DMG_ENTRY(2, PUPPET_DMGEFF_NORMAL),
    /* Slingshot     */ DMG_ENTRY(1, PUPPET_DMGEFF_NORMAL),
    /* Explosive     */ DMG_ENTRY(2, PUPPET_DMGEFF_NORMAL),
    /* Boomerang     */ DMG_ENTRY(0, PUPPET_DMGEFF_STUN),
    /* Normal arrow  */ DMG_ENTRY(2, PUPPET_DMGEFF_NORMAL),
    /* Hammer swing  */ DMG_ENTRY(2, PUPPET_DMGEFF_KNOCKBACK),
    /* Hookshot      */ DMG_ENTRY(0, PUPPET_DMGEFF_STUN),
    /* Kokiri sword  */ DMG_ENTRY(1, PUPPET_DMGEFF_NORMAL),
    /* Master sword  */ DMG_ENTRY(2, PUPPET_DMGEFF_NORMAL),
    /* Giant's Knife */ DMG_ENTRY(4, PUPPET_DMGEFF_NORMAL),
    /* Fire arrow    */ DMG_ENTRY(2, PUPPET_DMGEFF_FIRE),
    /* Ice arrow     */ DMG_ENTRY(2, PUPPET_DMGEFF_ICE),
    /* Light arrow   */ DMG_ENTRY(2, PUPPET_DMGEFF_THUNDER),
    /* Unk arrow 1   */ DMG_ENTRY(2, PUPPET_DMGEFF_NONE),
    /* Unk arrow 2   */ DMG_ENTRY(2, PUPPET_DMGEFF_NONE),
    /* Unk arrow 3   */ DMG_ENTRY(2, PUPPET_DMGEFF_NONE),
    /* Fire magic    */ DMG_ENTRY(2, PUPPET_DMGEFF_FIRE),
    /* Ice magic     */ DMG_ENTRY(0, PUPPET_DMGEFF_ICE),
    /* Light magic   */ DMG_ENTRY(3, PUPPET_DMGEFF_THUNDER),
    /* Shield        */ DMG_ENTRY(0, PUPPET_DMGEFF_NONE),
    /* Mirror Ray    */ DMG_ENTRY(0, PUPPET_DMGEFF_NONE),
    /* Kokiri spin   */ DMG_ENTRY(1, PUPPET_DMGEFF_NORMAL),
    /* Giant spin    */ DMG_ENTRY(4, PUPPET_DMGEFF_NORMAL),
    /* Master spin   */ DMG_ENTRY(2, PUPPET_DMGEFF_NORMAL),
    /* Kokiri jump   */ DMG_ENTRY(2, PUPPET_DMGEFF_NORMAL),
    /* Giant jump    */ DMG_ENTRY(8, PUPPET_DMGEFF_NORMAL),
    /* Master jump   */ DMG_ENTRY(4, PUPPET_DMGEFF_NORMAL),
    /* Unknown 1     */ DMG_ENTRY(0, PUPPET_DMGEFF_NONE),
    /* Unblockable   */ DMG_ENTRY(0, PUPPET_DMGEFF_NONE),
    /* Hammer jump   */ DMG_ENTRY(4, PUPPET_DMGEFF_KNOCKBACK),
    /* Unknown 2     */ DMG_ENTRY(0, PUPPET_DMGEFF_NONE),
};

void PropPuppet_Init(Actor* thisx, PlayState* play) {
    PropPuppet* this = (PropPuppet*)thisx;

    PlayerData playerData = Anchor_GetClientPlayerData(this->link.actor.params - 3);

    this->link.actor.room = -1;

    this->link.puppetAge = playerData.playerAge;

    this->propId = playerData.propId;

    /* 
        init skelanime based on whatever prop it is
     */
    switch (playerData.propId) {
        case PROP_LINK:
            SkelAnime_InitLink(play, &this->link.linkSkeleton, gPlayerSkelHeaders[((void)0, this->link.puppetAge)],
                            gPlayerAnim_link_normal_wait, 9, this->link.linkSkeleton.jointTable, this->link.linkSkeleton.morphTable,
                            PLAYER_LIMB_MAX);
            break;
        case PROP_STALFOS:
            SkelAnime_Init(play, &this->skelAnime, &gStalfosSkel, &gStalfosMiddleGuardAnim, NULL, NULL, STALFOS_LIMB_MAX);
            SkelAnime_Init(play, &this->upperSkelanime, &gStalfosSkel, &gStalfosMiddleGuardAnim, NULL, NULL, STALFOS_LIMB_MAX);
            break;
        case PROP_DOOR:
            break;
        case PROP_CHEST:
            break;
        case PROP_FIREWALL:
            break;
        case PROP_POE:
            break;
        case PROP_OCTOROK:
            break;
        case PROP_WEBS:
            break;
        case PROP_BOMB:
            break;
        case PROP_WALLMASTER:
            break;
        case PROP_DODONGO:
            break;
        case PROP_KEESE:
            break;
        case PROP_HORSE:
            break;
        case PROP_RUPEE_GREEN:
            break;
        case PROP_RUPEE_BLUE:
            break;
        case PROP_RUPEE_RED:
            break;
        case PROP_RUPEE_PURPLE:
            break;
        case PROP_RUPEE_PINK:
            break;
        case PROP_RUPEE_ORANGE:
            break;
        case PROP_HEART:
            break;
        case PROP_FAIRY:
            break;
        case PROP_CUCCO:
            break;
        case PROP_TEKTITE:
            break;
        case PROP_LEEVER:
            break;
        case PROP_PEEHAT:
            break;
        case PROP_PEEHAT_LARVA:
            break;
        case PROP_LIZALFOS:
            break;
        case PROP_DINALFOS:
            break;
        case PROP_FLAGPOLE:
            break;
        case PROP_GOMA_LARVA:
            break;
        case PROP_PUSH_CUBE:
            break;
        case PROP_SHABOM:
            break;
        case PROP_DODONGO_BABY:
            break;
        case PROP_DARK_LINK:
            break;
        case PROP_BIRI:
            break;
        case PROP_TAILPASARAN:
            break;
        case PROP_SKULLTULA:
            break;
        case PROP_SKULLTULA_SMALL:
            break;
        case PROP_SKULLTULA_LARGE:
            break;
        case PROP_SKULLTULA_GOLD:
            break;
        case PROP_WALLTULA:
            break;
        case PROP_TORCH_SLUG:
            break;
        case PROP_BLOCK_SMALL:
            break;
        case PROP_BLOCK_LARGE:
            break;
        case PROP_BLOCK_SMALL_ROT:
            break;
        case PROP_BLOCK_LARGE_ROT:
            break;
        case PROP_GRASS_CLUMP:
            break;
        case PROP_TREE_STUMP:
            break;
        case PROP_SIGNPOST_OBLONG:
            break;
        case PROP_SIGNPOST_ARROW:
            break;
        case PROP_BOULDER_FRAGMENT:
            break;
        case PROP_STINGER:
            break;
        case PROP_SHOPKEEPER:
            break;
        case PROP_HAMMER_STATUE:
            break;
        case PROP_STONE_BLOCK:
            break;
        case PROP_FLAMETHROWER_SPINNING:
            break;
        case PROP_FLAMETHROWER_STATIONARY:
            break;
        case PROP_SHEIK:
            break;
        case PROP_FLAME_CIRCLE:
            break;
        case PROP_MOBLINS:
            break;
        case PROP_BOMBFLOWER:
            break;
        case PROP_SPIKED_LOG:
            break;
        case PROP_ARMOS:
            break;
        case PROP_DEKU_BABA:
            break;
        case PROP_BOMBABLE_WALL:
            break;
        case PROP_TORCH:
            break;
        case PROP_HEART_CONTAINER:
            break;
        case PROP_MAD_SCRUB:
            break;
        case PROP_BARI:
            break;
        case PROP_BUBBLE:
            break;
        case PROP_MASTER_SWORD:
            break;
    }

    ActorShape_Init(&this->link.actor.shape, 0.0f, ActorShadow_DrawFeet, 90.0f);

    /* 
        Todo: re-edit sCylinderInit radius/height for new actor
     */

    Collider_InitCylinder(play, &this->link.collider);
    Collider_SetCylinder(play, &this->link.collider, &this->link.actor, &sCylinderInit);

    this->link.actor.colChkInfo.damageTable = sDamageTable;

    PlayerData playerData = Anchor_GetClientPlayerData(this->link.actor.params - 3);

    NameTag_RemoveAllForActor(thisx);

    /* 
        Only register if hunter on same team as other hunter
     */

    Color_RGB8 clientColor = Anchor_GetClientColor(this->link.actor.params - 3);
    const char* playerName = Anchor_GetClientName(this->link.actor.params - 3);
    this->link.nameTagOptions.yOffset = 0;
    NameTag_RegisterForActorWithOptions(&this->link.actor, playerName, this->link.nameTagOptions);
}

void PropPuppet_Destroy(Actor* thisx, PlayState* play) {
    PropPuppet* this = (PropPuppet*)thisx;

    Collider_DestroyCylinder(play, &this->link.collider);
}

void PropPuppet_Update(Actor* thisx, PlayState* play) {
    PropPuppet* this = (PropPuppet*)thisx;

    PlayerData playerData = Anchor_GetClientPlayerData(this->link.actor.params - 3);

    if (this->link.puppetAge != playerData.playerAge) {
        PropPuppet_Init(this, play);
        return;
    }

    if (this->propId != playerData.propId) {
        PropPuppet_Init(this, play);
        return;
    }

    this->link.actor.shape.yOffset = playerData.yOffset;

    if (this->link.damageTimer > 0) {
        this->link.damageTimer--;
    }

    if (this->link.collider.base.acFlags & AC_HIT && this->link.damageTimer <= 0) {
        this->link.collider.base.acFlags &= ~AC_HIT;
        gSaveContext.playerData.damageEffect = this->link.actor.colChkInfo.damageEffect;
        gSaveContext.playerData.damageValue = this->link.actor.colChkInfo.damage;
        Anchor_DamagePlayer(this->link.actor.params - 3, this->link.actor.colChkInfo.damageEffect, this->link.actor.colChkInfo.damage);

        if (gSaveContext.playerData.damageEffect == PUPPET_DMGEFF_STUN) {
            Audio_PlayActorSound2(&this->link.actor, NA_SE_EN_GOMA_JR_FREEZE);
            Actor_SetColorFilter(&this->link.actor, 0, 0xFF, 0, 40);
        } else if (gSaveContext.playerData.damageEffect != PUPPET_DMGEFF_NONE) {
            Actor_SetColorFilter(&this->link.actor, 0x4000, 255, 0, 24);
        }

        this->link.damageTimer = 18;
    }

    Collider_UpdateCylinder(thisx, &this->link.collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->link.collider.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->link.collider.base);

    if (this->link.actor.params >= 3) {
        if (Anchor_GetClientScene(this->link.actor.params - 3) == play->sceneNum) {
            PosRot playerPosRot = Anchor_GetClientPosition(this->link.actor.params - 3);
            this->link.actor.world.pos = playerPosRot.pos;
            this->link.actor.shape.rot = playerPosRot.rot;
        } else {
            this->link.actor.world.pos.x = -9999.0f;
            this->link.actor.world.pos.y = -9999.0f;
            this->link.actor.world.pos.z = -9999.0f;
        }
    }

    if (playerData.propId == PROP_LINK) {
        Vec3s* jointTable = Anchor_GetClientJointTable(this->link.actor.params - 3);

        this->link.linkSkeleton.jointTable = jointTable;
    }

    if (playerData.playerSound != 0) {
        Audio_PlaySoundGeneral(playerData.playerSound, &this->link.actor.projectedPos, 4, &D_801333E0, &D_801333E0,
                            &D_801333E8);
    }
}

void PropPuppet_Draw(Actor* thisx, PlayState* play) {
    PropPuppet* this = (PropPuppet*)thisx;

    PlayerData playerData = Anchor_GetClientPlayerData(this->link.actor.params - 3);

    switch (playerData.propId) {
        case PROP_LINK:
            LinkPuppet_Draw(this, play);
            return;
        case PROP_STALFOS:
            Gfx_SetupDL_25Opa(play->state.gfxCtx);
            func_8002EBCC(&this->link.actor, play, 1);
            SkelAnime_DrawSkeletonOpa(play, &this->skelAnime, NULL, NULL, this);
            break;
    }
}

#endif
