#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"
#include "UtilCxx/src/TaskManager.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "socialgraph/newsocialfeed/socialfeedworker/common/config_parser.h"
#include "socialgraph/newsocialfeed/socialfeedworker/builder/feed_push_task.h"

#include "socialfeedworker.h"

using namespace xce::serverstate;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::socialgraph;

void  MyUtil::initialize() {

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&SocialFeedWorker::instance(), service.createIdentity("SocialFeedWorker", ""));

  ConnectionPoolManager::instance().initialize();
  
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("SocialFeedWorker.Mod");
  int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("SocialFeedWorker.Interval", 5);

  //向controller注册     
  ServerStateSubscriber::instance().initialize("ControllerSocialFeedWorker", 
					&SocialFeedWorker::instance(), mod, sInterval, new MyUtil::SocialGraphChannel());

  ServerStateSubscriber::instance().setStatus(1);

}

//----------------------------------------------------    

namespace xce {
namespace socialgraph {

void SocialFeedWorker::NotifySocialFeedUserLogin(int userid, const Ice::Current& current) {
	MCE_INFO("SocialFeedWorker::NotifySocialFeedUserLogin login userid:" << userid);
        int task_size = TaskManager::instance().size(TASK_LEVEL_FEED_PUSH);   
        if (task_size > 10000){
            MCE_INFO("SocialFeedWorker::NotifySocialFeedUserLogin userid:" << userid << " task count:" << task_size); 
        }
        time_t  nowTime    = time(NULL);
        struct  tm* tm_now = localtime(&nowTime);
        if (((nowTime - _lastUpdateUidListTimeSec) > CONST_UPDATE_UID_LIST_INTERVAL) &&
                         (tm_now!=NULL && (tm_now->tm_hour >= 0 && tm_now->tm_hour < 5))){//do update
                IceUtil::RWRecMutex::WLock  lock(_monitor);
                if((nowTime - _lastUpdateUidListTimeSec) > CONST_UPDATE_UID_LIST_INTERVAL){
                   MCE_INFO("SocialFeedWorker::NotifySocialFeedUserLogin update white user list");
                   if(LoadWhiteUserList()){
                      _lastUpdateUidListTimeSec = time(NULL);
                   }
                }
        }else{//to update
                IceUtil::RWRecMutex::RLock  lock(_monitor);
                if(_userMap.find(userid) != _userMap.end()){
  	           TaskManager::instance().execute(new FeedPushTask(userid, 1));
                   MCE_INFO("SocialFeedWorker::NotifySocialFeedUserLogin userid:" << userid << " hit!");
                }
             }
}

bool   SocialFeedWorker::LoadWhiteUserList(){
          string  userid_file = CONST_WHITE_UID_LIST_FILE;   
          FILE*   fp = fopen(userid_file.c_str(), "r");
          if (NULL == fp){
              MCE_ERROR("Userid File is not exist.");
              return false;
          }
          MCE_INFO("SocialFeedWorker::LoadWhiteUserList loading...");
          _userMap.clear();
          char* charptr = NULL;
          ssize_t  read;
          size_t   len;
          while ((read = getline(&charptr, &len, fp)) != -1){   
                 std::string  buffer = charptr;
                 size_t    bufferlen = buffer.size();
                 if (bufferlen == 0) {   
                     continue;
                 }
                 if (buffer[bufferlen - 1] == '\r') {
                     buffer.resize (bufferlen - 1);
                 }
                 int tempuserid = (int)atoi(buffer.c_str());
                 std::pair<std::map<int, bool>::iterator, bool> pr =
                                             _userMap.insert(pair<int, bool>(tempuserid,true));
                 if (!pr.second){
                     MCE_ERROR("SocialFeedWorker::LoadWhiteUserList Userid File insert userid  failed: " << tempuserid);
                   //  return;
                 }
                 if (charptr) {
                     free(charptr);
                     charptr = NULL;
                 }
          }  
          fclose(fp);
          MCE_INFO("SocialFeedWorker::LoadWhiteUserList loaded ok");
          return  true;
   }
}
}
