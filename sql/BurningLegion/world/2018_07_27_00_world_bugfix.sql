---bug fix
UPDATE `quest_objectives` SET `Flags`=4 WHERE (`ID`=279739);
DELETE FROM `waypoint_data` WHERE `id` IN (645500,643200);
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES
('645500', '1', '870.331', '-4202.85', '-14.0742', '0', '45000', '0', '0', '100', '0'),
('645500', '2', '869.751', '-4190.07', '-14.1105', '0', '45000', '0', '0', '100', '0'),
('643200', '1', '843.944', '-4207.75', '-9.11563', '0', '0', '0', '0', '100', '0'),
('643200', '2', '855.389', '-4212.85', '-9.99841', '0', '0', '0', '0', '100', '0'),
('643200', '3', '867.336', '-4204.73', '-13.9766', '0', '0', '0', '0', '100', '0'),
('643200', '4', '879.138', '-4186.37', '-13.9981', '0', '0', '0', '0', '100', '0'),
('643200', '5', '898.795', '-4170.9', '-9.24759', '0', '0', '0', '0', '100', '0'),
('643200', '6', '894.318', '-4165.13', '-9.10758', '0', '0', '0', '0', '100', '0'),
('643200', '7', '881.809', '-4167.77', '-13.8924', '0', '0', '0', '0', '100', '0'),
('643200', '8', '864.355', '-4183.45', '-14.0399', '0', '0', '0', '0', '100', '0'),
('643200', '9', '847.975', '-4191.54', '-10.0094', '0', '0', '0', '0', '100', '0'),
('643200', '10', '847.094', '-4194.71', '-9.9878', '0', '0', '0', '0', '100', '0');
UPDATE `creature_addon` SET `path_id`='645500' WHERE `guid`='6455';
UPDATE `creature` SET `MovementType`='2' WHERE `guid`='6455';
DELETE FROM `creature_addon` WHERE `guid` = 6432;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES ('6432', '643200', '0', '0', '0', '0', '0');
UPDATE `creature` SET `MovementType`='2' WHERE `guid`='6432';
UPDATE creature_template SET AIName='NullCreatureAI', ScriptName='' WHERE entry IN(35083, 35080, 35084, 35079, 35104, 35105);
DELETE FROM waypoint_data WHERE id=2469050;
INSERT INTO waypoint_data VALUES (2469050, 1, 5870.29, 2147.65, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 2, 5867.22, 2155.17, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 3, 5865.35, 2163.15, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 4, 5864.42, 2172.41, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 5, 5865.06, 2179.38, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 6, 5867.25, 2189.65, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 7, 5870.01, 2198.61, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 8, 5874.43, 2205.42, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 9, 5882.62, 2205.31, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 10, 5889.66, 2201.28, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 11, 5893.07, 2195.17, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 12, 5890.64, 2188.6, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 13, 5887.31, 2182.44, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 14, 5881.91, 2174.78, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 15, 5879.15, 2167.14, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 16, 5880.75, 2159.18, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 17, 5882.9, 2155.02, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 18, 5887.01, 2146.66, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 19, 5886.56, 2142.06, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 20, 5884.42, 2136.59, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 21, 5880.85, 2133.55, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 22, 5878.56, 2133.22, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 23, 5875.22, 2134.27, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 24, 5873.69, 2135.99, 636.042, 0, 0, 0, 0, 100, 0),(2469050, 25, 5872.12, 2140.42, 636.042, 0, 0, 0, 0, 100, 0);
DELETE FROM waypoint_data WHERE id=2469150;
INSERT INTO waypoint_data VALUES (2469150, 1, 5913.05, 2065.04, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 2, 5917.99, 2060.07, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 3, 5925.71, 2054.87, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 4, 5936.31, 2049.94, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 5, 5946.34, 2046.82, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 6, 5954.52, 2046.6, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 7, 5960.7, 2050.03, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 8, 5963.44, 2055.16, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 9, 5963.36, 2059.85, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 10, 5961.52, 2065.36, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 11, 5954.64, 2069.68, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 12, 5946.58, 2070.59, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 13, 5937.28, 2071.07, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 14, 5929.96, 2074.59, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 15, 5923.44, 2081.23, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 16, 5918.98, 2089.4, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 17, 5916.49, 2093.29, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 18, 5913.62, 2095.3, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 19, 5910.21, 2096.08, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 20, 5908.02, 2095.35, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 21, 5905.89, 2094.44, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 22, 5903.17, 2092.24, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 23, 5901.87, 2089.06, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 24, 5901.34, 2085.6, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 25, 5903.01, 2079.96, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 26, 5905.72, 2074.75, 636.041, 0, 0, 0, 0, 100, 0),(2469150, 27, 5909.18, 2070.08, 636.041, 0, 0, 0, 0, 100, 0);
UPDATE creature_template SET faction=2037 WHERE entry=35133;
UPDATE creature_template SET faction=1981 WHERE entry=35135;
UPDATE creature_template SET faction=12 WHERE entry=35100;
UPDATE creature_template SET faction=29 WHERE entry=35093;
DELETE FROM creature_loot_template WHERE entry IN(31813, 31205, 28220, 32467, 31142);
UPDATE creature_template SET lootid=0 WHERE entry IN(31813, 31205, 28220, 32467, 31142);
UPDATE creature_template SET mechanic_immune_mask = mechanic_immune_mask & ~(1<<21) WHERE entry IN(2784 , 3057 , 3516 , 4949 , 4968 , 
7937 , 7999 , 10181 , 10540 , 11980 , 16472 , 16800 , 16802 , 16807 , 16808 , 16809 , 17257 , 17468 , 17536 , 17537 , 17711 , 
17723 , 17734 , 17770 , 17826 , 17882 , 17907 , 17940 , 17958 , 17978 , 17990 , 18105 , 18432 , 18434 , 18481 , 18680 , 18692 , 
18731 , 19218 , 19551 , 19710 , 19891 , 20032 , 20035 , 20036 , 20037 , 20039 , 20040 , 20041 , 20043 , 20044 , 20045 , 20046 , 
20049 , 20050 , 20060 , 20062 , 20063 , 20064 , 20164 , 20168 , 20169 , 20183 , 20184 , 20187 , 20189 , 20568 , 20596 , 20597 , 
20636 , 20864 , 20867 , 20868 , 20870 , 20873 , 20879 , 20880 , 20885 , 20886 , 20896 , 20901 , 20902 , 20905 , 20906 , 20908 , 
20909 , 20910 , 20911 , 20912 , 20992 , 21127 , 21174 , 21221 , 21224 , 21227 , 21298 , 21299 , 21301 , 21303 , 21304 , 21339 , 
21346 , 21362 , 21466 , 21467 , 21508 , 21525 , 21526 , 21581 , 21588 , 21589 , 21590 , 21591 , 21592 , 21593 , 21594 , 21595 , 
21596 , 21599 , 21600 , 21601 , 21605 , 21606 , 21608 , 21610 , 21611 , 21612 , 21616 , 21617 , 21618 , 21623 , 21624 , 21626 , 
21702 , 21838 , 21843 , 21958 , 21964 , 21965 , 21966 , 22055 , 22076 , 22119 , 22120 , 22128 , 22129 , 22330 , 22346 , 22844 , 
22847 , 22849 , 22855 , 22873 , 22877 , 22880 , 22884 , 22956 , 22960 , 22963 , 22964 , 23028 , 23030 , 23061 , 23196 , 23222 , 
23239 , 23261 , 23281 , 23282 , 23330 , 23337 , 23394 , 23397 , 23399 , 23400 , 23402 , 23403 , 23421 , 23597 , 24175 , 24374 , 
24560 , 24664 , 24723 , 24744 , 24857 , 24882 , 24892 , 25038 , 25165 , 25166 , 25315 , 25560 , 25562 , 25573 , 25741 , 25840 , 
28132 , 28171 , 28443 , 29611);
UPDATE creature_template SET AIName='AgressorAI' WHERE entry IN(27241, 27605, 28246, 29351, 29358, 29460, 29857, 30320, 31137, 31702);
UPDATE creature_template SET flags_extra=flags_extra|0x80 WHERE entry IN (16914, 16995, 17001, 17231, 17274, 17611, 17687, 17992, 17999, 18225, 18242, 18306, 18307, 18729, 18757, 18759, 19041, 19276, 19277, 19278, 19279, 19326, 19328, 19329, 19358, 19359, 19376, 19381, 19439, 19555, 19597, 19654, 19677, 19727, 20003, 20023, 20024, 20093, 20114, 20286, 20288, 20289, 20391, 20431, 20473, 20475, 20476, 20520, 20608, 20676, 20709, 20796, 20825, 20889, 20982, 21073, 21080, 21095, 21097, 21109, 21116, 21119, 21120, 21159, 21241, 21290, 21351, 21365, 21447, 21456, 21498, 21512, 21641, 21654, 21756, 21791, 21792, 21793, 21794, 21807, 21814, 21855, 21856, 21921, 21939, 21940, 21946, 21957, 22008, 22039, 22054, 22083, 22121, 22228, 22240, 22246, 22269, 22288, 22320, 22340, 22400, 22417, 22422, 22436, 22449, 22470, 22503, 22504, 22519, 22520, 22523, 22831, 22851, 22905, 23378, 23443, 22833, 22872, 22934, 23409, 23454, 22986, 15730, 17163, 17367, 17368, 17369, 18599, 18665, 19010, 19198, 19563, 19577, 20128, 20425, 20675, 20863, 20979, 21281, 21297, 21321, 21360, 21418, 21473, 21800, 21880, 21910, 21929, 22418, 22524, 22838, 23037, 23040, 23056, 23059, 23063, 23078, 23116, 23173, 23199, 23212, 23255, 23260, 23262, 23278, 23301, 23323, 23328, 23382, 23385, 23395, 23500, 23512, 23488, 24109, 23758, 19237, 22296, 22829, 22839, 22866, 22867, 22868, 22916, 22921, 22927, 23043, 23081, 23138, 23155, 23209, 23213, 23225, 23228, 23240, 23250, 23275, 23307, 23308, 23312, 23313, 23315, 23317, 23379, 23424, 23425, 23438, 23442, 23444, 23445, 23845, 23850, 23852, 23853, 23854, 23855, 24171, 24220, 23583, 23585, 23807, 23813, 23814, 23815, 23832, 23893, 23920, 24363, 24412, 24630, 24781, 24809, 24903, 24904, 24908, 24936, 25068, 25114, 25172, 25213, 25640, 25746, 25782, 25952, 25964, 25965, 25966, 26057, 26120, 26121, 26190, 26230, 26346, 26391, 27723, 27890, 26834, 26775, 26774, 29589, 29588, 28481, 24771, 23750, 23805, 23810, 23821, 23974, 24000, 24008, 24012, 24087, 24092, 24093, 24094, 24098, 24100, 24101, 24102, 24158, 24165, 24166, 24167, 24170, 24182, 24183, 24184, 24185, 24193, 24194, 24221, 24260, 24269, 24288, 24289, 24326, 24335, 24336, 24449, 24454, 24465, 24466, 24471, 24513, 24526, 24645, 24651, 24652, 24655, 24665, 24705, 24707, 24725, 24756, 24778, 24817, 24826, 24827, 24828, 24829, 24831, 24832, 24845, 24865, 24902, 24913, 24983, 25402, 25403, 25404, 25405, 25431, 25436, 25441, 25442, 25443, 25471, 25472, 25473, 25669, 25670, 25671, 25672, 25698, 25771, 25995, 26041, 26086, 26093, 26094, 26129, 26130, 26162, 26175, 26193, 26264, 26265, 26297, 26373, 26403, 26444, 26445, 26468, 26469, 26470, 26498, 26559, 26591, 26612, 26656, 26700, 26789, 26804, 26855, 26856, 26857, 26927, 26937, 27047, 27180, 27222, 27223, 27297, 27306, 27323, 27326, 27339, 27375, 27384, 27392, 27396, 27402, 27403, 27413, 27418, 27420, 27446, 27448, 27449, 27453, 27454, 27466, 27529, 27568, 27569, 27572, 27583, 27589, 27622, 27660, 27663, 27669, 27679, 27688, 27689, 27702, 27710, 27757, 27802, 27851, 27880, 27889, 27921, 27929, 27931, 27988, 27994, 28008, 28055, 28064, 28128, 28190, 28198, 28230, 28240, 28248, 28254, 28260, 28265, 28273, 28279, 28293, 28294, 28295, 28296, 28299, 28300, 28304, 28305, 28307, 28316, 28330, 28333, 28352, 28367, 28454, 28455, 28456, 28457, 28458, 28459, 28460, 28461, 28462, 28463, 28466, 28469, 28478, 28483, 28485, 28509, 28520, 28525, 28542, 28543, 28544, 28617, 28627, 28631, 28632, 28633, 28643, 28648, 28655, 28663, 28664, 28724, 28738, 28739, 28740, 28741, 28751, 28761, 28762, 28770, 28773, 28777, 28778, 28786, 28789, 28816, 28839, 28852, 28874, 28928, 28929, 28932, 28935, 29011, 29027, 29038, 29079, 29081, 29094, 29099, 29100, 29105, 29138, 29170, 29192, 29215, 29276, 29345, 29397, 29401, 29416, 29425, 29459, 29521, 29558, 29577, 29580, 29581, 29595, 29597, 29599, 29655, 29685, 29752, 29771, 29772, 29773, 29812, 29815, 29816, 29870, 29871, 29876, 29877, 29881, 29928, 29999, 30050, 30078, 30079, 30091, 30101, 30103, 30122, 30125, 30126, 30130, 30131, 30132, 30133, 30138, 30139, 30151, 30153, 30207, 30210, 30302, 30339, 30342, 30366, 30402, 30415, 30421, 30492, 30514, 30559, 30577, 30588, 30589, 30598, 30614, 30646, 30649, 30650, 30651, 30655, 30669, 30700, 30707, 30712, 30742, 30744, 30745, 30749, 30832, 30878, 30887, 30950, 30990, 31006, 31047, 31077, 31092, 31105, 31117, 31246, 31264, 31272, 31312, 31353, 31358, 31364, 31400, 31481, 31517, 31576, 31577, 31641, 31643, 31653, 31683, 31684, 31743, 31745, 31767, 31777, 31794, 31811, 31817, 31845, 31869, 31880, 31887, 31888, 31915, 32167, 32168, 32202, 32224, 32232, 32256, 32264, 32266, 32277, 32298, 32314, 32318, 32319, 32328, 32339, 32366, 32427, 32431, 32442, 32504, 32520, 32527, 32530, 32531, 32532, 32575, 32606, 32608, 32647, 32662, 32694, 32742, 32782, 32784, 32819, 32824, 32825, 32827, 32829, 32831, 33045, 33192, 33377, 33742, 33835, 33874, 33953, 33958, 34157, 34213, 34319, 26688, 35614, 34381, 35089, 35106, 34810, 36155, 35014, 34743, 34781, 35376, 35820, 35227, 35226, 35062, 35821, 34755, 36093, 35015, 34562, 34741, 34899, 34737, 34739, 34740, 36209, 34738, 35055, 35297, 36099, 34548, 34879, 36944, 36945, 36946, 36947, 36983, 37543, 37574, 38751, 37814, 35228, 36737, 35018, 36189, 38121, 38548, 37801, 37745, 38289, 38310, 38353, 38527, 38528, 38587, 38588, 38870, 38882, 38907, 39135, 39581, 39672, 39743, 39744, 39841, 39842, 40151, 40199, 40218, 40263, 40363, 40479, 40506, 40617)
AND lootid=0 AND skinloot=0 AND pickpocketloot=0 AND VehicleId=0 AND (IconName IS NULL OR IconName='') AND gossip_menu_id=0 AND npcflag=0 AND mingold=0 AND mechanic_immune_mask=0 AND ScriptName='';
UPDATE creature_template SET InhabitType=5 WHERE entry IN(21801);
UPDATE creature_template SET unit_flags=262144+33554432, flags_extra=130 WHERE entry=27869;
UPDATE creature_template SET faction=120 WHERE entry=8320;
UPDATE creature_template SET dynamicflags=0 WHERE entry=26873;
UPDATE creature_template SET modelid2=0 WHERE modelid2=17116;
UPDATE creature_template SET unit_flags=0 WHERE entry=19055;
UPDATE creature_template SET dynamicflags=8+32 WHERE entry=26514;
UPDATE creature_template SET unit_flags=32768 WHERE entry=30204;
DELETE FROM spell_linked_spell WHERE spell_trigger=56418;
INSERT INTO spell_linked_spell VALUES (56418, -56422, 0, "remove submerge on cast");