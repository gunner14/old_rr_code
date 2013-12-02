/*
  Copyright 2012-09-12 shiqiang.ding@renren-inc.com
                       dinggangyilang@gmail.com

  This file is part of XCS.
*/

#include "properties_configuration.h"
#include <map>
#include <string>
#include <utility>
#include <fstream>

namespace xcs {
std::string PropertiesConfiguration::Trim(std::string str) {
  int i, j;
  for (i = 0; i < str.size(); i++) {
    if (str[i] != ' ' && str[i] != '\t')
      break;
  }
  for (j = str.size() - 1; j >= 0; j--) {
    if (str[j] != ' ' && str[j] != '\t')
      break;
  }
  if (i > j)  return "";
  else  return str.substr(i, j - i + 1);
}
PropertiesConfiguration::PropertiesConfiguration(std::string file_path) {
  if (file_path == "")  return;
  std::ifstream cfg(file_path.c_str());
  if (cfg.is_open() && cfg.good()) {
    std::string cfg_line;
    std::map<std::string, std::string> props;
    while (getline(cfg, cfg_line)) {
      int index = cfg_line.find("=");
      if (index == -1)
        break;
      std::string key = cfg_line.substr(0, index);
      std::string value = cfg_line.substr(index + 1);
      props.insert(std::pair<std::string, std::string>(Trim(key), Trim(value)));
    }
    std::map<std::string, std::string>::iterator it;
    it = props.find("root");
    if (it != props.end())  root_ = it->second;
    it = props.find("username");
    if (it != props.end()) username_ = it->second;
    it = props.find("password");
    if (it != props.end()) password_ = it->second;
    it = props.find("cluster");
    if (it != props.end()) address_ = it->second;
  }
}
}
