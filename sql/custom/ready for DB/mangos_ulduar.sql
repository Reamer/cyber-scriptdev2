-- GENERAL
-- Teleporter
UPDATE gameobject_template SET ScriptName = "go_ulduar_teleporter" WHERE entry = 194569;
-- NPC respawnzeit
UPDATE creature SET spawntimesecs=604800 WHERE creature.map = 603;

-- ****** Ignis the Furnace Master ******
UPDATE creature_template SET lootid = entry, `mechanic_immune_mask` = 617299839 WHERE entry IN (33118, 33190);
UPDATE creature_template SET ScriptName = "mob_iron_construct" WHERE `entry` = 33121;
UPDATE creature_template SET faction_h=1925, faction_a=1925, scale=0.5, `AIName` = "EventAI" WHERE `entry` = 33221;
DELETE FROM creature_ai_scripts WHERE creature_id = 33221;
INSERT INTO creature_ai_scripts VALUES
(3322101, 33221, 0, 0, 100, 2, 3000, 3000, 0, 0, 11, 62548, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, "Scorch Trigger - Cast Scorch normal"),
(3322102, 33221, 0, 0, 100, 4, 3000, 3000, 0, 0, 11, 63476, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, "Scorch Trigger - Cast Scorch heroic"),
(3322103, 33221, 4, 0, 100, 6, 0, 0, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, "Scorch Trigger - Disable Combat Movement on Aggro");

DELETE FROM spell_script_target WHERE entry IN (64474,64475);
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES ('64474', '1', '33118'), ('64475', '1', '33118');

-- ****** Razorscale****** 
UPDATE creature_template SET scriptname='boss_razorscale' WHERE entry=33186;
UPDATE creature_template SET mechanic_immune_mask=617299803, `lootid` = `entry` WHERE entry IN (33186, 33724);
-- original x=587.547, y= -174.927, z = 391.517; make the boss fly before encounter starts
UPDATE creature SET position_x = 590.346741, position_y = -226.947647, position_z = 442.897583 WHERE id = 33186;
-- only 2 harpoons for 10 man
UPDATE gameobject SET spawnMask = 2 WHERE guid IN (73595, 73592);
-- mole machines & adds
UPDATE creature_template SET ScriptName = 'mob_mole_machine' WHERE entry = 33245;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_watcher' WHERE entry = 33453;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_sentinel' WHERE entry = 33846;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_guardian' WHERE entry = 33388;
UPDATE creature_template SET ScriptName = 'npc_expedition_commander' WHERE entry = 33210;
UPDATE creature_template SET ScriptName = 'mob_devouring_flame_target' WHERE entry IN (34189, 34188);
UPDATE gameobject_template SET data3 = 180000, ScriptName='go_repair_harpoon' WHERE entry IN (194543,194542,194541,194519);

DELETE FROM creature WHERE id = 33282;

-- expedition NPCs - don't enter the fight
UPDATE `creature_template` SET `unit_flags` = `unit_flags`|2|33554432, `flags_extra` = `flags_extra`|2 WHERE `entry` IN (33259, 33816, 33287);
-- harponen
DELETE FROM spell_script_target WHERE spell_script_target.entry = 63524;
INSERT INTO spell_script_target VALUES (63524 ,1,33282);
DELETE FROM spell_script_target WHERE spell_script_target.entry = 63657;
INSERT INTO spell_script_target VALUES (63657 ,1,33282);
DELETE FROM spell_script_target WHERE spell_script_target.entry = 63658;
INSERT INTO spell_script_target VALUES (63658 ,1,33282);
DELETE FROM spell_script_target WHERE spell_script_target.entry = 63659;
INSERT INTO spell_script_target VALUES (63659 ,1,33282);

DELETE FROM spell_script_target WHERE spell_script_target.entry = 62505;
INSERT INTO spell_script_target VALUES (62505 ,1,33186);

-- ****** XT-002 Deconstructor ******
UPDATE `creature_template` SET `ScriptName` = "boss_xt_002" WHERE `entry` = 33293;
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299839, `lootid` = `entry` WHERE entry IN (33293, 33885);
UPDATE `creature_template` SET `ScriptName` = "mob_xtheart" WHERE `entry` = 33329;
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299839 WHERE entry IN (33329, 33995);
UPDATE `creature_template` SET `ScriptName` = "mob_pummeler" WHERE `entry` = 33344;
UPDATE creature_template SET modelid_1 = 19139, modelid_2 = 19139, modelid_3 = 19139, modelid_4 = 19139, Scriptname='mob_boombot' WHERE entry=33346; -- boombot nh
UPDATE creature_template SET modelid_1 = 19139, modelid_2 = 19139, modelid_3 = 19139, modelid_4 = 19139 WHERE entry = 33886; -- boombot hero
UPDATE creature_template SET modelid_1 = 28585, modelid_2 = 28575, modelid_3 = 26442, scale = 2 WHERE entry IN (33343,33887);  -- XS-013 Scrapbot
UPDATE `creature_template` SET `ScriptName` = "mob_voidzone" WHERE `entry` = 34001;
UPDATE `creature_template` SET `ScriptName` = "mob_lifespark" WHERE `entry` = 34004;

-- ****** Iron Council (Assembly of Iron)******
UPDATE creature_template SET ScriptName='boss_brundir' WHERE entry = 32857;
UPDATE creature_template SET ScriptName='boss_molgeim' WHERE entry = 32927;
UPDATE creature_template SET ScriptName='boss_steelbreaker' WHERE entry = 32867;
UPDATE creature_template SET ScriptName = 'mob_rune_of_power' WHERE entry = 33705;
UPDATE creature_template SET ScriptName = 'mob_rune_of_summoning' WHERE entry = 33051;
UPDATE creature_template SET ScriptName = 'mob_ulduar_lightning_elemental' WHERE entry = 32958;
UPDATE creature_template SET mechanic_immune_mask = 617299835 WHERE entry IN (32927, 33692, 32867, 33693);
UPDATE creature_template SET mechanic_immune_mask = 617297787 WHERE entry IN (32857, 33694); -- Brundir stunnable
-- supercharge
DELETE FROM spell_script_target WHERE spell_script_target.entry = 61920;
INSERT INTO spell_script_target VALUES (61920 ,1,32857), (61920 ,1,32927), (61920 ,1,32867);

-- ****** Kologarn****** 
-- arms are mounted on the boss
DELETE FROM creature WHERE id IN (32933, 32934);
-- set proper Kologarn position
UPDATE creature SET position_x = 1797.15, position_y = -24.4027, position_z = 448.741 WHERE id = 32930;


UPDATE creature_template SET scriptname='boss_kologarn' WHERE entry = 32930;
UPDATE creature_template SET scriptname='boss_right_arm' WHERE entry=32934;
UPDATE creature_template SET scriptname='boss_left_arm' WHERE entry=32933;
UPDATE creature_template SET ScriptName = 'mob_ulduar_rubble' WHERE entry IN (33768, 33809, 33908, 33942);
UPDATE creature_template SET ScriptName = "mob_eyebeam_trigger" WHERE entry IN (33802, 33632);
UPDATE creature_template SET scriptname = "mob_kologarn_pit_kill_bunny" WHERE entry = 33742;
UPDATE creature SET DeathState = 1 WHERE id = 34297;
UPDATE creature_template SET dmg_multiplier = 0 WHERE entry IN (32934, 32933, 33911, 33910);
UPDATE creature_template SET mechanic_immune_mask = 617299803 WHERE entry IN (32930, 33909, 32934, 32933, 33911, 33910);
UPDATE creature_template SET RegenHealth = 1 WHERE entry = 33910;
UPDATE creature_template SET RegenHealth = 1 WHERE entry = 33911;

DELETE FROM spell_script_target WHERE spell_script_target.entry IN (63985, 62056);
INSERT INTO spell_script_target VALUES (63985,1,32934),(62056,1,32934);

DELETE FROM spell_script_target WHERE spell_script_target.entry IN (63352, 63702, 63676);
INSERT INTO spell_script_target VALUES (63352,1,32930),(63702,1,32930),(63676,1,32930);

-- ****** Auriaya****** 
UPDATE creature_template SET equipment_id = 103000, scriptname='boss_auriaya' WHERE entry=33515;
UPDATE creature_template SET scriptname='mob_feral_defender' WHERE entry IN (34035, 34171);
UPDATE creature_template SET minlevel=80, maxlevel=80, scriptname='mob_seeping_feral_essence' WHERE entry=34098;
UPDATE creature_template SET speed_walk= 1.66667, ScriptName = 'mob_sanctum_sentry' WHERE entry = 34014;
UPDATE `creature_template` SET `mechanic_immune_mask` = 617297919 WHERE `entry` IN (33515, 34175, 34035, 34171);

DELETE FROM spell_script_target WHERE entry = 64449;
INSERT INTO spell_script_target VALUES (64449, 1, 34096);

DELETE FROM creature_equip_template WHERE entry = 103000;
INSERT INTO creature_equip_template values (103000, 45315, 0, 0);

-- ****** Freya****** 
UPDATE creature_template SET ScriptName = 'boss_freya' WHERE entry = 32906;
UPDATE creature_template SET ScriptName = 'boss_elder_brightleaf' WHERE entry = 32915;
UPDATE creature_template SET ScriptName = 'boss_elder_ironbranch' WHERE entry = 32913;
UPDATE creature_template SET ScriptName = 'boss_elder_stonebark' WHERE entry = 32914;
UPDATE creature_template SET ScriptName = 'mob_iron_roots' WHERE entry IN (33088, 33168);
UPDATE creature_template SET ScriptName = 'mob_freya_ground' WHERE entry IN (33215, 33228, 33170, 33050, 34129);
UPDATE creature_template SET ScriptName = 'mob_freya_spawned' WHERE entry IN (32916, 32919, 33202, 33203, 32918);
UPDATE creature_template SET mechanic_immune_mask = 619397115 WHERE entry IN (32906, 32915, 32913, 32914, 33360, 33392, 33393, 33391);

-- some spell_script_target fixes, this may be wrong
DELETE FROM spell_script_target WHERE spell_script_target.entry IN (62584, 64185, 62385, 62386, 62387, 62483, 62484, 62485, 65585, 65586, 65587, 65588, 65589, 65590);
INSERT INTO spell_script_target (`entry`, `type`, `targetEntry`) VALUES
 (62584, 1, 32906 ), -- Lifebinder's Gift nonhero
 (64185, 1, 32906 ), -- Lifebinder's Gift hero

 (62385, 1, 32906 ), -- Brightleaf's Essence nonhero
 (62386, 1, 32906 ), -- Stonebark's Essence nonhero
 (62387, 1, 32906 ), -- Ironbranch's Essence nonhero
 (62483, 1, 32906 ), -- Stonebark's Essence Channel nonhero
 (62484, 1, 32906 ), -- Ironbranch's Essence Channel nonhero
 (62485, 1, 32906 ), -- Brightleaf's Essence Channel nonhero
 (65585, 1, 32906 ), -- Brightleaf's Essence hero
 (65586, 1, 32906 ), -- Ironbranch's Essence hero
 (65587, 1, 32906 ), -- Brightleaf's Essence Channel hero
 (65588, 1, 32906 ), -- Ironbranch's Essence Channel hero
 (65589, 1, 32906 ), -- Stonebark's Essence Channel hero
 (65590, 1, 32906 ); -- Stonebark's Essence hero

-- **********Hodir*************
UPDATE creature_template SET ScriptName = 'boss_hodir' WHERE entry = 32845;
UPDATE creature_template SET mechanic_immune_mask = 650854239 WHERE entry IN (32845, 32846);
UPDATE creature_template SET ScriptName = 'mob_icicle' WHERE entry IN (33169,33173,33174);
UPDATE creature_template SET ScriptName = 'mob_toasty_fire' WHERE entry = 33342;
UPDATE creature_template SET ScriptName = 'mob_flashFreeze' WHERE entry = 32926;
UPDATE creature_template SET ScriptName='mob_npc_flashFreeze' WHERE entry=32938;
DELETE FROM creature WHERE id = 32938;
UPDATE creature_template SET ScriptName = 'npc_hodir_helper' WHERE entry IN (32946, 32950, 32941, 32948);
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE id IN (32941, 32950, 32946, 32948,32938);



-- ************THORIM*********************
UPDATE creature_template SET ScriptName = 'boss_thorim' WHERE entry = 32865;
UPDATE creature_template SET mechanic_immune_mask = 650854239 WHERE entry IN (32865, 33147);
UPDATE creature_template SET unit_flags=0, ScriptName = 'boss_runic_colossus' WHERE entry = 32872;
UPDATE creature_template SET unit_flags=0 WHERE entry=33149; -- heroic
UPDATE creature_template SET unit_flags=0, ScriptName = 'boss_ancient_rune_giant' WHERE entry = 32873;
UPDATE creature_template SET unit_flags=0 WHERE entry=33148; -- heroic
UPDATE creature_template SET ScriptName = 'npc_lightning_orb' WHERE entry = 33138;
UPDATE creature_template SET ScriptName = 'mob_thorim_trap_bunny' WHERE entry IN (33725, 33054);
UPDATE creature_template SET ScriptName = 'mob_thorim_preadds' WHERE entry IN (32885, 32883, 32907, 32908, 32882);
UPDATE creature_template SET ScriptName = 'npc_sif' WHERE entry = 33196;
-- adds
UPDATE creature_template SET ScriptName = 'mob_dark_rune_acolyte' WHERE entry = 33110;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_champion' WHERE entry = 32876;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_commoner' WHERE entry = 32904;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_warbringer' WHERE entry = 32877;
UPDATE creature_template SET unit_flags=0, ScriptName = 'mob_dark_rune_ring_guard' WHERE entry = 32874;
UPDATE creature_template SET unit_flags=0 WHERE entry=33162; --  heroic
UPDATE creature_template SET ScriptName = 'mob_dark_rune_honor_guard' WHERE entry = 33125;

UPDATE `creature_template` SET `ScriptName`='mob_thorim_thunder_orb' WHERE (`entry`='33378');

REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62565', '1', '32865');

DELETE FROM spell_script_target WHERE entry = 62942 ;
INSERT INTO spell_script_target VALUES (62942 , 1, 33125);

-- *************Genral Vezax****************
UPDATE creature_template SET unit_flags = 0, ScriptName = 'boss_vezax' WHERE entry = 33271;
UPDATE creature_template SET mechanic_immune_mask = 617299963 WHERE `entry` IN (33271, 33449);
UPDATE creature_template SET armor=10673, mindmg=468,maxdmg=702,attackpower=175,dmg_multiplier=2, minhealth=1575785,maxhealth=1575785, faction_A=14,faction_H=14, minlevel=83,maxlevel=83, ScriptName = 'mob_saronite_animus' WHERE entry = 33524;
UPDATE creature_template SET armor=10673,mindmg=468, maxdmg=702,attackpower=175,dmg_multiplier=4, minhealth=9412875,maxhealth=9412875, faction_A=14,faction_H=14, minlevel=83,maxlevel=83 WHERE entry = 34152;
UPDATE creature_template SET ScriptName = 'mob_saronite_vapor', movementType = 1  WHERE entry = 33488;

-- **************Yogg Saron********************
DELETE FROM spell_script_target WHERE entry = 64466 ;       	-- Unvergängliche Wache spell auf yogg saron
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64466 , 1, 33288);
DELETE FROM spell_script_target WHERE entry = 64465 ;		-- yogg saron spell Machtvolle Schatten
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64465 , 1, 33988);  
DELETE FROM spell_script_target WHERE entry = 64172 ;		-- thorim spell zum killn
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64172 , 1, 33988);
DELETE FROM spell_script_target WHERE entry = 62714 ;		-- wachen AOE zum schwächen von sara - nonhero spell
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (62714 , 1, 33134);  
DELETE FROM spell_script_target WHERE entry = 65209 ;		-- wachen AOE zum schwächen von sara - hero spell
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (65209 , 1, 33134);
DELETE FROM spell_script_target WHERE entry = 62978;		-- Beschwörung von Wächter aus den Wolken
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (62978, 1, 33292);
DELETE FROM spell_script_target WHERE entry = 64173;		-- Zerschmetternde Illusion START
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64173, 1, 33292);	-- auf Yogg Saron
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64173, 1, 33985);	-- auf Corruptor Tentacle
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64173, 1, 33983);	-- auf Constrictor Tentacle
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64173, 1, 33966);	-- auf Crusher Tentacle
DELETE FROM spell_script_target WHERE entry = 65238;		-- Zerschmetternde Illusion ENDE
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (65238, 1, 33292);	-- auf Yogg Saron
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (65238, 1, 33985);	-- auf Corruptor Tentacle
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (65238, 1, 33983);	-- auf Constrictor Tentacle
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (65238, 1, 33966);	-- auf Crusher Tentacle
DELETE FROM spell_script_target WHERE entry = 63886;       	-- Todesstrahl visuel auf Sara
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (63886 , 1, 33134);
DELETE FROM spell_script_target WHERE entry = 63882 ;       	-- Todesstrahl visuel auf Sara
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (63882  , 1, 33134);
-- ***teleports**
DELETE FROM spell_target_position WHERE id = 63989;		-- Stormwind
INSERT INTO spell_target_position (id , target_map, target_position_x, target_position_y, target_position_z, target_orientation )
VALUES (63989, 603, 1941.587402 , 43.526680 ,  239.666336 , 2);
DELETE FROM spell_target_position WHERE id = 63997;		-- Wyrmruh
INSERT INTO spell_target_position (id , target_map, target_position_x, target_position_y, target_position_z, target_orientation )
VALUES (63997, 603, 2055.460938, -25.619570, 239.721176 , 0);
DELETE FROM spell_target_position WHERE id = 63998;		--  Lich King
INSERT INTO spell_target_position (id , target_map, target_position_x, target_position_y, target_position_z, target_orientation )
VALUES (63998, 603, 1941.131226, -94.654694, 239.989639 , 4);
DELETE FROM spell_target_position WHERE id = 63992;		-- Yogg Saron
INSERT INTO spell_target_position (id , target_map, target_position_x, target_position_y, target_position_z, target_orientation )
VALUES (63992, 603, 1951.097, -25.420, 326.162 , 3);

UPDATE creature_template SET ScriptName = 'boss_yogg_saron' WHERE entry = 33288;
UPDATE creature_template SET ScriptName = 'boss_sara' WHERE entry = 33134;
UPDATE creature_template SET ScriptName = 'boss_brain_of_yogg_saron' WHERE entry = 33890;
UPDATE creature_template SET mechanic_immune_mask = 650854271 WHERE `entry` IN (33288, 33955,33134, 34332,33890,33954);
UPDATE creature_template SET ScriptName = 'mob_corruptor_tentacle' WHERE entry = 33985;
UPDATE creature_template SET ScriptName = 'mob_constrictor_tentacle' WHERE entry = 33983;
UPDATE creature_template SET ScriptName = 'mob_vision_tentacle' WHERE entry = 33943;
UPDATE creature_template SET ScriptName = 'mob_crusher_tentacle' WHERE entry = 33966;
UPDATE creature_template SET ScriptName = 'mob_guardian_of_yogg_saron' WHERE entry = 33136;
UPDATE creature_template SET ScriptName = 'mob_immortal_guardian' WHERE entry = 33988;
UPDATE creature_template SET ScriptName = 'mob_death_orb' WHERE entry = 33882;
UPDATE creature_template SET ScriptName = 'mob_sanity_well' WHERE entry = 33991;
UPDATE creature_template SET scriptname='mob_madness_portal' WHERE `entry`=34072;
UPDATE creature_template SET scriptname='mob_laughing_skull' WHERE `entry`=33990;
UPDATE creature_template SET scriptname='mob_ominous_cloud' WHERE `entry`=33292;

-- Keepers
-- Keepers images
UPDATE creature_template SET ScriptName = 'keeper_hodir' WHERE entry = 33213;
UPDATE creature_template SET ScriptName = 'keeper_freya' WHERE entry = 33241;
UPDATE creature_template SET ScriptName = 'keeper_thorim' WHERE entry = 33242;
UPDATE creature_template SET ScriptName = 'keeper_mimiron' WHERE entry = 33244;
-- images
-- UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = 'hodir_image' WHERE entry = 33411;
-- UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = 'freya_image' WHERE entry = 33410;
-- UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = 'thorim_image' WHERE entry = 33413;
-- UPDATE creature_template SET `npcflag` = 1, `unit_flags` = 2, ScriptName = 'mimiron_image' WHERE entry = 33412;

DELETE FROM creature_template WHERE entry in (1000103);
INSERT INTO creature_template(entry,difficulty_entry_1,difficulty_entry_2,difficulty_entry_3,KillCredit1,KillCredit2,modelid_1,modelid_2,modelid_3,modelid_4,name,subname,IconName,gossip_menu_id,minlevel,maxlevel,minhealth,maxhealth,minmana,maxmana,armor,faction_A,faction_H,npcflag,speed_walk,speed_run,scale,rank,mindmg,maxdmg,dmgschool,attackpower,dmg_multiplier,baseattacktime,rangeattacktime,unit_class,unit_flags,dynamicflags,family,trainer_type,trainer_spell,trainer_class,trainer_race,minrangedmg,maxrangedmg,rangedattackpower,type,type_flags,lootid,pickpocketloot,skinloot,resistance1,resistance2,resistance3,resistance4,resistance5,resistance6,spell1,spell2,spell3,spell4,PetSpellDataId,mingold,maxgold,AIName,MovementType,InhabitType,unk16,unk17,RacialLeader,questItem1,questItem2,questItem3,questItem4,questItem5,questItem6,movementId,RegenHealth,equipment_id,mechanic_immune_mask,flags_extra,ScriptName) VALUES 
(1000103,0,0,0,0,0,25680,0,25680,0,'Der etwas andere Abenteurer','Cyberneticworld',null,0,83,83,120000,120000,0,0,7,35,35,1,1,1.14286,1,0,2,2,0,24,1,2000,0,0,0,0,0,0,0,0,0,1,1,100,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,4,1,1,0,0,0,0,0,0,0,121,1,0,0,2,'adventurer' );

-- **************Mimiron*************************
UPDATE creature_template SET ScriptName = 'boss_mimiron' WHERE entry = 33350;
UPDATE creature_template SET ScriptName = 'boss_leviathan_mk' WHERE entry = 33432;
UPDATE creature_template SET ScriptName = 'leviathan_turret' WHERE entry = 34071;
UPDATE creature_template SET ScriptName = 'mob_mimiron_flames' WHERE entry IN (34363, 34121);
UPDATE creature_template SET ScriptName = 'boss_vx001' WHERE entry = 33651;
UPDATE creature_template SET ScriptName = 'boss_aerial_command_unit' WHERE entry = 33670;
UPDATE creature_template SET mechanic_immune_mask = 617299839 WHERE `entry` IN (33432, 34106,33651, 34108,33670,34109);
UPDATE gameobject_template SET data2 = 2000, ScriptName='go_red_button' WHERE entry = 194739;
UPDATE creature_template SET ScriptName = 'mob_proximity_mine' WHERE entry = 34362;
UPDATE creature_template SET ScriptName = 'mob_bomb_bot' WHERE entry IN (33836, 34192);
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, `minlevel` = 80, `maxlevel` = 80, minhealth = 17600, maxhealth= 17600,  ScriptName = 'mob_emergency_bot' WHERE entry = 34147;
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, `minlevel` = 80, `maxlevel` = 80, minhealth = 55400, maxhealth= 55400 WHERE entry = 34148;
UPDATE creature_template SET ScriptName = 'mob_frost_bomb_ulduar' WHERE entry = 34149;
UPDATE creature_template SET ScriptName = 'mob_mimiron_inferno' WHERE entry = 33370;
UPDATE creature_template SET lootid = 34057, ScriptName = 'mob_assault_bot' WHERE entry = 34057;
UPDATE creature_template SET ScriptName = 'mob_magnetic_core' WHERE entry = 34068;
-- spells, may not be correct
-- REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('64444', '1', '33670');
-- REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('63414', '1', '33651');
-- REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('65101', '1', '33350');
-- REPLACE INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('62909', '1', '33350');
-- DELETE FROM spell_script_target WHERE entry = 63274;       	-- Laserslave
-- INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (63274, 1,33651);
-- DELETE FROM spell_script_target WHERE entry = 63414;       	-- Laserslave
-- INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (63414, 1,33651);
-- DELETE FROM spell_script_target WHERE entry = 63414;       	-- Laserslave
-- INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (63414, 1,34192);
DELETE FROM spell_script_target WHERE entry = 64444;       	-- Magnetischer Kern
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64444, 1,33670);
DELETE FROM spell_script_target WHERE entry = 64436;       	-- Magnetischer Kern
INSERT INTO spell_script_target (entry, type, targetEntry) VALUES (64436, 1,33670);

DELETE FROM creature_loot_template WHERE entry = 34057;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, mincountOrRef, maxcount)
VALUES (34057, 46029, 100,1,1);

-- *******Chest Spawning************

-- Freya
DELETE FROM gameobject WHERE id IN (194324, 194327,194328,194331);
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(800000, 194324, 603, 1, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1),
(800001, 194327, 603, 1, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1),
(800002, 194328, 603, 2, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1),
(800003, 194331, 603, 2, 65535, 2374.876221, -78.512665, 425.073608, 3.260976, 0, 0, 0.706026, 0.708186, -604800, 100, 1);

-- Mimiron
DELETE FROM `gameobject` WHERE `id` IN (194789, 194956, 194957, 194958); 
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800004, 194789, 603, 1, 65535, 2734.73, 2568.98, 364.314, 0.0139475, 0, 0, 0.00697369, 0.999976, -604800, 100, 1),
(800005, 194956, 603, 1, 65535, 2734.73, 2568.98, 364.314, 0.0139475, 0, 0, 0.00697369, 0.999976, -604800, 100, 1),
(800006, 194957, 603, 2, 65535, 2734.73, 2568.98, 364.314, 0.0139475, 0, 0, 0.00697369, 0.999976, -604800, 100, 1),
(800007, 194958, 603, 2, 65535, 2734.73, 2568.98, 364.314, 0.0139475, 0, 0, 0.00697369, 0.999976, -604800, 100, 1);

-- Thorim
DELETE FROM `gameobject` WHERE `id` IN (194312, 194313, 194314, 194315);
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(800008, 194312, 603, 1, 65535, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.00697369, 0.999976, -604800, 100, 1),
(800009, 194313, 603, 1, 65535, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.00697369, 0.999976, -604800, 100, 1),
(800010, 194315, 603, 2, 65535, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.00697369, 0.999976, -604800, 100, 1),
(800011, 194314, 603, 2, 65535, 2134.95, -286.436, 419.505, 1.58825, 0, 0, 0.00697369, 0.999976, -604800, 100, 1);

-- Hodir
UPDATE gameobject SET animprogress = 100 WHERE id IN ( 194307, 194308, 194200, 194201);
UPDATE gameobject SET spawntimesecs = -604800 WHERE id IN ( 194307, 194308, 194200, 194201);

UPDATE gameobject_template SET faction = 0, flags = 0, size = 2  WHERE entry IN ( 194307, 194308, 194200, 194201, 194312, 194313, 194314, 194315, 194789, 194956, 194957, 194958,194324, 194327,194328,194331);

