#ifndef _NOTIFY_CONTENT_DBLOADER_H_
#define _NOTIFY_CONTENT_DBLOADER_H_

#include "Singleton.h"
#include "Notify.h"
#include "QueryRunner.h"
#include "ServiceI.h"
#include "ObjectCache.h"

namespace xce {
namespace notify {

using namespace MyUtil;
using namespace com::xiaonei::xce;

class NotifyContentDBLoader: public MyUtil::Singleton<NotifyContentDBLoader>{
 public:
  NotifyContentDict createNotifyContentParal(const std::vector<int64_t>& nids);
  bool LoadNotifyContentsParal(const std::vector<int64_t>& nids, 
      NotifyContentDict* notify_cont_dict);
  bool DeleteNotifyContents(const MyUtil::LongSeq& nids);
 private:
  int GetTableCount(){
    return 100;
  }
  const string GetTableName( int tableIndex ){
    ostringstream tableName;
    tableName << "content_" << tableIndex % 100;
    return tableName.str();
  }
  const string GetInstance() {
    return "notify";
  }
  const string GetExpression(int tableIndex) {
    ostringstream expr;
    expr << "content_" << tableIndex % 100;
    return expr.str();
  }
  class ResultHandlerI: public com::xiaonei::xce::ResultHandler {
   public:
    ResultHandlerI(ObjectResultPtr result):_result(result){}
    virtual bool handle(mysqlpp::Row& row) const;
   private:
    ObjectResultPtr _result;
  };
};

class ParaLoader;
typedef IceUtil::Handle<ParaLoader> ParaLoaderPtr;

class ParaLoader: public IceUtil::Shared, 
    public IceUtil::Monitor<IceUtil::Mutex> {
 public:
  ParaLoader() :  _count(0) ,not_exist_(0){ }
  NotifyContentDict query(int & not_exist);
  void addQuery(const string& dbInstance, const string& sql, 
      const string& pattern,const vector<Ice::Long> & fids);
  void finish(const NotifyContentDict& ndict,int  not_exist) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    ndict_.insert(ndict.begin(), ndict.end());
    not_exist_ += not_exist;
    if (--_count == 0) { notify();  };
  }
 void finish(int  not_exist) {
   IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
   not_exist_ += not_exist;
   if (--_count == 0) {
     notify();
   }
 }
 private:
  int _count;
  int not_exist_;
  vector<MyUtil::TaskPtr> _taskSeq;
  NotifyContentDict ndict_;

  class SqlExecuteTask: public MyUtil::Task {
   public:
    SqlExecuteTask(const ParaLoaderPtr& loader) :
      Task(), _loader(loader) {
    }
    void init(const string& dbInstance, const string& sql,
      const string& pattern,vector<Ice::Long> fids) {
      _sql << sql;
      _dbInstance = dbInstance;
      _pattern = pattern;
      fids_.insert(fids.begin(),fids.end());
    }
    virtual void handle();
   private:
    ParaLoaderPtr _loader;
    Statement _sql;
    string _dbInstance;
    string _pattern;
    set<Ice::Long> fids_;
};
};

}
}

#endif //_NOTIFY_CONTENT_DBLOADER_H_

