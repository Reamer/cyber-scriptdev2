-- portale obsidiansanktum
DELETE FROM gameobject_template WHERE gameobject_template.entry = 193988;
INSERT INTO gameobject_template (entry, type, displayId,name,faction, flags, size,data0)
VALUES
(193988,22,1327,'Twilight Portal',35,0,2,57620);

DELETE FROM gameobject WHERE gameobject.id = 193988;


-- Fly-Modus for Subboss
UPDATE `creature_template` SET `InhabitType`='7' WHERE (`entry`='30449');
UPDATE `creature_template` SET `InhabitType`='7' WHERE (`entry`='30451');
UPDATE `creature_template` SET `InhabitType`='7' WHERE (`entry`='30452');

UPDATE `creature_template` SET `InhabitType`='7' WHERE (`entry`='31535');
UPDATE `creature_template` SET `InhabitType`='7' WHERE (`entry`='31520');
UPDATE `creature_template` SET `InhabitType`='7' WHERE (`entry`='31534');

-- Obsidian sanctum (from PSZ and MaxXx2021)
UPDATE creature_template SET AIName = "", ScriptName='mob_fire_cyclone' WHERE entry = 30648;
UPDATE creature_template SET AIName = "", ScriptName='mob_flame_tsunami' WHERE entry = 30616;
UPDATE creature_template SET AIName = "", ScriptName='mob_lava_blaze' WHERE entry = 30643;

-- CUSTOM hack to different hard mode loot
--  DELETE FROM creature_template WHERE entry IN (288601, 288602, 288603, 313111, 313112, 313113)
-- REPLACE INTO creature_template (entry, difficulty_entry_1, modelid_1, modelid_3, faction_A, faction_H, NAME, subname, minhealth, maxhealth, rank, lootid) VALUES
-- (288601, 313111, 27035, 27035, 103, 103, 'Sartharion', 'The Onyx Guardian', 1, 1, 3, 288601),
-- (288602, 313112, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 288602),
-- (288603, 313113, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 288603),
-- (313111, 0, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 313111),
-- (313112, 0, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 313112),
-- (313113, 0, 27035, 27035, 103, 103,  'Sartharion', 'The Onyx Guardian', 1, 1, 3, 313113);

-- boss loot
DELETE FROM creature_loot_template WHERE entry IN (30452, 30451, 30449, 28860, 288601, 288602, 288603, 31534, 31520, 31535, 31311 ,313111, 313112, 313113);
-- Tenebron, Shadron, Vesperon
-- UPDATE creature_template SET lootid=30449 WHERE entry IN (30452, 30451, 30449, 31534, 31520, 31535);
-- INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
-- (30449, 40753, 100, 0, 1, 1, 0, 0, 0);
-- Sartharion
-- references
DELETE FROM reference_loot_template WHERE entry IN (615000, 615001, 615002, 615010, 615011, 615012);
-- INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, groupid, mincountOrRef, maxcount, lootcondition, condition_value1, condition_value2) VALUES
-- normal, no drakes
(28860, 40426, 0, 2, 1, 1, 0, 0, 0),
(28860, 40427, 0, 2, 1, 1, 0, 0, 0),
(28860, 40428, 0, 2, 1, 1, 0, 0, 0),
(28860, 40429, 0, 2, 1, 1, 0, 0, 0),
(28860, 40430, 0, 2, 1, 1, 0, 0, 0),
(28860, 40613, 0, 1, 1, 1, 0, 0, 0),
(28860, 40614, 0, 1, 1, 1, 0, 0, 0),
(28860, 40615, 0, 1, 1, 1, 0, 0, 0),
(28860, 43345, 100, 0, 1, 1, 0, 0, 0),
(28860, 43347, 100, 0, 1, 1, 0, 0, 0),
(28860, 40753, 100, 0, 4, 4, 0, 0, 0), -- marks
-- normal, 1 drake
(28860, 43992, 0, 3, 1, 1, 18, 615, 1),
(28860, 43988, 0, 3, 1, 1, 18, 615, 1),
(28860, 43990, 0, 3, 1, 1, 18, 615, 1),
(28860, 43989, 0, 3, 1, 1, 18, 615, 1),
(28860, 43991, 0, 3, 1, 1, 18, 615, 1),
-- normal, 2 drakes
(28860, 43995, 0, 4, 1, 1, 18, 615, 2),
(28860, 43998, 0, 4, 1, 1, 18, 615, 2),
(28860, 43994, 0, 4, 1, 1, 18, 615, 2),
(28860, 43996, 0, 4, 1, 1, 18, 615, 2),
(28860, 43993, 0, 4, 1, 1, 18, 615, 2),
-- normal, 3 drakes
(28860, 43986, 100, 5, 1, 1, 18, 615, 3),


-- heroic, no drakes
(31311, 40431, 0, 2, 1, 1, 0, 0, 0),
(31311, 40432, 0, 3, 1, 1, 0, 0, 0),
(31311, 40433, 0, 3, 1, 1, 0, 0, 0),
(31311, 40437, 0, 2, 1, 1, 0, 0, 0),
(31311, 40438, 0, 3, 1, 1, 0, 0, 0),
(31311, 40439, 0, 3, 1, 1, 0, 0, 0),
(31311, 40446, 0, 2, 1, 1, 0, 0, 0),
(31311, 40451, 0, 2, 1, 1, 0, 0, 0),
(31311, 40453, 0, 3, 1, 1, 0, 0, 0),
(31311, 40455, 0, 2, 1, 1, 0, 0, 0),
(31311, 40628, 0, 1, 1, 1, 0, 0, 0),
(31311, 40629, 0, 1, 1, 1, 0, 0, 0),
(31311, 40630, 0, 1, 1, 1, 0, 0, 0),
(31311, 43345, 100, 0, 1, 1, 0, 0, 0),
(31311, 43346, 100, 0, 1, 1, 0, 0, 0),
(31311, 40752, 100, 0, 4, 4, 0, 0, 0), -- marks
-- heroic, 1 drake
(31311, 44003, 0, 4, 1, 1, 18, 615, 1),
(31311, 44002, 0, 4, 1, 1, 18, 615, 1),
(31311, 44000, 0, 4, 1, 1, 18, 615, 1),
(31311, 44004, 0, 4, 1, 1, 18, 615, 1),
-- heroic, 2 drakes
(31311, 44007, 0, 5, 1, 1, 18, 615, 2),
(31311, 44006, 0, 5, 1, 1, 18, 615, 2),
(31311, 44005, 0, 5, 1, 1, 18, 615, 2),
(31311, 44008, 0, 5, 1, 1, 18, 615, 2),
(31311, 44011, 0, 5, 1, 1, 18, 615, 2),
-- heroic, 3 drakes
(31311, 43954, 100, 6, 1, 1, 18, 615, 3);
