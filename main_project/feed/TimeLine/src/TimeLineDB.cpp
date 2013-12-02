///////////////////////////////////////////////////////////
//
//       Title: DbHelper.cpp
//       Athor: Feed
//  CreateTime: 08/28/2012
//       Email: zhi.lin@renren-inc.com
// Description:  
//           1. implement of TimeLineFeedI class 's DB operation
//           

#include "preheader.h"
#include "TimeLineFeedI.h"
#include "TimeLineInternal.h"
#include "Task.h"

using namespace boost;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace xce::serverstate;
using namespace xce::feed::timeline;
using namespace xce::subtle;
using namespace Ice;

#define foreach BOOST_FOREACH

#define CLEAR_SS() do{ ss.clear();ss.str("");}while(0)
#define CLEAR_SS_SQL() do{ ss.clear();ss.str("");sql.clear();}while(0)

namespace xce { 
namespace feed { 
namespace timeline { 


/// \brief function to record DB operation timecost \ref TimeHelper
void TmlDBTimeHook(TimeLineFeedI * feedI, TimeHelper * th)
{
    long elapse = th->elapsed();
    if(elapse<50*1000) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[ktmlDB0_50ms]),1);
    } else if(elapse < 300*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[ktmlDB50_300ms]),1);
    } else {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[ktmlDB300ms]),1);
    }
}
void MiniDBTimeHook(TimeLineFeedI * feedI, TimeHelper * th)
{
    long elapse = th->elapsed();
    if(elapse<50*1000) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kminiDB0_50ms]),1);
    } else if(elapse < 300*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kminiDB50_300ms]),1);
    } else {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kminiDB300ms]),1);
    }
}
bool TimeLineFeedI::loadSetting(int index)
{
    FEEDI_DEBUG(0, " load setting to index:"<<index);
    Statement sql;
    stringstream ss;
    map<string,string> config;
    string json;
    string service_name = "TimeLineFeed";
    service_name += char('0' + mod_);
    try {
        mysqlpp::StoreQueryResult store_res;
        ss<<"select * from descriptor_property where service_id=124 and `key`='"<<service_name<<"'";
        //FEEDI_DEBUG(0," loadSetting sql:"<<ss.str());
        sql<<ss.str();
        store_res=QueryRunner("xce_descriptor",CDbWServer,"descriptor_property").store(sql);
        if(store_res.num_rows()!=0) { //we found config for this service
            FEEDI_DEBUG(0," found setting json for this service.");
	    json=(string)store_res[0]["value"];
            if(setting_[index].loadJson(json)) {
		return true;
            }
            FEEDI_DEBUG(0,"bad service's json setting of this mod in DB. load failed.");
        }
        CLEAR_SS_SQL();
        ss<<"select * from descriptor_property where service_id=124 and `key`='default'";
        sql<<ss.str();
        //FEEDI_DEBUG(0," loadSetting sql:"<<ss.str());
        store_res=QueryRunner("xce_descriptor",CDbWServer,"descriptor_property").store(sql);
        if(store_res.num_rows()!=0) {
            json=(string)store_res[0]["value"];
            if(setting_[index].loadJson(json)) {
                FEEDI_INFO(0,"Load successful !!");
		return true;
            }
            FEEDI_INFO(0," bad json in default config in DB, load failed.");
            return false;
        }
	else {
            FEEDI_INFO(0," Can't find default configuration in DB, load failed.");
            return false;
        }
    }catch (mysqlpp::Exception &e) {
        FEEDI_WARN(0," mysqlException: "<<e.what());
        NoBarrier_AtomicIncrement(&(stat_.values_[kotherDBExcepn]),1);
        return false;
    }
}
bool TimeLineFeedI::reloadSetting()
{
    //if DB failed or a value is of wrong format, we will give up reload
    //if when we write the ilde setting and other thread switch the idx, server might crash, so we need a mutex protect idx-switch
    IceUtil::Mutex::Lock lock(setting_reload_mutex_); 
    if(!loadSetting(setting_idx_ilde()) ) 
        return false;
    //change idx
    switch_setting_idx();
    return true;
}


/// \brief update time of a feed in DB, maybe move feed between mini & mini_bc
/// \param[in] uid id of the user
/// \param[in] old_year year of the feed
/// \param[in] new_year year of the new date
/// \param[in] new_time time_t of the new date
/// \param[in] feed id of the feed
/// \return  **true** if succeed, else **false**
bool TimeLineFeedI::moveFeedDB(int uid,int old_year,int new_year,time_t new_time,::Ice::Long feed)
{
    Statement sql;
    stringstream ss;
    string old_table; string new_table;
    mysqlpp::SimpleResult simple_res;  mysqlpp::StoreQueryResult store_res;
    GetFeedMiniTable(uid,old_table,old_year>2009);
    GetFeedMiniTable(uid,new_table,new_year>2009);
    const char * old_feed_db=GetFeedDBInstance(old_year>2009);
    const char * new_feed_db=GetFeedDBInstance(new_year>2009);
    
    int temp_year,temp_month,temp_day,temp_hour,temp_min,temp_sec;
    getDayTime(new_time,temp_year,temp_month,temp_day,temp_hour,temp_min,temp_sec);    
    
    if(old_table==new_table) {
        CLEAR_SS();ss<<" moveFeedDB,"<<uid<<(",update sql ");
        TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
        CLEAR_SS_SQL();
        ss<<"update "<<old_table<<" set time = '"<<temp_year<<"-"<<temp_month<<"-"<<temp_day<<" "<<temp_hour<<":"
        <<temp_min<<":"<<temp_sec<<"' where user="<<uid<<" and feed="<<feed;
        FEEDI_DEBUG(uid," update sql:"<<ss.str());
        sql<<ss.str();
        try {
            simple_res = QueryRunner(old_feed_db, CDbWServer,old_table).execute(sql);
        }catch(mysqlpp::Exception& e) {
            FEEDI_WARN(uid," mysqlException:"<<e.what());
            NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
            return false; //if DB failed, we won't modify timeline
        }
        if(simple_res.rows()==0) {
            FEEDI_INFO(uid," update failed.uid: "<<uid);
            //sometimes return failed, when it actually successed.
            //
        }
    } else {
        int big_type,small_type,unread,type,actor;Ice::Long source,mini_merge;
        //read feed ,remove feed in old_table, add it to new_table;
        {
            CLEAR_SS();ss<<" moveFeedDB,"<<uid<<(",select sql ");
            TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
            CLEAR_SS_SQL();
            ss<<"select * from "<<old_table<<" where user= "<<uid<<" and feed= "<<feed;
            FEEDI_DEBUG(uid," sql before delete:"<<ss.str());
            sql<<ss.str();
            try {
                store_res = QueryRunner(old_feed_db, CDbRServer, old_table).store(sql);
            }catch(mysqlpp::Exception & e) {
                FEEDI_WARN(uid," mysqlException: "<<e.what());
                NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
                return false;
            }
            if(store_res.num_rows()==0) {
                FEEDI_INFO(uid, " cann't find feed:"<<feed);
                return false;//
            }
            // 0      1       2         3           4       5       6         7         8      9
            //user ,feed, big_type, small_type,   source, time,  unread,  mnini_merge, type, actor
            big_type=(int)store_res.at(0)[2];
            small_type=(int)store_res.at(0)[3];
            source=(Ice::Long)store_res.at(0)[4];
            unread=(int)store_res.at(0)[6];
            mini_merge=(Ice::Long)store_res.at(0)[7];
            type=(int)store_res.at(0)[8];
            actor=(int)store_res.at(0)[9];
            FEEDI_DEBUG(uid," big:"<<big_type<<" stype "<<small_type<<" source:"
            <<source<<" unread"<<unread<<" merge:"<<mini_merge<<" type:"<<type<<" actor:"<<actor);       
        }
        //remove
        {
            CLEAR_SS();ss<<" moveFeedDB,"<<uid<<(",delete sql ");
            TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
            CLEAR_SS_SQL();
            ss<<"delete from "<<old_table<<" where user = "<<uid<<" and feed= "<<feed;
            FEEDI_DEBUG(uid," delete sql: "<<ss.str());
            sql<<ss.str();
            try {
                simple_res =QueryRunner(old_feed_db,CDbWServer, old_table).execute(sql);
            }catch(mysqlpp::Exception &e) {
                FEEDI_WARN(uid," mysqlException: "<<e.what());
                NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
                //go on
            }
        }
        {
            //insert
            CLEAR_SS();ss<<" moveFeedDB,"<<uid<<(",insert sql ");
            TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
            CLEAR_SS_SQL();
            ss<<"insert into "<<new_table<<" (user, feed, big_type, small_type, source, time, unread, mini_merge, type, actor) values ("
            <<uid<<","<<feed<<" ,"<<big_type<<","<<small_type<<","<<source<<",'"<<temp_year<<"-"<<temp_month<<"-"<<temp_day<<" "<<temp_hour<<":"
            <<temp_min<<":"<<temp_sec<<"' ," <<unread<<","<<mini_merge
            <<","<<type<<","<<actor<<")";
            FEEDI_DEBUG(uid," insert sql: "<<ss.str());        
            sql<<ss.str();
            try {
                simple_res =QueryRunner(new_feed_db,CDbWServer, new_table).execute(sql);
            } catch(mysqlpp::Exception &e) {
                FEEDI_WARN(uid, "  mysqlException: "<<e.what());
                NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
                //go on
                return false;
            }
        }
    }
    return true;
}
/// \brief set wide style of a feed in DB
/// \param[in] uid the id of the user
/// \param[in] feed_year year of the feed
/// \param[in] feed id of the feed
/// \param[in] old_type previous type value
/// \param[in] wide wide or narrow style
/// \return **true** if succeed, else **false**
bool TimeLineFeedI::setFeedWideDB(int uid,int feed_year,::Ice::Long feed,int old_type,bool wide)
{
    stringstream ss;
    ss<<"setFeedWideDB,"<<uid<<(",update sql ");
    TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
    
    Statement sql;
    string mini_table;
    GetFeedMiniTable(uid,mini_table,feed_year>2009);
    const char * feed_db=GetFeedDBInstance(feed_year>2009);
    
    mysqlpp::SimpleResult simple_res;
    FEEDI_DEBUG(uid," old type:"<<bitset<32>(old_type)<<"--"<<old_type);
    FEEDI_DEBUG(uid," new type:"<<bitset<32>(makeWide(old_type,wide))<<"--"<<makeWide(old_type,wide));
    CLEAR_SS();
    ss<< "update "<< mini_table <<" set type = "<<makeWide(old_type,wide)<<" where user="<<uid<<" and feed="<<feed;
    FEEDI_DEBUG(uid, " sql"<<ss.str());
    sql<<ss.str();
    try {
        simple_res=QueryRunner(feed_db,CDbWServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        FEEDI_WARN(uid," mysqlException! "<<e.what());
        NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
        return false;
    }
    if(simple_res.rows()==0) {
        FEEDI_DEBUG(uid, " update DB failed.");
    }
    return true;
}

/// \brief delete a feed in DB
/// \param[in] uid id of the user.
/// \param[in] feed_year year of the feed
/// \param[in] feed id of the feed
/// \return  **true** if succeed, **false** if feed doesn't exist in database or database exception
bool TimeLineFeedI::deleteFeedByIdDB(int uid,int feed_year,::Ice::Long feed)
{
    stringstream ss;
    ss<<" deleteFeedByIdDB,"<<uid<<(",delete sql ");
    TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));

    ///execute sql: delete from (table) where user=(uid) and feed=(feed)
    Statement sql;
    mysqlpp::StoreQueryResult store_res;mysqlpp::SimpleResult simple_res;
    string table; GetFeedMiniTable(uid,table,feed_year>2009);
    const char * feed_db=GetFeedDBInstance(feed_year>2009);
    
    CLEAR_SS();
    ss<<"delete from "<<table<<" where user="<<uid<<" and feed="<<feed;
    sql<<ss.str();
    FEEDI_DEBUG(uid, " sql:"<<ss.str());
    try {
        simple_res = QueryRunner(feed_db,CDbWServer,table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        FEEDI_WARN(uid," mysqlException! "<<e.what());
        NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
        return false;
    }
    if(simple_res.rows()==0) {
        FEEDI_INFO(uid," delete feed failed");
        return false;
    }
    return true;
}


/// \brief delete a feed in DB
/// \param[in] uid id of the user.
/// \param[in] feed_year year of the feed
/// \param[in] stype stype of the feed
/// \param[in] merge merge id of the feed
/// \return  **true** if succeed, **false** if feed doesn't exist in database or database exception
bool TimeLineFeedI::deleteFeedByMergeDB(int uid,int feed_year,int stype,::Ice::Long merge)
{
    stringstream ss;
    ss<<" deleteFeedByMergeDB,"<<uid<<(",delete sql ");
    TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
    ///execute sql: delete from (table) where user=(uid) and small_type=(stype) and mini_merge=(merge)
    Statement sql;
    
    mysqlpp::StoreQueryResult store_res;mysqlpp::SimpleResult simple_res;
    string table; GetFeedMiniTable(uid,table,feed_year>2009);
    const char * feed_db=GetFeedDBInstance(feed_year>2009);
    
    CLEAR_SS();
    ss<<"delete from "<<table<<" where user="<<uid<<" and small_type="<<stype<<" and mini_merge="<<merge;
    sql<<ss.str();
    FEEDI_DEBUG(uid, " sql:"<<ss.str());
    try {
        simple_res = QueryRunner(feed_db,CDbWServer,table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        FEEDI_WARN(uid," mysqlException! "<<e.what());
        NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
        return false;
    }
    if(simple_res.rows()==0) {
        FEEDI_INFO(uid," delete feed failed");
        return false;
    }
    return true;
}

/// \brief load a feed and all other feeds with same month
/// \param[in] ptr cache pointer of the user
/// \param[in] uid id of the user
/// \param[in] after2009 if the month of feed is later than 2009/12
/// \param[in] feed id of the feed
/// \param[out] type store type of the feed
/// \param[out] time store time of the feed
/// \param[out] merge store merge id of the feed
/// \return **true** if succeed, **false** if the feed doesn't exist or database exception
bool TimeLineFeedI::loadFeed(TimeLineObjectPtr & ptr,int uid,bool after2009,::Ice::Long feed,int &type,time_t & time,Ice::Long& merge)
{
    stringstream ss;
    {//for TimeHelper th  scope.
        ss<<" loadFeed,"<<uid<<(",select sql ");
        TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
        /// 1 .  query database for time,type and merge id of the feed.
        FEEDI_DEBUG(uid," feed:"<<feed);
        string mini_table;GetFeedMiniTable(uid,mini_table,after2009);
        const char *feed_db=GetFeedDBInstance(after2009);
        Statement sql;
        mysqlpp::StoreQueryResult store_res;
        const int kTimeIdx=0;
        const int kTypeIdx=1;
        const int kMergeIdx=2;
        sql << "select time,type,mini_merge from "<<mini_table<<" where user="<<uid<<" and feed="<<feed;
        try {
            store_res=QueryRunner(feed_db,CDbRServer,mini_table).store(sql);
        } catch (mysqlpp::Exception& e ) {
            FEEDI_WARN(uid, " mysqlException! "<<e.what());
            NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
            return false;
        }
        if( store_res.num_rows()==0) {
            FEEDI_DEBUG(uid, " no this feed.");
            return false;
        }
    
        time = (time_t) mysqlpp::DateTime( (store_res.at(0)[kTimeIdx]) );
        type= (int)(store_res.at(0)[kTypeIdx]);
        merge=(Ice::Long)(store_res.at(0)[kMergeIdx]);
    }
    //2nd. make year-month
    int year,month,day,year_next,month_next;
    getYearMonthDay(time,year,month,day);
    getNextMonth(year,month,year_next,month_next);
    //3rd. load month
    FEEDI_DEBUG(uid," load month:"<<year<<"-"<<month);
    /// 2 .  get month of the feed, then call \ref loadFeedInRange to load the month.
    loadFeedInRange(ptr,uid,year,month,1,0,0,0,year_next,month_next,1,0,0,0);
    return true;
}


/// \brief load feeds in a time range,range shouldn't accross 2010/1/1
/// \param[in] ptr cache pointer of the user
/// \param[in] uid id of the user
/// \param[in] start_year year of start
/// \param[in] start_month month of start
/// \param[in] start_day day of start
/// \param[in] start_hour hour of start
/// \param[in] start_min minute of start
/// \param[in] start_sec second of start
/// \param[in] end_year year of end
/// \param[in] end_month month of end
/// \param[in] end_day day of end
/// \param[in] end_hour hour of end
/// \param[in] end_min minute of end
/// \param[in] end_sec second of end
bool TimeLineFeedI::loadFeedInRange(TimeLineObjectPtr & ptr,int uid,int start_year,int start_month,int start_day,int start_hour,int start_min,int start_sec,int end_year,int end_month,int end_day,int end_hour,int end_min,int end_sec)
{
    stringstream ss;
    ss<<" loadFeedInRange,"<<uid<<(",select sql ");
    TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
    /// 1 .  execute sql: select feed,time,type,mini_merge from (table) where user=(uid) and time>='**-**-** **:**:**' and time<'XX-XX-XX XX:XX:XX' 
    string mini_table;
    GetFeedMiniTable(uid,mini_table,start_year>2009);
    const char *feed_db=GetFeedDBInstance(start_year>2009);
    //1st: read feed,type,time from mini_table         
    
    Statement sql;
    const int kFeedIdx=0;
    const int kTimeIdx=1;
    const int kTypeIdx=2;
    const int kMergeIdx=3;
    CLEAR_SS();
    ss<<"select feed,time,type,mini_merge from "<<mini_table<<" where user="<<uid
      <<" and time >='"<<start_year<<"-"<<start_month<<"-"<<start_day<<" "
      <<start_hour<<":"<<start_min<<":"<<start_sec<<"' and time < '"
      <<end_year<<"-"<<end_month<<"-"<<end_day<<" "<<end_hour<<":"<<end_min<<":"<<end_sec<<"'";
    FEEDI_DEBUG(uid," sql: "<<ss.str()<<"  db instance:  "<<feed_db);
    sql<<ss.str();mysqlpp::StoreQueryResult store_res;
    try {
        store_res=QueryRunner(feed_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e ) {
        FEEDI_WARN(uid, " mysqlException! "<<e.what());
        NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
        return false;
    }
    if(!store_res) {
        FEEDI_DEBUG(uid," result error.");
        return false;
    }
    ::Ice::Long temp_feed;time_t temp_time;int temp_type;::Ice::Long temp_merge;
    //time_index_ref index = ptr->feedMap_.get<MIC_TIME_INDEX>();
    FEEDI_DEBUG(uid," data_nums:"<<store_res.num_rows());
    /// 2 .  then insert feeds into cache
    for(size_t i=0;i<store_res.num_rows();i++ ) {
        temp_feed = (::Ice::Long) store_res[i][kFeedIdx];
        temp_time = (time_t) mysqlpp::DateTime(store_res[i][kTimeIdx]); 
        temp_type =(int) store_res[i][kTypeIdx];
        temp_merge = (::Ice::Long) store_res[i][kMergeIdx];
        //2nd: insert to cache
        //insertCache(ptr,uid,temp_feed,temp_time,temp_type,type2stype(temp_type),temp_merge);
        insertCache(ptr,uid,temp_feed,temp_time,temp_type);
    }
    return true;
}
/// \brief query timeline from database
/// \param[in] uid id of the user
/// \param[out] result timeline string
/// \return **true** if succeed, **false** if database exception.
bool TimeLineFeedI::readTimeLineDB(int uid, string & result)
{
    stringstream ss;
    ss<<" readTimeLineDB,"<<uid<<(",select sql ");
    TimeHelper th(ss.str());th.function(bind(TmlDBTimeHook,this,_1));
    string table;
    GetTimeLineTable(uid,table);
    Statement sql;
    const int kTimeLineIdx=0;
    CLEAR_SS();
    ss<< "select timeline from "<< table << " where uid = "<<uid;
    FEEDI_DEBUG(uid, " sql:"<<ss.str());
    sql<<ss.str();
    mysqlpp::StoreQueryResult res;
    try{
        res = QueryRunner(kTimeLineDB,CDbRServer,table).store(sql);
    }
    catch (mysqlpp::Exception& e) {
        FEEDI_WARN(uid," mysqlException:"<<e.what());
        NoBarrier_AtomicIncrement(&(stat_.values_[ktmlDBExcepn]),1);
        return false;
    }
    if(!res ) {
        FEEDI_DEBUG(uid," mysql error"<<uid);
        return false;
    }
    else if (res.num_rows()==0) {
        result="";
        FEEDI_DEBUG(uid," no timeline data");
    }
    else {
        result=(mysqlpp::Null<string, mysqlpp::NullIsBlank>) (res.at(0)[kTimeLineIdx]);
    }
    return true;
}
/// \brief query timeline from database
/// \param[in] uid id of the user
/// \param[in] timeline result timeline string
/// \return **true** if succeed, **false** if database exception.
bool TimeLineFeedI::writeTimeLineDB(int uid,const string & timeline)
{
    stringstream ss;
    ss<<" writeTimeLineDB,"<<uid<<(",replace sql ");
    TimeHelper th(ss.str());th.function(bind(TmlDBTimeHook,this,_1));

    string table; GetTimeLineTable(uid,table);
    Statement sql;
    CLEAR_SS();
    ss<< "replace into "<<table<<" (uid,timeline) VALUES ('"<<uid<<"','"<<timeline<<"' )";
    FEEDI_DEBUG(uid, " sql: "<<ss.str());
    sql<<ss.str();
    mysqlpp::SimpleResult result;
    try{
        result= QueryRunner(kTimeLineDB,CDbWServer,table).execute(sql);
    }
    catch (mysqlpp::Exception& e) {
        FEEDI_WARN(uid," mysqlException:"<<e.what());
        NoBarrier_AtomicIncrement(&(stat_.values_[ktmlDBExcepn]),1);
        return false;
    } 
    if(result.rows()==0 ) {
        FEEDI_DEBUG(uid,"replace failed");
        return false;
    }
    return true;
}

/// \brief rebuild timeline string for a user
/// \param[in] ptr cache pointer of the user
/// \param[in] uid id of the user
/// \param[out] timeline store the new timeline
/// \return true if succeed, false if database exception.
bool TimeLineFeedI::rebuildTimeLine(TimeLineObjectPtr& ptr,int uid,std::string & timeline)
{
    YearMonthMap temp_map;
    time_t temp_time;
    int temp_year , temp_month, temp_day;
    string mini_table;string bc_table;
    GetFeedMiniTable(uid,mini_table,true);
    GetFeedMiniTable(uid,bc_table,false);
    const char * mini_db=GetFeedDBInstance(true);
    const char * bc_db=GetFeedDBInstance(false);
    Statement sql;
    mysqlpp::StoreQueryResult store_res;
    stringstream ss;
    {//for TimeHelper th scope
        ss<<" rebuildTimeLine,"<<uid<<(",select sql >2009 ");
        TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
        
        CLEAR_SS();
        // >2009
        ss<<"select time from "<<mini_table<<" where user="<<uid;
        sql<<ss.str();
        try {
            store_res=QueryRunner(mini_db,CDbRServer,mini_table).store(sql);
        } catch (mysqlpp::Exception& e) {
            FEEDI_WARN(uid," mysql exception :"<<e.what()<<endl);
            NoBarrier_AtomicIncrement(&(stat_.values_[ktmlDBExcepn]),1);
            return false;
        } 
        for(size_t i=0;i<store_res.num_rows();++i) {
            temp_time = (time_t) mysqlpp::DateTime(store_res[i][0]);
            getYearMonthDay(temp_time,temp_year,temp_month,temp_day);
            if(temp_year>2009) {//
                temp_map[temp_year].insert(temp_month);
            } else {
                //FEEDI_DEBUG(uid," a feed <=2009 in mini !"); 
            }
        }
    }
    {
        CLEAR_SS();ss<<" rebuildTimeLine,"<<uid<<(",select sql <=2009 ");
        TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
        // <=2009
        CLEAR_SS_SQL();
        ss<<"select time from "<<bc_table<<" where user="<<uid;
        sql<<ss.str();
        try {
            store_res=QueryRunner(bc_db,CDbRServer,bc_table).store(sql);
        } catch(mysqlpp::Exception & e) {
            FEEDI_WARN(uid," mysql exception :"<<e.what()<<endl);
            NoBarrier_AtomicIncrement(&(stat_.values_[ktmlDBExcepn]),1);
            return false;
        }
        for(size_t i=0;i<store_res.num_rows();++i) {
            temp_time = (time_t) mysqlpp::DateTime(store_res[i][0]); 
            getYearMonthDay(temp_time,temp_year,temp_month,temp_day);
            if(temp_year<=2009) {
                temp_map[temp_year].insert(temp_month);
            } else {
                FEEDI_DEBUG(uid," a feed >2009 in mini_bc !");
            }
        }
    }
    mapToTimeLine(temp_map,timeline);
    ptr->timeLine_=timeline;
    writeTimeLineDB(uid,timeline);
    return true;
}
/// \brief load all feeds of a user into cache
/// \param[in] ptr cache pointer of the user
/// \param[in] uid id of the user
/// \return return true if succeed, else false
bool TimeLineFeedI::loadAllFeeds(TimeLineObjectPtr& ptr,int uid)
{
    string mini_table;string bc_table;
    GetFeedMiniTable(uid,mini_table,true);
    GetFeedMiniTable(uid,bc_table,false);
    const char * mini_db=GetFeedDBInstance(true);
    const char * bc_db=GetFeedDBInstance(false);
    Statement sql;
    mysqlpp::StoreQueryResult store_res;

    //feed,time,type,mini_merge
    Ice::Long temp_feed; time_t temp_time; int temp_type; ::Ice::Long temp_merge;
    const int kFeedIdx=0;
    const int kTimeIdx=1;
    const int kTypeIdx=2;
    const int kMergeIdx=3;
    //Notice: there are feeds whoes year<2010 in mini_table.we should drop it
    time_t division_line=getTime(2010,1,1);
        
    stringstream ss;
    {
        ss<<" loadAllFeeds,"<<uid<<(",select sql >2009 ");
        TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
        //read >2009
        CLEAR_SS();
        ss<<"select feed,time,type,mini_merge from "<<mini_table<<" where user="<<uid;// <<" and time>"2010/1/1 0:0:0"
        sql<<ss.str();
        try {
            store_res=QueryRunner(mini_db,CDbRServer,mini_table).store(sql);
        } catch (mysqlpp::Exception& e) {
            FEEDI_WARN(uid," mysql exception :"<<e.what()<<endl);
            NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
            return false;
        }
        FEEDI_DEBUG(uid," >2009 data_nums:"<<store_res.num_rows());
        size_t i=0;
        for(i=0;i<store_res.num_rows();++i) {
            temp_feed = (::Ice::Long) store_res[i][kFeedIdx];
            temp_time = (time_t) mysqlpp::DateTime(store_res[i][kTimeIdx]); 
            temp_type =(int) store_res[i][kTypeIdx]; 
            temp_merge = (::Ice::Long) store_res[i][kMergeIdx]; 
            if( temp_time < division_line ) {
                //FEEDI_DEBUG(uid," a feed before 2010 in mini !");
                continue;
            }
            //2nd: insert to cache 
            //insertCache(ptr,uid,temp_feed,temp_time,temp_type,type2stype(temp_type),temp_merge);
            insertCache(ptr,uid,temp_feed,temp_time,temp_type);
        }
    }
    {
        CLEAR_SS();ss<<" loadAllFeeds,"<<uid<<(",select sql <=2009 ");
        TimeHelper th(ss.str());th.function(bind(MiniDBTimeHook,this,_1));
        //read <=2009
        CLEAR_SS_SQL();
        ss<<"select feed,time,type,mini_merge from "<<bc_table<<" where user="<<uid;
        sql<<ss.str();
        try {
            store_res=QueryRunner(bc_db,CDbRServer,bc_table).store(sql);
        } catch (mysqlpp::Exception& e) {
            FEEDI_WARN(uid," mysql exception :"<<e.what()<<endl);
            NoBarrier_AtomicIncrement(&(stat_.values_[kminiDBExcepn]),1);
            return false;
        }
        FEEDI_DEBUG(uid," <=2009 data_nums:"<<store_res.num_rows());
        for(size_t i=0;i<store_res.num_rows();++i ) {
            temp_feed = (::Ice::Long) store_res[i][kFeedIdx];
            temp_time = (time_t) mysqlpp::DateTime(store_res[i][kTimeIdx]); 
            temp_type =(int) store_res[i][kTypeIdx]; 
            temp_merge = (::Ice::Long) store_res[i][kMergeIdx]; 
            
            if( temp_time >= division_line ) {
                //FEEDI_DEBUG(uid," a feed after 2009 in mini_bc !");
                continue;
            }
            //2nd: insert to cache
            //insertCache(ptr,uid,temp_feed,temp_time,temp_type,type2stype(temp_type),temp_merge); 
            insertCache(ptr,uid,temp_feed,temp_time,temp_type); 
        }
    }
    return true;
}



}
}
}
