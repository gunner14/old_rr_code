/*
 * =====================================================================================
 *
 *       Filename:  DBShare.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月17日 11时10分48秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __DB_SHARE_H__
#define __DB_SHARE_H__

#include "ServiceI.h"
#include <IceUtil/IceUtil.h>
#include <zookeeper/zookeeper.h>

#include <DBManager.h>
#include <vector>
#include <string>
#include <Ice/Ice.h>
#include <math.h>

namespace xce {
namespace dbmanager {

using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;

static const int OP_DONOTHING = -1;
//static const string DBWRITER_CONFIG_ZK_ADDRESS = "TestZooKeeper1:2181,TestZooKeeper2:2181,TestZooKeeper3:2181,TestZooKeeper4:2181,TestZooKeeper5:2181/DBWriterConfig";
static const string DBWRITER_CONFIG_ZK_ADDRESS = "TestZooKeeper3:2181/DBWriterConfig";
static const string TRIPOD_ZK_ADDRESS = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/Tripod";
static const string DBWRITER_CONFIG_PATH = "../etc/dbwriter_config";
static const int OP_INSERT = 0;
static const int OP_UPDATE = 1;
static const int OP_DELETE = 2;
static const int BUFFER_LEN = 1048576;

static const int NEVER_EXPIRE_TIME = 0;

static const string CONFIG_DBSOURCE = "DBSource";
static const string CONFIG_IP = "Ip";

//zookeeper.h中没有定义ZOO_PERSISTENT这个CreateMode, 在这里定义，使用方便
static const int ZOO_PERSISTENT = 0;

class WriteInfo : public Ice::Object {
public:
  WriteInfo(const string& dbSrc, const string& pattern, const string& sql) : dbSrc_(dbSrc), pattern_(pattern), sql_(sql) {
  }
private:
  string dbSrc_;
  string pattern_;
  string sql_;
  virtual vector<string> getOpKeys() const {
    vector<string> result;
    return result;
  }

  virtual int getOp() const {
    return OP_DONOTHING;
  }
};

typedef IceUtil::Handle<WriteInfo> WriteInfoPtr; 

class WriteResultInfo : virtual public WriteResult {
public:
  WriteResultInfo();
  WriteResultInfo(const WriteResultPtr&);
  void setResult(const mysqlpp::SimpleResult& simpleResult, bool loadCacheOk);
  void setInfo(const string& pInfo);
};

typedef IceUtil::Handle<WriteResultInfo> WriteResultInfoPtr;

class WriteInfoWithFilter : virtual public WriteInfo {
public:
  WriteInfoWithFilter(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace, const string& businessName,
      const string& filterSql, const string& keyCol, int op) : WriteInfo(dbSrc, pattern, sql), nameSpace_(nameSpace), businessName_(businessName),
      filterSql_(filterSql), keyCol_(keyCol), op_(op) {
  }
  virtual vector<string> getOpKeys() const;
  virtual int getOp() const {
    return op_;
  }
private:
  string nameSpace_;
  string businessName_;
  string filterSql_;
  string keyCol_;
  int op_;
};

typedef IceUtil::Handle<WriteInfoWithFilter> WriteInfoWithFilterPtr; 

class WriteInfoWithKeys : virtual public WriteInfo {
public:
  WriteInfoWithKeys(const string& dbSrc, const string& pattern, const string& sql, const string& nameSpace, const string& businessName,
      const vector<string>& keys, int op) : WriteInfo(dbSrc, pattern, sql), nameSpace_(nameSpace), businessName_(businessName), keys_(keys), op_(op) {
    Ice::ObjectPtr ptr;
    WriteInfoWithFilterPtr mPtr = WriteInfoWithFilterPtr::dynamicCast(ptr);
  }

  virtual vector<string> getOpKeys() const {
    return keys_;
  }

  virtual int getOp() const {
    return op_;
  }
private:
  string nameSpace_;
  string businessName_;
  vector<string> keys_;
  int op_;
};

typedef IceUtil::Handle<WriteInfoWithKeys> WriteInfoWithKeysPtr; 

class ZkManager : public Ice::Object, public MyUtil::Singleton<ZkManager> { 
public:
  ZkManager() : zkAddress_(DBWRITER_CONFIG_ZK_ADDRESS) {
  }
  
  bool exist(const string& path);
  int write(const string& zkPath, const string& data);
  vector<string> readChildren(const string& zkPath);
  int createNode(const string& path, int nodeType);
  vector<string> readChildrenData(const string& path);
  string readData(const string& path);
  int deleteNode(const string& path);
  void init();
  void initZKConnection();
  zhandle_t* getZK() const {
    return zk_;
  }

private:
  zhandle_t *zk_;
  string zkAddress_;
};

typedef IceUtil::Handle<ZkManager> ZkManagerPtr;

/***
  这个线程的作用是维护一个临时节点，当链接断了以后，重新链接，创建临时节点，写入数据
***/
class MaintainNodeThread : virtual public IceUtil::Thread {
public:
  MaintainNodeThread(const ZkManagerPtr& zkManager, const vector<string>& paths) : zkManager_(zkManager), paths_(paths) {
  }
  ~MaintainNodeThread() {
  }
  virtual void run();
private:
  ZkManagerPtr zkManager_;
  vector<string> paths_;
};

typedef IceUtil::Handle<MaintainNodeThread> MaintainNodeThreadPtr;

//维护DBSource到DBWriter服务的endpoint的映射
class MaintainPrxThread : virtual public IceUtil::Thread {
public:
  MaintainPrxThread(); 
  ~MaintainPrxThread() {
  }

  virtual void run();

  DBWriterPrx getDBWriterPrx(const string& dbSource);

private:
  ZkManagerPtr zkManager_;
  DBWriterPrx parsePrxFromString(const string& identifier);
  map<string, vector<string> > getDbSourceToEndpoints();
  //返回值表示实际有没有执行更新操作
  void updateIdToDBWriterPrx(const map<string, vector<string> >& newMap,
      map<string, map<string, DBWriterPrx> >& idToDBWriterPrx);
  map<string, map<string, DBWriterPrx> > idToDBWriterPrx_;
  //修改, 读取idToDBWriterPrx_的内容时要加锁
  IceUtil::RWRecMutex mutex_;
};

typedef IceUtil::Handle<MaintainPrxThread> MaintainPrxThreadPtr;

}
}

#endif
