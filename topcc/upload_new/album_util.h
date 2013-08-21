/*
 * album_util.h
 *
 *  Created on: 2012-2-16
 *      Author: Administrator
 *      Copyright: renren-inc
 */

#ifndef TOPCC_UPLOAD_NEW_ALBUM_UTIL_H_
#define TOPCC_UPLOAD_NEW_ALBUM_UTIL_H_

#include <stdio.h>
#include <map>
#include <string>
#include <vector>

namespace album_util {

struct GroupName_Domain {
  const char* group_name;
  const char* domain;
};
class OpiConstants {
 public:
  static const std::string kUrlPhoto2Album;
  static const std::string kUrlPhotoAlbum;
  static const std::string kUrlHead;
  static const std::string k5q;
  static const std::string kPic001;
  static const std::string kPic002;
  static const std::string kUrlPic001;
  static const std::string kUrlPic002;
  static const std::string kUrlImg;
  static const std::string kKaixinPrefix;
  static const char kFileSeparator;
};
class Node {
 public:
  Node();
  explicit Node(const char* domain);
  const std::string& domain() const;
 private:
  std::string domain_;
};
/**
 * a \Group has one or more \Nodes
 */
class Group {
 public:
  explicit Group(const char* name);
  ~Group();
 public:
  void addNode(const char* domain);
  /**
   * pick a node from group's nodes according to fileName's hash code
   */
  const Node* getNode(const std::string& fileName) const;
  const std::string& name() const;
 private:
  std::string name_;
  std::vector<Node*> nodes_;
};
class Domain {
 public:
  typedef std::map<std::string, Group*> NameGroupMap;
  Domain();
  ~Domain();
 public:
  Group* getGroup(const std::string& groupName);
 private:
  void loadGroups();
 private:
  NameGroupMap name_group_map_;
};
class Allocator {
 protected:
  Allocator();
  ~Allocator();
 public:
  static Allocator& getInstance();
  std::string locate(const std::string& pattern);
 private:
  Domain* domain_;
 private:
  static Allocator self_;
};

class AlbumUtil {
 public:
  AlbumUtil();
 public:
  static std::string getPhotoURL(const std::string& path);
 private:
  static std::string getPhotoDomain(const std::string& flag);
  static bool pathStartsWithDate(const std::string& path, std::string& url);
};
}  // album_util namespace

#endif  // TOPCC_UPLOAD_NEW_ALBUM_UTIL_H_
