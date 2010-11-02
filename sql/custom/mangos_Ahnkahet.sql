-- Ahn'kahet fixes

-- boss Elder Nadox
UPDATE `creature_template` SET `AIName`='',`ScriptName`='mob_nadox_guardian' WHERE (`entry`='30176') ;
-- boss Elder Nadox Achievment
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7317') AND (`type`='11') ;
-- eventAI for trashmobs
UPDATE `creature_template` SET `AIName` = "EventAI" WHERE `entry` IN (30178);
DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (30176,30178);
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`comment`) VALUES 
(3017801,30178,0,100,7,0,500,25000,40000,11,56354,0,2,"Ahn'kahar Swarmer - Cast Sprint");

-- boss Prince Taldaram
UPDATE `creature_template` SET `ScriptName` = "mob_taldaram_flame_orb" WHERE `entry` = 30702;

-- boss jedoga
UPDATE `creature_template` SET `ScriptName`="mob_jedoga_volunteer" WHERE `entry`=30385;
-- boss jedoga Achievment
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7359') AND (`type`='11') ;

-- boss Herals Volazj
-- TODO: find the right NPC for this script:
UPDATE `creature_template` SET `ScriptName` = "mob_volazj_clone" WHERE `entry` = 31627;

-- boss Amanitar
UPDATE `creature_template` SET `ScriptName` = "boss_amanitar" WHERE `entry` = 30258;
UPDATE `creature_template` SET `modelid_1`='31043',`modelid_2`='0',`modelid_3`='0',`modelid_4`='0',`scale`='1',`ScriptName` = "npc_amanitar_healthy_mushroom" WHERE `entry` = 30391;
UPDATE `creature_template` SET `modelid_1`='31043',`modelid_2`='0',`modelid_3`='0',`modelid_4`='0',`scale`='1',`ScriptName` = "npc_amanitar_poisonous_mushroom" WHERE `entry` = 30435;
UPDATE `creature_template` SET `modelid_1`='31043',`modelid_2`='0',`modelid_3`='0',`modelid_4`='0',`scale`='1' WHERE `entry`=31461;
UPDATE `creature_template` SET `modelid_1`='31043',`modelid_2`='0',`modelid_3`='0',`modelid_4`='0',`scale`='1' WHERE `entry`=31462;  