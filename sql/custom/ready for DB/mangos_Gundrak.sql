UPDATE `creature_template` SET `ScriptName`='mob_sladran_constrictor' WHERE (`entry`='29713') ;

UPDATE `creature_template` SET `ScriptName`='npc_snake_wrap' WHERE (`entry`='29742') ;
-- Slad'ran Viper
UPDATE `creature_template` SET `AIName`='EventAI' WHERE (`entry`='29680');

DELETE FROM `creature_ai_scripts` WHERE (`creature_id`=29680);
INSERT INTO `creature_ai_scripts` VALUES 
( 2968001, 29680, 0, 0, 100, 3, 0, 4000, 7000, 11000, 11, 54978, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Sladran Viper - Casts Venomous Bite(Normal)'),
(2968002, 29680, 0, 0, 100, 5, 0, 4000, 7000, 11000, 11, 58996, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Sladran Viper - Casts Venomous Bite(Heroic)');



-- Achievements
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7363') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7319') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7136') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7583') AND (`type`='11');