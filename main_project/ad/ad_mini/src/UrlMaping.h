/*
 * AdUrlMaping.h
 *
 *  Created on: 2010-12-13
 *      Author: ark
 */

#ifndef ADURLMAPING_H_
#define ADURLMAPING_H_

#include <string>
#include <set>
#include <vector>
#include <Ice/Ice.h>
#include "Singleton.h"
#include "TaskManager.h"

namespace xce {
namespace ad {

class AdUrlMaping : public MyUtil::Singleton<AdUrlMaping> {
public:
  AdUrlMaping();
  virtual ~AdUrlMaping();
  void GetAdZone(const string& Url, set<Ice::Long>& AdZones);
  bool Load(const string& db);

private:
  void PreProcess(const string& Url, vector<string>& UrlSeq);
  map< string, set<Ice::Long> > url_adzone_map_;
  map<pair<string,int>, set<Ice::Long> > map_;
  IceUtil::RWRecMutex mutex_;
};

class AdUrlMapingLoadTimer : public MyUtil::Timer{
public:
  AdUrlMapingLoadTimer(const string& db, int time = 60*1000):Timer(time), db_(db){}
  virtual void handle() {
    MyUtil::TaskManager::instance().schedule(new AdUrlMapingLoadTimer(db_, 60*60*1000));//60分钟
    AdUrlMaping::instance().Load(db_);
  }
private:
  string db_;
};
}
}
#endif /* ADURLMAPING_H_ */
