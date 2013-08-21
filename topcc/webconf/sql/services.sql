/*
Navicat MySQL Data Transfer

Source Server         : web
Source Server Version : 50162
Source Host           : 127.0.0.1:3306
Source Database       : web

Target Server Type    : MYSQL
Target Server Version : 50162
File Encoding         : 65001

Date: 2012-07-27 10:21:02
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `services`
-- ----------------------------
DROP TABLE IF EXISTS `services`;
CREATE TABLE `services` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` bigint(20) unsigned DEFAULT NULL,
  `channel_id` bigint(20) unsigned DEFAULT NULL,
  `name` varchar(64) NOT NULL DEFAULT '',
  `watermark` tinyint(1) DEFAULT '0',
  `markpath` varchar(512) DEFAULT NULL,
  `savesubdir` varchar(512) DEFAULT NULL,
  `handleexif` tinyint(1) DEFAULT '1',
  `checkcomment` tinyint(1) DEFAULT '0',
  `outputtype_id` tinyint(4) unsigned DEFAULT NULL,
  `tickettype_id` tinyint(4) unsigned DEFAULT NULL,
  `ticketsuffix` varchar(255) DEFAULT '',
  `autorotate` tinyint(1) unsigned DEFAULT '1',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_k` (`name`) USING BTREE,
  KEY `userid_fk` (`user_id`),
  KEY `outputtypeid_fk` (`outputtype_id`),
  KEY `ticketid_fk` (`tickettype_id`),
  KEY `channelid_fk` (`channel_id`),
  CONSTRAINT `channelid_fk` FOREIGN KEY (`channel_id`) REFERENCES `channels` (`id`),
  CONSTRAINT `outputtypeid_fk` FOREIGN KEY (`outputtype_id`) REFERENCES `outputtypes` (`id`),
  CONSTRAINT `ticketid_fk` FOREIGN KEY (`tickettype_id`) REFERENCES `tickets` (`id`),
  CONSTRAINT `userid_fk` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of services
-- ----------------------------
