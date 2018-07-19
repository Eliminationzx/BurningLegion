
-- change wrong faction to AH=green -- H/A 83/84  775/774  1733/1734
UPDATE creature_template SET faction=35, AIName="SmartAI" where entry=16781;

-- double spawned creature
DELETE FROM creature WHERE id=16781 AND guid IN 
(370375, 370376, 370377, 370374, 370208, 370210, 370417, 370284, 370290, 370288, 370212, 370211, 370403, 370406, 370404, 
 370405, 369977, 370389, 370391, 370390, 370458, 370456, 370457, 370785, 370395, 370394, 370397, 370396, 370031, 369986, 
 369985, 370057, 370000, 370043, 370045, 369999, 369992, 369991, 370035, 369830, 369831, 369832, 369833, 370021, 369961, 
 370022, 370055, 370436, 370453, 370434, 370435, 369860, 369699, 369702, 369712, 370475, 370476, 370463, 370477, 369971, 
 369969, 370410, 370409, 370415, 369705, 370416, 369721, 370414, 370418, 369709, 369708, 369741, 369734, 369739, 369739, 
 369740, 369766, 369768, 369754, 369767, 369763, 369765, 369743, 369764, 370339, 370341, 370338, 370340, 369855, 369851, 
 369852, 369841, 369850, 369837, 369848, 369838, 370445, 370442, 370448, 370441, 370237, 370316, 370269, 370270, 370472, 
 370470, 370471, 370473, 370464, 370478, 370479, 370474, 369891, 369894, 369892, 369893, 369824, 369809, 369792, 369823,
 370383, 370382, 370380, 370381, 370336, 370337, 370334, 370335, 370387, 370386, 370393, 370392, 370344, 370343, 370345, 
 370342, 369880, 369881, 369878, 369879, 370230, 370267, 370231, 370233, 369877, 369874, 369875, 369876, 370461, 370460, 
 370462, 370455, 369870, 369871, 369873, 369872, 369794, 369797, 369811, 369798, 370201, 370199, 370203, 370202, 370450, 
 370452, 370449, 370451, 370197, 370198, 370200, 370079, 370592, 370593, 370570, 370571, 369520, 369527, 369524, 369523, 
 370100, 370152, 370388, 370099, 369774, 369775, 369777, 369776, 370166, 370063, 370171, 370168, 370576, 370579, 370578, 
 370577, 369968, 370039, 369967, 370040, 370350, 370058, 370333, 370332, 369657, 369656, 369560, 369559, 370428, 370426, 
 370427, 370430, 370408, 370407, 370372, 370371, 369890, 369889, 369888, 369887, 370346, 370348, 370347, 370349, 369500, 
 369507, 369508, 369506, 370355, 370356, 370358, 370357, 369540, 369532, 369541, 369533, 369949, 370010, 370011, 370009, 
 370443, 370444, 370447, 370446, 370361, 370362, 370359, 370360, 370432, 370765, 370764, 370433, 369770, 369771, 369884, 
 369744, 370354, 370353, 370352, 370351, 369788, 369787, 369784, 369782, 370770, 370768, 370769, 370767, 370373, 370468, 
 370469, 370419, 370087, 370090, 369562, 370439, 369563, 370440, 369558, 369561, 369898, 369899, 369896, 369897, 370363, 
 370364, 370385, 370384, 370245, 370278, 370366, 370365, 370368, 370370, 370369, 370367, 369882, 369883, 369815, 369819, 
 370097, 370148, 370378, 370379, 369857, 369866, 369865, 369842, 369864, 369862, 369861, 369863, 369736, 369738, 369737, 369735, 
 370421, 370401, 370424, 370420, 370400, 370399, 370398, 370402, 370412, 370423, 370425, 370784, 369829, 369828, 369827, 369742);
 
DELETE FROM game_event_creature WHERE guid IN 
(370375, 370376, 370377, 370374, 370208, 370210, 370417, 370284, 370290, 370288, 370212, 370211, 370403, 370406, 370404, 
 370405, 369977, 370389, 370391, 370390, 370458, 370456, 370457, 370785, 370395, 370394, 370397, 370396, 370031, 369986, 
 369985, 370057, 370000, 370043, 370045, 369999, 369992, 369991, 370035, 369830, 369831, 369832, 369833, 370021, 369961, 
 370022, 370055, 370436, 370453, 370434, 370435, 369860, 369699, 369702, 369712, 370475, 370476, 370463, 370477, 369971, 
 369969, 370410, 370409, 370415, 369705, 370416, 369721, 370414, 370418, 369709, 369708, 369741, 369734, 369739, 369739, 
 369740, 369766, 369768, 369754, 369767, 369763, 369765, 369743, 369764, 370339, 370341, 370338, 370340, 369855, 369851, 
 369852, 369841, 369850, 369837, 369848, 369838, 370445, 370442, 370448, 370441, 370237, 370316, 370269, 370270, 370472, 
 370470, 370471, 370473, 370464, 370478, 370479, 370474, 369891, 369894, 369892, 369893, 369824, 369809, 369792, 369823,
 370383, 370382, 370380, 370381, 370336, 370337, 370334, 370335, 370387, 370386, 370393, 370392, 370344, 370343, 370345, 
 370342, 369880, 369881, 369878, 369879, 370230, 370267, 370231, 370233, 369877, 369874, 369875, 369876, 370461, 370460, 
 370462, 370455, 369870, 369871, 369873, 369872, 369794, 369797, 369811, 369798, 370201, 370199, 370203, 370202, 370450, 
 370452, 370449, 370451, 370197, 370198, 370200, 370079, 370592, 370593, 370570, 370571, 369520, 369527, 369524, 369523, 
 370100, 370152, 370388, 370099, 369774, 369775, 369777, 369776, 370166, 370063, 370171, 370168, 370576, 370579, 370578, 
 370577, 369968, 370039, 369967, 370040, 370350, 370058, 370333, 370332, 369657, 369656, 369560, 369559, 370428, 370426, 
 370427, 370430, 370408, 370407, 370372, 370371, 369890, 369889, 369888, 369887, 370346, 370348, 370347, 370349, 369500, 
 369507, 369508, 369506, 370355, 370356, 370358, 370357, 369540, 369532, 369541, 369533, 369949, 370010, 370011, 370009, 
 370443, 370444, 370447, 370446, 370361, 370362, 370359, 370360, 370432, 370765, 370764, 370433, 369770, 369771, 369884, 
 369744, 370354, 370353, 370352, 370351, 369788, 369787, 369784, 369782, 370770, 370768, 370769, 370767, 370373, 370468, 
 370469, 370419, 370087, 370090, 369562, 370439, 369563, 370440, 369558, 369561, 369898, 369899, 369896, 369897, 370363, 
 370364, 370385, 370384, 370245, 370278, 370366, 370365, 370368, 370370, 370369, 370367, 369882, 369883, 369815, 369819, 
 370097, 370148, 370378, 370379, 369857, 369866, 369865, 369842, 369864, 369862, 369861, 369863, 369736, 369738, 369737, 369735, 
 370421, 370401, 370424, 370420, 370400, 370399, 370398, 370402, 370412, 370423, 370425, 370784, 369829, 369828, 369827, 369742);
 
DELETE FROM creature_addon WHERE guid IN 
(370375, 370376, 370377, 370374, 370208, 370210, 370417, 370284, 370290, 370288, 370212, 370211, 370403, 370406, 370404, 
 370405, 369977, 370389, 370391, 370390, 370458, 370456, 370457, 370785, 370395, 370394, 370397, 370396, 370031, 369986, 
 369985, 370057, 370000, 370043, 370045, 369999, 369992, 369991, 370035, 369830, 369831, 369832, 369833, 370021, 369961, 
 370022, 370055, 370436, 370453, 370434, 370435, 369860, 369699, 369702, 369712, 370475, 370476, 370463, 370477, 369971, 
 369969, 370410, 370409, 370415, 369705, 370416, 369721, 370414, 370418, 369709, 369708, 369741, 369734, 369739, 369739, 
 369740, 369766, 369768, 369754, 369767, 369763, 369765, 369743, 369764, 370339, 370341, 370338, 370340, 369855, 369851, 
 369852, 369841, 369850, 369837, 369848, 369838, 370445, 370442, 370448, 370441, 370237, 370316, 370269, 370270, 370472, 
 370470, 370471, 370473, 370464, 370478, 370479, 370474, 369891, 369894, 369892, 369893, 369824, 369809, 369792, 369823,
 370383, 370382, 370380, 370381, 370336, 370337, 370334, 370335, 370387, 370386, 370393, 370392, 370344, 370343, 370345, 
 370342, 369880, 369881, 369878, 369879, 370230, 370267, 370231, 370233, 369877, 369874, 369875, 369876, 370461, 370460, 
 370462, 370455, 369870, 369871, 369873, 369872, 369794, 369797, 369811, 369798, 370201, 370199, 370203, 370202, 370450, 
 370452, 370449, 370451, 370197, 370198, 370200, 370079, 370592, 370593, 370570, 370571, 369520, 369527, 369524, 369523, 
 370100, 370152, 370388, 370099, 369774, 369775, 369777, 369776, 370166, 370063, 370171, 370168, 370576, 370579, 370578, 
 370577, 369968, 370039, 369967, 370040, 370350, 370058, 370333, 370332, 369657, 369656, 369560, 369559, 370428, 370426, 
 370427, 370430, 370408, 370407, 370372, 370371, 369890, 369889, 369888, 369887, 370346, 370348, 370347, 370349, 369500, 
 369507, 369508, 369506, 370355, 370356, 370358, 370357, 369540, 369532, 369541, 369533, 369949, 370010, 370011, 370009, 
 370443, 370444, 370447, 370446, 370361, 370362, 370359, 370360, 370432, 370765, 370764, 370433, 369770, 369771, 369884, 
 369744, 370354, 370353, 370352, 370351, 369788, 369787, 369784, 369782, 370770, 370768, 370769, 370767, 370373, 370468, 
 370469, 370419, 370087, 370090, 369562, 370439, 369563, 370440, 369558, 369561, 369898, 369899, 369896, 369897, 370363, 
 370364, 370385, 370384, 370245, 370278, 370366, 370365, 370368, 370370, 370369, 370367, 369882, 369883, 369815, 369819, 
 370097, 370148, 370378, 370379, 369857, 369866, 369865, 369842, 369864, 369862, 369861, 369863, 369736, 369738, 369737, 369735, 
 370421, 370401, 370424, 370420, 370400, 370399, 370398, 370402, 370412, 370423, 370425, 370784, 369829, 369828, 369827, 369742);

DELETE FROM creature_text WHERE CreatureID=16781;
INSERT INTO creature_text 
(CreatureID, groupid, id, text, type, language, probability, emote, duration, sound, comment, BroadcastTextID)
VALUES
(16781, 0, 1, "The Midsummer Fire Festival has something for everyone to enjoy!", 12, 0, 50.0, 396, 0, 0, "", 24528),
(16781, 0, 2, "Across the lands our fires burn strong and true!", 12, 0, 50.0, 396, 0, 0, "", 25068),
(16781, 0, 3, "Have you danced at the ribbon pole yet?", 12, 0, 50.0, 396, 0, 0, "", 25069),
(16781, 0, 4, "I admire the skill that torch jugglers have.", 12, 0, 50.0, 396, 0, 0, "", 25070);

DELETE FROM smart_scripts WHERE entryorguid=16781;
INSERT INTO smart_scripts 
(entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment)
VALUES
(16781, 0, 0, 0, 1, 0, 50, 0, 500, 1000, 45000, 120000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, "Say random text");
