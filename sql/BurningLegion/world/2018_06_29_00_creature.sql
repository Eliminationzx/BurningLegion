---Fix paladin order hall guard
UPDATE `creature_template` SET `ScriptName`='npc_sanctum_of_light_guard' WHERE `entry` IN (92148, 92151);
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES ('92151', '0', '0', 'You don\'t belong here!', '12', '0', '100', '0', '0', '0', '0', '0', '');
INSERT INTO `creature_text` (`CreatureID`, `GroupID`, `ID`, `Text`, `Type`, `Language`, `Probability`, `Emote`, `Duration`, `Sound`, `BroadcastTextId`, `TextRange`, `comment`) VALUES ('92148', '0', '0', 'Get out!', '12', '0', '100', '0', '0', '0', '0', '0', '');
INSERT INTO `creature_text_locale` (`CreatureID`, `GroupID`, `ID`, `Locale`, `Text`) VALUES ('92148', '0', '0', 'ruRU', 'Убирайся!');
INSERT INTO `creature_text_locale` (`CreatureID`, `GroupID`, `ID`, `Locale`, `Text`) VALUES ('92151', '0', '0', 'ruRU', 'Тебе здесь не место!');