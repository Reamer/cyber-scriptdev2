/* ULDUAR from Xfurry rewrite by Goldi */
DELETE from `script_texts` where `entry` between -1603500 and -1603000;
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc3`, `sound`, `type`, `language`, `emote`, `comment`) VALUES 
-- Leviathan: needs intro & outro
(-1603001,"Threat assessment routine modified. Current target threat level: zero. Acquiring new target.","",15521,1,0,0,"Flame Leviathan Slay"),
(-1603002,"Total systems failure. Defense protocols breached. Leviathan Unit shutting down.","",15520,1,0,0,"Flame Leviathan Death"),
(-1603003,"Hostile entities detected. Threat assessment protocol active. Primary target engaged. Time minus thirty seconds to re-evaluation.","",15506,1,0,0,"Flame Leviathan Aggro"),
(-1603004,"Threat re-evaluated. Target assessment complete. Changing course.","",15507,1,0,0,"Flame Leviathan change1"),
(-1603005,"Pursuit objective modified. Changing course.","",15508,1,0,0,"Flame Leviathan change2"),
(-1603006,"Hostile entity stratagem predicted. Rerouting battle function. Changing course.","",15509,1,0,0,"Flame Leviathan change3"),
(-1603007,"Unauthorized entity attempting circuit overload. Activating anti-personnel countermeasures.","",15516,1,0,0,"Flame Leviathan player on top"),
(-1603008,"System malfunction. Diverting power to support systems.","",15517,1,0,0,"Flame Leviathan overload1"),
(-1603009,"Combat matrix overload. Powering do-o-o-own...","",15518,1,0,0,"Flame Leviathan overload2"),
(-1603010,"System restart required. Deactivating weapon systems.","",15519,1,0,0,"Flame Leviathan overload3"),
(-1603011,"Orbital countermeasures enabled.","",15510,1,0,0,"Flame Leviathan hard mode"),
(-1603012,"Alert! Static defense system failure. Orbital countermeasures disabled.","",15511,1,0,0,"Flame Leviathan towers down"),
(-1603013,"'Hodir's Fury' online. Acquiring target.","",15512,1,0,0,"Flame Leviathan frost"),
(-1603014,"'Mimiron's Inferno' online. Acquiring target.","",15513,1,0,0,"Flame Leviathan fire"),
(-1603015,"'Thorim's Hammer' online. Acquiring target.","",15515,1,0,0,"Flame Leviathan energy"),
(-1603016,"'Freya's Ward' online. Acquiring target.","",15514,1,0,0,"Flame Leviathan nature"),
(-1603017,'PursueWarn' ,"",0,3,0,0,'EMOTE_pursue'),

-- Ignis:
(-1603020, "Insolent whelps! Your blood will temper the weapons used to reclaim this world!", "Ihr anma�enden Wichte! Euer Blut wird die Waffen h�rten, mit denen diese Welt erobert wird!",15564, 1, 0, 0, 'IgnisAggro'),
(-1603021, "Let the inferno consume you!", "Das Inferno wird Euch verschlingen!",15567, 1, 0, 0, 'Ignis Scorch1'),
(-1603022, "BURN! Burn in the makers fire!", "BREEEEEEENT! Brennt in den Flammen des Sch�pfers!",15568, 1, 0, 0, 'Ignis scroch2'),
(-1603023, "I will burn away your impurities!", "Ich werde Euch eure Unreinheiten ausbrennen!",15566, 1, 0, 0, 'Ignis Slagpot'),
(-1603024, 'Ignis the Furnace Master begins to cast Flame Jets!',"Ignis, Meister des Eisenwerks beginnt Flammens�ulen zu beschw�ren! ",0,3,0,0,'EMOTE_FLAME_JETS'),
(-1603025, "Arise, soldiers of the Iron Crucible! The Makers\' will be done!", "Erhebt euch, Soldaten des eisernen Schmelztigels! Der Wille des Sch�pfers wird erf�llt!",15565, 1, 0, 0, 'Ignis summon'),
(-1603026, "More scraps for the scrapheap!", "Mehr Schrott f�r den Schrotthaufen!",15569, 1, 0, 0, 'IgnisSlay1'),
(-1603027, "Your bones will serve as kindling!", "Eure Knochen werden das Feuer sch�ren!",15570, 1, 0, 0, 'IgnisSlay2'),
(-1603028, "Let it be finished!", "Jetzt geht es zu Ende!",15571, 1, 0, 0, 'IgnisBerserk'),
(-1603029, "I. Have. Failed.", "Ich. Habe. Versagt.",15572, 1, 0, 0, 'Ignis death'),

-- razorscale
(-1603030,'Welcome, champions! All of our attempts at grounding her have failed. We could use a hand in bring her down with these harpoon guns.',"Willkommen Helden! All unsere Versuche, die Bestie von dort oben runterzuholen, sind gescheitert. Wir k�nnten Hilfe dabei gebrauchen, sie mit diesen Harpunenkanonen abzuschie�en!",15647,0,0,0,'razorscale intro - commander'),
(-1603031,'Give us a moment to prepare to build the turrets.',"Gebt uns einen Moment Zeit um diese Gesch�tze zu reparieren!",0,1,0,0,'razor aggro 1 - eng'),
(-1603032,'Be on the lookout! Mole machines will be surfacing soon with those nasty Iron dwarves aboard!',"Seid auf der Hut! Es kommen neue Eisenzwerge aus den Maulwurffahrzeugen!",0,1,0,0,'razor aggro 2 - commander'),
(-1603033,'Ready to move out, keep those dwarves off of our backs!',"Wir sind feuerbereit, haltet uns den R�cken frei! ",0,1,0,0,'razor aggro 3 - eng'),
(-1603034,'Move! Quickly! She won�t remain grounded for long.',"Beeilt euch! Sie wird nicht lange am Boden bleiben",15648,1,0,0,'razor ground - commander'),
(-1603035,'Razorscale takes a deep breath...',"Klingenschuppe holt tief Luft!",0,3,0,0,'razor deep breath'),
(-1603036,'Fires out! Lets rebuild those turrets!',"Feuer einstellen! Lasst uns die Gesch�tze reparieren!",0,1,0,0,'razor extinguish fires'),
(-1603037,'Harpoon Turret is ready for use!',"Harpunenkanone ist bereit!",0,3,0,0,'EMOTE_harpoon'),
(-1603038,'Razorscale grounded permanently!',"Klingenschuppe k�mpft am Boden weiter!",0,3,0,0,'EMOTE_ground'),

-- Xt002
(-1603040,"You are bad... Toys... Very... Baaaaad!","Ihr seid b����s����...... Spielzeug�����....B����sss���",15731,1,0,0,"XT-002 Death"),
(-1603041,"I'm tired of these toys. I don't want to play anymore!","Diese Spielzeuge langweilen mich! Ich will jetzt nichtmehr spielen!",15730,1,0,0,"XT-002 Berserk"),
(-1603042,"Time for a new game! My old toys will fight my new toys!","Zeit f�r ein neues Spiel! Meine alten Spielzeuge k�mpfen gegen meine neuen!",15732,1,0,0,"XT-002 Adds"),
(-1603043,"I'm ready to play!","Wir k�nnen weiterspieln!",15726,1,0,0,"XT-002 Heart Closed"),
(-1603044,"So tired. I will rest for just a moment!","Ohhhhhh :O...sooooo m����dddeee. Ich will mich nur einen Moment ausruhen!",15725,1,0,0,"XT-002 Heart Opened"),
(-1603045,"I guess it doesn't bend that way.","Mhm so rum soll man das wohl nicht klicken.",15729,1,0,0,"XT-002 Slay 2"),
(-1603046,"I... I think I broke it.","Ich......glaub ich habe es kaputt gemaaaaachht.",15728,1,0,0,"XT-002 Slay 1"),
(-1603047,"NO! NO! NO! NO! NO!","Nein! Nein! Nein! Nein! Nein! Nein! Nein! *wein*",15727,1,0,0,"XT-002 Tympanic Tantrum"),
(-1603048,"New toys? For me? I promise I won't break them this time!","Neue Spielzeuge? F�r mich? Woah, ich verspreche sie diesesmal nicht zu zerbrechen!",15724,1,0,0,"XT-002 Aggro"),
(-1603049,'XT-002 Deconstructors heart is exposed and leaking energy!' ,"XT-002 Dekonstruktors Herz liegt frei und l�sst Energie entweichen!",0,3,0,0,'EMOTE_expose_heart'),
(-1603050,'XT-002 Deconstructor consumes a scrapbot to repair himself.' ,"XT-002 Dekonstruktor verschlingt einen Schrottbot und repariert sich selbst! ",0,3,0,0,'EMOTE_repair'),

-- Iron Council
-- Molgeim
(-1603060,"Nothing short of total decimation will suffice!","Nur vollst�ndige Dezimierung wird mich zufriedenstellen!",15657,1,0,0,"MolgeimAggro"),
(-1603061, "The legacy of storms shall not be undone...","Das Verm�chtnis des Sturms wird niemals vergehen...",15662, 1, 0, 0, "MolgeimDeath1"),
(-1603062, "What have you gained from my defeat? You are no less doomed, mortals...","Was gewinnt ihr durch meinen Tod? Ihr seid dennoch verdammt, Sterbliche...",15663, 1, 0, 0, 'MolgeimDeath2'),
(-1603063, "Decipher this!","Entziffert das hier!",15660, 1, 0, 0, 'MolgeimDeathRune'),
(-1603064, "Face the lightning surge!","M�ge euch der Blitz durchfahren!",15661, 1, 0, 0, 'MolgeimSummon'),
(-1603065, "The world on suffers yet another insignificant loss!","Ein weiterer, unbedeutender Verlust f�r die Welt!",15658, 1, 0, 0, 'MolgeimSlay1'),
(-1603066, "Death is the price of your arrogance.","Der preis f�r eure Arroganz ist der Tod!",15659, 1, 0, 0, 'MolgeimSlay2'),
(-1603067, "This meeting of the Assembly of Iron is adjourned!","Diese Sitzung der Versammlung des Eisens wird vertagt!",15664, 1, 0, 0, 'MolgeimBerserk'),
-- Steelbreaker
(-1603070, "You will not defeat the Assembly of Iron so easily, invaders!","So leicht werdet ihr die Versammlung des Eisens nicht bezwingen!",15674, 1, 0, 0, 'SteelAggro'),
(-1603071, "My death only serves to hasten your demise.","Mein Tod wird euren Untergang nur beschleunigen!",15678, 1, 0, 0, 'SteelDeath1'),
(-1603072, "Impossible!","Unm�glich!",15679, 1, 0, 0, 'SteelDeath2'),
(-1603073, "So fragile and weak!","So schwach und zerbrechlich!",15675, 1, 0, 0, 'SteelSlay1'),
(-1603074, "Flesh... such a hindrance.","Fleisch...*haha* welch unn�tzes Hindernis!",15676, 1, 0, 0, 'SteelSlay2'),
(-1603075, "You seek the secrets of Ulduar? Then take them!","Ihr sucht die Geheimnisse Ulduars? Dann nehmt sie euch!",15677, 1, 0, 0, 'SteelOverwhelming'),
(-1603076, "This meeting of the Assembly of Iron is adjourned!","Diese Sitzung der Versammlung des Eisens wird vertagt!",15680, 1, 0, 0, 'SteelBerserk'),
-- Brudir
(-1603080, "Whether the world's greatest gnats or the world's greatest heroes, you're still only mortal.","Auch wenn ihr die gr��ten Helden der Welt seid, seid ihr nichts weiter, als Sterbliche!",15684, 1, 0, 0, 'BrundirAggro'),
(-1603081, "Stand still and stare into the light!","Steht still und starrt in das Licht!",15687, 1, 0, 0, 'BrundirWhirl'),
(-1603082, "The power of the storm lives on...","Die macht der Strums wird fortbestehen...",15689, 1, 0, 0, 'BrundirDeath1'),
(-1603083, "You rush headlong into the maw of madness!","Ihr lauft geradewegs in den Schlund des Wahnsinns!",15690, 1, 0, 0, 'BrundirDeath2'),
(-1603084, "A merciful kill!","Ein Gnadensto�!",15685, 1, 0, 0, 'BrundirSlay1'),
(-1603085, "HAH!","HAH!",15686, 1, 0, 0, 'BrundirSlay2'),
(-1603086, "This meeting of the Assembly of Iron is adjourned!","Diese Sitzung der Versammlung des Eisens wird vertagt!",15691, 1, 0, 0, 'BrundirBerserk'),
(-1603087, "Let the storm clouds rise and rain down death from above!","Lasst Sturmwolken aufziehen und den Tod herabregnen!",15688, 1, 0, 0, 'BrundirFly'),

-- Kologarn:
(-1603090, "None shall pass!","Ihr k�nnt nicht vorbei!",15586, 1, 0, 0, 'KologarnAggro'),
(-1603091, "OBLIVION!","AUUUUUSSSSL�SCHUUUUUUNNNGG!",15591, 1, 0, 0, 'Kologarn shockweave'),
(-1603092, "I will squeeze the life from you!","Ich werde das Leben aus euch herausquetschen!",15592, 1, 0, 0, 'Kologarn grab'),
(-1603093, "Just a scratch!","Das ist nur ein Kratzer!",15589, 1, 0, 0, 'left arm lost'),
(-1603094, "Only a flesh wound!","Ist nur ne Fleischwunde!",15590, 1, 0, 0, 'right arm lost'),
(-1603095, "KOL-THARISH!","KOL-THARISH!",15587, 1, 0, 0, 'KologarnSlay1'),
(-1603096, "YOU FAIL!","VERSAGER!",15588, 1, 0, 0, 'KologarnSlay2'),
(-1603097, "I am invincible!","Ich bin unbesiegbar!",15594, 1, 0, 0, 'KologarnBerserk'),
(-1603098, "Master, they come...","Meister...sie...kommm...en....",15593, 1, 0, 0, 'Kologarndeath'),
(-1603099,'The Right Arm has regrown!',"Der rechte Arm ist nachgewachsen!",0,3,0,0,'EMOTE_right hand'),
(-1603100,'The Left Arm has regrown!',"Der linke Arm ist nachgewachsen! ",0,3,0,0,'EMOTE_left hand'),
(-1603101,'Kologarn casts Stone Grip!',"Kologarn wirkt Steingriff!",0,3,0,0,'EMOTE_stone grip'),

-- Auriaya:
(-1603110,"Some things are better left alone!","In manche Dinge mischt man sich besser nicht ein!",15473,1,0,0,"auriaya aggro"),
(-1603111,"The secret dies with you!","Das Geheimnis stirbt mit euch!",15474,1,0,0,"auriaya Slay 2"),
(-1603112,"There is no escape!","Es gibt kein Entrinnen!",15475,1,0,0,"auriaya Slay 1"),
(-1603113,"You waste my time!","Ihr verschwendet MEINE Zeit!",15477,1,0,0,"auriaya berserk"),
(-1603114,"Auriaya screams in agony.","AAAAAAAAHHHHHHAAAAAAAHHHHHHHAAAAA!",15476,2,0,0,"auriaya death"),
(-1603115,'Auriaya begins to cast Terrifying Screech.',"Auriaya beginnt schreckliches Kreischen zu wirken!",0,3,0,0,'EMOTE_screech'),
(-1603116,'Auriaya begins to activate the Feral Defender!',"Auriaya beschw�rt einen wilden Verteidiger!",0,3,0,0,'EMOTE_defender'),

-- Hodir
(-1603120,"<Furious Roar>","ROARRR",15556,2,0,0,"Hodir Frozen Blows"),
(-1603121,"Winds of the north consume you!","Winde des Nordens verschlingt sie!",15555,1,0,0,"Hodir Flash Freeze"),
(-1603122,"Welcome to the endless winter.","Willkommen, im endlosen Winter.",15554,1,0,0,"Hodir Slay 2"),
(-1603123,"Tragic. To come so far, only to fail.","Tragisch. Soweit zu kommen, nur um zu scheitern.",15553,1,0,0,"Hodir Slay 1"),
(-1603124,"I... I am released from his grasp... at last.","Ich...Ich bin von ihm befreit....endlich....",15557,1,0,0,"Hodir Death"),
(-1603125,"You will suffer for this trespass!","F�r euer Eindringen werdet ihr bezahlen!",15552,1,0,0,"Hodir Aggro"),
(-1603126,"The veil of winter will protect you, champions!","Der Schleier des Winters wird euch besch�tzen, Helden!",15559,1,0,0,"Hodir yogg"),
(-1603127,"Enough! This ends now!","Genug! Dies endet jetzt!",15558,1,0,0,"Hodir berserk"),
(-1603128,'Hodir begins to cast Flash Freeze!' ,"Hodir wirkt Blitzeis!",0,3,0,0,'EMOTE_flash freeze'),
(-1603129,'Hodir gains Frozen Blows!' ,"Hodir verst�rkt seine Waffe!",0,3,0,0,'EMOTE_frozen blows'),
(-1603130,"","Ihr armseligen Schw�chlinge k�nnte mich nicht bezwingen!!",0,3,0,0,'EMOTE_hardmode ends'),

-- Freya: 
(-1603140,"The Conservatory must be protected!","Das Konservatorium...muss verteidigt werden!",15526,1,0,0,"freya aggro"),
(-1603141,"Elders, grant me your strength!","Ihr �ltesten, gew�hrt mir eure Macht!",15527,1,0,0,"freya aggro hard"),
(-1603142,"Eonar, your servant requires aid!","Eonar, eure Dienerin braucht Hilfe!",15528,1,0,0,"summon conservator"),
(-1603143,"Children, assist me!","Helft mir Kinder!",15533,1,0,0,"summon trio"),
(-1603144,"The swarm of the elements shall overtake you!","Der Schwarm der Elemente soll �ber euch kommen!",15534,1,0,0,"summon lashers"),
(-1603145,"Forgive me.","Vergebt mir.",15529,1,0,0,"freya slay1"),
(-1603146,"From your death springs life anew!","Aus eurem Tod entspringt neues Leben!",15530,1,0,0,"freya slay2"),
(-1603147,"His hold on me dissipates. I can see clearly once more. Thank you, heroes.","Seine Macht �ber mich beginnt zu schwinden. Endlich kann ich wieder klar sehen. Ich danke euch, Helden.",15531,1,0,0,"freya Death"),
(-1603148,"You have strayed too far, wasted too much time!","Ihr seid zuweit abgekommen, habt zu viel zeit vergeudet!",15532,1,0,0,"freya berserk"),
(-1603149,"Eonar, your servant calls for your blessing!","Eonar, eure Dienerin bittet um euren Segen!",15535,1,0,0,"freya yogg"),
(-1603150,'Allies of Nature have appeared!' ,"Verb�ndete der Natur sind erschienen!",0,3,0,0,'EMOTE_allies'),
(-1603151,'A Lifebinders Gift begins to grow!' ,"Ein Geschenk der Lebensbinderin beginnt zu wachsen!",0,3,0,0,'EMOTE_lifebinders'),
(-1603152,'Freya begins to cast Ground Tremor!' ,"Freya l�sst die Erde beben!",0,3,0,0,'EMOTE_ground tremor'),
(-1603153,'Freya casts Strenghtened Iron Roots!' ,"Freya beschw�rt Eiserne Wurzeln",0,3,0,0,'EMOTE_iron roots'),
-- Brightleaf
(-1603160,"Matron, the Conservatory has been breached!","Matrone, Eindringlinge im Konservatorium!",15483,1,0,0,"brightleaf aggro"),
(-1603161,"Fertilizer.","D�nger.",15485,1,0,0,"brightleaf slay1"),
(-1603162,"Your corpse will nourish the soil!","Euer Leichnam wird das Erdreich n�hren!",15486,1,0,0,"brightleaf slay2"),
(-1603163,"Matron, one has fallen!","Matrone, einer ist gefallen!",15487,1,0,0,"brightleaf dead"),
-- Ironbranch
(-1603170,"Mortals have no place here!","Sterbliche haben hier nichts zu suchen!",15493,1,0,0,"ironbranch aggro"),
(-1603171,"I return you whence you came!","Zur�ck mit euch, wo ihr herkommt!",15494,1,0,0,"ironbranch slay1"),
(-1603172,"BEGONE!","HINFORT!",15495,1,0,0,"ironbranch slay2"),
(-1603173,"Freya! They come for you.","Freya! Sie sind hinter euch her.",15496,1,0,0,"summon trio"),
-- Stonebark
(-1603180,"This place will serve as your graveyard.","Dieser Ort wird zu eurem Grab werden!",15500,1,0,0,"stonebark aggro"),
(-1603181,"<Angry roar>","Roarrrr!",15501,1,0,0,"stonebark slay1"),
(-1603182,"Such a waste.","Welch Verschwendung!",15502,1,0,0,"stonebark slay2"),
(-1603183,"Matron, flee! They are ruthless....","Flieht, Matrone! Sie... sind.... ohne Skrupel....",15503,1,0,0,"stonebark death"),

-- Thorim:
(-1603190,"Interlopers! You mortals who dare to interfere with my sport will pay... Wait--you...","Eindringlinge! Ihr Sterblichen die ihr es wagt euch in mein Vergn�gen einzumischen werdet... Wartet--Ihr...",15733,1,0,0,"thorim aggro 1"),
(-1603191,"I remember you... In the mountains... But you... what is this? Where am--","Ich erkenne euch... In den Bergen... Aber ihr... was geht hier vor? Wo bin ich--",15734,1,0,0,"thorim aggro 2"),
(-1603192,"Behold the power of the storms and despair!","Werdet Zeugen der Macht der St�rme und verzweifelt!",15735,1,0,0,"thorim special 1"),
(-1603193,"Do not hold back! Destroy them!","Z�gert nicht! Vernichtet sie!",15736,1,0,0,"thorim special 2"),
(-1603194,"Have you begun to regret your intrusion?","Nun, bereut ihr euer Eindringen bereits?",15737,1,0,0,"thorim special 3"),
(-1603195,"Impertinent whelps! You dare challenge me atop my pedestal! I will crush you myself!","Ihr unversch�mtes Geschmei�! Ihr wagt es mich in meinem Refugium herauszufordern! ich werde euch eigenh�ndig zerschmettern!",15738,1,0,0,"thorim jump"),
(-1603196,"Can't you at least put up a fight!?","K�nnt ihr euch nicht wenigstens ein bisschen wehren!?",15739,1,0,0,"thorim slay1"),
(-1603197,"Pathetic!","Armselig!",15740,1,0,0,"thorim slay2"),
(-1603198,"My patience has reached its limit!","Meine Geduld ist am Ende!",15741,1,0,0,"Thorim berserk"),
(-1603199,"Failures! Weaklings!","Missgeburten! Schw�chlinge!",15742,1,0,0,"thorim arena wipe"),
(-1603200,"Stay your arms! I yield!","Senkt eure Waffen! Ich ergebe mich!",15743,1,0,0,"thorim defeat"),
(-1603201,"I feel as though I am awakening from a nightmare, but the shadows in this place yet linger.","Ich f�hle mich, als w�rde ich aus einem Alptraum erwachen, doch seine Schatten w�rden mich noch umschlingen.",15744,1,0,0,"thorim outro n1"),
(-1603202,"Sif... was Sif here? Impossible--she died by my brother's hand. A dark nightmare indeed....","Sif... war Sif hier? Unm�glich- sie starb durch meines Bruders Hand. Ein wahrlich dunkler Alptraum....",15745,1,0,0,"thorim outro n2"),
(-1603203,"I need time to reflect.... I will aid your cause if you should require it. I owe you at least that much. Farewell.","Ich brauche Zeit zum Nachdenken... Ich werde eure Sache unterst�tzen, solltet ihr dies w�nschen. Das ist das mindeste was ich euch schulde. Lebt wohl.",15746,1,0,0,"thorim outro n3"),
(-1603204,"You! Fiend! You are not my beloved! Be gone!","Ihr! Ihr seid nicht meine Geliebte! D�mon! Hinfort!",15747,1,0,0,"thorim outro hard1"),
(-1603205,"Behold the hand behind all the evil that has befallen Ulduar! Left my kingdom in ruins, corrupted my brother and slain my wife!","Seht die Hand, die all das �bel wirkt, das Ulduar befallen hat! Die mein K�nigreich in Tr�mmer legte, meinen Bruder verdarb und meine Frau ermordete!",15748,1,0,0,"thorim outro hard2"),
(-1603206,"And now it falls to you, champions, to avenge us all! The task before you is great, but I will lend you my aid as I am able. You must prevail!","Und nun liegt es an euch, ihr Helden, uns alle zu r�chen! Gro�e Aufgaben liegen vor euch, doch ich werde euch helfen, wo ich es vermag. ihr m�sst obsiegen!",15749,1,0,0,"thorim outro hard3"),
(-1603207,"Golganneth, lend me your strengh! Grant my mortal allies the power of thunder!","Golganneth, leiht mir eure St�rke! Gew�hrt meinen sterblichen Verb�ndeten die macht des Donners!",15750,1,0,0,"thorim yogg"),
-- Sif:
(-1603210,"Thorim, my lord, why else would these invaders have come into your sanctum but to slay you? They must be stopped!","Thorim mein Herr, aus welchem anderen Grund sollten die Eindringlinge in euer Heiligtum gekommen sein, als um euch zu t�ten? Sie m�ssen aufgehalten werden!",15668,1,0,0,"sif start"),
(-1603211,"Impossible! Lord Thorim, I will bring your foes a frigid death!","Unm�glich! Lord Thorim, ich werde euren Feinden einen kalten Tod bescheren!",15670,1,0,0,"sif event"),
(-1603212,"These pathetic mortals are harmless, beneath my station. Dispose of them!","Diese armseligen Sterblichen sind harmlos, unter meiner w�rde. Entfernt sie!",15669,1,0,0,"sif despawn"),
(-1603213,'Runic Colossus surrounds itself with a crackling Runic Barrier!' ,"Runenverzierter Koloss umh�llt sich selbst mit einer knisternden Runenbarriere!",0,3,0,0,'EMOTE_barrier'),
(-1603214,'Ancient Rune Giant fortifies nearby allies with runic might!' ,"Uralter Runengigant st�rkt nahe Verb�ndete!",0,3,0,0,'EMOTE_barrier'),

-- Mimiron:
(-1603220,"Oh, my! I wasn't expecting company! The workshop is such a mess! How embarrassing!","Meine G�te! Ich hatte keine Gesellschaft erwartet! Diese Werkstatt ist solch ein Saustall!Wie unangenehm! Gruml!",15611,1,0,0,"mimiron aggro"), 
(-1603221,"Now why would you go and do something like that? Didn't you see the sign that said 'DO NOT PUSH THIS BUTTON!'? How will we finish testing with the self-destruct mechanism active?","Warum habt ihr das jetzt gemacht? Habt ihr das Schild nicht gesehen auf dem steht: DIESEN KNOPF NICHT DR�CKEN! Wie sollen wir die Tests abschlie�en, solange der Selbstzerst�rungsmachanismus aktiv ist?!",15629,1,0,0,"mimiron hard mode"),
(-1603222,"Oh, my! It would seem that we are out of time, my friends!","Oh nein! Es scheint als w�re uns die Zeit davongelaufen, Freunde!",15628,1,0,0,"mimiron berserk"),
(-1603223,"We haven't much time, friends! You're going to help me test out my latest and greatest creation. Now, before you change your minds, remember, that you kind of owe it to me after the mess you made with the XT-002.","Wir haben nicht viel Zeit Freunde! Ihr werdet mir dabei helfen meine neueste und gro�artigste Kreation zu testen! Bevor ihr eure Meinung �ndert, denkt daran, dass ihr mir etwas schuldig seid, nach dem Unfug den ihr mit dem XT-002 angestellt habt!",15612,1,0,0,"tank active"),
(-1603224,"MEDIC!","SANIT�TAAAAAHH",15613,1,0,0,"tank kill1"),
(-1603225,"I can fix that... or, maybe not! Sheesh, what a mess...","Ich kann das reparieren...oder vielleicht auch nicht! Pahh, was f�r eine Sauerei...Grumel!",15614,1,0,0,"tank kill2"),
(-1603226,"WONDERFUL! Positively marvelous results! Hull integrity at 98.9 percent! Barely a dent! Moving right along.","UHAHAHA WUNDERBAAAR! Das sind Ergebnisse nach meinem Geschmack! Intigirit�t der H�lle bei 98,9 %! Sogut wie keine Dellen! Und weiter gehts!",15615,1,0,0,"tank dead"),
(-1603227,"Behold the VX-001 Anti-personnel Assault Cannon! You might want to take cover.","Ich pr�sentiere die VX-001 Personenangriffskanone! Ihr solltet vielleicht in Deckung gehen!",15616,1,0,0,"torso active"),
(-1603228,"Fascinating. I think they call that a 'clean kill'.","Faszinierend! Das nennt man wohl einen 'sauberen Abschuss'!",15617,1,0,0,"torso kill1"),
(-1603229,"Note to self: Cannon highly effective against flesh.","Notiz an mich selbat: Kanone �u�erst effektiv gegen Zellgewebe!",15618,1,0,0,"torso kill2"),
(-1603230,"Thank you, friends! Your efforts have yielded some fantastic data! Now, where did I put-- oh, there it is!","Danke Freunde! Eure Anstrenungen haben fantastische Daten geliefert! So, wo hab ich doch gleich....ahh, hier ist es! Grumel, murmel... ",15619,1,0,0,"torso dead"),
(-1603231,"Isn't it beautiful? I call it the magnificent aerial command unit!","UHAHAHA, ist sie nicht atemberaubend? Ich nenne sie die �berragende Luftkommandoeinheit!",15620,1,0,0,"head active"),
(-1603232,"Outplayed!","MMHMmm...ausgespielt!",15621,1,0,0,"head kill1"),
(-1603233,"You can do better than that!","Das k�nnt ihr aber besser!",15622,1,0,0,"head kill2"),
(-1603234,"Preliminary testing phase complete. Now comes the true test!!","Vorversuchsphase abgeschlossen. Jetzt kommt der eigentliche Test, MUHAHA!!",15623,1,0,0,"head defeat"),
(-1603235,"Gaze upon its magnificence! Bask in its glorious, um, glory! I present you... V-07-TR-0N!","Bestaunt seine Gro�artigkeit! Sonnt euch in seiner glorreichen...�hm..�hm...Glorie! ich pr�sentiere euch V-07-TR-0N!",15624,1,0,0,"robot active"),
(-1603236,"Prognosis: Negative!","Prognose: NEGATIV! HA!",15625,1,0,0,"robot kill1"),
(-1603237,"You're not going to get up from that one, friend.","Davon werdet ihr euch nichtmehr erhohlen, Freundchen!",15626,1,0,0,"robot kill2"),
(-1603238,"It would appear that I've made a slight miscalculation. I allowed my mind to be corrupted by the fiend in the prison, overriding my primary directive. All systems seem to be functional now. Clear.","Es scheint, als w�re mir eine klitze kleine Fehlkalkulation unterlaufen! Ich habe zugelassen, dass das Scheusal im Gef�ngnis meine Primi�redirektive �berschreitet! Alles ist eben nun funktionst�chtig!",15627,1,0,0,"robot defeat"),
(-1603239,"Combat matrix enhanced. Behold wonderous rapidity!","Kampfmatrix verbessert. Werdet Zeugen au�ergew�hnlicher Geschwindigkeit!",15630,1,0,0,"mimiron yogg"),
(-1603240,'Leviathan Mk II begins to cast Plasma Blast!',"Leviathan Mk II wirkt Plasmaeruption!",0,3,0,0,'EMOTE_plasma_blast'),
(-1603241,'',"VX-001 l�dt seine Zwillingskanonen auf! Noch 4 Sekunden bis zum Abschuss!",0,3,0,0,'EMOTE_laser'),

-- vezax
(-1603350,'Your destruction will herald a new age of suffering!',"Eure Vernichtung wird ein neues Zeitalter des leid anleuten!",15542,1,0,0,'vezax aggro'),
(-1603351,'You thought to stand before the legions of death... and survive?',"Dachtet ihr, ihr k�nntet vor den legionen des Todes stehen...und �berleben?!",15543,1,0,0,'vezax kill1'),
(-1603352,'Defiance... a flaw of mortality.',"Aufs�ssigkeit...uhahaha...eine Schw�che der Sterblichen!",15544,1,0,0,'vezax kill2'),
(-1603353,'The black blood of Yogg-Saron courses through me! I. AM. UNSTOPPABLE!',"Mich... durchstr�mt... das schwarze Blut von Yogg-Saron! ICH! BIN! NICHT! AUFZUHALTEN!",15545,1,0,0,'vezaz surge'),
(-1603354,'Oh, what horrors await....',"UHAHAHAHA, oh, welche Gr�uel warten....",15546,1,0,0,'vezax death'),
(-1603355,'Your defeat was inevitable!',"Eure Niederlage war unausweichlich!",15547,1,0,0,'vezax enrage'),
(-1603356,'Behold, now! Terror, absolute!',"Werdet nun Zeuge... grenzenlosen Schreckens!",15548,1,0,0,'vezax hard'),
(-1603357,'A cloud of saronite vapors coalesces nearby!',"Eine Wolke aus Saronitd�mpfen entsteht in der N�he!",0,3,0,0,'EMOTE_vapors'),
(-1603358,'General Vezax roars and surges with dark might!',"General Vezax br�llt und wird mit dunkler Macht erf�llt!",0,3,0,0,'EMOTE_might'),
(-1603359,'The saronite vapors mass and swirl violently, merging into a monstrous form!',"Der Saronitdampf verdichtet sich und verschmilzt in eine gr�ssliche Form!",0,3,0,0,'EMOTE_animus'),

-- Yogg:
-- Sara:
(-1603360,'The time to strike at the head of the beast will soon be upon us! Focus your anger and hatred on his minions!',"Bald ist die Zeit gekommen, dem Untier den Kopf abzuschlagen! Konzentiert euren Zorn und euren Hass auf seine Diener!",15775,1,0,0,'sara aggro'),
(-1603361,'Yes! YES! Show them no mercy! Give no pause to your attacks!',"Jah! JAH! Gew�hrt ihnen keine Gnade! Lasst sie nicht zu Atem kommen!",15773,1,0,0,'sara help1'),
(-1603362,'Let hatred and rage guide your blows!',"Lasst eure Angriffe vom Hass und vom Zorn leiten!",15774,1,0,0,'sara help2'),
(-1603363,'Could they have been saved?',"H�tten sie gerettet werden k�nnen?",15779,1,0,0,'sara kill1'),
(-1603364,'Powerless to act...',"Zu schwach um zu k�mpfen...",15778,1,0,0,'sara kill 2'),
(-1603365,'Tremble, mortals, before the coming of the end!',"Erzittert, Sterbliche, denn das sichere Ende ist nah!",15777,1,0,0,'sara yell1 p2'),
(-1603366,'Suffocate upon your own hate!',"Erstickt an eurem eigenen Hass!",15776,1,0,0,'sara yell2 p2'),
(-1603367,'Aaaaaaaaaaaaaaaaa... Help me!!! Please got to help me!',"AAAAAaaaaaaa...helft mir! Bitte haltet sie von mir fern!",15771,1,0,0,'sara prefight'),
(-1603368,'What do you want from me? Leave me alone!',"Was wollt ihr von mir? Lasst mich in Ruhe!",15772,1,0,0,'sara prefight2'),
(-1603369,'Weak-minded fools!',"Schwachsinniger Narr!",15780,4,0,0,'sara slay phase1'),

-- Yogg:
(-1603370,'I am the lucid dream. The monster in your nightmares. The fiend of a thousand faces. Cower before my true form. BOW DOWN BEFORE THE GOD OF DEATH!',"Ich bin der strahlende Traum. Das Monster in euren Alptr�umen. Der Verderber mit den tausend Gesichtern. Kriecht vor meiner wahren Natur. KNIET NIEDER VOR DEM GOTT DES TODES!  ",15754,1,0,0,'yogg p2 intro'),
(-1603371,'MADNESS WILL CONSUME YOU!',"DER WAHNSINN WIRD EUCH VERSCHLINGEN!",15756,1,0,0,'yogg vision'),
(-1603372,'Look upon the true face of death and know that your end comes soon!',"Erblicket das wahre Antliz des Todes und wisset, euer Ende ist nah!",15755,1,0,0,'yogg phase 3'),
(-1603373,'Hoohehehahahaha... AHAHAHAHAHAHA!',"Hoohehehahahaha... AHAHAHAHAHAHA!",15757,1,0,0,'yogg slay1'),
(-1603374,'Eternal suffering awaits!',"Euch erwartet ewiges Leid!",15758,1,0,0,'yogg slay2'),
(-1603375,'Your fate is sealed. The end of days is finally upon you and ALL who inhabit this miserable little seedling. Uulwi ifis halahs gag erhongg wssh.',"Euer Schicksal ist besiegelt. Das Ende aller Tage steht euch bevor, so wie allen, die diesen armseligen Planeten bewohnen. Uulwi ifis halahs gag erhongg wssh.",15761,1,0,0,'yogg death'),
(-1603376,'Your will is no longer you own...',"Euer Wille gehorcht euch nicht l�nger....",15759,4,0,0,'yogg insanity1'),
(-1603377,'Destroy them minion, your master commands it!',"Vernichte sie mein Diener, dein Meister befiehlt es dir!",15760,4,0,0,'yogg insanity2'),
(-1603378,'Portals open into Yogg-Sarons mind!',"Portal ins Gehirn von Yogg-Saron sind nun ge�ffnet!",0,3,0,0,'EMOTE_vision_blast'),
(-1603379,'The illusion shatters and a path to the central chamber opens!',"Die Illusion zerschmettert und ein Pfad in die Hauptkammer �ffnet sich!",0,3,0,0,'EMOTE_shatter_blast'),
-- Visions:
-- lich king v3
(-1603380,'Your resilience is admirable.',"Eure Ausdauer ist bewundernswert.",15598,0,0,0,'v1 lich king1'),
(-1603381,'Arrrrrrgh!',"Aaaaahhhhhhhrrrrgggghhh!",15470,1,0,0,'v1 champ1'),
(-1603382,'I am not afraid of you!',"Uhaaa! Ihr macht mir keine Angst!",15471,0,0,0,'v1 champ2'),
(-1603383,'I will break you as I broke him.',"Ich werde euch brechen, wie einst ihn!",15599,0,0,0,'v1 lich king2'),
(-1603384,'Yrr n lyeth... shuul anagg!',"Yrr n lyeth... shuul anagg!",15766,0,0,0,'v1 yogg1'),
(-1603385,'He will learn... no king rules forever; only death is eternal!',"Er wird lernen... kein K�nig regiert f�r immer, nur der Tod ist ewig!",15767,0,0,0,'v1 yogg2'),
-- dragons v2
(-1603390,'It is done... All have been given that which must be given. I now seal the Dragon Soul forever...',"Es ist vollbracht... Alle haben gegeben, was gegeben werden musste. Nun werde ich die Drachenseele f�r immer versiegeln....",15631,0,0,0,'v2 neltharion1'),
(-1603391,'That terrible glow... should that be?',"Dieses schreckliche Gl�hen... ist es so bestimmt?",15784,0,0,0,'v2 ysera1'),
(-1603392,'For it to be as it must, yes.',"Damit sie so sei, wie sie sein muss, jaahh.",15632,0,0,0,'v2 neltharion2'),
(-1603393,'It is a weapon like no other. It must be like no other.',"Sie ist meine Waffe wie keine andere. Sie muss wie keine andere sein!",15610,0,0,0,'v2 malygos1'),
(-1603394,'His brood learned their lesson before too long, you shall soon learn yours!',"Seine Brut hat die Lektion recht schnell gelernt, und bald werdet ihr die eure lernen!",15765,0,0,0,'v2 yogg1'),
-- stormwind v1
(-1603400,'Bad news sire.',"Schlechte Neuigkeiten Herr.",15538,0,0,0,'v3 garona1'),
(-1603401,'The clans are united under Blackhand in this assault. They will stand together until Stormwind has fallen.',"Die Clans haben sich f�r diesen Angriff unter Schwarzfaust vereint. Sie werden Seite an Seite stehen, bis Sturmwind f�llt.",15539,0,0,0,'v3 garona2'),
(-1603402,'Guldan is bringing up his warlocks by nightfall. Until then, the Blackrock clan will be trying to take the Eastern Wall.',"Guldan wird bei Einbruch der Nacht seine Hexenmeister mobilisieren. Bis dahin wird der Schwarzfelsklan versuchen, den Ostwall zu nehmen! ",15540,0,0,0,'v3 garona3'),
(-1603403,'A thousand deaths... ',"Tausend Tode...",15762,0,0,0,'v3 yogg1'),
(-1603404,'or one murder.',"oder ein Mord!",15763,0,0,0,'v3 yogg2'),
(-1603405,'We will hold until the reinforcements come. As long as men with stout hearts are manning the walls and throne Stormwind will hold.',"Wir werden standhalten, bis Verst�rkung eintrifft. Solange M�nner mit tapferem Herzen W�lle und Thron besetzten, wird Sturmwind bestehen!",15585,0,0,0,'v3 king llane1'),
(-1603406,'The orc leaders agree with your assessment.',"Die Anf�herer der Orcs stimmen mit eurer Einsch�tzung �berein!",15541,0,0,0,'v3 garona4'),
(-1603407,'Your petty quarrels only make me stronger!',"Eure l�cherlichen Streiterein machen mich nur m�chtiger!",15764,0,0,0,'v3 yogg3'),

/*-- Alagon:
(-1603410,"Your actions are illogical. All possible results for this encounter have been calculated. The pantheon will receive the observer\'s message regardless outcome.","",15386,1,0,0,'Agro_algalon the observer'),
(-1603411,'See your world through my eyes. A universe so vast as to be immeasurable. Incomprehensible even to your greatest minds.',"",15390,1,0,0,'Engaged for the first time algalon'),
(-1603412,'Witness the fury of cosmos!',"",15396,1,0,0,'BIG BANG 1_Algalon'),
(-1603413,'Behold the tools of creation!',"",15397,1,0,0,'BIG BANG 2_Algalon\r\n'),
(-1603414,'Beware!',"",15391,1,0,0,'Phase2_algalon\r\n'),
(-1603415,'Loss of life, unavoidable.',"",15387,1,0,0,'Killing a player_alagalon\r\n'),
(-1603416,'I do what I must.',"",15388,1,0,0,'killing a player2_algalon\r\n'),
(-1603417,'You are... out of time.',"",15394,1,0,0,'BERSEKER_ALGALON'),
(-1603418,'The stars come to my aid.',"",15392,1,0,0,'Summoning Collapsing Stars_Algalon1'),
(-1603419,'I lack the strength to transmit this signal. You must hurry. Find a place of power, close to the skies.',"",15403,1,0,0,'Summoning Collapsing Stars_Algalon2'),
(-1603420,"Do not worry about my fate <name>. If the signal is not transmitted in time re-origination will proceed regardless. Save. Your. World.","","","","","","","","",15404,1,0,0,"algalon outro 5"),
(-1603421,"I've rearranged the reply code. Your planet will be spared. I cannot be certain of my own calculations anymore.","","","","","","","","",15402,1,0,0,"algalon outro 3"),
(-1603422,"Perhaps it is your imperfection that which grants you free will. That allows you to persevere against cosmically calculated odds. You prevailed where the Titans' own perfect creations have failed.","",15401,1,0,0,"algalon outro 2"),
(-1603423,"I have seen worlds bathed in the Makers' flames. Their denizens fading without so much as a whimper. Entire planetary systems born and raised in the time that it takes your mortal hearts to beat once. Yet all throughout, my own heart, devoid of emotion... of empathy. I... have... felt... NOTHING! A million, million lives wasted. Had they all held within them your tenacity? Had they all loved life as you do?","",15393,1,0,0,"algalon outro1"),
(-1603424,"Farewell, mortals. Your bravery is admirable, for such flawed creatures.","",15400,1,0,0,"algalon despwnd 3"),
(-1603425,"Begin uplink: Reply Code: 'Omega'. Planetary re-origination requested.","",15399,1,0,0,"algalon despawn 2"),
(-1603426,"Analysis complete. There is partial corruption in the plane's life-support systems as well as complete corruption in most of the planet's defense mechanisms.","",15398,1,0,0,"algalon despawn1"),
(-1603427,"It is in the universe's best interest to re-originate this planet should my analysis find systemic corruption. Do not interfere.","",15407,1,0,0,"algalon intro3"),
(-1603428,"Stand back, mortals. I am not here to fight you.","",15406,1,0,0,"Algalon intro2"),
(-1603429,"Trans-location complete. Commencing planetary analysis of Azeroth.","",15405,1,0,0,"Algalon intro1"),

-- Archivum dialogue: TODO:
-- Brann
-- Archivum
*/ 
-- FUN
(-1603430,"Rin and Goldi are watching you!","Rin and Goldi are watching you!",0,6,0,0,"FUN Stuff");

