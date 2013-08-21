#include "restoremanage.h"
#include "util.h"

namespace upload {

RestoreManager::RestoreManager()
: init_create_hour_(12*60) // 12小时
, interval_time_(20)      //  20分钟
, last_time_(0)          //   
, time_slot_(30)         // 每次建30分钟的目录,基本上比interval_time_大一些
, next_time_(8*60) {}    // 当前时间的偏移8小时,基本上init_create_hour_小一些

// fmn053/xiaozhan/20110801/2005/x_large_oXOk_5277000000055c93.jpg
// 创建某个业务的存储目录,根据时间段
bool MakeAllDir(const ServiceConf& sc, int time_slot, int next_time) {
  //std::vector<SaveGroupConf> savegroups_; ///
  //std::string savesubdir_;                ///
  std::string dir;
  for (std::vector<SaveGroupConf>::const_iterator it = sc.savegroups_.begin(); it != sc.savegroups_.end(); ++it) {
  //std::string groupname_;           ///<组名,dbpath必须依此开头
  //std::string savenodepath_;        ///<保存节点路径,filepath必须依次开头,注:节点目前仅支持1个节点
    dir = it->savenodepath_;
    dir += "/";
    if (!sc.savesubdir_.empty()) {
      dir += sc.savesubdir_;
      dir += "/";
    }
    for (int i = 0; i < time_slot; i += 5) {
      string timestr = GetTimeStr(time(NULL) + (next_time + i) * 60);
      dir += timestr;
      Mkdir(dir);
    }
  }
  return true;
}

bool RestoreManager::TillAllDir(const RuntimeConfig& conf) {
  const std::map<std::string,ServiceConf>& scs = conf.uploadconfs();
  for (std::map<std::string,ServiceConf>::const_iterator it = scs.begin(); it != scs.end(); ++it) {
    const ServiceConf& sc = it->second;
    MakeAllDir(sc, time_slot_, next_time_);
  }
  return true;
}

// 得到fmn/time/ 和/bigpool/fmn/time/目录
bool RestoreManager::GetPath(const ServiceConf& sc, const bool gif, std::string *urlpath, std::string *savepath) {
  if (!urlpath || !savepath)
    return false;
  const std::vector<SaveGroupConf>* savegroups = NULL;
  if (gif) {
    savegroups = &sc.gif_savegroups_;
  }
  else {
    savegroups = &sc.savegroups_;
  }
  if (savegroups->empty()) return false;

  int size = savegroups->size();
  int index = GetRandomNumber(0, size);
  const SaveGroupConf& sg = (*savegroups)[index];
  *urlpath = sg.groupname_; *urlpath += "/";
  *savepath = sg.savenodepath_; *savepath += "/";
  if (!sc.savesubdir_.empty()) {
    *urlpath += sc.savesubdir_; *urlpath += "/";
    *savepath += sc.savesubdir_; *savepath += "/";
  }
  std::string timestr = GetTimeStr();
  *urlpath += timestr; *urlpath += "/";
  *savepath += timestr; *savepath += "/";
  //Mkdir(*savepath);
  return true;
}

bool RestoreManager::Init(const RuntimeConfig& conf) {
  for (std::map<std::string,ServiceConf>::const_iterator it = conf.uploadconfs().begin(); it != conf.uploadconfs().end(); ++it) {
    const ServiceConf& sc = it->second;
    MakeAllDir(sc, time_slot_, next_time_);
  }
  return true;
}

bool RestoreManager::SaveFile(const string &path, const string& content) {
  int err;
  if (!::SaveFile(path, content, &err)) {
    if (err == 2)
      Mkdir2(path);
    else
      return false;
    return ::SaveFile(path, content);
  }
  return true;
}

}
