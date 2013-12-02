/////////////////////////////////////////////////
//
//
// ChangeLog: zhi.lin  05/25/2012  timeline support
//

#include "DbHelper.h"
#include "QueryRunner.h"
#include "IceLogger.h"

#include "TimeLineFeedReplicaAdapter.h"

using namespace xce::feed;
using namespace com::xiaonei::xce;
using namespace MyUtil;

//for timeline
static const string kYear="year";
static const string kMonth="month";
static const string kDay="day";
const int kWideScreenBit=24;// 0-based
const unsigned int kWideFlag = 1<<kWideScreenBit;
//time helper function
static time_t mktime_bj(struct tm *t)
{
    register short  month, year;
    register time_t result;
    static int      m_to_d[12] =
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    month = t->tm_mon;
    year = t->tm_year + month / 12 + 1900;
    month %= 12;
    if (month < 0)
    {
        year -= 1;
        month += 12;
    }
    result = (year - 1970) * 365 + (year - 1969) / 4 + m_to_d[month];
    result = (year - 1970) * 365 + m_to_d[month];
    if (month <= 1)
        year -= 1;
    result += (year - 1968) / 4;
    result -= (year - 1900) / 100;
    result += (year - 1600) / 400;
    result += t->tm_mday;
    result -= 1;
    result *= 24;
    result += t->tm_hour;
    result *= 60;
    result += t->tm_min;
    result *= 60;
    result += t->tm_sec;
    return(result) -28800;//Bj timezone
}
inline time_t getTime(int year,int month,int day) 
{
    struct tm t;
    t.tm_year=year-1900;
    t.tm_mon=month-1;//-1,0-based
    t.tm_mday=day;
    t.tm_hour=0;t.tm_min=0;
    t.tm_sec=0;t.tm_isdst=0;
    return mktime_bj(&t);
}
inline void getYearMonthDay(time_t t,int & year,int & month, int &day)
{
    struct tm * temp=localtime(&t);
    year=temp->tm_year+1900;
    month=temp->tm_mon+1;
    day=temp->tm_mday;
}


//  02/31/2000 -> 02/29/2000
//  04/31/2000 -> 04/30/2000
//  and can't be future;
inline void fix_date(int & year,int & month,int &day)
{
    int max_date=31;
    if(month==4||month==6||month==9||month==11) {
        max_date=30;
    } else if( month==2) {
        if((year%400==0) || (year%100!=0) && (year%4==0))
            max_date=29;
        else
            max_date=28;
    }
    if( day>max_date )
        day=max_date;
    if( getTime(year,month,day) > time(NULL) ) {
        getYearMonthDay(time(NULL),year,month,day);
    }
}
inline int type2stype(const int & type) {
    return type&0xFFFF;
}
inline int makeWide(int orignal ,bool wide) {
    if(wide)
        return orignal | kWideFlag ;
    else
        return orignal & (~kWideFlag);
}

FeedIndexSeq DbHelper::getFeedIndex(int stype, Ice::Long source, int actor) {
  IndexLoaderPtr loader = new IndexLoader();
  for (size_t i = 0; i < 10; ++i) {
    ostringstream sql;
    sql << "select * from " << getIndexTab(i) << " where " << COL_SMALLTYPE
        << " = " << stype << " and " << COL_SOURCE << " = " << source;
    if(  actor >0) {
        sql << " and " << COL_ACTOR << " = " << actor << " and (time>='2010-01-01 00:00:00' or time < '2009-06-01 00:00:00' ) limit 5";
    } else if (actor == -1) {
        sql << " and (time>='2010-01-01 00:00:00' or time < '2009-06-01 00:00:00' ) limit 5";
    } else {
       MCE_WARN("DbHelper::getFeedIndex --> actor:"<<actor<<" is illgeal")
       return FeedIndexSeq();
    }
    loader->addQuery(DB_INSTANCE, sql.str(), getIndexTab(i), true);
		//MCE_INFO("DbHelper::getFeedIndex --> addQuery, i = " << i << ",sql = " << sql.str());
  }
  return loader->query();
}

FeedIndexSeq DbHelper::getFeedIndexByTime(Ice::Long source, int stype, int actor, int time) {
  FeedIndexSeq  ret;
  TimeStat ts;
  Statement sql;
  sql << "SELECT * FROM " << getMiniTab(actor, true) << " WHERE "  
      << "user = " << actor << " AND UNIX_TIMESTAMP(time) > " << time - 30 <<  " AND UNIX_TIMESTAMP(time) <"<< time + 60;
  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, getMiniTab(actor,true)).store(sql);
  if (!res) {
		MCE_WARN("DbHelper::getFeedIndexByTime --> query db fail! source:" << source << " stype:" << stype << " actor:" << actor << " time:" << time);
    return ret;
  }
	if(res.empty()){
		MCE_INFO("DbHelper::getFeedIndexByTime --> res is empty! source:"<< source << " stype:" << stype << " actor:" << actor << " time:" << time);
		return ret;
	}
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row& row = res.at(i);
    if(source == (Ice::Long)row[COL_SOURCE]  && stype == (int)row[COL_SMALLTYPE]) {
      FeedIndexPtr index = new FeedIndex;
      index->feed = row[COL_FEED];
      index->newsMerge = 0;
      index->miniMerge = row[COL_MINI_MERGE];
      index->source = source;
      index->psource = source;
      index->stype = stype;
      index->ptype = row[COL_BIG_TYPE];
      index->actor = actor;
      index->time = (time_t) mysqlpp::DateTime(row[COL_TIME]);
      ret.push_back(index);
    }
  }
  MCE_INFO("DbHelper::getFeedIndexByTime --> cost:" << ts.getTime() << " index size:" << ret.size());
  return ret;
}
FeedIndexSeq DbHelper::getFeedIndexBySource(int stype, Ice::Long source) {
  IndexLoaderPtr loader = new IndexLoader();
  for (size_t i = 0; i < 10; ++i) {
    ostringstream sql;
    sql << "select * from " << getIndexTab(i) << " where " << COL_SMALLTYPE
        << " = " << stype << " and " << COL_SOURCE << " = " << source
        << " order by time desc limit 10";
    loader->addQuery(DB_INSTANCE, sql.str(), getIndexTab(i), true);
  }
  return loader->query();
}

FeedIndexSeq DbHelper::getFeedIndexByPsource(int ptype, Ice::Long psource) {
  IndexLoaderPtr loader = new IndexLoader();
  for (size_t i = 0; i < 10; ++i) {
    ostringstream sql;
    sql << "select * from " << getIndexTab(i) << " where " << COL_PTYPE
        << " = " << ptype << " and " << COL_PSOURCE << " = " << psource
        << " order by time desc limit 30";
    loader->addQuery(DB_INSTANCE, sql.str(), getIndexTab(i), true);
  }
  return loader->query();
}
FeedIndexPtr DbHelper::getFeedIndex(Ice::Long feed) {
  TimeStat ts;
  Statement sql;
  sql << "SELECT * FROM " << getIndexTab(feed) << " WHERE " << COL_FEED
      << " = " << feed;
  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer,
      getIndexTab(feed)).store(sql);
  if (!res) {
		MCE_WARN("DbHelper::getFeedIndex --> query db fail ");
    return 0;
  }	if(res.empty()){
		MCE_INFO("DbHelper::getFeedIndex --> res is empty");
		return 0;
	}
  FeedIndexPtr index = new FeedIndex;
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row& row = res.at(i);
    index->feed = row[COL_FEED];
    index->newsMerge = row[COL_NEWS_MERGE];
    index->miniMerge = row[COL_MINI_MERGE];
    index->source = row[COL_SOURCE];
    index->psource = row[COL_PSOURCE];
    index->stype = row[COL_SMALLTYPE];
    index->ptype = row[COL_PTYPE];
    index->actor = row[COL_ACTOR];
    index->time = (time_t) mysqlpp::DateTime(row[COL_TIME]);
  }
  MCE_INFO("DbHelper::getFeedIndex --> cost:" << ts.getTime());
  return index;
}

FeedIndexSeq DbHelper::getFeedIndexBC(int stype, Ice::Long source, int actor) {
  IndexLoaderPtr loader = new IndexLoader();
  for (int i = 0; i > -10; --i) {
    ostringstream sql;
    sql << "select * from " << getIndexBCTab(i) << " where " << COL_SMALLTYPE
      << " = " << stype << " and " << COL_SOURCE << " = " << source;
    if(  actor >0) {
      sql << " and " << COL_ACTOR << " = " << actor << " limit 5";
    } else if (actor == -1) {
      sql << " limit 5";
    } else {
      MCE_WARN("DbHelper::getFeedIndex --> actor:"<<actor<<" is illgeal");
      return FeedIndexSeq();
    }
    loader->addQuery(DB_BC_INSTANCE, sql.str(), getIndexBCTab(i), true);
    //MCE_INFO("DbHelper::getFeedIndex --> addQuery, i = " << i << ",sql = " << sql.str());
  }
  return loader->query();
}             
map<string, string> DbHelper::getFeedConfigProp(Ice::Long fid) {
  TimeStat ts;
  map<string, string> prop;
  Statement sql;
  sql << "SELECT prop FROM " << DbHelper::instance().getContentTab(fid)
    << " WHERE feed=" << fid;
  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer,
      getContentTab(fid)).store(sql);
  string propStr;
  if (!res || res.num_rows()==0) {
    MCE_WARN("DbHelper::getFeedConfigProp --> load from db null, fid:" << fid);
    return prop;
  }
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    propStr = row["prop"].data();
    size_t pos = 0;
    string::size_type idx;
    string key;
    string val;

    while (pos < propStr.size()) {

      idx = propStr.find('=', pos);
      if (idx == string::npos) {
        return prop;
      }

      key = propStr.substr(pos, idx - pos);
      pos = idx + 1;
      idx = propStr.find(',', pos);
      if (idx == string::npos) {
        return prop;
      }

      val = propStr.substr(pos, idx - pos);
      pos = idx + 1;

      prop[key] = val;

    }

    return prop;
  }
  MCE_INFO("DbHelper::getFeedConfigProp --> cost:" << ts.getTime());
  return prop;

}

map<string, string> DbHelper::getFeedZoneConfigProp(Ice::Long fid) {
  TimeStat ts;
  map<string, string> prop;
  Statement sql;
  sql << "SELECT prop FROM " << DbHelper::instance().getZoneContentTab(fid)
    << " WHERE feed=" << fid;
  mysqlpp::StoreQueryResult res = QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbRServer,
      getZoneContentTab(fid)).store(sql);
  string propStr;
  if (!res || res.num_rows()==0) {
    MCE_WARN("DbHelper::getFeedZoneConfigProp --> load from db null, fid:" << fid);
    return prop;
  }
  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);
    propStr = row["prop"].data();
    size_t pos = 0;
    string::size_type idx;
    string key;
    string val;

    while (pos < propStr.size()) {

      idx = propStr.find('=', pos);
      if (idx == string::npos) {
        return prop;
      }

      key = propStr.substr(pos, idx - pos);
      pos = idx + 1;
      idx = propStr.find(',', pos);
      if (idx == string::npos) {
        return prop;
      }

      val = propStr.substr(pos, idx - pos);
      pos = idx + 1;

      prop[key] = val;

    }

    return prop;
  }
  MCE_INFO("DbHelper::getFeedZoneConfigProp --> cost:" << ts.getTime());
  return prop;

}

map<string, string> DbHelper::getFeedContent(Ice::Long feed_id) {
  TimeStat ts;
  Statement sql;
  sql << "select * from " << getContentTab(feed_id) << " where feed = "
    << feed_id;

  mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer,
      getContentTab(feed_id)).store(sql);

  map<string, string> data;
  if (!res) {
    return data;
  }

  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);

    data["feed"] = row["feed"] .data();
    data["source"] = row["source"] .data();
    data["type"] = row["type"] .data();
    data["big_type"] = row["big_type"] .data();
    data["small_type"] = row["small_type"] .data();
    data["actor"] = row["actor"] .data();
    data["time"] = row["time"] .data();
    data["prop"] = row["prop"] .data();
    data["xml"] = row["xml"] .data();
    data["count_expr"] = row["count_expr"] .data();
    data["older_reply_id"] = row["older_reply_id"].data();
    data["older_reply"] = row["older_reply"] .data();
    data["newer_reply_id"] = row["newer_reply_id"].data();
    data["newer_reply"] = row["newer_reply"] .data();
    data["psource"] = row["psource"] .data();
    data["ptype"] = row["ptype"] .data();
    data["mini_merge"] = row["mini_merge"] .data();
    data["news_merge"] = row["news_merge"] .data();
    data["comment_count"] = row["comment_count"] .data();
  }
  /*

     for (map<string, string>::iterator it = data.begin(); it != data.end(); ++it) {
     MCE_INFO("key:" << it->first << ", value:"<< it->second);
     }*/
  MCE_INFO("DbHelper::getFeedContent --> cost:" << ts.getTime());
  return data;
}

map<string, string> DbHelper::getFeedZoneContent(Ice::Long feed_id) {
  TimeStat ts;
  Statement sql;
  sql << "select * from " << getZoneContentTab(feed_id) << " where feed = "
    << feed_id;

  mysqlpp::StoreQueryResult res = QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbRServer,
      getZoneContentTab(feed_id)).store(sql);

  map<string, string> data;
  if (!res) {
    return data;
  }

  for (size_t i = 0; i < res.num_rows(); ++i) {
    mysqlpp::Row row = res.at(i);

    data["feed"] = row["feed"] .data();
    data["source"] = row["source"] .data();
    data["type"] = row["type"] .data();
    data["big_type"] = row["big_type"] .data();
    data["small_type"] = row["small_type"] .data();
    data["actor"] = row["actor"] .data();
    data["time"] = row["time"] .data();
    data["prop"] = row["prop"] .data();
    data["xml"] = row["xml"] .data();
    data["count_expr"] = row["count_expr"] .data();
    data["older_reply_id"] = row["older_reply_id"].data();
    data["older_reply"] = row["older_reply"] .data();
    data["newer_reply_id"] = row["newer_reply_id"].data();
    data["newer_reply"] = row["newer_reply"] .data();
    data["psource"] = row["psource"] .data();
    data["ptype"] = row["ptype"] .data();
    data["mini_merge"] = row["mini_merge"] .data();
    data["news_merge"] = row["news_merge"] .data();
    data["comment_count"] = row["comment_count"] .data();
  }
  /*

     for (map<string, string>::iterator it = data.begin(); it != data.end(); ++it) {
     MCE_INFO("key:" << it->first << ", value:"<< it->second);
     }*/
  MCE_INFO("DbHelper::getZoneFeedContent --> cost:" << ts.getTime());
  return data;
}

bool DbHelper::copyFeedContent(Ice::Long old_feed_id, Ice::Long new_feed_id,
    int new_type, const string& new_props) {

  TimeStat ts;
  map<string, string> data = getFeedContent(old_feed_id);
  if (data.empty()) {
    return false;
  }
  data["feed"] = boost::lexical_cast<string>(new_feed_id);
  data["type"] = boost::lexical_cast<string>(new_type);
  data["big_type"] = boost::lexical_cast<string>(
      ((new_type & 0xffff) / 100) * 100);
  data["small_type"] = boost::lexical_cast<string>(new_type & 0xffff);
  data["prop"] = new_props;

  data["older_reply_id"] = "0";
  data["older_reply"] = "";
  data["newer_reply_id"] = "0";
  data["newer_reply"] = "";
  data["comment_count"] = "0";
  data.erase("time");

  Statement sql;
  sql << "insert into " << getContentTab(new_feed_id) << " (";
  for (map<string, string>::iterator it = data.begin(); it != data.end(); ++it) {
    if (it != data.begin()) {
      sql << ",";
    }
    sql << it->first;
  }
  sql << ") values (";
  for (map<string, string>::iterator it = data.begin(); it != data.end(); ++it) {
    if (it != data.begin()) {
      sql << ",";
    }
    sql << mysqlpp::quote << it->second;
  }
  sql << ")";
  QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(new_feed_id)).store(sql);
  MCE_INFO("DbHelper::copyFeedContent --> cost:" << ts.getTime());

  return true;
}

bool DbHelper::copyFeedZoneContent(Ice::Long old_feed_id, Ice::Long new_feed_id,
    int new_type, const string& new_props) {

  TimeStat ts;
  map<string, string> data = getFeedZoneContent(old_feed_id);
  if (data.empty()) {
    return false;
  }
  data["feed"] = boost::lexical_cast<string>(new_feed_id);
  data["type"] = boost::lexical_cast<string>(new_type);
  data["big_type"] = boost::lexical_cast<string>(
      ((new_type & 0xffff) / 100) * 100);
  data["small_type"] = boost::lexical_cast<string>(new_type & 0xffff);
  data["prop"] = new_props;

  data["older_reply_id"] = "0";
  data["older_reply"] = "";
  data["newer_reply_id"] = "0";
  data["newer_reply"] = "";
  data["comment_count"] = "0";
  data.erase("time");

  Statement sql;
  sql << "insert into " << getZoneContentTab(new_feed_id) << " (";
  for (map<string, string>::iterator it = data.begin(); it != data.end(); ++it) {
    if (it != data.begin()) {
      sql << ",";
    }
    sql << it->first;
  }
  sql << ") values (";
  for (map<string, string>::iterator it = data.begin(); it != data.end(); ++it) {
    if (it != data.begin()) {
      sql << ",";
    }
    sql << mysqlpp::quote << it->second;
  }
  sql << ")";
  QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(new_feed_id)).store(sql);
  MCE_INFO("DbHelper::copyFeedZoneContent --> cost:" << ts.getTime());

  return true;
}


/*
   Ice::Long DbHelper::createId() {
   IceUtil::Mutex::Lock lock(_mutex);
   while (1) {
   MCE_DEBUG("_curIndex:"<<_curIndex << ", _topIndex:"<<_topIndex);
   if (_curIndex != -1 && _curIndex < _topIndex) {
   return _curIndex++;
   }
   {
   Statement sql;
   sql << "update " << TAB_ID_SEQ << " SET id = id +"
   << ID_SEQ_STEP;
   QueryRunner(DB_INSTANCE, CDbWServer, TAB_ID_SEQ).store(sql);
   }
   {
   Statement sql;
   sql << "SELECT * FROM " << TAB_ID_SEQ;
   mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE,
   CDbRServer, TAB_ID_SEQ).store(sql);
   if (!res) {
   return -1;
   }
   for (size_t i = 0; i < res.num_rows(); ++i) {
   mysqlpp::Row row = res.at(i);
   _topIndex = (int) row["ID"];
   _curIndex = _topIndex - ID_SEQ_STEP;
   MCE_DEBUG("topIndex:"<<_topIndex);
   break;
   }
   }

   }
   }
   */
//---------------------------------------------------------------------------

FeedIndexSeq IndexLoader::query() {
  for (size_t i = 0; i < _taskSeq.size(); ++i) {
    //MCE_INFO("IndexLoader::query --> execute task i = " << i);
    MyUtil::TaskManager::instance().execute(_taskSeq.at(i));
  }
  _taskSeq.clear();

  IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
  if (!timedWait(IceUtil::Time::milliSeconds(LOAD_TIMEOUT))) {
    MCE_WARN("IndexLoader::query -> count: " << _count << " missing");
  } else {
    //MCE_INFO("IndexLoader::query -> success");
  }
  return _indexSeq;
}
void IndexLoader::addQuery(const string& dbInstance, const string& sql,
    const string& pattern, bool isSelect) {
  SqlExecuteTask* t = new SqlExecuteTask(this, isSelect);
  t->init(dbInstance, sql, pattern);
  _taskSeq.push_back(t);
  ++_count;
}

void IndexLoader::SqlExecuteTask::handle() {
  //	MCE_DEBUG("IndexLoader --> SqlExecuteTask  seq:"<<_sql);
  TimeStat ts;
  if (_isSelect) {
    mysqlpp::StoreQueryResult res;
    try{
      res = QueryRunner(_dbInstance, CDbRServer, _pattern).store(_sql);
    }catch(std::exception& e){
      MCE_WARN("IndexLoader::SqlExecuteTask --> err, " << e.what());
    }
    if (!res) {
      MCE_WARN("IndexLoader::SqlExecuteTask::handle --> query return null");
      return;
    }
    //MCE_INFO("IndexLoader::SqlExecuteTask::handle -->1 cost:" << ts.getTime() << " rows:" << res.num_rows() << " expr:" << _pattern);
    FeedIndexSeq seq;
    for (size_t i = 0; i < res.num_rows(); ++i) {
      mysqlpp::Row row = res.at(i);

      FeedIndexPtr index = new FeedIndex;
      index->feed = row[COL_FEED];
      index->newsMerge = row[COL_NEWS_MERGE];
      index->miniMerge = row[COL_MINI_MERGE];
      index->source = row[COL_SOURCE];
      index->psource = row[COL_PSOURCE];
      index->stype = row[COL_SMALLTYPE];
      index->ptype = row[COL_PTYPE];
      index->actor = row[COL_ACTOR];
      index->time = (time_t) mysqlpp::DateTime(row[COL_TIME]);
      seq.push_back(index);

    }
    _loader->finish(seq);

  } else {
    try{
//TODO     QueryRunner(_dbInstance, CDbWServer, _pattern).store(_sql);
    }catch(std::exception& e){
      MCE_WARN("IndexLoader::SqlExecuteTask --> err, " << e.what());
    }
    _loader->finish();
  }
  //MCE_INFO("IndexLoader::SqlExecuteTask::handle  --> cost:" << ts.getTime());
}




