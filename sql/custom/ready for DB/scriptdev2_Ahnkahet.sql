-- fixes for ahn'kahet

-- texts for boss herals volazj
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1619044 AND -1619033;
INSERT INTO `script_texts` (`entry`,`content_default`,`sound`,`type`,`language`,`comment`) VALUES 
(-1619033,"Shgla\'yos plahf mh\'naus.",0,0,0,"volazj SAY_AGGRO"),
(-1619034,"Gul\'kafh an\'shel. Yoq\'al shn ky ywaq nuul.",0,0,0,"volazj SAY_INSANITY"),
(-1619035,"Ywaq puul skshgn: on'ma yeh'glu zuq.",0,0,0,"volazj SAY_SLAY_1"),
(-1619036,"Ywaq ma phgwa\'cul hnakf.",0,0,0,"volazj SAY_SLAY_2"),
(-1619037,"Ywaq maq oou; ywaq maq ssaggh. Ywaq ma shg\'fhn.",0,0,0,"volazj SAY_SLAY_3"),
(-1619038,"Iilth vwah, uhn'agth fhssh za.",0,0,0,"volazj SAY_DEATH"),
(-1619039,"They who dine on lost souls know only hunger.",0,4,0,"volazj WHISPER_AGGRO"),
(-1619040,"Gaze into the void. It is the perpetuity in which they dwell.",0,4,0,"volazj WHISPER_INSANITY"),
(-1619041,"They drink your fear: it is the blood of life.",0,4,0,"volazj WHISPER_SLAY_1"),
(-1619042,"They are the whisper on the shivering wind.",0,4,0,"volazj WHISPER_SLAY_2"),
(-1619043,"They do not die; they do not live. They are outside the cycle.",0,4,0,"volazj WHISPER_SLAY_3"),
(-1619044,"Where one falls, many shall take its place.",0,4,0,"volazj WHISPER_DEATH");
