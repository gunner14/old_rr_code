#ifndef _RESTORE_MANAGE_H_
#define _RESTORE_MANAGE_H_
#include "runtimeconfig.h"

namespace upload {
// 功能为提前建立一个形如
// /bigpool/photos/hdn121/20101230/1200的目录结构,目前每5分钟一个目录
class RestoreManager {
 public:
  RestoreManager();
  bool Init(const RuntimeConfig& conf);
  static bool SaveFile(const std::string &path,const std::string& content);
  static bool GetPath(const ServiceConf& sc, const bool gif, std::string *urlpath, std::string *savepath);
  bool TillAllDir(const RuntimeConfig& conf);
 private:
  int init_create_hour_; // 初始化是创建的分钟数
  int interval_time_;    // 其后多长时间建立一次,分钟为单位
  time_t last_time_;     // 最后一次建立时间
  int time_slot_;        // 每次建多长时间的目录,分钟为一个单位
  int next_time_;        // 向后延迟的时间,比如建24小时后的time_slot_时间的目录,分钟为单位
  // 1,----------[     init_create_hour_        ]---------------------
  // 2,------------now----[last_time_]--------------------------------
  // 3,-------------------[now,last_time_]---[next_time_,time_slot_]--
};
}
#endif
