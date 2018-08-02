-- PREMIUM ACCOUNTS

CREATE TABLE IF NOT EXISTS `account_premium` (
  `id` int(11) NOT NULL DEFAULT '0' COMMENT 'Account id',
  `setdate` int(4) NOT NULL DEFAULT '0',
  `unsetdate` int(4) NOT NULL DEFAULT '0',
  `premium_type` tinyint(4) unsigned NOT NULL DEFAULT '0',
  `active` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `active` (`active`),
  KEY `setdate` (`setdate`),
  KEY `unsetdate` (`unsetdate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;