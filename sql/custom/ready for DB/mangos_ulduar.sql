-- GENERAL
-- Teleporter
UPDATE gameobject_template SET ScriptName = "go_ulduar_teleporter" WHERE entry = 194569;
-- NPC respawnzeit
UPDATE creature SET spawntimesecs=604800 WHERE creature.map = 603;

-- ****** Ignis the Furnace Master ******
UPDATE `creature_template` SET `ScriptName` = "boss_ignis" WHERE `entry` = 33118;
UPDATE `creature_template` SET lootid = entry, `mechanic_immune_mask` = 617299839 WHERE entry IN (33118, 33190);
UPDATE `creature_template` SET `ScriptName` = "mob_iron_construct" WHERE `entry` = 33121;
UPDATE `creature_template` SET faction_h=1925, faction_a=1925, scale=0.5, `AIName` = "EventAI" WHERE `entry` = 33221;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 33221;
INSERT INTO `creature_ai_scripts` VALUES
(3322101, 33221, 0, 0, 100, 2, 3000, 3000, 0, 0, 11, 62548, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, "Scorch Trigger - Cast Scorch normal"),
(3322102, 33221, 0, 0, 100, 4, 3000, 3000, 0, 0, 11, 63476, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, "Scorch Trigger - Cast Scorch heroic"),
(3322103, 33221, 4, 0, 100, 6, 0, 0, 0, 0, 20, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, "Scorch Trigger - Disable Combat Movement on Aggro");

DELETE FROM spell_script_target WHERE entry IN (64474,64475);
INSERT INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('64474', '1', '33118'), ('64475', '1', '33118');

-- ****** Razorscale****** 
UPDATE creature_template SET scriptname='boss_razorscale' WHERE entry=33186;
UPDATE creature_template SET `ScriptName`="boss_razorscale" WHERE entry=33186;
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
UPDATE creature_template SET mechanic_immune_mask = 619397115 WHERE entry IN (32857, 33694, 32927, 33692, 32867, 33693);
UPDATE creature_template SET mechanic_immune_mask = 619395067 WHERE entry IN (32857, 33694); -- Brundir stunnable
-- supercharge
DELETE FROM spell_script_target WHERE spell_script_target.entry = 61920;
INSERT INTO spell_script_target VALUES (61920 ,1,32857), (61920 ,1,32927), (61920 ,1,32867);

-- ****** Kologarn****** 
-- arms are mounted on the boss
DELETE FROM creature WHERE id IN (32933, 32934);
-- set proper Kologarn position
UPDATE creature SET position_x = 1797.15, position_y = -24.4027, position_z = 448.741 WHERE id = 32930;

UPDATE creature_model_info SET bounding_radius = 20, combat_reach = 20 WHERE modelid IN (28638, 28822, 28821);
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

DELETE FROM spell_script_target WHERE spell_script_target.entry IN (63702, 63676);
INSERT INTO spell_script_target VALUES (63702,1,32930),(63676,1,32930);

-- ****** Auriaya****** 
UPDATE creature_template SET equipment_id = 103000, scriptname='boss_auriaya' WHERE entry=33515;
UPDATE creature_template SET scriptname='mob_feral_defender' WHERE entry IN (34035, 34171);
UPDATE creature_template SET minlevel=80, maxlevel=80, scriptname='mob_seeping_feral_essence' WHERE entry=34098;
UPDATE creature_template SET ScriptName = 'mob_sanctum_sentry' WHERE entry = 34014;
UPDATE `creature_template` SET `mechanic_immune_mask` = 619395071 WHERE `entry` IN (33515, 34175, 34035, 34171);

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
UPDATE `creature_template` SET `mechanic_immune_mask` = 619397115 WHERE `entry` IN (32906, 32915, 32913, 32914, 33360, 33392, 33393, 33391);

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
