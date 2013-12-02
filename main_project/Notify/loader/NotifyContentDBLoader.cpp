#include <map>
#include <vector>
#include <string>
#include "quicklz.h"
#include "QueryRunner.h"
#include "NotifyContentDBLoader.h"
#include "Notify/util/Timestamp.h"

using namespace std;
using namespace xce::notify;
using namespace MyUtil;
using namespace com::xiaonei::xce;

const static int TIMEOUT = 100;

bool NotifyContentDBLoader::LoadNotifyContentsParal(const vector<int64_t>& nids,
    NotifyContentDict* notify_cont_dict){
  map<int, vector<int64_t> > table_group;
  Timestamp start_time = Timestamp::now();
  for (size_t i = 0; i < nids.size(); ++i) {
    table_group[nids[i] % GetTableCount()].push_back(nids[i]);
  }
  ParaLoaderPtr loader = new ParaLoader();
  map<int, vector<int64_t> >::iterator it;
  for (it = table_group.begin(); it != table_group.end(); ++it) {
    ostringstream oss;
    oss << " SELECT * FROM "<<GetTableName(it->first)
        <<" WHERE id in ( ";
    for (size_t i = 0; i < (it->second).size(); ++i) {
      if (i != 0) {
        oss << ",";
      }
      oss << (it->second).at(i);
    }
    oss << ") order by time";
    loader->addQuery(GetInstance(),oss.str(), 
        GetExpression(it->first),it->second);
  }
  int not_exist = -1;
  NotifyContentDict query_res = loader->query(not_exist);
  if (query_res.empty()) {
    MCE_WARN("NotifyContentDBLoader::LoadNotifyContentsParal --> load nothing");
    return false;
  }
  notify_cont_dict->insert(query_res.begin(), query_res.end());
  double load_cost = timeDifference(Timestamp::now(), start_time);
  MCE_INFO("NotifyContentDBLoader::LoadNotifyContentsParal --> load from DB: " 
      << query_res.size() << " cost:" << load_cost);
  return true;
}

bool NotifyContentDBLoader::DeleteNotifyContents(const MyUtil::LongSeq& nids) {
  map<int, MyUtil::LongSeq> table_group;
  for (size_t h = 0; h < nids.size(); ++h) {
    table_group[nids.at(h) % GetTableCount()].push_back(nids.at(h));
  }
  map<int, MyUtil::LongSeq>::iterator iter;
  iter = table_group.begin();
  try{
    while (iter != table_group.end()) {
      Statement sql;
      sql << " DELETE FROM " << GetTableName(iter->first) << " WHERE id in ( ";
      for (size_t h = 0; h < (iter->second).size(); ++h){
        if( h ) {
          sql << "," ;
        }
        sql << (iter->second).at(h);
      } 
      sql << ") ";
      QueryRunner(GetInstance(), CDbRServer, 
          GetExpression(iter->first)).store(sql);
      iter++;
    }
  } catch (std::exception& e) {
    MCE_WARN("NotifyContentDBLoader::DeleteNotifyContents --> exception:" << e.what());
    return false;
  }
  return true;
}

void ParaLoader::addQuery(const string& dbInstance, const string& sql,
        const string& pattern, const vector<Ice::Long>& fids) {
  SqlExecuteTask* t = new SqlExecuteTask(this);
  t->init(dbInstance, sql, pattern, fids);
  _taskSeq.push_back(t);
  ++_count;
}

map<Ice::Long,NotifyContentPtr> ParaLoader::query(int & not_exist) {
  int total = _count;
  for (size_t i = 0; i < _taskSeq.size(); ++i) {
    MyUtil::TaskManager::instance().execute(_taskSeq.at(i));
  }
  _taskSeq.clear();

  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  if (!timedWait(IceUtil::Time::milliSeconds(TIMEOUT))) {
    MCE_WARN("ParaLoader::query --> total:" << total << " miss:" << _count << "  fail!");
  } else {
    MCE_DEBUG("ParaLoader::query --> total:" << total << " miss:" << _count << " success");
  }
  not_exist = not_exist_;
  return ndict_;
}

void ParaLoader::SqlExecuteTask::handle() {
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(_dbInstance, CDbRServer, _pattern).store(_sql);
  } catch (std::exception& e) {
    MCE_WARN("ParaLoader::SqlExecuteTask --> exception: " << e.what());
    _loader->finish(0);//read from database failed, believe feed exist
    return;
  }
  if (!res) {
    MCE_WARN("ParaLoader::SqlExecuteTask::handle --> query return null");
    _loader->finish(0);//read from database failed, believe feed exist
    return;
  }
  NotifyContentDict ndict;
  for (size_t i = 0; i < res.num_rows(); ++i){
    mysqlpp::Row row = res.at(i);
    NotifyContentPtr reply = new NotifyContent;
    reply->id = (Ice::Long) row["id"];
    reply->source = (Ice::Long) row["source"];
    reply->owner = (int) row["owner"];
    reply->from = (int) row["from_id"];
    reply->type = (int) row["type"];
    reply->time = (time_t) mysqlpp::DateTime(row["time"]);
    string props = row["props"].data();
    reply->props.swap(props);
    reply->type &= 0x7FFFFFFF;
    ndict[reply->id] = reply;
    fids_.erase(reply->id);
    MCE_DEBUG("NotifyContentDBLoader::ParaLoader::props no compress"
        << " nid[" << reply->id << "] props.size[" << props.size() << "]");
  }
  for(set<Ice::Long>::iterator it = fids_.begin(); it != fids_.end();it++)
    ndict[*it] = NULL;
   _loader->finish(ndict, fids_.size());
}

