
-- add loot to boss Commander Stoutbeard
DELETE FROM creature_loot_template WHERE creature_loot_template.entry = 30398;

INSERT INTO creature_loot_template
(entry, item,ChanceOrQuestChance, groupid, mincountOrRef, maxcount)
VALUES
(30398, 40752, 100, 0 ,1 ,1),
(30398, 37728, 0, 1, 1, 1),
(30398, 37729, 0, 1, 1, 1),
(30398, 37730, 0, 1, 1, 1),
(30398, 37731, 0, 1, 1, 1);

UPDATE creature_template SET lootid=30398 WHERE entry=30398;