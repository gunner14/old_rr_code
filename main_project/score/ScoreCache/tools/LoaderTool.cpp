#include <UtilCxx/src/LogWrapper.h>
#include "IceUtil/IceUtil.h"
#include "ScoreCache.h"
#include "DbCxxPool/src/ConnectionHolder.h"
#include "score/ScoreCache/share/ScoreCacheDataFactory.h"
#include "OceCxxAdapter/src/ScoreCacheAdapter.h"
#include <Ice/Ice.h>
#include <IceGrid/IceGrid.h>
#include <string>
#include <list>
#include <vector>
#include <fstream>

using namespace std;
using namespace xce::scorecache;
using namespace com::xiaonei::xce;
using namespace xce::scorecache::adapter;


void help() {
  cout << "usage:" << endl;
  cout << "./LoaderTool toHotCacheIndexs userIdFile option [batchNum=100]" << endl;
  cout << "1)./LoaderTool 0,1,3 userIdFile loadAll 100" << endl;
  cout << "2)./LoaderTool 2,4 userIdFile loadMiss 100" << endl;
}

void setData(const vector<ScoreCacheNManagerPrx>& prxs, const MyUtil::ObjectResultPtr& resultPtr, int times) {
  for (vector<ScoreCacheNManagerPrx>::const_iterator itPrx = prxs.begin();
      itPrx != prxs.end(); itPrx++) {
    try {
      (*itPrx)->setData(resultPtr);
    } catch(exception& ex) {
      MCE_WARN("batchTimes:" << times << ", prx:" << (*itPrx) << ", setData exception: " << ex.what());
    } catch(...) {
      MCE_WARN("batchTimes:" << times << ", unknown exception.");
    }
  }
}

// try to get data from prxs, if all prxs success, return -1, else return first failed index
int try2GetCacheData(const vector<ScoreCacheNManagerPrx>& prxs, int userId) {
  int i = 0;
  for (vector<ScoreCacheNManagerPrx>::const_iterator itPrx = prxs.begin();
    itPrx != prxs.end(); itPrx++, i++) {
    try {
      (*itPrx)->getScoreDataN(userId);
      //ScoreDataN data = (*itPrx)->getScoreDataN(userId);
      //cout << "id:" << userId << ", hd:" << data.historyLoginDays << ", cd:" << data.continueLoginDays << ", score:" << data.score << ", vip:" << data.vip << endl;
    } catch(exception& ex) {
      MCE_WARN("userId:" << userId << ", try to get data from " << (*itPrx) << ", exception: " << ex.what() << ", will reload.");
      return i;
    } catch(...) {
      MCE_WARN("userId:" << userId << ", try to get data from " << (*itPrx) << ", unknown exception, will reload.");
      return i;
    }// try
  }// for
  return -1;
}

MyUtil::ObjectResultPtr getDataFromDB(const vector<int>& userIds) {
  MyUtil::ObjectResultPtr result = new MyUtil::ObjectResult();
  try {
    result = ScoreCacheDataNFactory::instance().create(userIds);
  } catch(exception& ex) {
    MCE_WARN("userIds.size: " << userIds.size() << ", create data from DB exception: " << ex.what());
  } catch(...) {
    MCE_WARN("userIds.size: " << userIds.size() << ", create data from DB unknown exception.");
  }
  return result;
}

// this method can be useful when connceting DB has exception and there are hot caches(at least one) to get data 
MyUtil::ObjectResultPtr getDataFromHotCache(vector<ScoreCacheNManagerPrx>& hotPrxs, const vector<int>& userIds) {
  MyUtil::ObjectResultPtr result = new MyUtil::ObjectResult();
  for (vector<ScoreCacheNManagerPrx>::const_iterator itPrx = hotPrxs.begin();
      itPrx != hotPrxs.end(); itPrx++) {
    try {
      ScoreDataNMap dataMap = (*itPrx)->getScoreDataNMapNotNull(userIds);
      for (ScoreDataNMap::const_iterator it = dataMap.begin(); it != dataMap.end(); it++) {
        ScoreCacheDataNPtr data = new ScoreCacheDataN();
        data->historyLoginDays  = it->second.historyLoginDays;
        data->continueLoginDays = it->second.continueLoginDays;
        data->activeDays        = it->second.activeDays;
        data->lastLoginTime     = it->second.lastLoginTime;
        data->loginType         = it->second.loginType;
        data->score             = it->second.score;
        data->awardCount        = it->second.awardCount;
        data->vip               = it->second.vip;
        data->sign              = it->second.sign;
        result->data.insert(make_pair(it->first, data));
      }
      return result;
    } catch(exception& ex) {
      MCE_WARN("userIds.size: " << userIds.size() << ", get data from hot cache: " << (*itPrx) << ", exception:" << ex.what());
    } catch(...) {
      MCE_WARN("userIds.size: " << userIds.size() << ", get data from hot cache: " << (*itPrx) << ", unknown exception.");
    }
  }
  return result;
}

vector<string> processCacheIndexs(const string& indexs, string sep = ",") {
  vector<string> result;
  size_t posStart = indexs.find_first_not_of(sep);
  size_t posEnd;
  while (posStart != string::npos) {
    posEnd = indexs.find(sep, posStart);
    if (posEnd != string::npos) {
      string seg = indexs.substr(posStart, posEnd - posStart);
      posStart = posEnd + sep.length();
      if (!seg.empty()) {
        result.push_back("M@ScoreCacheN" + seg);
      }
    } else {
      string seg = indexs.substr(posStart);
      posStart = posEnd;
      if (!seg.empty()) {
        result.push_back("M@ScoreCacheN" + seg);
      }
    }
  }
  return result;
}


int main(int argc, char ** argv) {
  if (argc < 4) {
    help();
    return 1;
  }
  
  string log_level = "INFO";
  MCE_DEFAULT_INIT(log_level);
  //////  argv[1] cacheIndexs
  vector<string> toHotProxySeq = processCacheIndexs(argv[1]);   //to hot cache proxy
  if (toHotProxySeq.empty()) {
    MCE_INFO("no cache to hot, please check cacheIndexs.");
    return 1;
  }

  ostringstream proxyStr;
  for (int i = 0; i < toHotProxySeq.size(); i++) {
    proxyStr << toHotProxySeq[i] << " ";
  }
  MCE_INFO("to hot cache list: " << proxyStr.str());

  //////  argv[3] option 
  string option(argv[3]);
  if (option != "loadMiss" && option != "loadAll") {
    help();
    return 1;
  }

  ////// argv[2] userIdFile
  string fileName(argv[2]);
  ifstream fin;
  fin.open(fileName.c_str());
  list<int> userIds;
  int id;
  while (fin >> id) {
    userIds.push_back(id);
  }
  fin.close();
  MCE_INFO("file:" << fileName << ", userIds' size: " << userIds.size());

  vector<string> hotProxySeq;
//  hotProxySeq.push_back("M@ScoreCacheN0");   //hot cache
  vector<ScoreCacheNManagerPrx> hotPrxs;
  vector<ScoreCacheNManagerPrx> toHotPrxs;

  int status = 0;
  Ice::CommunicatorPtr ic;
  Ice::ObjectPrx base;
  string op;
  try {
    Ice::PropertiesPtr properties = Ice::createProperties();
    properties->setProperty("Ice.Override.ConnectTimeout", "300");
    properties->setProperty("IceGrid.InstanceName", "ScoreNew");
    properties->setProperty("Ice.ThreadPool.Client.StackSize", "65535");
    properties->setProperty("Ice.ThreadPool.Server.StackSize", "65535");
    properties->setProperty("Ice.MessageSizeMax", "20480");
    properties->setProperty("Ice.Default.Host", "127.0.0.1");

    properties->setProperty("Ice.Default.Locator", "ScoreNew/Locator:tcp -h ScoreNewMaster -p 4070:tcp -h ScoreNewReplica1 -p 4070:tcp -h ScoreNewReplica2 -p 4070");
    Ice::InitializationData id;
    id.properties = properties;
    ic = Ice::initialize(id);
    for (vector<string>::const_iterator itStr = hotProxySeq.begin(); itStr != hotProxySeq.end(); ++itStr) {
      Ice::ObjectPrx prx = ic->stringToProxy(*itStr)->ice_timeout(300);
      hotPrxs.push_back(ScoreCacheNManagerPrx::uncheckedCast(prx));
    }
    for (vector<string>::const_iterator itStr = toHotProxySeq.begin(); itStr != toHotProxySeq.end(); ++itStr) {
      Ice::ObjectPrx prx = ic->stringToProxy(*itStr)->ice_timeout(300);
      toHotPrxs.push_back(ScoreCacheNManagerPrx::uncheckedCast(prx));
    }
   
    ConnectionPoolManager::instance().initialize();

    int batchCreateNum = 100;
    if (argc >= 5) {
      batchCreateNum = atoi(argv[4]);
    }
    int batchTimes = 0;
    for (list<int>::const_iterator it = userIds.begin(); it != userIds.end();) {
      vector<int> batchIds;
      int count = 0;
      if (option == "loadAll") {
        while (count < batchCreateNum && it != userIds.end()) {
          batchIds.push_back(*it);
          ++count;
          ++it;
        }
      } else {
         while (count < batchCreateNum && it != userIds.end()) {
           if (try2GetCacheData(toHotPrxs, *it) >= 0) {
             batchIds.push_back(*it);
             ++count;
           }
           ++it;  
        }//while 
      }//else
      
      MyUtil::ObjectResultPtr result = getDataFromDB(batchIds);
//      MyUtil::ObjectResultPtr result = getDataFromHotCache(hotPrxs, batchIds);
      MCE_INFO("batchTimes:" << batchTimes << ", id-size:" << batchIds.size() << ", Query DBResult-size:" << result->data.size());
      if (!(result->data.empty())) {
        setData(toHotPrxs, result, batchTimes);
      }
    }//for list
    
  } catch (const Ice::Exception & ex) {
    MCE_WARN("ice::exception: " << ex.what());
    status = 1;
  } catch (exception & ex) {
    MCE_WARN("std::exception: " << ex.what());
    status = 1;
  } catch (...) {
    MCE_WARN("unknown exception.");
  }


  if (ic) {
    try {
      ic->destroy();
    } catch (const Ice::Exception & ex) {
      cerr << ex << endl;
      status = 1;
    }
  }
  return status;
}
