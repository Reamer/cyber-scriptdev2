DELETE FROM script_texts WHERE entry BETWEEN -1000574 AND -1000571;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1000571,'Help! I\'ve only one hand to defend myself with.',0,0,0,0,'SAY_MIRAN_1'),
(-1000572,'Feel the power of the Dark Iron Dwarves!',0,0,0,0,'SAY_DARK_IRON_DWARF'),
(-1000573,'Send them on! I\'m not afraid of some scrawny beasts!',0,0,0,0,'SAY_MIRAN_2'),
(-1000574,'Ah, here at last! It\'s going to feel so good to get rid of these barrels.',0,0,0,0,'SAY_MIRAN_3');

DELETE FROM script_waypoint WHERE entry=1379;
INSERT INTO script_waypoint VALUES
(1379,01,-5751.12,-3441.01,301.743,0,''),
(1379,02,-5738.58,-3485.14,302.410,0,''),
(1379,03,-5721.62,-3507.85,304.011,0,''),
(1379,04,-5710.21,-3527.97,304.708,0,''),
(1379,05,-5706.92,-3542.89,304.871,0,''),
(1379,06,-5701.53,-3551.24,305.962,0,''),
(1379,07,-5699.53,-3555.69,306.505,0,''),
(1379,08,-5690.56,-3571.98,309.035,0,''),
(1379,09,-5678.61,-3587.17,310.607,0,''),
(1379,10,-5677.05,-3594.35,311.527,0,''),
(1379,11,-5674.39,-3605.19,312.239,0,''),
(1379,12,-5674.45,-3614.39,312.337,0,''),
(1379,13,-5673.05,-3630.56,311.105,0,''),
(1379,14,-5680.34,-3645.44,315.185,0,''),
(1379,15,-5684.46,-3650.05,314.687,0,''),
(1379,16,-5693.9,-3674.14,313.03,0,''),
(1379,17,-5701.43,-3712.54,313.959,0,''),
(1379,18,-5698.79,-3720.88,316.943,0,''),
(1379,19,-5699.95,-3733.63,318.597,0,'Protecting the Shipment - Ambush'),
(1379,20,-5698.61,-3754.74,322.047,0,''),
(1379,21,-5688.68,-3769,323.957,0,''),
(1379,22,-5688.14,-3782.65,322.667,0,''),
(1379,23,-5699.23,-3792.65,322.448,30000,'Protecting the Shipment - End'),
(1379,24,-5700.80,-3792.78,322.588,0,'');

