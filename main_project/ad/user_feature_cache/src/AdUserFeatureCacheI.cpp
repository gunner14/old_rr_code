/* AdUserFeatureCacheI.cpp
 *
 *  Created on: 2012-03-16
 *      Author: lei.yu1
 */

#include <sstream>
#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include <boost/lexical_cast.hpp>
#include "../../util/src/AdCommonUtil.h"
#include "../include/AdUserFeatureCacheI.h"
#include "client/linux/handler/exception_handler.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace std;
using namespace boost;
using namespace MyUtil;
using namespace mysqlpp;
using namespace xce::ad;

IceUtil::RWRecMutex DATA_MUTEX;
IceUtil::RWRecMutex VERSION_MUTEX;

const int TEN_MINUTES = 10 * 60 * 1000 * 1000;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
  printf("got crash %s %s\n", dump_path, minidump_id);
  return true;
}

void MyUtil::initialize() {
  //google breakpad for dump
  srand(time(NULL));
  static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
  // service start
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&AdUserFeatureCacheI::instance(), service.createIdentity("ADEDM", ""));
  TaskManager::instance().execute(new LoadTask());

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("AdUserFeatureCache.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AdUserFeatureCache.Interval",5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerAdUserFeatureCache",&AdUserFeatureCacheI::instance(),mod,interval,new EdmChannel());
  MCE_INFO("MyUtil::initialize start ");
}

AdUserFeatureCacheI::AdUserFeatureCacheI() : initOK_(false), isLoading_(false){
}

string AdUserFeatureCacheI::GetFeature(Ice::Long userID, const Ice::Current&){
    if (!initOK_){
        MCE_INFO("AdUserFeatureCacheI::Not Init OK!");
	return "";
    }

    return userFeatureDataPtr_->GetUserFeature(userID);
}

bool AdUserFeatureCacheI::Rollback(const string& fileName, const Ice::Current&){
    if (fileName == GetNowFile()){
	MCE_WARN("Rollback File: " << fileName << " already exist in cache!");
	return false;	
    }
    /*
     * Invalid the cache file before load rollback file.
     * In case of after rollback file load success, reload the invalid data again 
     */
    if (initOK_){
        userFeatureDataPtr_->InvalidData();
    }
    
    bool ret = Load(fileName);
    return ret;
}


bool AdUserFeatureCacheI::HasNewFile(){
    stringstream sql;
    sql << " SELECT local_file, md5 FROM user_feature_version "
        << " WHERE id = (SELECT max(id) FROM user_feature_version) "
        << " AND valid = 1 ";

    StoreQueryResult res;
    try{
        const char* DB_HOST = "10.3.23.69";
        const char* DB_NAME = "ad_strategy";
        const char* DB_USER = "ad";
        const char* DB_PASSWORD = "rrad";

	Connection conn(DB_NAME, DB_HOST, DB_USER, DB_PASSWORD);
	Query query(&conn, true, sql.str().c_str());
	res = query.store();
    } catch (Ice::Exception& e){
        MCE_WARN("AdUserFeatureCacheI::HasNewVersion user_feature_version ice error! " << e.what());
        return false;
    } catch (std::exception& e){
        MCE_WARN("AdUserFeatureCacheI::HasNewVersion user_feature_version std error! " << e.what());
        return false;
    }

    if (!res){
        MCE_WARN("Query Result Error!");
        return false;
    }
    else if (res.empty()){
        MCE_INFO("nothing in db!");
        return false;
    }
    else if (res.size() > 1){
	MCE_WARN("There dirty data in user_feature_version.");
	return false;
    }

    string fileName = lexical_cast<string>(res.at(0)["local_file"]);
    string md5Value = lexical_cast<string>(res.at(0)["md5"]);
    //Loading Data of the newFileName now, not finished.
    if (isLoading_){
	MCE_INFO("The File: " << newFileName_ << " is loading.");
	return false;
    }

    /*1). Never Load Data Before. initOK_ == false
     *2). Already there is data in cache, and new file is ready.
     *    newFileName != data's fileName in cache,
     *    and the newFile content md5 != data's content md5 in cache.
     */  
    if ((!initOK_) 
        || ( (fileName != userFeatureDataPtr_->GetFileName())
             && (md5Value != userFeatureDataPtr_->GetMd5Value())
           )
       ){
        IceUtil::RWRecMutex::WLock lock(VERSION_MUTEX);
        {
            newFileName_ = fileName;
            newMd5Value_ = md5Value;;
        }
	return true;
    }
    return false;
}

string AdUserFeatureCacheI::GetNewFile(){
    IceUtil::RWRecMutex::RLock lock(VERSION_MUTEX);
    {
        return newFileName_;
    }
}

bool AdUserFeatureCacheI::Load(const string& fileName){
    if (0 == userFeatureDataPtr_.get()){
	userFeatureDataPtr_ = new AdUserFeatureData(fileName, newMd5Value_);
    }

    if (!userFeatureDataPtr_->IsValidFile(fileName, newMd5Value_)){
	return false;
    }

    string oldFileName = userFeatureDataPtr_->GetFileName();
    string oldMd5Value = userFeatureDataPtr_->GetMd5Value();

    bool ret = userFeatureDataPtr_->LoadData(fileName, newMd5Value_);
    if (!ret) {
        userFeatureDataPtr_->Clear();
        if ((0 != oldFileName.size()) 
	   && (0 != oldMd5Value.size())){
	    userFeatureDataPtr_->LoadData(oldFileName, oldMd5Value);
        }
	MCE_WARN("AdUserFeatureCacheI::Load " << fileName << " Error!");
	return false;
    }
    
    return true;
}

string AdUserFeatureCacheI::GetNowFile(const Ice::Current&){
    if (initOK_){
	return userFeatureDataPtr_->GetFileName();
    }
    return "";
}


void LoadTask::handle(){
    try{
        while (true){
	    MCE_INFO("LoadTask::handle --> AdUserFeatureCache::load start");
            if (AdUserFeatureCacheI::instance().HasNewFile()){
                std::string newFile = AdUserFeatureCacheI::instance().GetNewFile();
		AdUserFeatureCacheI::instance().BeginLoad();
	        AdUserFeatureCacheI::instance().Load(newFile);
		AdUserFeatureCacheI::instance().EndLoad();
            }//end if(AdUserFeatureCacheI::instance().HasNewFile())
            MCE_INFO("LoadTask::handle --> AdUserFeatureCache::load done");
            usleep(TEN_MINUTES);
       }// end while (true)
    } catch (std::exception& e){
       MCE_WARN("LoadTask --> handle ERR LINE: " << __LINE__ << " err: " << e.what());
    } catch (...){
       MCE_WARN("LoadTask --> handle ERR LINE: " << __LINE__);
    }
}
