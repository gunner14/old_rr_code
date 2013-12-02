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
/*
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

//code above has bug. so give up. return to system's implement. 
    tm  timeinfo;
    timeinfo.tm_year = year-1900;
    timeinfo.tm_mon = month-1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = min;
    timeinfo.tm_sec = sec;
    timeinfo.tm_isdst=0;*/
    return mktime(t);

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



void DbHelper::addFeedIndex(const FeedIndexPtr& itemPtr) {
  if (!itemPtr) {
    return;
  }
  TimeStat ts;
  Statement sql;
  sql << "INSERT INTO " << getIndexTab(itemPtr->feed)
      << " (feed,source,psource,small_type,big_type,ptype,actor,time,mini_merge,news_merge)"
      << " VALUES (" << itemPtr->feed << "," << itemPtr->source << ","
      << itemPtr->psource << ", " << itemPtr->stype << ","
      << int(itemPtr->stype / 100) * 100 << "," << itemPtr->ptype << ","
      << itemPtr->actor << ",from_unixtime(" << itemPtr->time << "),"
      << itemPtr->miniMerge << "," << itemPtr->newsMerge << ")";

  QueryRunner(DB_INSTANCE, CDbWServer, getIndexTab(itemPtr->feed)).store(sql);
  MCE_INFO("DbHelper::addFeedIndex --> cost:" << ts.getTime());
}

FeedIndexSeq DbHelper::getFeedIndex(int stype, Ice::Long source, int actor) {
  IndexLoaderPtr loader = new IndexLoader();
  for (size_t i = 0; i < 10; ++i) {
    ostringstream sql;
    sql << "select * from " << getIndexTab(i) << " where " << COL_SMALLTYPE
        << " = " << stype << " and " << COL_SOURCE << " = " << source;
    if(  actor >0) { //避免在feed_index表中读10年之前的新鲜事,同时为兼容广告时间为负引起的ｂｕｇ
        sql << " and " << COL_ACTOR << " = " << actor << " and (time>='2010-01-01 00:00:00' or time < '2009-06-01 00:00:00' ) limit 5";
    } else if (actor == -1) {
        sql << " and time>=(time>='2010-01-01 00:00:00' or time < '2009-06-01 00:00:00' ) limit 5";
    } else {
       MCE_WARN("DbHelper::getFeedIndex --> actor:"<<actor<<" is illgeal")
       continue;
    }
    loader->addQuery(DB_INSTANCE, sql.str(), getIndexTab(i), true);
		//MCE_INFO("DbHelper::getFeedIndex --> addQuery, i = " << i << ",sql = " << sql.str());
  }
  return loader->query();
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
       MCE_WARN("DbHelper::getFeedIndex --> actor:"<<actor<<" is illgeal")
       continue;
    }
    loader->addQuery(DB_BC_INSTANCE, sql.str(), getIndexBCTab(i), true);
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
void DbHelper::removeFeedIndex(const MyUtil::LongSeq& feeds) {
	MCE_INFO("DbHelper::removeAllFeedMini --> feeds size = " << feeds.size());
  map<int, MyUtil::LongSeq> tmp;
  for (size_t i = 0; i < feeds.size(); ++i) {
    tmp[feeds.at(i) % 10].push_back(feeds.at(i));
  }
  IndexLoaderPtr loader = new IndexLoader();
  for (map<int, MyUtil::LongSeq>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    ostringstream sql;
    sql << "delete from " << getIndexTab(it->first) << " where " << COL_FEED
        << " in (";
    for (size_t i = 0; i < it->second.size(); ++i) {
      if (i) {
        sql << ",";
      }
      sql << it->second.at(i);
    }
    sql << ")";
    loader->addQuery(DB_INSTANCE, sql.str(), getIndexTab(it->first), false);
  }
  loader->query();
}
void DbHelper::removeFeedIndexBysource(Ice::Long source,int stype,int actor) {
  timeval t1,t2;
  gettimeofday(&t1,NULL);
  MCE_INFO("DbHelper::removeFeedIndexBysource --> source:"<<source<<" stype:"<<stype<<" actor:"<<actor);
  Statement sql;
  ostringstream osql;
  osql<<"delete  from "<<getIndexBysourceTab(source)<<" where source = "<<source
     <<" and small_type ="<<stype;
  if (actor >0 ) {
    osql<<" and actor="<<actor;
  }
  sql<<osql.str();
  try {
    QueryRunner(DB_INSTANCE_INDEX_BY_SOURCE, CDbWServer, getIndexBysourceTab(source)).store(sql);
  } catch (mysqlpp::Exception &e) {
    MCE_WARN("DbHelper::removeFeedIndexBysource --> err:"<<e.what());
  }
  gettimeofday(&t2,NULL);
  float tc =(t2.tv_usec - t1.tv_usec)*1.0 /1000  + (t2.tv_sec -  t1.tv_sec)*1000;
  MCE_INFO("DbHelper::removeFeedIndexBysource --> time cost:"<<tc);
}
void DbHelper::addFeedIndexBysource(const FeedIndexPtr& itemPtr) {
  if (!itemPtr) {
    return;
  }
  timeval t1,t2;
  gettimeofday(&t1,NULL);
  Statement sql;
  sql << "INSERT INTO " << getIndexBysourceTab(itemPtr->source)
      << " (feed,source,psource,small_type,big_type,ptype,actor,time,mini_merge,news_merge)"
      << " VALUES (" << itemPtr->feed << "," << itemPtr->source << ","
      << itemPtr->psource << ", " << itemPtr->stype << ","
      << int(itemPtr->stype / 100) * 100 << "," << itemPtr->ptype << ","
      << itemPtr->actor << ",from_unixtime(" << itemPtr->time << "),"
      << itemPtr->miniMerge << "," << itemPtr->newsMerge << ")";
  try {
    QueryRunner(DB_INSTANCE_INDEX_BY_SOURCE, CDbWServer, getIndexBysourceTab(itemPtr->source)).store(sql);
  } catch (mysqlpp::Exception &e) {
    MCE_WARN("DbHelper::addFeedBysource --> err:"<<e.what());
  }
  gettimeofday(&t2,NULL);
  float tc =(t2.tv_usec - t1.tv_usec)*1.0 /1000  + (t2.tv_sec -  t1.tv_sec)*1000;
  MCE_INFO("DbHelper::addFeedIndexBysource --> time cost:"<<tc);
}

void DbHelper::addFeedContent(const FeedSeedPtr& seed) {
  if (!seed) {
    return;
  }
  TimeStat ts;
  ostringstream os;
  for (MyUtil::Str2StrMap::iterator it = seed->extraProps.begin(); it
      != seed->extraProps.end(); ++it) {
    os << it->first << '=' << it->second << ',';
  }
  Statement sql;
  sql << "insert into " << DbHelper::instance().getContentTab(seed->feed)
      << " (feed,source,type,big_type,small_type,actor,time,prop,xml,psource,ptype,mini_merge,news_merge) values ("
      << seed->feed << "," << seed->source << "," << seed->type << ","
      << int((seed->type & 0xffff) / 100) * 100 << "," << (seed->type
      & 0xffff) << "," << seed->actor << "," << "from_unixtime("
      << seed->time << ")," << mysqlpp::quote << os.str() << ","
      << mysqlpp::quote << seed->xml << "," << seed->psource << ","
      << seed->ptype << "," << seed->miniMerge << "," << seed->newsMerge << ")";

  QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(seed->feed)).store(sql);
  MCE_INFO("DbHelper::addFeedContent --> cost:" << ts.getTime());
}

void DbHelper::addZoneFeedContent(const FeedSeedPtr& seed) {
  if (!seed) {
    return;
  }
  TimeStat ts;
  ostringstream os;
  for (MyUtil::Str2StrMap::iterator it = seed->extraProps.begin(); it
      != seed->extraProps.end(); ++it) {
    os << it->first << '=' << it->second << ',';
  }
  Statement sql;
  sql << "insert into " << DbHelper::instance().getZoneContentTab(seed->feed)
      << " (feed,source,type,big_type,small_type,actor,time,prop,xml,psource,ptype,mini_merge,news_merge) values ("
      << seed->feed << "," << seed->source << "," << seed->type << ","
      << int((seed->type & 0xffff) / 100) * 100 << "," << (seed->type
      & 0xffff) << "," << seed->actor << "," << "from_unixtime("
      << seed->time << ")," << mysqlpp::quote << os.str() << ","
      << mysqlpp::quote << seed->xml << "," << seed->psource << ","
      << seed->ptype << "," << seed->miniMerge << "," << seed->newsMerge << ")";

  QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(seed->feed)).store(sql);
  MCE_INFO("DbHelper::addZoneFeedContent --> cost:" << ts.getTime());
}

//替换XML
void DbHelper::ReplaceXML(const  MyUtil::LongSeq & feeds , const string & xml ){
	MCE_DEBUG("DbHelper::ReplaceXML --> feeds size = " << feeds.size());
  if (feeds.empty()) {
    return;
  }
  TimeStat ts;
  map<int, MyUtil::LongSeq> ids;
  for (size_t i = 0; i < feeds.size(); ++i) {
    ids[feeds.at(i) % 100].push_back(feeds.at(i));
  }

  for (map<int, MyUtil::LongSeq>::iterator it = ids.begin(); it != ids.end(); ++it) {
    Statement sql;
    sql << "update " << getContentTab(it->first) << " set xml = " << mysqlpp::quote  << xml << " where feed in (";
    for (size_t i = 0; i < it->second.size(); ++i) {
      if (i) {
        sql << ",";
      }
      sql << it->second.at(i);
    }
    sql << ")";
    QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(it->first)).store(sql);
  }
  MCE_INFO("DbHelper::ReplaceXML --> cost:" << ts.getTime());
}

void DbHelper::ReplaceZoneXML(const  MyUtil::LongSeq & feeds , const string & xml ){
	MCE_DEBUG("DbHelper::ReplaceZoneXML --> feeds size = " << feeds.size());
  if (feeds.empty()) {
    return;
  }
  TimeStat ts;
  map<int, map<int, MyUtil::LongSeq> > ids;
  for (size_t i = 0; i < feeds.size(); ++i) {
    ids[feeds.at(i) % 100][feeds.at(i)/DB_CONTENT_ZONE].push_back(feeds.at(i));
  }

  for (map<int, map<int, MyUtil::LongSeq> >::iterator it = ids.begin(); it != ids.end(); ++it) {
	  for (map<int, MyUtil::LongSeq>::iterator ib = it->second.begin();
			  ib != it->second.end(); ++ib) {
		  Statement sql;
		  sql << "update " << getZoneContentTab(it->first,ib->first) << " set xml = " << mysqlpp::quote  << xml << " where feed in (";
		  for (size_t i = 0; i < ib->second.size(); ++i) {
			  if (i) {
				  sql << ",";
			  }
			  sql << ib->second.at(i);
		  }
		  sql << ")";
		  QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(it->first,ib->first)).store(sql);
	  }
  }
  MCE_INFO("DbHelper::ReplaceZoneXML --> cost:" << ts.getTime());
}
void DbHelper::removeFeedContent(const MyUtil::LongSeq& feeds) {
  if (feeds.empty()) {
    return;
  }
  TimeStat ts;
  map<int, MyUtil::LongSeq> ids;
  for (size_t i = 0; i < feeds.size(); ++i) {
    ids[feeds.at(i) % 100].push_back(feeds.at(i));
  }
  for (map<int, MyUtil::LongSeq>::iterator it = ids.begin(); it != ids.end(); ++it) {
    Statement sql;
    sql << "delete from " << getContentTab(it->first) << " where feed in (";
    for (size_t i = 0; i < it->second.size(); ++i) {
      if (i) {
        sql << ",";
      }
      sql << it->second.at(i);
    }
    sql << ")";

    QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(it->first)).store(sql);

  }
  MCE_INFO("DbHelper::removeFeedContent --> cost:" << ts.getTime());
}

void DbHelper::removeFeedZoneContent(const MyUtil::LongSeq& feeds) {
  if (feeds.empty()) {
    return;
  }
  TimeStat ts;
  map<int, map<int, MyUtil::LongSeq> > ids;
  for (size_t i = 0; i < feeds.size(); ++i) {
    ids[feeds.at(i) % 100][feeds.at(i)/DB_CONTENT_ZONE].push_back(feeds.at(i));
  }
  for (map<int, map<int, MyUtil::LongSeq> >::iterator it = ids.begin(); it != ids.end(); ++it) {
	  for (map<int, MyUtil::LongSeq>::iterator ib = it->second.begin();
			  ib != it->second.end();++ib) {
		  Statement sql;
		  sql << "delete from " << getZoneContentTab(it->first, ib->first) << " where feed in (";
		  for (size_t i = 0; i < ib->second.size(); ++i) {
			  if (i) {
				  sql << ",";
			  }
			  sql << ib->second.at(i);
		  }
		  sql << ")";

		  QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(it->first, ib->first)).store(sql);

	  }
  }
  MCE_INFO("DbHelper::removeFeedZoneContent --> cost:" << ts.getTime());
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

void DbHelper::addFeedMini(const FeedSeedPtr& seed) {
  MCE_DEBUG("DbHelper::addFeedMini --> uid:"<<seed->actor);
  if (!seed) {
    return;
  }
  TimeStat ts;

  // if no year props, use seed->time
  // if has year pros and date is today , use seed->time
  // if has year pros and date isn't today , use time_t=getTime("year","month","day")
  int year_now,month_now,day_now;
  getYearMonthDay(time(NULL),year_now,month_now,day_now);
  
  Str2StrMap & context = seed->extraProps;
  int year=9999,month=12,day=31;
  bool contains_date=false;
  Str2StrMap::iterator it;
  time_t temp_time=seed->time;
  bool is_today=false;
  bool after_2009=true;
  
  if((it=context.find(kYear))!=context.end()) {
     contains_date=true;
     stringstream stream; stream.str(it->second);
     stream>>year;
     if (!stream.fail()) {
        if ( ( it=context.find(kMonth) ) !=context.end() ) {
            stream.clear();
            stream.str(it->second);
            stream>>month;
            if(stream.fail()) {
                 month=12;
            }
        }
        if ( ( it=context.find(kDay) ) !=context.end()){
            stream.clear();
            stream.str(it->second);
            stream>>day;
            if(stream.fail() ){
                day=31;
            }
        }
     }else {
         year=9999;
     }
     //fix day of month
     fix_date(year,month,day);

     if( year != year_now || month !=month_now || day!=day_now ) {
         is_today=false;
         // we need pass the time_t value to TimeLineFeed, so we call getTime to caculate it.
         // we don't use this value in later sql because mysql's from_unixtime() 
         // doesn't support time before 1970 which may happen here.
         temp_time=getTime(year,month,day);
     } else {
         is_today=true;
     }
     if(year <=2009) {
         after_2009=false; 
     }
  }

  string miniDB=getMiniDB(after_2009);
  string miniTable=getMiniTab(seed->actor,after_2009);
  
  if ((seed->type & 0x30000000) >> 28 == FeedMergeReplace) {
    Statement sql;
    sql << " delete from " << miniTable << " where mini_merge="
        << seed->miniMerge << " AND  small_type=" << int(seed->type
        & 0xffff) << " AND user=" << seed->actor;
    QueryRunner(miniDB, CDbWServer, miniTable).store(sql);
  }
  //for 1205( life-event feed),set wide screen
  int temp_type=seed->type;
  if( type2stype(temp_type)==1205 )
  {
      temp_type=makeWide(temp_type,true);
  }
  
  Statement sql; stringstream ss;
  ss << "insert into " << miniTable
  << " (user, feed, big_type,small_type,source,time,unread,mini_merge,type,actor) values ("
  << seed->actor << "," << seed->feed << "," << int((seed->type
  & 0xffff) / 100) * 100 << "," << int(seed->type & 0xffff)
  << "," << seed->source << ",";
  if(contains_date && ! is_today ) {
    ss<<" \""<<year<<"-"<<month<<"-"<<day<<" 00:00:00\" ,";
  } else {
    ss<<" from_unixtime(" << temp_time <<") ,";
  }
  ss<<" 1 ,"<< seed->miniMerge << " , " << temp_type << "," << seed->actor<< " ) ";
  //MCE_INFO("DbHelper::addFeedMini -->  sql:"<<ss.str());
  sql<<ss.str();
  mysqlpp::SimpleResult res;
  try {
    res=QueryRunner(miniDB, CDbWServer,miniTable).execute(sql);
  }catch(mysqlpp::Exception& e)
  {
    MCE_INFO("DbHelper::addFeedMini --> exception!!"<<e.what());
  }
  if(!res) {
    //MCE_DEBUG("DbHelper::addFeedMini --> insert failed");
  }
  MCE_INFO("DbHelper::addFeedMini --> update TimeLine uid:"<<seed->actor<<" time:"<<temp_time);
  //Update TimeLine
  try {
      timeline::TimeLineFeedReplicaAdapter::instance().AddFeedMiniNotify(seed,temp_type,temp_time);
  } catch (const ::Ice::Exception &e) {
     MCE_INFO("DbHelper::addFeedMini --> exception TimeLineFeed.AddFeedMiniNotify:"<<seed->actor<<", "<<e.what());
  }
}


void DbHelper::addFeedGroup(int groupid, const FeedSeedPtr& seed) {
  if (!seed) {
    return;
  }
  if ((seed->type & 0x30000000) >> 28 == FeedMergeReplace) {
    Statement sql;
    sql << " delete from " << getGroupTab(groupid) << " where mini_merge="
        << seed->miniMerge << " AND  small_type=" << int(seed->type
        & 0xffff) << " AND group_id=" << groupid;
    QueryRunner(GROUP_DB_INSTANCE, CDbWServer, getGroupTab(groupid)).store(sql);
  }
  
  {
    Statement sql;
    sql << "insert into " << getGroupTab(groupid)
        << " (group_id, feed, big_type,small_type,source,mini_merge,type,actor) values ("
        << groupid << "," << seed->feed << "," << int((seed->type
        & 0xffff) / 100) * 100 << "," << int(seed->type & 0xffff)
        << "," << seed->source << ","
        << seed->miniMerge << " , " << seed->type << "," << seed->actor
        << " ) ";
    QueryRunner(GROUP_DB_INSTANCE, CDbWServer, getGroupTab(groupid)).store(sql);
  }
}

void DbHelper::removeFeedMini(int user, int stype, Ice::Long miniMerge) {
  //without test
  TimeStat ts;
  //Statement sql;
  //we don't know the year of feed, so delete it from mini & mini_bc
  //sql << "delete from " << getMiniTab(user,true) << " where user=" << user
  //    << " and small_type=" << stype << " and mini_merge=" << miniMerge;
  //QueryRunner(getMiniDB(true), CDbWServer, getMiniTab(user,true)).execute(sql);
  //sql.clear();
  
  //sql << "delete from " << getMiniTab(user,false) << " where user=" << user
  //    << " and small_type=" << stype << " and mini_merge=" << miniMerge;
  //QueryRunner(getMiniDB(false), CDbWServer, getMiniTab(user,false)).execute(sql);

  MCE_INFO("DbHelper::removeFeedMini --> Notify timelinefeed to do removeFeedMini, user:"<<user<<" stype:"<<stype<<" merge:"<<miniMerge);
  try {
      timeline::TimeLineFeedReplicaAdapter::instance().SetReadMini(user,-1,stype,miniMerge);
  } catch (std::exception & e) {
      //timelinefeed sucks, so delete DB on my own.
      MCE_WARN("(DbHelper::removeFeedMini --> call timeline->SetReadMini exception:"<<e.what());
      
      try {
          Statement sql;
          //we don't know the year of feed, so delete it from mini & mini_bc
          sql << "delete from " << getMiniTab(user,true) << " where user=" << user
            << " and small_type=" << stype << " and mini_merge=" << miniMerge;
          QueryRunner(getMiniDB(true), CDbWServer, getMiniTab(user,true)).execute(sql);
          sql.clear();
          sql << "delete from " << getMiniTab(user,false) << " where user=" << user
             << " and small_type=" << stype << " and mini_merge=" << miniMerge;
          QueryRunner(getMiniDB(false), CDbWServer, getMiniTab(user,false)).execute(sql);
      }catch (mysqlpp::Exception & e) {
          MCE_WARN("DbHelper::removeFeedMini --> QueryRunner mysqlpp Exception err:"<<e.what());
      }
  }
  MCE_INFO("DbHelper::removeFeedMini --> cost:" << ts.getTime());
}

void DbHelper::removeAllFeedMini(int user) {
  //without test
  Statement sql;
  sql << "delete from " << getMiniTab(user,true) << " where user=" << user;
  QueryRunner(getMiniDB(true), CDbWServer, getMiniTab(user,true)).store(sql);
  sql << "delete from " << getMiniTab(user,false) << " where user=" << user;
  QueryRunner(getMiniDB(false), CDbWServer, getMiniTab(user,false)).store(sql);
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

void DbHelper::UpdateReply(const map<Ice::Long, FeedReplyPtr>& buffer){
  typedef map<Ice::Long, FeedReplyPtr> BufferMap;
  typedef BufferMap::const_iterator BufferMapIter;
  map<int, vector<BufferMapIter> > tmp;
  for (BufferMapIter iter = buffer.begin(); iter != buffer.end(); ++iter) {
    tmp[iter->first % 100].push_back(iter);
  }
  for (map<int, vector<BufferMapIter> >::iterator iter = tmp.begin(); iter
      != tmp.end(); ++iter) {
    Statement sql;
    //MCE_INFO("DbHelper::UpdateReply --> index:" << iter->first << " size:" << iter->second.size());
    sql << " insert into " << getContentTab(iter->first)
        << " (feed,older_reply_id,older_reply,newer_reply_id,newer_reply, comment_count) values ";
    for (vector<BufferMapIter>::iterator h = iter->second.begin(); h
        != iter->second.end(); ++h) {
      if (h != iter->second.begin()) {
        sql << " , ";
      }
      sql << " (" << (*h)->first << "," << (*h)->second->oldReplyId
          << "," << mysqlpp::quote << (*h)->second->oldReply << ","
          << (*h)->second->newReplyId << "," << mysqlpp::quote
          << (*h)->second->newReply << "," << (*h)->second->count
          << ") ";
    }
    sql
        << " on duplicate key update older_reply_id=values(older_reply_id),older_reply=values(older_reply),newer_reply_id=values(newer_reply_id),newer_reply=values(newer_reply),comment_count=values(comment_count)";
    QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(iter->first)).schedule(sql);
  } 
}

void DbHelper::UpdateZoneReply(const map<Ice::Long, FeedReplyPtr>& buffer){
  typedef map<Ice::Long, FeedReplyPtr> BufferMap;
  typedef BufferMap::const_iterator BufferMapIter;
  map<int, map<int, vector<BufferMapIter> > > tmp;
  for (BufferMapIter iter = buffer.begin(); iter != buffer.end(); ++iter) {
    tmp[iter->first % 100][iter->first/DB_CONTENT_ZONE].push_back(iter);
  }
  for (map<int, map<int, vector<BufferMapIter> > >::iterator iter = tmp.begin(); iter
      != tmp.end(); ++iter) {
	  for (map<int, vector<BufferMapIter> >::iterator ib = iter->second.begin();
			  ib != iter->second.end(); ++ib) {
		  Statement sql;
		  //MCE_INFO("DbHelper::UpdateReply --> index:" << iter->first << " size:" << iter->second.size());
		  sql << " insert into " << getZoneContentTab(iter->first,ib->first)
			  << " (feed,older_reply_id,older_reply,newer_reply_id,newer_reply, comment_count) values ";
		  for (vector<BufferMapIter>::iterator h = ib->second.begin(); h
				  != ib->second.end(); ++h) {
			  if (h != ib->second.begin()) {
				  sql << " , ";
			  }
			  sql << " (" << (*h)->first << "," << (*h)->second->oldReplyId
				  << "," << mysqlpp::quote << (*h)->second->oldReply << ","
				  << (*h)->second->newReplyId << "," << mysqlpp::quote
				  << (*h)->second->newReply << "," << (*h)->second->count
				  << ") ";
		  }
		  sql
			  << " on duplicate key update older_reply_id=values(older_reply_id),older_reply=values(older_reply),newer_reply_id=values(newer_reply_id),newer_reply=values(newer_reply),comment_count=values(comment_count)";
		  QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(iter->first,ib->first)).schedule(sql);
	  } 
  }
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
      QueryRunner(_dbInstance, CDbWServer, _pattern).store(_sql);
    }catch(std::exception& e){
      MCE_WARN("IndexLoader::SqlExecuteTask --> err, " << e.what());
    }
    _loader->finish();
  }
  //MCE_INFO("IndexLoader::SqlExecuteTask::handle  --> cost:" << ts.getTime());
}




