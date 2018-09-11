SET @OGUID = 5929;
SET @OENTRY = 125;
 
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 184758;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(184758, 183400, 0, 'Firmament Stone');
 
DELETE FROM `gameobject` WHERE `guid` = @OGUID+1;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `PhaseId`, `PhaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `VerifiedBuild`) VALUES
(@OGUID+1, @OENTRY, 1220, 0, 0, 3, 1, 0, 2996.744141, 3828.953857, 1361.763062, 4.919419, -0, -0, -0.5952, 0.803577, 120, 255, 1, 24461);
 
DELETE FROM `gameobject_template` WHERE `entry` = @OENTRY;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `Data24`, `Data25`, `Data26`, `Data27`, `Data28`, `Data29`, `Data30`, `Data31`, `Data32`, `RequiredLevel`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES
(@OENTRY, 8, 0, 'Firmament Stone(Spell Focus)', '', '', '', 1, 1864, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0);

DELETE FROM `spell_loot_template` WHERE `entry` IN (188307, 188310, 188313, 188316, 188319, 188322, 188325, 188331, 188334, 188337, 188340, 188343, 188346, 188349, 188299, 188800);
INSERT INTO `spell_loot_template` (`entry`,`Item`,`Chance`,`GroupId`,`MinCount`,`MaxCount`) VALUES
(188307, '127837', '100', '0', '1','1'),
(188310, '127838', '100', '0', '1','1'),
(188313, '127839', '100', '0', '1','1'),
(188316, '127840', '100', '0', '1','1'),
(188319, '127841', '100', '0', '1','1'),
(188322, '127842', '100', '0', '1','1'),
(188325, '127843', '100', '0', '1','1'),
(188331, '127845', '100', '0', '1','1'),
(188334, '127846', '100', '0', '1','1'),
(188337, '127847', '100', '0', '1','1'),
(188340, '127848', '100', '0', '1','1'),
(188343, '127849', '100', '0', '1','1'),
(188346, '127850', '100', '0', '1','1'),
(188349, '127851', '100', '0', '1','1'),
(188299, '127834', '100', '0', '1','1'),
(188800, '137597', '0', '1', '1','1'),
(188800, '137596', '0', '1', '1','1'),
(188800, '137595', '0', '1', '1','1');