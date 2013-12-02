/*
 * =====================================================================================
 *
 *       Filename:  GeneralWhiteList.h
 *
 *    Description:  通用白名单，需要PropertiesManager支持。
 *
 *        Version:  1.0
 *        Created:  2012年09月19日 15时15分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Duo Zhang (), duo.zhang@renren-inc.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _GENERAL_WHITE_LIST_H_
#define _GENERAL_WHITE_LIST_H_
#include "PropertiesManager.h"
#include <map>
#include <set>
#include <string>

namespace xce {
namespace feed {

class GeneralWhiteList : public UpdaterBase, public MyUtil::Singleton<GeneralWhiteList> {
  typedef std::vector<std::set<long> > WhiteListVec;
public:
  GeneralWhiteList() {
    PropertiesManager::instance().AppendUpdater(this);
  }

  virtual void Update() {
    WhiteListVec temp;
    {
      IceUtil::RWRecMutex::RLock lock(datamutex_);
      for(unsigned int i = 0; i < data_.size(); ++i) {
        temp.push_back(std::set<long>());
        insertSet(keys_[temp.size()-1], *temp.rbegin());
      }
    }
    IceUtil::RWRecMutex::WLock lock(datamutex_);
    for(unsigned int i = 0; i < temp.size(); ++i) {
      if (temp[i].size() <= 0) {
        continue;
      }
      temp[i].swap(data_[i]);
      MCE_INFO("GeneralWhiteList::Update -> " << keys_[i] << " is Available!");
    }
  }

  //增加白名单的名称到维护的列表中，返回key对应的id。在其他操作中使用key对应的id来标识一个白名单
  //在实际配置中的key为："Service." + getBusiness() + ".GeneralWhiteList." + key
  int AppendMap(const std::string & key) {
    MCE_INFO("GeneralWhiteList::AppendMap >> " << key);
    IceUtil::RWRecMutex::WLock lock(datamutex_);
    keys_.push_back(key);
    data_.push_back(std::set<long>());
    insertSet(keys_[data_.size()-1], *data_.rbegin());
    return data_.size()-1;
  }

  bool IsInWhiteList(int key, long id) {
    IceUtil::RWRecMutex::RLock lock(datamutex_);
    return data_[key].find(id) != data_[key].end();
  }

  std::set<long> GetWhiteList(int key) {
    IceUtil::RWRecMutex::RLock lock(datamutex_);
    return data_[key];
  }

  int ChangeKeytoId(const std::string & keystr) {
    IceUtil::RWRecMutex::RLock lock(datamutex_);
    for(unsigned int n = 0; n < keys_.size(); ++n) {
      if(keys_[n] == keystr) {
        return n;
      }
    }
    return -1;
  }

private:
  void insertSet(const std::string & key, std::set<long> & wl) {
    if (key == "") {
      return ;
    }
    const std::string & value = getProperty(key);
    std::vector<std::string> strvec;
    boost::algorithm::split(strvec, value, boost::algorithm::is_space());
    std::set<long> res;
    for(unsigned int i = 0; i < strvec.size(); ++i) {
      long num = std::atol(strvec[i].c_str());
      if (num == 0) {
        continue;
      }
      wl.insert(num);
    }
  }

  std::string getProperty(const std::string & key) {
    ServiceI & service = ServiceI::instance();
    std::ostringstream oskey;
    oskey << "Service." << service.getBusiness() << ".GeneralWhiteList." << key.c_str();
    return service.getCommunicator()->getProperties()->getPropertyWithDefault(oskey.str(), "");
  }

  IceUtil::RWRecMutex datamutex_;
  WhiteListVec data_;
  std::vector<std::string> keys_;
};

}
}

#endif
