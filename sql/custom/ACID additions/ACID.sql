-- fix quest 11300 (eventAI timers not blizzlike!)
UPDATE creature_template SET AIName = 'EventAI' WHERE entry IN (24213,24215,24214,23931);
DELETE FROM creature_ai_scripts WHERE creature_id IN (24213,24215,24214,23931);

-- eventAI for NPC 24213 (Firjus the Soul Crusher)
INSERT INTO creature_ai_scripts(id,creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,comment) VALUES 
(2421301,24213,0,0,100,1,1000,2000,5000,8000,11,15284,1,0,'Firjus - Cast Cleave'),
(2421302,24213,0,0,100,1,2500,4000,5000,8000,11,43348,1,0,'Firjus - Cast Head Crush'),
(2421303,24213,6,0,100,0,0,0,0,0,12,24215,0,300000,'Firjus - Spawn Jlarborn On Death'),

-- eventAI for NPC 24215 (Jlarborn the Strategist)
(2421501,24215,11,0,100,1,0,0,0,0,2,35,0,0,'Jlarbor - Change faction to friendly On Spawn'),
(2421502,24215,1,0,100,0,15000,15000,0,0,2,0,0,0,'Jlarbor - Change faction to hostile after 15sec'),
(2421503,24215,4,0,100,1,0,0,0,0,11,19131,1,0,'Jlarbor - Cast Shield Charge on Aggro'),
(2421504,24215,0,0,100,1,1000,2000,8000,15000,11,8374,1,0,'Jlarbor - Cast Arcing Smash'),
(2421505,24215,0,0,100,1,2500,4000,10000,20000,11,38233,1,0,'Jlarbor - Cast Shield Bash'),
(2421506,24215,0,0,100,1,0,0,8000,16000,11,12169,0,0,'Jlarbor - Cast Shield Block'),
(2421507,24215,6,0,100,0,0,0,0,0,12,24214,0,300000,'Jlarbor - Spawn Yorus On Death'),

-- eventAI for NPC 24214 (Yorus the Flesh Harvester)
(2421401,24214,11,0,100,1,0,0,0,0,2,35,0,0,'Yorus - Change faction to friendly On Spawn'),
(2421402,24214,1,0,100,0,15000,15000,0,0,2,0,0,0,'Yorus - Change faction to hostile after 15sec'),
(2421403,24214,0,0,100,1,3000,5000,15000,25000,11,41056,0,0,'Yorus - Cast Whirlwind'),
(2421404,24214,0,0,100,1,2500,6000,6000,15000,11,15284,1,0,'Yorus - Cast Cleave'),
(2421405,24214,6,0,100,0,0,0,0,0,12,23931,0,300000,'Yorus - Spawn Oluf the Violent On Death'),

-- eventAI for NPC 23931 (Oluf the Violent)
(2393101,23931,11,0,100,1,0,0,0,0,2,35,0,0,'Oluf - Change faction to friendly On Spawn'),
(2393102,23931,1,0,100,0,15000,15000,0,0,2,0,0,0,'Oluf - Change faction to hostile after 15sec'),
(2393103,23931,0,0,100,1,5000,7000,11000,15000,11,15284,1,0,'Oluf the Violent - Cleave'),
(2393104,23931,0,0,100,1,5000,10000,30000,35000,11,13730,0,0,'Oluf the Violent - Demoralizing Shout'),
(2393105,23931,9,0,100,1,0,20,7000,9000,11,6533,1,0,'Oluf the Violent - Net'),
(2393106,23931,0,0,100,1,15000,20000,15000,20000,11,42870,1,0,'Oluf the Violent - Throw Dragonflayer Harpoon'),
(2393107,23931,0,0,100,1,11000,14000,14000,19000,11,41056,0,0,'Oluf the Violent - Whirlwind'),
(2393108,23931,6,0,100,0,0,0,0,0,11,43326,0,3,'Oluf - Spawn Ancient Cypher On Death');;

-- Quest Lasst niemanden zurück Zul Drak
update creature_template set AIName = 'EventAI' where entry in (28136, 28142, 28148);
replace into creature_ai_scripts(id, creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(2813601, 28136, 8, 0, 100, 1, 50662, -1, 0, 0, 33, 28136, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Quest 12512 - Jonathan rezz'),
(2814201, 28142, 8, 0, 100, 1, 50662, -1, 0, 0, 33, 28142, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Quest 12512 - Lamoof rezz'),
(2814801, 28148, 8, 0, 100, 1, 50662, -1, 0, 0, 33, 28148, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Quest 12512 - Josephine rezz');

-- 4 Reiter Void zone
UPDATE creature_ai_scripts SET event_flags='7',action2_type='21' WHERE id in (1669701, 1669702);

-- Bring Me The Egg! - Windroc Matriarch
UPDATE creature_template SET AIname = 'EventAI' WHERE entry = 19055;
DELETE FROM creature_ai_scripts where creature_id = 19055;
INSERT INTO creature_ai_scripts(id,creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,action2_type,action2_param1,action2_param2,action2_param3,action3_type,action3_param1,action3_param2,action3_param3,comment) VALUES 
(1905501, 19055, 1, 0, 100, 0, 5000, 5000, 0, 0, 19, 256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Windroc Matriarch - Set Attackable'), 
(1905502, 19055, 0, 0, 100, 1, 10000, 10000, 10000, 15000, 11, 30285, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Windroc Matriarch - Eagle Claw'), 
(1905503, 19055, 0, 0, 100, 1, 10000, 15000, 15000, 20000, 11, 32914, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Windroc Matriarch - Wing Buffet');

-- Hyjal Boss Fixes in Event_AI
DELETE from creature_ai_scripts where creature_id in (17767, 17808, 17888, 17842, 17968, 17818, 17864, 17897, 17898, 17905, 17906, 17907, 17908, 17916);

INSERT INTO creature_ai_scripts(id,creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,action2_type,action2_param1,action2_param2,action2_param3,action3_type,action3_param1,action3_param2,action3_param3,comment) VALUES
-- Winterchill - Frostnova fix und Timer verkürzt = öfter casts 
(1776701,17767,4,0,100,2,0,0,0,0,1,-125,0,0,34,11,1,0,0,0,0,0,'Rage Winterchill - Yell and Set Inst Data on Aggro'),
(1776702,17767,0,0,100,3,10000,15000,10000,15000,11,31256,0,0,0,0,0,0,0,0,0,0,'Rage Winterchill - Cast Frost Armor'),
(1776703,17767,0,0,100,3,35000,35000,30000,40000,11,31258,4,0,1,-128,-129,0,0,0,0,0,'Rage Winterchill - Cast Death and Decay with Yells'),
(1776704,17767,0,0,100,3,15000,20000,15000,20000,11,31250,0,4,1,-126,-127,0,0,0,0,0,'Rage Winterchill - Cast Frost Nova with Yells'),
(1776705,17767,0,0,100,3,10000,15000,5000,10000,11,31249,4,0,0,0,0,0,0,0,0,0,'Rage Winterchill - Cast Icebolt'),
(1776706,17767,0,0,100,3,600000,600000,300000,300000,11,26662,0,0,0,0,0,0,0,0,0,0,'Rage Winterchill - Enrage in 10 minutes'),
(1776707,17767,5,0,100,3,5000,5000,0,0,1,-130,-131,-112,0,0,0,0,0,0,0,0,'Rage Winterchill - Random Yell on Player Kill'),
(1776708,17767,6,0,100,2,0,0,0,0,1,-132,0,0,34,11,3,0,0,0,0,0,'Rage Winterchill - Yell and Set Inst Data on Death'),
(1776709,17767,7,0,100,2,0,0,0,0,34,11,0,0,0,0,0,0,0,0,0,0,'Rage Winterchill - Set Inst Data on Evade'),
-- Anetheron - Timer verkürzt
(1780801,17808,4,0,100,2,0,0,0,0,1,-217,0,0,34,2,1,0,0,0,0,0,'Anetheron - Yell and Set Inst Data on Aggro'),
(1780802,17808,0,0,100,3,20000,25000,20000,25000,11,31306,4,0,1,-222,-223,0,0,0,0,0,'Anetheron - Cast Carrion Swarm and Random Yell'),
(1780803,17808,0,0,100,3,25000,30000,15000,20000,11,31298,4,7,1,-224,-225,0,0,0,0,0,'Anetheron - Cast Sleep and Random Yell'),
(1780804,17808,0,0,100,2,0,0,0,0,11,31317,0,6,0,0,0,0,0,0,0,0,'Anetheron - Cast Vampiric Aura'),
(1780805,17808,0,0,100,3,45000,60000,30000,45000,11,31299,4,1,1,-226,-227,0,0,0,0,0,'Anetheron - Cast Inferno and Random Yell'),
(1780806,17808,0,0,100,3,600000,600000,300000,300000,11,26662,0,0,0,0,0,0,0,0,0,0,'Anetheron - Enrage After 10 Minutes'),
(1780807,17808,5,0,100,3,3000,3000,0,0,1,-218,-219,-220,0,0,0,0,0,0,0,0,'Anetheron - Yell on Player Kill'),
(1780808,17808,6,0,100,2,0,0,0,0,1,-221,0,0,34,2,3,0,0,0,0,0,'Anetheron - Yell and Set Inst Data on Death'),
(1780809,17808,7,0,100,2,0,0,0,0,34,2,0,0,0,0,0,0,0,0,0,0,'Anetheron - Set Inst Data on Evade'),
-- Towering Infernal summoned by Anetheron
(1781801, 17818, 11, 0, 100, 2, 0, 0, 0, 0, 11, 31302, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Towering Infernal - Cast Inferno Effect on Spawn'),
(1781802, 17818, 0, 0, 100, 2, 1000, 1000, 0, 0, 11, 31304, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 'Towering Infernal - Cast Immolation aura on Aggro'),
-- Kaz´rogal - Timer verkürzt
(1788801,17888,4,0,100,2,0,0,0,0,1,-570,0,0,34,9,1,0,0,0,0,0,'Kaz''rogal - Yell and Set Inst Data on Aggro'),
(1788802,17888,0,0,100,3,5000,5000,5000,10000,11,31436,1,0,0,0,0,0,0,0,0,0,'Kaz''rogal - Cast Malevolent Cleave'),
(1788803,17888,0,0,100,3,15000,15000,10000,15000,11,31480,0,4,0,0,0,0,0,0,0,0,'Kaz''rogal - Cast War Stomp'),
(1788804,17888,0,0,100,3,6000,6000,10000,12000,11,31477,4,4,0,0,0,0,0,0,0,0,'Kaz''rogal - Cast Cripple'),
(1788805,17888,0,0,100,2,45000,45000,0,0,11,31447,0,4,1,-571,-572,0,0,0,0,0,'Kaz''rogal - First Cast Mark Of Kaz''rogal and Random Yells'),
(1788806,17888,0,0,100,2,85000,85000,0,0,11,31447,0,4,1,-571,-572,0,0,0,0,0,'Kaz''rogal - Second Cast Mark Of Kaz''rogal and Random Yells'),
(1788807,17888,0,0,100,2,120000,120000,0,0,11,31447,0,4,1,-571,-572,0,0,0,0,0,'Kaz''rogal - Third Cast Mark Of Kaz''rogal and Random Yells'),
(1788808,17888,0,0,100,2,150000,150000,0,0,11,31447,0,4,1,-571,-572,0,0,0,0,0,'Kaz''rogal - Fourth Cast Mark Of Kaz''rogal and Random Yells'),
(1788809,17888,0,0,100,2,175000,175000,0,0,11,31447,0,4,1,-571,-572,0,0,0,0,0,'Kaz''rogal - Fifth Cast Mark Of Kaz''rogal and Random Yells'),
(1788810,17888,0,0,100,2,195000,195000,0,0,11,31447,0,4,1,-571,-572,0,0,0,0,0,'Kaz''rogal - Sixth Cast Mark Of Kaz''rogal and Random Yells'),
(1788811,17888,0,0,100,3,210000,210000,10000,10000,11,31447,0,4,1,-571,-572,0,0,0,0,0,'Kaz''rogal - Repeatable Cast Mark Of Kaz''rogal and Random Yells'),
(1788812,17888,5,0,100,3,0,0,0,0,1,-573,-574,-575,0,0,0,0,0,0,0,0,'Kaz''rogal - Yell on Player Kill'),
(1788813,17888,7,0,100,2,0,0,0,0,34,9,0,0,0,0,0,0,0,0,0,0,'Kaz''rogal - Set Inst Data on Evade'),
(1788814,17888,6,0,100,2,0,0,0,0,4,11018,0,0,34,9,3,0,0,0,0,0,'Kaz''rogal - Yell and Set Inst Data on Death'),
-- Azgalor - Timer verkürzt
(1784201,17842,4,0,100,2,0,0,0,0,1,-117,0,0,34,6,1,0,0,0,0,0,'Azgalor - Yell and Set Inst Data on Aggro (After 8th Wave)'),
(1784202,17842,0,0,100,3,20000,25000,15000,20000,11,31340,4,0,0,0,0,0,0,0,0,0,'Azgalor - Cast Rain of Fire'),
(1784203,17842,0,0,100,3,45000,45000,30000,45000,11,31347,5,0,1,-119,-199,0,0,0,0,0,'Azgalor - Cast Doom and Random Yell'),
(1784204,17842,0,0,100,3,15000,15000,8000,13000,11,31344,0,0,0,0,0,0,0,0,0,0,'Azgalor - Cast Howl of Azgalor'),
(1784205,17842,0,0,100,3,10000,10000,5000,10000,11,31345,1,0,0,0,0,0,0,0,0,0,'Azgalor - Cast Cleave'),
(1784206,17842,0,0,100,3,600000,600000,300000,300000,11,26662,0,0,0,0,0,0,0,0,0,0,'Azgalor - Enrage in 10 Minutes'),
(1784207,17842,5,0,100,2,0,0,0,0,1,-118,-120,-122,0,0,0,0,0,0,0,0,'Azgalor - Random Yell on Player Kill'),
(1784208,17842,7,0,100,2,0,0,0,0,34,6,0,0,0,0,0,0,0,0,0,0,'Azgalor - Set Inst Data on Evade'),
(1784209,17842,6,0,100,2,0,0,0,0,34,6,3,0,1,-121,0,0,12,13083,0,35000,'Azgalor - Set Inst Data and Yell and Spawn Echo of Archimonde on Death'),
-- Lesser Doomguard spawned after doomed player died
(1786401,17864,0,0,100,3,5000,6000,10000,20000,11,31406,4,4,0,0,0,0,0,0,0,0,'Lesser Doomguard - Cast Cripple'),
(1786402,17864,0,0,100,3,7000,8000,10000,15000,11,31408,0,4,0,0,0,0,0,0,0,0,'Lesser Doomguard - Cast War Stomp'),

-- Hyjal Trash Mobs AI
-- Crypt Fiend
('1789701','17897','0','0','100','3','15000','25000','15000','20000','11','28991','1','0','0','0','0','0','0','0','0','0','Crypt Fiend - Cast Web'),
('1789702','17897','6','0','100','2','0','0','0','0','34','14','0','0','0','0','0','0','0','0','0','0','Crypt Fiend - On Death Set Instance Data'),
-- Abomination
('1789801','17898','0','0','100','3','10000','25000','20000','30000','11','31610','1','0','0','0','0','0','0','0','0','0','Abomination - Cast Knockdown'),
('1789802','17898','0','0','100','2','0','0','1000','1000','11','31607','0','0','0','0','0','0','0','0','0','0','Abomination - Apply Disease Cloud'),
('1789803','17898','6','0','100','2','0','0','0','0','34','14','0','0','0','0','0','0','0','0','0','0','Abomination - On Death Set Instance Data'),
-- Banshee
('1790501','17905','0','0','100','3','15000','20000','15000','20000','11','31651','1','0','0','0','0','0','0','0','0','0','Banshee - Cast Banshee Curse'),
('1790502','17905','0','0','100','3','10000','10000','15000','15000','11','38183','1','0','0','0','0','0','0','0','0','0','Banshee - Cast Banshee Wail'),
('1790503','17905','0','0','100','3','30000','60000','30000','60000','11','31662','0','0','0','0','0','0','0','0','0','0','Banshee - Cast Anti-Magic Shell'),
('1790504','17905','6','0','100','2','0','0','0','0','34','14','0','0','0','0','0','0','0','0','0','0','Banshee - On Death Set Instance Data'),
-- Gargoyle
('1790601','17906','0','0','100','3','10000','15000','10000','15000','11','31664','1','0','0','0','0','0','0','0','0','0','Gargoyle - Cast Gargoyle Strike'),
('1790602','17906','6','0','100','2','0','0','0','0','34','14','0','0','0','0','0','0','0','0','0','0','Gargoyle - On Death Set Instance Data'),
-- Frost Wyrm
('1790701','17907','0','0','100','3','10000','15000','10000','15000','11','31688','1','0','0','0','0','0','0','0','0','0','Frost Wyrm - Cast Frost Breath'),
('1790702','17907','6','0','100','2','0','0','0','0','34','14','0','0','0','0','0','0','0','0','0','0','Frost Wyrm - On Death Set Instance Data'),
-- Giant Infernal
('1790802','17908','0','0','100','2','1000','5000','0','0','11','37059','0','0','0','0','0','0','0','0','0','0','Giant Infernal - Cast Immolation'),
('1790803','17908','6','0','100','2','0','0','0','0','34','14','0','0','0','0','0','0','0','0','0','0','Giant Infernal - On Death Set Instance Data'),
-- Fel Stalker
('1791601','17916','0','0','100','3','9000','15000','8000','12000','11','31729','4','0','0','0','0','0','0','0','0','0','Fel Stalker - Cast Mana Burn'),
('1791602','17916','6','0','100','2','0','0','0','0','34','14','0','0','0','0','0','0','0','0','0','0','Fel Stalker - On Death Set Instance Data');

-- Dudu Epicfliegen Quest
delete from creature_ai_scripts where creature_id in (22993, 22994, 22992);
INSERT INTO creature_ai_scripts(id,creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,action2_type,action2_param1,action2_param2,action2_param3,action3_type,action3_param1,action3_param2,action3_param3,comment) VALUES 
(2299301,22993,11,0,100,0,0,0,0,0,21,0,0,0,20,0,0,0,18,2,0,0,'Guardian of the Eagle - Set Unattackable'),
(2299302,22993,11,0,100,0,0,0,0,0,5,27,0,0,0,0,0,0,0,0,0,0,'Guardian of the Eagle - Emote Ready Unarmed'),
(2299303,22993,1,0,100,0,1000,1000,0,0,1,-240,-1,0,0,0,0,0,0,0,0,0,'Guardian of the Eagle - Say 1'),
(2299304,22993,1,0,100,0,7500,7500,0,0,1,-241,-1,0,0,0,0,0,0,0,0,0,'Guardian of the Eagle - Say 2'),
(2299305,22993,1,0,100,0,12000,12000,0,0,21,1,0,0,20,1,0,0,19,2,0,0,'Guardian of the Eagle - Set Attackable'),
(2299306,22993,1,0,100,0,12000,12000,0,0,5,0,0,0,0,0,0,0,0,0,0,0,'Guardian of the Eagle - Remove Emote'),
(2299307,22993,0,0,100,1,15000,15000,7000,7000,11,9739,1,0,0,0,0,0,0,0,0,0,'Guardian of the Eagle - Cast Wrath'),
(2299308,22993,0,0,100,1,18000,18000,7000,7000,11,40339,1,0,0,0,0,0,0,0,0,0,'Guardian of the Eagle - Summon Eagle Add'),
(2299309,22993,0,0,100,1,24000,24000,30000,30000,11,40344,1,0,0,0,0,0,0,0,0,0,'Guardian of the Eagle - Cast Starfire'),

(2299401,22994,11,0,100,0,0,0,0,0,21,0,0,0,20,0,0,0,18,2,0,0,'Guardian of the Falcon - Set Unattackable'),
(2299402,22994,11,0,100,0,0,0,0,0,5,27,0,0,0,0,0,0,0,0,0,0,'Guardian of the Falcon - Emote Ready Unarmed'),
(2299403,22994,1,0,100,0,0,0,1000,1000,1,-242,-1,0,0,0,0,0,0,0,0,0,'Guardian of the Falcon - Say 1'),
(2299404,22994,1,0,100,0,0,0,7500,7500,1,-243,-1,0,0,0,0,0,0,0,0,0,'Guardian of the Falcon - Say 2'),
(2299405,22994,1,0,100,0,0,0,12000,12000,21,1,0,0,20,1,0,0,19,2,0,0,'Guardian of the Falcon - Set Attackable'),
(2299406,22994,1,0,100,0,0,0,12000,12000,5,0,0,0,0,0,0,0,0,0,0,0,'Guardian of the Falcon - Remove Emote'),
(2299407,22994,0,0,100,1,15000,20000,15000,20000,11,40367,1,0,0,0,0,0,0,0,0,0,'Guardian of the Falcon - Cast Twisting Blast'),
(2299408,22994,0,0,100,1,20000,25000,20000,25000,11,9613,1,0,0,0,0,0,0,0,0,0,'Guardian of the Falcon - Cast Shadow Bolt'),

(2299201,22992,11,0,100,0,0,0,0,0,21,0,0,0,20,0,0,0,18,2,0,0,'Guardian of the Hawk - Set Unattackable'),
(2299202,22992,11,0,100,0,0,0,0,0,5,27,0,0,0,0,0,0,0,0,0,0,'Guardian of the Hawk - Emote Ready Unarmed'),
(2299203,22992,1,0,100,0,0,0,1000,1000,1,-238,-1,0,0,0,0,0,0,0,0,0,'Guardian of the Hawk - Say 1'),
(2299204,22992,1,0,100,0,0,0,7500,7500,1,-239,-1,0,0,0,0,0,0,0,0,0,'Guardian of the Hawk - Say 2'),
(2299205,22992,1,0,100,0,0,0,12000,12000,21,1,0,0,20,1,0,0,19,2,0,0,'Guardian of the Hawk - Set Attackable'),
(2299206,22992,1,0,100,0,0,0,12000,12000,5,0,0,0,0,0,0,0,0,0,0,0,'Guardian of the Hawk - Remove Emote'),
(2299207,22992,0,0,100,1,20000,20000,120000,120000,11,40412,1,0,0,0,0,0,0,0,0,0,'Guardian of the Hawk - Cast Curse of Blood');


-- Cyber Türsteher
update creature_template set AIName = 'EventAI' where entry in (670070);
REPLACE INTO creature_ai_scripts(id,creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,action2_type,action2_param1,action2_param2,action2_param3,action3_type,action3_param1,action3_param2,action3_param3,comment) VALUES 
(67007001,670070,4,0,100,6,0,0,0,0,1,-71,0,0,11,55799,0,6,0,0,0,0,'Arthur - Yell and Frost Aura'),
(67007002,670070,9,0,100,7,0,20,1000,3000,0,0,0,0,11,53534,4,1,0,0,0,0,'Arthur - Yell and Ice Chains'),
(67007003,670070,0,0,100,7,5000,10000,10000,15000,1,-931,0,0,11,66019,1,4,0,0,0,0,'Arthur - Yell and Death Coil'),
(67007004,670070,0,0,100,7,25000,30000,25000,30000,1,-918,0,0,11,45657,0,4,0,0,0,0,'Arthur - Yell and Ultimate');

-- Körper und Geist Dudu Quest fix. Lässt den Geist von Mondklaue spawnen
update creature_ai_scripts set action1_type = '12', action1_param1 = '12144', action1_param3 = '30000' where id = '1213802';

-- Ahnkaheb
UPDATE `creature_template` SET `AIName` = "EventAI" WHERE `entry` IN (30178);
DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (30176,30178);
INSERT INTO `creature_ai_scripts` (`id`,`creature_id`,`event_type`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action1_type`,`action1_param1`,`action1_param2`,`action1_param3`,`comment`) VALUES 
(3017801,30178,0,100,7,0,500,25000,40000,11,56354,0,2,"Ahn'kahar Swarmer - Cast Sprint");

-- Azjol Nerub
DELETE FROM creature_ai_scripts WHERE creature_id=105000;
INSERT INTO creature_ai_scripts (id, creature_id, event_type, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, comment) VALUES
    (10500001, 105000, 0, 100, 2, 2500, 2700, 50000, 60000, 11, 53454, 0, 1, 'Impale trigger - Cast Impale (normal mode)'),
    (10500002, 105000, 0, 100, 4, 2500, 2700, 50000, 60000, 11, 59446, 0, 1, 'Impale trigger - Cast Impale (heroic mode)'),
    (10500003, 105000, 1, 100, 2, 2500, 2700, 50000, 60000, 11, 53454, 0, 1, 'Impale trigger - Cast Impale OOC (normal mode)'),
    (10500004, 105000, 1, 100, 4, 2500, 2700, 50000, 60000, 11, 59446, 0, 1, 'Impale trigger - Cast Impale OOC (heroic mode)'),
    (10500005, 105000, 1, 100, 6, 100, 150, 50000, 60000, 11, 42373, 0, 1, 'Impale trigger - Burrow OOC'),
    (10500006, 105000, 0, 100, 6, 100, 150, 50000, 60000, 11, 42373, 0, 1, 'Impale trigger - Burrow'),
    (10500007, 105000, 0, 100, 6, 100, 150, 0, 0, 20, 0, 0, 0, 'Impale trigger - Set no auto attack'),
    (10500008, 105000, 0, 100, 6, 100, 150, 0, 0, 21, 0, 0, 0, 'Impale trigger - Set no combat movement');

-- Gundrak
DELETE FROM `creature_ai_scripts` WHERE (`creature_id`=29680);
INSERT INTO `creature_ai_scripts` VALUES 
( 2968001, 29680, 0, 0, 100, 3, 0, 4000, 7000, 11000, 11, 54978, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Sladran Viper - Casts Venomous Bite(Normal)'),
(2968002, 29680, 0, 0, 100, 5, 0, 4000, 7000, 11000, 11, 58996, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Sladran Viper - Casts Venomous Bite(Heroic)');
