/*
Navicat MySQL Data Transfer

Source Server         : web
Source Server Version : 50162
Source Host           : 127.0.0.1:3306
Source Database       : web

Target Server Type    : MYSQL
Target Server Version : 50162
File Encoding         : 65001

Date: 2012-07-27 10:20:57
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `savegroups`
-- ----------------------------
DROP TABLE IF EXISTS `savegroups`;
CREATE TABLE `savegroups` (
  `id` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `value` varchar(1024) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of savegroups
-- ----------------------------
INSERT INTO `savegroups` VALUES ('1', 'abc');
