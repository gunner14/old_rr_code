/*
Navicat MySQL Data Transfer

Source Server         : web
Source Server Version : 50162
Source Host           : 127.0.0.1:3306
Source Database       : web

Target Server Type    : MYSQL
Target Server Version : 50162
File Encoding         : 65001

Date: 2012-07-27 10:20:42
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `channels`
-- ----------------------------
DROP TABLE IF EXISTS `channels`;
CREATE TABLE `channels` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(64) NOT NULL DEFAULT '',
  `savegroup_id` tinyint(3) unsigned zerofill DEFAULT NULL,
  `handleimages` varchar(1024) DEFAULT NULL,
  `minwidth` int(11) DEFAULT NULL,
  `minheight` int(11) DEFAULT NULL,
  `maxfilesize` tinyint(3) unsigned DEFAULT '15',
  `exts` varchar(128) DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_k` (`name`) USING BTREE,
  KEY `savegroup_fk` (`savegroup_id`),
  CONSTRAINT `savegroup_fk` FOREIGN KEY (`savegroup_id`) REFERENCES `savegroups` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of channels
-- ----------------------------
INSERT INTO `channels` VALUES ('1', 'abc', '001', 'adf', '1', '1', '15', '');
