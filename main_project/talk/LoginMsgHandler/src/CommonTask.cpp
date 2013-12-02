#include "CommonTask.h"
#include "LoginHandlerI.h"
#include "XmppTools.h"
#include "IceLogger.h"
#include "QueryRunner.h"
#include <boost/lexical_cast.hpp>
#include "TalkStatAdapter.h"

using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk;
using namespace MyUtil;
using namespace com::xiaonei::xce;

StatManager::StatManager() {
  MyUtil::TaskManager::instance().schedule(new StatTask(5000));
};

void StatManager::addToAction(const StatInfoPtr& st) {
  IceUtil::Mutex::Lock lock(_mutex);
  if (!st) {
    return;
  }
  _actionSeq.insert(_actionSeq.end(),st);
}
void StatManager::addToAction(const StatInfoSeq& seq) {
  IceUtil::Mutex::Lock lock(_mutex);
  if (seq.empty()) {
    return;
  }
  _actionSeq.insert(_actionSeq.end(), seq.begin(), seq.end());
}

void StatManager::doInsertAction() {
  StatInfoSeq seq;
  {
    IceUtil::Mutex::Lock lock(_mutex);
    _actionSeq.swap(seq);
  }
  if (!seq.empty()) {
    MyUtil::IntSeq loginSeq,wloginSeq;
    Statement sql;
    sql << "insert into xntalk_action (userid, time,action,value) values";
    for (size_t i = 0; i < seq.size(); i++) {
      if(seq.at(i)->action == "talk_login"){
        loginSeq.push_back(seq.at(i)->userId);
      }else if(seq.at(i)->action == "webpager_login"){
        wloginSeq.push_back(seq.at(i)->userId);
      }
      if (i)sql<<",";
      sql<<"(" <<seq.at(i)->userId <<",from_unixtime(" <<seq.at(i)->timestamp<<"),'"<<seq.at(i)->action <<"',"<<seq.at(i)->value<<")";
    }

    if(!loginSeq.empty()){
      try{
        ::talk::stat::TalkStatAdapter::instance().login(loginSeq);
      } catch(Ice::Exception & e){
        MCE_WARN("StatManager::doInsertAction --> do TalkStat.login err:"<< e);
      } catch(...){
        MCE_WARN("StatManager::doInsertAction --> do TalkStat.login err");
      }
    }
    if(!wloginSeq.empty()){
      try{
        ::talk::stat::TalkStatAdapter::instance().wLogin(wloginSeq);
      } catch(Ice::Exception& e){
        MCE_WARN("StatManager::doInsertAction --> do TalkStat.wlogin err:"<<e);
      } catch(...){
        MCE_WARN("StatManager::doInsertAction --> do TalkStat.wlogin err");
      }
    }
    QueryRunner("im_stat", CDbWServer).store(sql);
  }

}

//-----------------------------------------------------

void StatManager::StatTask::handle() {
  MCE_DEBUG("run StatTask::handle()");
  MyUtil::TaskManager::instance().schedule(new StatManager::StatTask(_timeslot));
  StatManager::instance().doInsertAction();

}

void IpLogTask::handle(){
  if(ip_.empty()){
    return;
  }
  Statement sql;
  sql << "insert into iplog (id, ip, time, type) values(" << userid_ << ", '" << ip_ << "', FROM_UNIXTIME(" << time(NULL) << "), 3);";
  QueryRunner("logging", CDbWServer).store(sql);
}



