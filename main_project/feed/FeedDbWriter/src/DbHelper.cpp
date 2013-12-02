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

//time helper function
void DbHelper::addFeedIndex(const FeedIndexPtr& itemPtr) {
  if (!itemPtr) {
    return ;
  }
  Statement sql;
  sql << "INSERT INTO " << getIndexTab(itemPtr->feed)
      << " (feed,source,psource,small_type,big_type,ptype,actor,time,mini_merge,news_merge)"
      << " VALUES (" << itemPtr->feed << "," << itemPtr->source << ","
      << itemPtr->psource << ", " << itemPtr->stype << ","
      << int(itemPtr->stype / 100) * 100 << "," << itemPtr->ptype << ","
      << itemPtr->actor << ",from_unixtime(" << itemPtr->time << "),"
      << itemPtr->miniMerge << "," << itemPtr->newsMerge << ")";
   try{
     QueryRunner(DB_INSTANCE, CDbWServer, getIndexTab(itemPtr->feed)).store(sql);
   } catch ( mysqlpp::Exception & e) {
     MCE_WARN("DbHelper::addFeedIndex --> QueryRunner mysqlpp Exception err:"<<e.what());
   } catch (std::exception &e ) {
     MCE_WARN("DbHelper::addFeedIndex --> QueryRunner Exception err:"<<e.what());
   } catch (...) {
     MCE_WARN("DbHelper::addFeedIndex --> QueryRunner unkown exception");
   }
}

void DbHelper::removeFeedIndex(const MyUtil::LongSeq& feeds) {
	MCE_INFO("DbHelper::removeFeedIndex --> feeds size = " << feeds.size());
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
 //   MCE_INFO("TEST: "<<sql.str());
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
    MCE_WARN("DbHelper::addFeedIndexBysource --> err:"<<e.what());
  }
  gettimeofday(&t2,NULL);
  float tc =(t2.tv_usec - t1.tv_usec)*1.0 /1000  + (t2.tv_sec -  t1.tv_sec)*1000;
  MCE_INFO("DbHelper::addFeedIndexBysource --> time cost:"<<tc);
}


void DbHelper::addFeedContent(const FeedSeedPtr& seed) {
  if (!seed) {
    return ;
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
  try {
    QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(seed->feed)).store(sql);
  } catch ( mysqlpp::Exception & e) {
    MCE_WARN("DbHelper::addFeedContent --> QueryRunner mysqlpp Exception err:"<<e.what());
  } catch (std::exception &e ) {
    MCE_WARN("DbHelper::addFeedContent --> QueryRunner Exception err:"<<e.what());
  } catch (...) {
    MCE_WARN("DbHelper::addFeedContent --> QueryRunner unkown exception");
  }
}

void DbHelper::addZoneFeedContent(const FeedSeedPtr& seed) {
  if (!seed) {
    return ;
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
  try{
    QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(seed->feed)).store(sql);
  } catch ( mysqlpp::Exception & e) {
    MCE_WARN("DbHelper::addFeedZoneContent --> QueryRunner mysqlpp Exception err:"<<e.what());
  } catch (std::exception &e ) {
    MCE_WARN("DbHelper::addFeedZoneContent --> QueryRunner Exception err:"<<e.what());
  } catch (...) {
    MCE_WARN("DbHelper::addFeedZoneContent --> QueryRunner unkown exception");
  }
}

//Ìæ»»XML
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
    //ostringstream sql;
    sql << "update " << getContentTab(it->first) << " set xml = " << mysqlpp::quote  << xml << " where feed in (";
    for (size_t i = 0; i < it->second.size(); ++i) {
      if (i) {
        sql << ",";
      }
      sql << it->second.at(i);
    }
    sql << ")";
    try {
      QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(it->first)).store(sql);
    } catch (mysqlpp::Exception & e) {
      MCE_WARN("DbHelper::RelplaceXML --> QueryRunner mysqlpp Exception err:"<<e.what());
    } catch (std::exception & e) {
      MCE_WARN("DbHelper::ReplaceXML --> QueryRunner Exception err:"<<e.what());
    } catch ( ...) {
      MCE_WARN("DbHelper::ReplaceXML --> QueryRunner unkwon exception!");
    }
    //MCE_INFO("TEST: "<<sql.str());
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
      //ostringstream sql;
		  sql << "update " << getZoneContentTab(it->first,ib->first) << " set xml = " << mysqlpp::quote  << xml << " where feed in (";
		  for (size_t i = 0; i < ib->second.size(); ++i) {
			  if (i) {
				  sql << ",";
			  }
			  sql << ib->second.at(i);
		  }
		  sql << ")";
      try {
  		  QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(it->first,ib->first)).store(sql);
      } catch (mysqlpp::Exception & e ) {
        MCE_WARN("DbHelper::ReplaceZoneXML --> QueryRunner mysqlpp Exception err:"<<e.what());
      } catch (std::exception & e) {
        MCE_WARN("DbHelper::RelaceZoneXML --> QueryRunner Exception err:"<<e.what());
      } catch (...) {
        MCE_WARN("DbHelper::ReplaceZoneXML -->QueryRunner  unkown exception");
      }
     // MCE_INFO("TEST: "<<sql.str());
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
   // ostringstream sql;
    sql << "delete from " << getContentTab(it->first) << " where feed in (";
    for (size_t i = 0; i < it->second.size(); ++i) {
      if (i) {
        sql << ",";
      }
      sql << it->second.at(i);
    }
    sql << ")";
    //MCE_INFO("TEST: "<<sql.str());
    try {
    QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(it->first)).store(sql);
    } catch(mysqlpp::Exception & e) {
      MCE_WARN("DbHelper::removeFeedContent --> QueryRunner mysqlpp Exception err:"<<e.what()); 
    } catch (std::exception & e ) {
      MCE_WARN("DbHelper::removeFeedContent --> QueryRunner mysqlpp Exception err:"<<e.what());
    } catch (...) {
      MCE_WARN("DbHelper::removeFeedContent --> QueryRunner unkown exception!");
    }
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
    //ostringstream sql;
		  sql << "delete from " << getZoneContentTab(it->first, ib->first) << " where feed in (";
		  for (size_t i = 0; i < ib->second.size(); ++i) {
			  if (i) {
				  sql << ",";
			  }
			  sql << ib->second.at(i);
		  }
		  sql << ")";
     // MCE_INFO("TEST: "<<sql.str());
     try{
		  QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(it->first, ib->first)).store(sql);
     } catch (mysqlpp::Exception & e) {
       MCE_WARN("DbHelper::removeFeedZoneContent --> QueryRunner mysqlpp Exception err:"<<e.what()); 
     } catch (std::exception & e) {
       MCE_WARN("DbHelper::removeFeedZoneContent --> QueryRunner  Exception err:"<<e.what());
     } catch (...) {
       MCE_WARN("DbHelper::removeFeedZoneContent --> QueryRunner unkown exception !");
     }

	  }
  }
  MCE_INFO("DbHelper::removeFeedZoneContent --> cost:" << ts.getTime());
}

void DbHelper::addFeedMini(const FeedSeedPtr& seed ) {
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
  float step1 = ts.getTime(true);
  if ((seed->type & 0x30000000) >> 28 == FeedMergeReplace) {
    Statement sql;
    sql << " delete from " << miniTable << " where user=" << seed->actor<<" AND  mini_merge="
        << seed->miniMerge << " AND  small_type=" << int(seed->type & 0xffff);
    QueryRunner(miniDB, CDbWServer, miniTable).store(sql);
  }
  float step2 = ts.getTime(true);
  //for 1205( life-event feed),set wide screen
  int temp_type=seed->type;
  if( type2stype(temp_type)==1205 )
  {
      temp_type=makeWide(temp_type,true);
  }
  
  Statement sql;
  stringstream ss;
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
  sql<<ss.str();
  mysqlpp::SimpleResult res;
  try {
    res=QueryRunner(miniDB, CDbWServer,miniTable).execute(sql);
  } catch(mysqlpp::Exception& e) {
    MCE_WARN("DbHelper::addFeedMini --> exception!!"<<e.what());
  }
  float step3 = ts.getTime(true);
  //Update TimeLine
  try {
      timeline::TimeLineFeedReplicaAdapter::instance().AddFeedMiniNotify(seed,temp_type,temp_time);
  } catch (const ::Ice::Exception &e) {
     MCE_WARN("DbHelper::addFeedMini --> exception TimeLineFeed.AddFeedMiniNotify:"<<seed->actor<<", "<<e.what());
  }
  float step4 = ts.getTime(true);

  MCE_DEBUG("DbHelper::addFeedMini --> caculate time :"<<step1<<" delete sql:"<<step2<<" insert sql:"<<step3<<" update timeline:"<<step4);

}
void DbHelper::addFeedSchool(int schoolid, const FeedSeedPtr& seed) {
  if (!seed) {
    return;
  }
  TimeStat ts;
  ostringstream os;
  os << "feed_mini_" << schoolid%10 ;
  if(schoolid%10!=0){
    os << "0";
  }
  if ((seed->type & 0x30000000) >> 28 == FeedMergeReplace) {
    Statement sql;
    sql << " delete from " << getSchoolTab(schoolid) << " where mini_merge="
        << seed->miniMerge << " AND  small_type=" << int(seed->type
        & 0xffff) << " AND user=" << schoolid;
    QueryRunner(DB_INSTANCE, CDbWServer, os.str()).store(sql);
  }
  
  {
    Statement sql;
    sql << "insert into " << getSchoolTab(schoolid)
        << " (user, feed, big_type,small_type,source,time,unread,mini_merge,type,actor) values ("
        << schoolid << "," << seed->feed << "," << int((seed->type
        & 0xffff) / 100) * 100 << "," << int(seed->type & 0xffff)
        << "," << seed->source << ",from_unixtime(" << seed->time << "), 1 ,"
        << seed->miniMerge << " , " << seed->type << "," << seed->actor
        << " ) ";
    QueryRunner(DB_INSTANCE, CDbWServer, os.str()).store(sql);
  }
  MCE_INFO("DbHelper::addFeedSchool --> cost:" << ts.getTime());
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
    try{
      QueryRunner(GROUP_DB_INSTANCE, CDbWServer, getGroupTab(groupid)).store(sql);
    } catch (mysqlpp::Exception & e) {
      MCE_WARN("DbHelper::addFeedGroup --> QueryRunner mysqlpp Exception err:"<<e.what());
    } 

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
    try{
      QueryRunner(GROUP_DB_INSTANCE, CDbWServer, getGroupTab(groupid)).store(sql);
    } catch (mysqlpp::Exception & e) {
      MCE_WARN("DbHelper::addFeedGroup -->QueryRunner mysqlpp Exception err:"<<e.what());
    } catch (std::exception & e) {
      MCE_WARN("DbHelper::addFeedGroup -->QueryRunner Exception err:"<<e.what());
    } catch (...) {
      MCE_WARN("DbHelper::addFeedGroup -->QueryRunner Exception err");
    }
  }
}

void DbHelper::removeFeedMini(int user, int stype, Ice::Long miniMerge) {
  //without test
  TimeStat ts;
  Statement sql;
  //ostringstream sql;

  //we don't know the year of feed, so delete it from mini & mini_bc
  /*
  sql << "delete from " << getMiniTab(user,true) << " where user=" << user
    << " and small_type=" << stype << " and mini_merge=" << miniMerge;
  try{
    QueryRunner(getMiniDB(true), CDbWServer, getMiniTab(user,true)).execute(sql);
  } catch ( mysqlpp::Exception & e) {
    MCE_WARN("DbHelper::removeFeedMini --> QueryRunner mysqlpp Exception err:"<<e.what());
  } catch ( std::exception &e) {
    MCE_WARN("DbHelper::removeFeedMini --> QueryRunner Exception err:"<<e.what());
  } catch ( ...) {
    MCE_WARN("DbHelper::removeFeedMini --> QueryRunner unkown excetion!");
  }
  sql.clear();
 // MCE_INFO("TEST: "<<sql.str());
   // sql.str("");
  sql << "delete from " << getMiniTab(user,false) << " where user=" << user
    << " and small_type=" << stype << " and mini_merge=" << miniMerge;
  try {
    QueryRunner(getMiniDB(false), CDbWServer, getMiniTab(user,false)).execute(sql);
  } catch (mysqlpp::Exception & e) {
    MCE_WARN("DbHelper::removeFeedMini --> QueryRunner mysqlpp Exception err:"<<e.what());
  } catch (std::exception & e) {
    MCE_WARN("DbHelper::removeFeedMini --> QueryRunner Exception err:"<<e.what());
  } catch (...) {
    MCE_WARN("DbHelper::removeFeedMini --> QueryRunner unkown exception!");
  }
  //MCE_INFO("TEST: "<<sql.str());
  */
  MCE_INFO("DbHelper::removeFeedMini --> Notify timelinefeed to do removeFeedMini, user:"<<user<<" stype:"<<stype<<" merge:"<<miniMerge);
  try {
      timeline::TimeLineFeedReplicaAdapter::instance().SetReadMini(user,-1,stype,miniMerge);
  } catch (std::exception & e) {

      MCE_WARN("(DbHelper::removeFeedMini --> call timeline->SetReadMini exception:"<<e.what());

      //we don't know the year of feed, so delete it from mini & mini_bc
      Statement sql;
      sql << "delete from " << getMiniTab(user,true) << " where user=" << user
        << " and small_type=" << stype << " and mini_merge=" << miniMerge;
      try{
          QueryRunner(getMiniDB(true), CDbWServer, getMiniTab(user,true)).execute(sql);
      } catch ( mysqlpp::Exception & e) {
          MCE_WARN("DbHelper::removeFeedMini --> QueryRunner mysqlpp Exception err:"<<e.what());
      } catch ( std::exception &e) {
          MCE_WARN("DbHelper::removeFeedMini --> QueryRunner Exception err:"<<e.what());
      } catch ( ...) {
          MCE_WARN("DbHelper::removeFeedMini --> QueryRunner unkown excetion!");
      }
      sql.clear();
 // MCE_INFO("TEST: "<<sql.str());
   // sql.str("");
      sql << "delete from " << getMiniTab(user,false) << " where user=" << user
        << " and small_type=" << stype << " and mini_merge=" << miniMerge;
      try {
        QueryRunner(getMiniDB(false), CDbWServer, getMiniTab(user,false)).execute(sql);
      } catch (mysqlpp::Exception & e) {
        MCE_WARN("DbHelper::removeFeedMini --> QueryRunner mysqlpp Exception err:"<<e.what());
      } catch (std::exception & e) {
        MCE_WARN("DbHelper::removeFeedMini --> QueryRunner Exception err:"<<e.what());
      } catch (...) {
        MCE_WARN("DbHelper::removeFeedMini --> QueryRunner unkown exception!");
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
    //ostringstream sql; 
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
    try {
      QueryRunner(DB_INSTANCE, CDbWServer, getContentTab(iter->first)).schedule(sql);
    } catch (mysqlpp::Exception & e) {
      MCE_WARN("DbHelper::UpdateReply --> QueryRunner mysqlpp Exception err:"<<e.what());
    } catch (std::exception & e) {
      MCE_WARN("DbHelper::UpdateReply --> QueryRunner Exception err:"<<e.what());
    } catch (...) {
      MCE_WARN("DbHelper::UpdateReply --> QueryRunner unkown exception!");
    }
    //MCE_INFO("TEST: "<<sql.str());
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
      //ostringstream sql;
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
    try{
       QueryRunner(DB_ZONE_CONTENT_INSTANCE, CDbWServer, getZoneContentTab(iter->first,ib->first)).schedule(sql);
    } catch (mysqlpp::Exception & e) {
      MCE_WARN("DbHelper::UpdateZoneReply --> QueryRunner mysqlpp Exception err:"<<e.what());
    } catch (std::exception & e) {
      MCE_WARN("DbHelper::UpdateZoneReply --> QueryRunner Exception err:"<<e.what());
    } catch (...) {
      MCE_WARN("DbHelper::UpdateZoneReply --> QueryRunner unkown exception!");
    }

      //MCE_INFO("TEST: "<<sql.str());
    } 
  }
}

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




