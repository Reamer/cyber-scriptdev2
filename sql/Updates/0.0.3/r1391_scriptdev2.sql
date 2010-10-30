DELETE FROM script_texts WHERE entry BETWEEN -1000531 AND -1000511;
INSERT INTO script_texts (entry,content_default,sound,type,language,emote,comment) VALUES
(-1000511,'Aright, listen up! Form a circle around me and move out!',0,0,0,0,'letoll SAY_LE_START'),
(-1000512,'Aright, $r, just keep us safe from harm while we work. We\'ll pay you when we return.',0,0,0,0,'letoll SAY_LE_KEEP_SAFE'),
(-1000513,'The dig site is just north of here.',0,0,0,0,'letoll SAY_LE_NORTH'),
(-1000514,'We\'re here! Start diggin\'!',0,0,0,0,'letoll SAY_LE_ARRIVE'),
(-1000515,'I think there\'s somethin\' buried here, beneath the sand!',0,0,0,0,'letoll SAY_LE_BURIED'),
(-1000516,'Almost got it!',0,0,0,0,'letoll SAY_LE_ALMOST'),
(-1000517,'By brann\'s brittle bananas! What is it!? It... It looks like a drum.',0,0,0,0,'letoll SAY_LE_DRUM'),
(-1000518,'Wow... a drum.',0,0,0,0,'letoll SAY_LE_DRUM_REPLY'),
(-1000519,'This discovery will surely rock the foundation of modern archaeology.',0,0,0,0,'letoll SAY_LE_DISCOVERY'),
(-1000520,'Yea, great. Can we leave now? This desert is giving me hives.',0,0,0,0,'letoll SAY_LE_DISCOVERY_REPLY'),
(-1000521,'Have ye gone mad? You expect me to leave behind a drum without first beatin\' on it? Not this son of Ironforge! No sir!',0,0,0,0,'letoll SAY_LE_NO_LEAVE'),
(-1000522,'This reminds me of that one time where you made us search Silithus for evidence of sand gnomes.',0,0,0,0,'letoll SAY_LE_NO_LEAVE_REPLY1'),
(-1000523,'Or that time when you told us that you\'d discovered the cure for the plague of the 20th century. What is that even? 20th century?',0,0,0,0,'letoll SAY_LE_NO_LEAVE_REPLY2'),
(-1000524,'I don\'t think it can top the one time where he told us that he\'d heard that Artha\'s "cousin\'s" skeleton was frozen beneath a glacier in Winterspring. I\'ll never forgive you for that one, Letoll. I mean honestly... Artha\'s cousin?',0,0,0,0,'letoll SAY_LE_NO_LEAVE_REPLY3'),
(-1000525,'I dunno. It can\'t possibly beat the time he tried to convince us that we\'re all actually a figment of some being\'s imagination and that they only use us for their own personal amusement. That went over well during dinner with the family.',0,0,0,0,'letoll SAY_LE_NO_LEAVE_REPLY4'),
(-1000526,'Shut yer yaps! I\'m gonna bang on this drum and that\'s that!',0,0,0,0,'letoll SAY_LE_SHUT'),
(-1000527,'Say, do you guys hear that?',0,0,0,0,'letoll SAY_LE_REPLY_HEAR'),
(-1000528,'IN YOUR FACE! I told you there was somethin\' here!',0,0,0,0,'letoll SAY_LE_IN_YOUR_FACE'),
(-1000529,'Don\'t just stand there! Help him out!',0,0,0,0,'letoll SAY_LE_HELP_HIM'),
(-1000530,'%s picks up the drum.',0,2,0,0,'letoll EMOTE_LE_PICK_UP'),
(-1000531,'You\'ve been a tremendous help, $r! Let\'s get out of here before more of those things show up! I\'ll let Dwarfowitz know you did the job asked of ya\' admirably.',0,0,0,0,'letoll SAY_LE_THANKS');

DELETE FROM script_waypoint WHERE entry=22458;
INSERT INTO script_waypoint VALUES
(22458, 0, -3739.907959, 5393.691895, -4.213, 5000, 'SAY_LE_KEEP_SAFE'),
(22458, 1, -3733.334229, 5389.243164, -5.331, 0, ''),
(22458, 2, -3728.771729, 5385.649414, -3.704, 0, ''),
(22458, 3, -3717.267090, 5379.179199, -4.400, 0, ''),
(22458, 4, -3705.626465, 5379.261719, -7.711, 0, ''),
(22458, 5, -3688.279541, 5379.716309, -9.400, 0, ''),
(22458, 6, -3649.186523, 5389.111816, -11.917, 0, ''),
(22458, 7, -3612.791504, 5392.812500, -13.655, 0, ''),
(22458, 8, -3574.865479, 5412.704590, -16.543, 0, ''),
(22458, 9, -3564.438232, 5422.615723, -16.104, 0, ''),
(22458, 10, -3553.387695, 5444.732910, -12.184, 2500, 'arivve dig site SAY_LE_ARRIVE'),
(22458, 11, -3557.291016, 5465.319336, -9.282, 7500, 'dig 1'),
(22458, 12, -3548.102051, 5453.417969, -12.282, 10000, 'dig 2 SAY_LE_BURIED pause'),
(22458, 13, -3556.580322, 5446.475098, -11.920, 0, 'start returning'),
(22458, 14, -3564.438232, 5422.615723, -16.104, 0, ''),
(22458, 15, -3574.865479, 5412.704590, -16.543, 0, ''),
(22458, 16, -3612.791504, 5392.812500, -13.655, 0, ''),
(22458, 17, -3649.186523, 5389.111816, -11.917, 0, ''),
(22458, 18, -3688.279541, 5379.716309, -9.400, 0, ''),
(22458, 19, -3705.626465, 5379.261719, -7.711, 0, ''),
(22458, 20, -3717.267090, 5379.179199, -4.400, 0, ''),
(22458, 21, -3728.771729, 5385.649414, -3.704, 0, ''),
(22458, 22, -3733.334229, 5389.243164, -5.331, 0, ''),
(22458, 23, -3739.907959, 5393.691895, -4.213, 0, '');
