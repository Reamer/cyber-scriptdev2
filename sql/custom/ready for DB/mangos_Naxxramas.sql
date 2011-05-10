-- Achievement
# faerlina
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7265') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7549') AND (`type`='11');
# heigan
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7264') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7548') AND (`type`='11');
# loatheb
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7612') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7613') AND (`type`='11');
# thaddius
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7604') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7605') AND (`type`='11');
# sapphiron
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7567') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7568') AND (`type`='11');
# kelthuzad
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7614') AND (`type`='11');
UPDATE `achievement_criteria_requirement` SET `type`='18' WHERE (`criteria_id`='7615') AND (`type`='11');

# generell
DELETE FROM `achievement_criteria_requirement` WHERE criteria_id IN (13235, 13234, 7616, 13236, 13233, 13240, 13239, 7617, 13238, 13237);
INSERT INTO `achievement_criteria_requirement` VALUES
-- 25mann
(13235, 12 , 1, 0),
(13235, 18 , 0, 0),
(13234, 12 , 1, 0),
(13234, 18 , 0, 0),
( 7616, 12 , 1, 0),
( 7616, 18 , 0, 0),
(13236, 12 , 1, 0),
(13236, 18 , 0, 0),
(13233, 12 , 1, 0),
(13233, 18 , 0, 0),
-- 10mann
(13240, 12 , 0, 0),
(13240, 18 , 0, 0),
(13239, 12 , 0, 0),
(13239, 18 , 0, 0),
( 7617, 12 , 0, 0),
( 7617, 18 , 0, 0),
(13238, 12 , 0, 0),
(13238, 18 , 0, 0),
(13237, 12 , 0, 0),
(13237, 18 , 0, 0);


-- ********PORTALS*************
DELETE FROM spell_target_position WHERE id = 28444;
INSERT INTO spell_target_position VALUES (28444, 533, 2956.474609, -3434.322266,299.549896, 0.068890);
UPDATE gameobject SET spawnMask = 3, flags = 48WHERE gameobject.id IN (181575,181576,181577,181578);