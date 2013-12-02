/*
 * =====================================================================================
 *
 *       Filename:  DBWriterI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月17日 10时35分17秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "DBWriterI.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "ActiveTrackAdapter.h"
#include "app/Utils/src/DBUtils.h"

using namespace xce::utils;
using namespace xce::dbmanager;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace xce::log;
using namespace xce::serverstate;

void MyUtil::initialize() {
  MCE_INFO("[MyUtil::initialize]");
  ServiceI& service = ServiceI::instance();
  string portNum = service.getCommunicator()->getProperties()->getPropertyWithDefault("DBWriter.PortNum", "12346");
  MCE_INFO("[MyUtil::initialize] port = " << portNum);
  DBWriterI::instance().init(portNum);
  ::Ice::ObjectPrx prx = service.getAdapter()->add(&DBWriterI::instance(), service.createIdentity("DBW", ""));
  MCE_INFO("[MyUtil::initialize] prx = " << service.getCommunicator()->proxyToString(prx));
  // 加入向Controller注册
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("DBWriter.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("DBWriter.Interval", 5);
  string ctrEndpoints = service.getCommunicator()->getProperties()
    ->getPropertyWithDefault("DBWriter.ControllerEndpoint", "ControllerDBWriter");
  ServerStateSubscriber::instance().initialize(ctrEndpoints,
      &DBWriterI::instance(), mod, interval);
}

map<string, string> DBWriterI::processConfigFile() {
  ifstream in(DBWRITER_CONFIG_PATH.c_str());
  map<string, string> configMap;
  while (!in.eof()) {
    string line;
    getline(in, line);
    size_t pos = line.find('=');
    if (pos != string::npos) {
      configMap.insert(make_pair<string, string>(line.substr(0, pos), line.substr(pos + 1)));
    }
  }
  return configMap;
}

string DBWriterI::getValueFromMap(const map<string, string>& configMap, const string& key) {
  map<string, string>::const_iterator it = configMap.find(key);
  if (it == configMap.end()) {
    return "";
  } else {
    return it->second;
  }
}

vector<string> DBWriterI::getDBSources(const string& dbSource) {
  vector<string> dbSources;
  size_t pos = 0, startIndex = 0;
  while (true) {
    pos = dbSource.find(':', startIndex);
    if (pos == string::npos) {
      dbSources.push_back(dbSource.substr(startIndex));
      break;
    }
    dbSources.push_back(dbSource.substr(startIndex, pos - startIndex));
    startIndex = pos + 1;
  }
  return dbSources;
}

void DBWriterI::init(const string& port) {
  map<string, string> configMap = processConfigFile();
  string dbSource = getValueFromMap(configMap, CONFIG_DBSOURCE);
  string ip = getValueFromMap(configMap, CONFIG_IP);
  zkManager_ = &ZkManager::instance();
  zkManager_->init();
  vector<string> dbSources = getDBSources(dbSource);
  vector<string> childPathToMaintain;
  for (vector<string>::const_iterator it = dbSources.begin(); it != dbSources.end(); ++it) {
    MCE_INFO("[DBWriterI::init] dbSource = " << (*it) << ", ip = " << ip << ", port = " << port);
    zkManager_->createNode("/" + (*it), ZOO_PERSISTENT);
    string childPath = "/" + (*it) + "/" + ip + ":" + port;
    childPathToMaintain.push_back(childPath);
    string data = ip + ":" + port;
    zkManager_->createNode(childPath, ZOO_EPHEMERAL);
  }
  //起一个线程来维护这些临时节点
  MaintainNodeThreadPtr mainNodeThreadPtr = new MaintainNodeThread(zkManager_, childPathToMaintain);
  mainNodeThreadPtr->start(128*1024).detach();
}

WriteResultPtr DBWriterI::write(const string& dbSrc, const string& pattern, const string& sql, const Ice::Current& current) {
  MCE_INFO("[DBWriterI::write] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "]");
  Statement sqlToExecute;
  sqlToExecute << sql;
  WriteResultInfoPtr writeResultInfoPtr = new WriteResultInfo();
  try {
    mysqlpp::SimpleResult simpleResult = QueryRunner(dbSrc, CDbWServer, pattern).execute(sqlToExecute);
    MCE_INFO("[DBWriterI::write] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "] executed success");
    writeResultInfoPtr->setResult(simpleResult, true);
  } catch (mysqlpp::Exception ex) {
    MCE_INFO("[DBWriterI::write] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], mysqlpp::exception = " << ex.what() 
        << ", executed failure");
    writeResultInfoPtr->setInfo(ex.what());
  } catch (exception ex) { 
    MCE_INFO("[DBWriterI::write] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], exception = " << ex.what() << ", executed failure");
    writeResultInfoPtr->setInfo(ex.what());
  } catch (...) {
    MCE_INFO("[DBWriterI::write] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], unknown exception, executed failure");
    writeResultInfoPtr->setInfo("unknown exception");
  }
  return writeResultInfoPtr;
}

WriteResultPtr DBWriterI::writeWithFilter(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace, 
    const string& businessName, const string& filterSql, const string& key, int op, int expireTime, const Ice::Current& current) {
  MCE_INFO("[DBWriterI::writeWithFilter] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", filterSql = [ " << filterSql << "], key = " << key
        << ", op = " << op);
  //用filerSql和key来确定TripodCache中需要操作的keys
  Statement querySql;
  querySql << filterSql;
  vector<string> tripodCacheKeys;
  BatchIdResultHandler<string> handler(key, tripodCacheKeys);
  QueryRunner(dbSrc, CDbRServer).query(querySql, handler);
  MCE_INFO("[DBWriterI::writeWithFilter] filter keys.size = " << tripodCacheKeys.size());
  for (vector<string>::const_iterator it = tripodCacheKeys.begin(); it != tripodCacheKeys.end(); ++it) {
    MCE_INFO("[DBWriterI::writeWithFilter] key = " << *it);
  }
  Statement sqlToExecute;
  sqlToExecute << sql;
  WriteResultInfoPtr writeResultInfoPtr = new WriteResultInfo();
  try {
    mysqlpp::SimpleResult simpleResult = QueryRunner(dbSrc, CDbWServer, pattern).execute(sqlToExecute);
    MCE_INFO("[DBWriterI::writeWithFilter] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", filterSql = [ " << filterSql << "], key = " << key
        << ", op = " << op << " executed success");
    bool loadTripodCacheOk = true;
    if (!tripodCacheKeys.empty() && !opTripodCache(nameSpace, businessName, tripodCacheKeys, op, expireTime)) {
      loadTripodCacheOk = false;
    }
    writeResultInfoPtr->setResult(simpleResult, loadTripodCacheOk);
  } catch (mysqlpp::Exception ex) {
    MCE_INFO("[DBWriterI::writeWithFilter] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", filterSql = [ " << filterSql << "], key = " << key
        << ", op = " << op << ", mysqlpp::exception = " << ex.what() << " executed failure");
    writeResultInfoPtr->setInfo(ex.what());
  } catch (exception ex) {
    MCE_INFO("[DBWriterI::writeWithFilter] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", filterSql = [ " << filterSql << "], key = " << key
        << ", op = " << op << ", exception = " << ex.what() << " executed failure");
    writeResultInfoPtr->setInfo(ex.what());
  } catch (...) {
    MCE_INFO("[DBWriterI::writeWithFilter] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", filterSql = [ " << filterSql << "], key = " << key
        << ", op = " << op << ", unknown exception executed failure");
    writeResultInfoPtr->setInfo("unknown exception");
  }
  return writeResultInfoPtr;
}

WriteResultPtr DBWriterI::writeWithKeys(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace,
    const string& businessName, const MyUtil::StrSeq& keys, int op, int expireTime, const Ice::Current& current) {
  MCE_INFO("[DBWriterI::writeWithKeys] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
      << nameSpace << ", businessName = " << businessName << ", keys.size = " << keys.size() << ", op = " << op);
  Statement sqlToExecute;
  sqlToExecute << sql;
  WriteResultInfoPtr writeResultInfoPtr = new WriteResultInfo();
  try {
    mysqlpp::SimpleResult simpleResult= QueryRunner(dbSrc, CDbWServer, pattern).execute(sqlToExecute);
    MCE_INFO("[DBWriterI::writeWithKeys] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", keys.size = " << keys.size() << ", op = " << op << " executed success");
    bool loadTripodCacheOk = true;
    if (!opTripodCache(nameSpace, businessName, keys, op, expireTime)) {
      loadTripodCacheOk = false;
    }
    writeResultInfoPtr->setResult(simpleResult, loadTripodCacheOk);
  } catch (mysqlpp::Exception ex) {
    MCE_INFO("[DBWriterI::writeWithKeys] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", keys.size = " << keys.size() << ", op = " << op << ", mysqlpp::exception = "
        << ex.what() << ", executed failure");
    writeResultInfoPtr->setInfo(ex.what());
  } catch (exception ex) {
    MCE_INFO("[DBWriterI::writeWithKeys] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", keys.size = " << keys.size() << ", op = " << op << ", exception = "
        << ex.what() << ", executed failure");
    writeResultInfoPtr->setInfo(ex.what());
  } catch (...) {
    MCE_INFO("[DBWriterI::writeWithKeys] dbSrc = " << dbSrc << ", pattern = " << pattern << ", sql = [" << sql << "], nameSpace = "
        << nameSpace << ", businessName = " << businessName << ", keys.size = " << keys.size() << ", op = " << op << ", unknown exception"
        << ", executed failure");
    writeResultInfoPtr->setInfo("unknown exception");
  }
  return writeResultInfoPtr;
}

bool DBWriterI::opTripodCache(const string& nameSpace, const string& businessName, const MyUtil::StrSeq& keys, int op, int expireTime) {
  com::renren::tripod::TripodClient* client = getTripodClient(nameSpace, businessName);
  try {
    if (op == OP_INSERT) {
      client->load(keys, expireTime, true);
    } else if (op == OP_UPDATE) {
      client->load(keys, expireTime, true);
    } else if (op == OP_DELETE) {
      client->deleteCache(keys, 2000);
    }
  } catch (Ice::TimeoutException ex) {
    MCE_INFO("[DBWriterI::opTripodCache] nameSpace = " << nameSpace << ", businessName = " << businessName << ", keys.size = " 
        << keys.size() << ", op = " << op << " exception = " << ex.what() << ", operate TripodCache failure");
  } catch (exception ex) {
    MCE_INFO("[DBWriterI::opTripodCache] nameSpace = " << nameSpace << ", businessName = " << businessName << ", keys.size = " 
        << keys.size() << ", op = " << op << " exception = " << ex.what() << ", operate TripodCache failure");
    return false;
  } catch (...) {
    MCE_INFO("[DBWriterI::opTripodCache] nameSpace = " << nameSpace << ", businessName = " << businessName << ", keys.size = " 
        << keys.size() << ", op = " << op << " unknown exception, operate TripodCache failure");
    return false;
  }
  return true;
}

com::renren::tripod::TripodClient* DBWriterI::getTripodClient(const string& nameSpace, const string& businessName) {
  com::renren::tripod::TripodClient* client = 0;
  string id = nameSpace + "_*_" + businessName;
  {
    IceUtil::RWRecMutex::RLock lock(tripodClientMapMutex_);
    map<string, com::renren::tripod::TripodClient*>::iterator it = tripodClientMap_.find(id);
    if (it != tripodClientMap_.end()) {
      client = it->second; 
    }
  }
  if (client == 0) {
    client = new com::renren::tripod::TripodClient(TRIPOD_ZK_ADDRESS, nameSpace, businessName);
    IceUtil::RWRecMutex::WLock lock(tripodClientMapMutex_);
    tripodClientMap_.insert(make_pair<string, com::renren::tripod::TripodClient*>(id, client));
  }
  return client;
}
