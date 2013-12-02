/*
 * =====================================================================================
 *
 *       Filename:  DBShare.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月03日 18时16分13秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#include "DBShare.h"

using namespace std;
using namespace xce::dbmanager;

void WriteResultInfo::setResult(const mysqlpp::SimpleResult& simpleResult, bool loadCacheOk) {
  insertId = simpleResult.insert_id();
  affectRows = simpleResult.rows();
  info = string(simpleResult.info());
  sqlExecuteOk = true;
  loadTripodCacheOk = loadCacheOk;
}

void WriteResultInfo::setInfo(const string& pInfo) {
  info = pInfo;
}

WriteResultInfo::WriteResultInfo() : WriteResult(0, 0, "", false, false) {
}

WriteResultInfo::WriteResultInfo(const WriteResultPtr& writeResultPtr) {
  insertId = writeResultPtr->insertId;
  affectRows = writeResultPtr->affectRows;
  info = writeResultPtr->info;
  sqlExecuteOk = writeResultPtr->sqlExecuteOk;
  loadTripodCacheOk = writeResultPtr->loadTripodCacheOk;
}

vector<string> WriteInfoWithFilter::getOpKeys() const {
  vector<string> keys;
  return keys;
}

//这个Watcher是负责建立链接的，当建立链接不成功的时候，不断地进行链接
void ZkConnectWatcher(zhandle_t *zzh, int type, int state, const char *path, void *watcherCtx) {
  MCE_INFO("ZkConnectWatcher type = " << type << ", steate = " << state << ", path = " << path);
  if (type == ZOO_SESSION_EVENT) {
    if (state == ZOO_EXPIRED_SESSION_STATE) {
      //链接没有成功，继续链接
      MCE_INFO("[ZkConnectWatcher] connect to zookeeper failed, try again");
      ZkManager::instance().initZKConnection();
    }
  }
}

void ZkManager::init() {
  MCE_INFO("[ZkManager::init]");
  initZKConnection();
}

void ZkManager::initZKConnection() {
  MCE_INFO("[ZkManager::initZKConnection]");
  try {
    MCE_INFO("[ZkManager::initZKConnection] start to connect to ZKServer ");
    zk_ = zookeeper_init(zkAddress_.c_str(), ZkConnectWatcher, 10000, 0, 0, 0);
    sleep(5);
    if (zk_ == 0) {
      MCE_INFO("[ZkManager::initZKConnection] can't connect to " << zkAddress_);
    }
    MCE_INFO("[ZkManager::initZKConnection] connected to ZKServer ");
  } catch (exception ex) {
    MCE_INFO("[ZkManager::initZKConnection] exception = " << ex.what() << ", failed");
  } catch (...) {
    MCE_INFO("[ZkManager::initZKConnection] exception = known exception, failed");
  }
}

bool ZkManager::exist(const string& path) {
  //MCE_INFO("[ZkManager::exist] path = " << path);
  int rc = -1;
  try {
    //MCE_INFO("[ZkManager::exist] path = " << path << " before call zoo_exist");
    rc = zoo_exists(zk_, path.c_str(), 0, 0);
    //MCE_INFO("[ZkManager::exist] path = " << path << " after call zoo_exist");
  } catch (exception ex) {
    MCE_INFO("[ZkManager::exist] path = " << path << ", exception = " << ex.what());
  } catch (...) {
    MCE_INFO("[ZkManager::exist] path = " << path << ", exception = unknown exception");
  }
  if (rc != ZOK) {
    return false;
  }
  return true;
}

int ZkManager::write(const string& zkPath, const string& data) {
  MCE_INFO("[ZkManager::write] zkPath = " << zkPath << ", data = " << data);
  int rc = zoo_set(zk_, zkPath.c_str(), data.c_str(), data.length(), -1);
  if (rc != ZOK) {
    MCE_INFO("[ZkManager::write] zkPath = " << zkPath << ", data = " << data << " failed");
    return 1;
  } else {
    return 0;
  }
}

vector<string> ZkManager::readChildren(const string& zkPath) {
  vector<string> result;
  struct String_vector children;
  int rc = zoo_get_children(zk_, zkPath.c_str(), 0, &children);
  if (rc != ZOK) {
    MCE_INFO("[ZkManager::readChildren] zkPath = " << zkPath << " failed");
    exit(-1);
  }
  for (int i = 0; i < children.count; ++i) {
    string child = string(children.data[i]);
    result.push_back(child);
  }
  return result;
}

//nodeType是ZOO_EPHEMERAL或者ZOO_SEQUENCE
int ZkManager::createNode(const string& path, int nodeType) {
  MCE_INFO("[ZkManager::createNode] path = " << path << ", nodeType = " << nodeType);
  //原来有那个路径，也认为是创建成功了
  if (exist(path)) {
    MCE_INFO("[ZkManager::createNode] path = " << path << ", nodeType = " << nodeType << ", path already exist");
    return 1;
  }
  MCE_INFO("[ZkManager::createNode] path = " << path << ", nodeType = " << nodeType << ", not exist, start to create");
  int rc = -1;
  try {
    rc = zoo_create(zk_, path.c_str(), NULL, 0, &ZOO_OPEN_ACL_UNSAFE, nodeType, 0, 0);
  } catch (exception ex) {
    MCE_INFO("[ZkManager::createNode] path = " << path << ", nodeType = " << nodeType << ", exception = " << ex.what());
  } catch (...) {
    MCE_INFO("[ZkManager::createNode] path = " << path << ", nodeType = " << nodeType << ", exception = unknown exception");
  }
  if (rc != ZOK) {
    MCE_INFO("[ZkManager::createNode] path = " << path << ", nodeType = " << nodeType << " rc = " << rc << " failed");
    return 1;
  } else {
    MCE_INFO("[ZkManager::createNode] path = " << path << ", nodeType = " << nodeType << " success");
    return 0;
  }
}

int ZkManager::deleteNode(const string& path) {
  if (exist(path)) {
    int rc = zoo_delete(zk_, path.c_str(), -1);
    if (rc != ZOK) {
      MCE_INFO("[ZkManager::deleteNode] path = " << path << " failed");
      return 1;
    } else {
      return 0;
    }
  } else {
    return 1;
  }
}

string ZkManager::readData(const string& path) {
  char buffer[BUFFER_LEN];
  int buflen = BUFFER_LEN;
  int rc = zoo_get(zk_, path.c_str(), 0, buffer, & buflen, 0);
  if (rc != ZOK) {
    MCE_INFO("[ZkManager::readData] path = " << path << ", failed");
    return "-------------ERROR------------";
  } else {
    string data(buffer, buflen);
    return data;
  }
}

vector<string> ZkManager::readChildrenData(const string& path) {
  vector<string> result;
  vector<string> children = readChildren(path);
  char buffer[BUFFER_LEN];
  int buflen = BUFFER_LEN;
  for (vector<string>::const_iterator it = children.begin(); it != children.end(); ++it) {
    string childPath = path + "/" + (*it);
    int rc = zoo_get(zk_, childPath.c_str(), 0, buffer, & buflen, 0);
    if (rc != ZOK) {
      MCE_INFO("[ZkManager::readChildrenData] path = " << childPath << " get data failed");
    } else {
      string data(buffer, 0, buflen);
      result.push_back(data);
    }
  }
  return result;
}

void MaintainNodeThread::run() {
  //不断地查看临时节点是否存在，如果不存在则创建临时节点
  while (true) {
    int disappearedNum = 0;
    for (vector<string>::const_iterator it = paths_.begin(); it != paths_.end(); ++it) {
      if (zkManager_->exist(*it)) {
        continue;
      } else {
        ++disappearedNum;
        MCE_INFO("[[MaintainNodeThread::run] path = " << *it << " disppears, try to create it again");
        try {
          zkManager_->createNode(*it, ZOO_EPHEMERAL);
        } catch (exception ex) {
          MCE_INFO("[MaintainNodeThread::run] path = " << *it << ", exception = " << ex.what() << ", op failed");
        } catch (...) {
          MCE_INFO("[MaintainNodeThread::run] path = " << *it << ", unknown exception, op failed");
        }
      }
    }
    if (disappearedNum == 0) {
      sleep(1);
    }
  }
}

DBWriterPrx MaintainPrxThread::parsePrxFromString(const string& identifier) {
  MCE_INFO("[MaintainPrxThread::parsePrxFromString] identifier = " << identifier);
  string ip, port;
  size_t pos = identifier.find(':');
  if (pos == string::npos) {
    return 0;
  } else {
    ip = identifier.substr(0, pos);
    port = identifier.substr(pos + 1);
    MCE_INFO("[MaintainPrxThread::parsePrxFromString] ip = " << ip << ", port = " << port);
  }
  Ice::CommunicatorPtr ic = Ice::initialize();
  MCE_INFO("[MaintainPrxThread::parsePrxFromString] ic = " << ic);
  DBWriterPrx prx = 0;
  try {
    Ice::ObjectPrx base = ic->stringToProxy("DBW:tcp -h " + ip + " -p " + port);
    prx = DBWriterPrx::checkedCast(base);
  } catch (Ice::ConnectionRefusedException ex) {
    MCE_INFO("[MaintainPrxThread::parsePrxFromString] ip = " << ip << ", port = " << port << ", exception = " << ex.what());
  } catch (Ice::TimeoutException ex) {
    MCE_INFO("[MaintainPrxThread::parsePrxFromString] ip = " << ip << ", port = " << port << ", exception = " << ex.what());
  } catch (Ice::Exception ex) {
    MCE_INFO("[MaintainPrxThread::parsePrxFromString] ip = " << ip << ", port = " << port << ", exception = " << ex.what());
  } catch (...) {
    MCE_INFO("[MaintainPrxThread::parsePrxFromString] ip = " << ip << ", port = " << port << ", exception = unknown exception");
  }
  return prx;
}

MaintainPrxThread::MaintainPrxThread() {
  MCE_INFO("[MaintainPrxThread::MaintainPrxThread] start to init connection to zookeeper");
  zkManager_ = &ZkManager::instance();
  zkManager_->init();
  map<string, vector<string> > tempDbSourceToEndpoints = getDbSourceToEndpoints();
  updateIdToDBWriterPrx(tempDbSourceToEndpoints, idToDBWriterPrx_);
}

map<string, vector<string> > MaintainPrxThread::getDbSourceToEndpoints() {
  map<string, vector<string> > resultMap;
  vector<string> dbSources = zkManager_->readChildren("/");
  sort(dbSources.begin(), dbSources.end());
  for (vector<string>::const_iterator it = dbSources.begin(); it != dbSources.end(); ++it) {
    vector<string> endpoints = zkManager_->readChildren("/" + *it);
    sort(endpoints.begin(), endpoints.end());
    resultMap.insert(make_pair<string, vector<string> >(*it, endpoints));
  }
  return resultMap;
}

void MaintainPrxThread::updateIdToDBWriterPrx(const map<string, vector<string> >& newMap,
    map<string, map<string, DBWriterPrx> >& idToDBWriterPrx) {
  MCE_INFO("[MaintainPrxThread::updateIdToDBWriterPrx] newMap.size = " << newMap.size()
      << ", idToDBWriterPrx.size = " << idToDBWriterPrx.size());
  map<string, map<string, DBWriterPrx> > tempIdToDBWriterPrx;
  {
    IceUtil::RWRecMutex::RLock lock(mutex_);
    tempIdToDBWriterPrx = idToDBWriterPrx;
    MCE_INFO("[MaintainPrxThread::updateIdToDBWriterPrx] old idToDBWriterPrx.size = " << idToDBWriterPrx.size());
  }
  map<string, map<string, DBWriterPrx> > newIdToDBWriterPrx;
  bool needUpdate = false;
  if (newMap.size() < tempIdToDBWriterPrx.size()) {
    needUpdate = true;
  }
  for (map<string, vector<string> >::const_iterator it = newMap.begin(); it != newMap.end(); ++it) {
    map<string, map<string, DBWriterPrx> >::const_iterator itm = tempIdToDBWriterPrx.find(it->first);
    map<string, DBWriterPrx> prxs;
    if (itm == tempIdToDBWriterPrx.end()) {
      needUpdate = true;
      for (vector<string>::const_iterator itx = it->second.begin(); itx != it->second.end(); ++itx) {
        DBWriterPrx tempPrx = parsePrxFromString(*itx);
        if (tempPrx != 0) {
          prxs.insert(make_pair<string, DBWriterPrx>(*itx, tempPrx));
        }
      }
    } else {
      if (it->second.size() != itm->second.size()) {
        needUpdate = true;
      }
      for (vector<string>::const_iterator itx = it->second.begin(); itx != it->second.end(); ++itx) {
        map<string, DBWriterPrx>::const_iterator ity = itm->second.find(*itx);
        if (ity == itm->second.end()) {
          needUpdate = true;
          DBWriterPrx tempPrx = parsePrxFromString(*itx);
          if (tempPrx != 0) {
            prxs.insert(make_pair<string, DBWriterPrx>(*itx, tempPrx));
          }
        } else {
          prxs.insert(make_pair<string, DBWriterPrx>(*itx, ity->second));
        }
      }
    }
    newIdToDBWriterPrx.insert(make_pair<string, map<string, DBWriterPrx> >(it->first, prxs));
  }
  if (needUpdate) {
    MCE_INFO("[MaintainPrxThread::updateIdToDBWriterPrx] updated");
    IceUtil::RWRecMutex::WLock lock(mutex_);
    idToDBWriterPrx = newIdToDBWriterPrx;
  }
}

void MaintainPrxThread::run() {
  while (true) {
    map<string, vector<string> > tempDbSourceToEndpoints = getDbSourceToEndpoints();
    updateIdToDBWriterPrx(tempDbSourceToEndpoints, idToDBWriterPrx_);
    sleep(1);
  }
}

DBWriterPrx MaintainPrxThread::getDBWriterPrx(const string& dbSource) {
  MCE_INFO("[MaintainPrxThread::getDBWriterPrx] dbSource = " << dbSource);
  IceUtil::RWRecMutex::RLock lock(mutex_);
  map<string, map<string, DBWriterPrx> >::const_iterator it = idToDBWriterPrx_.find(dbSource);
  if (it == idToDBWriterPrx_.end()) {
    MCE_INFO("[MaintainPrxThread::getDBWriterPrx] no prx for " << dbSource);
    return 0;
  } else {
    if (it->second.empty()) {
      MCE_INFO("[MaintainPrxThread::getDBWriterPrx] no prx for " << dbSource);
      return 0;
    }
    int index = abs(rand()) % it->second.size();
    map<string, DBWriterPrx>::const_iterator itm = it->second.begin();
    while (index--) {
      ++itm;
    }
    return itm->second;
  }
}
