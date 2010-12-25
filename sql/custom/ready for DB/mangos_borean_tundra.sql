-- Quest: Drake Hunt (11940/11919)
UPDATE creature_template SET ScriptName='npc_nexus_drake', AIName='' WHERE entry = 26127;

-- Quest: Merciful Freedom (11676)
UPDATE gameobject_template SET ScriptName='go_scourge_cage' WHERE entry IN (187854,187855,187856,187857,187858,187859,187860,187862,187863,187864,187865,187866,187867,187868,187870,187871,187872,187873,187874,187861,190803);

-- Quest: Abduction (11590)
UPDATE creature_template SET
minhealth=6387,
maxhealth=7185,
minlevel=64,
maxlevel=70,
minmana=7031,
maxmana=7196
WHERE entry=25474;

UPDATE creature_template SET
ScriptName='npc_beryl_sorcerer' , AIName=''
WHERE entry=25316;