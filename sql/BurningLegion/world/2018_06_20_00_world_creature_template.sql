
--- Trueshot lodge sentinel detection announcement text
DELETE FROM creature_text WHERE CreatureID IN (100709, 100491, 103444);
INSERT INTO creature_text (CreatureID, GroupID, ID, Text, Type, Language, Probability, Emote, Duration, Sound, BroadcastTextId, TextRange, comment) VALUES 
(100709, 0, 0, 'This sacred peak is for hunters alone. Turn back, or the eagle spirit will carry you away!', 42, 0, 100, 0, 0, 0, 0, 0, ''),
(100491, 0, 0, 'This sacred peak is for hunters alone. Turn back, or the eagle spirit will carry you away!', 42, 0, 100, 0, 0, 0, 0, 0, ''),
(103444, 0, 0, 'This sacred peak is for hunters alone. Turn back, or the eagle spirit will carry you away!', 42, 0, 100, 0, 0, 0, 0, 0, '');

INSERT INTO creature_text_locale (CreatureID, GroupID, ID, Locale, Text) VALUES 
(100709, 0, 0, 'ruRU', 'На эту священную вершину допускаются только охотники, Поворачивайте назад, или дух орла унесет вас отсюда!'),
(100491, 0, 0, 'ruRU', 'На эту священную вершину допускаются только охотники, Поворачивайте назад, или дух орла унесет вас отсюда!'), 
(103444, 0, 0, 'ruRU', 'На эту священную вершину допускаются только охотники, Поворачивайте назад, или дух орла унесет вас отсюда!');

-- Trueshot lodge sentinel
UPDATE creature_template SET ScriptName = 'npc_trueshot_lodge_sentinel' WHERE entry =100709;
UPDATE creature_template SET ScriptName = 'npc_trueshot_lodge_sentinel' WHERE entry=100491;
UPDATE creature_template SET ScriptName = 'npc_trueshot_lodge_sentinel' WHERE entry =103444;
