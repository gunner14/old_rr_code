
#include <sstream>
#include <ctime>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/tuple/tuple.hpp>
#include <algorithm>
#include <bitset>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/static_assert.hpp>
#include <boost/make_shared.hpp>
#include "QueryRunner.h"
#include "ConnectionPoolManager.h"
#include "ServiceI.h"
#include <iterator>
#include "RFeed.h"
#include "ServiceI.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "ObjectCache.h"
#include "QueryRunner.h"
#include "util/cpp/TimeCost.h"


using namespace boost;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace xce::serverstate; 
using namespace Ice;
using namespace xce::feed;
using namespace std;
using namespace boost;
using namespace MyUtil; 


#define foreach BOOST_FOREACH


enum work_mode{ TestDelete, FullDelete, QuickDelete } ;

work_mode mode=TestDelete;


set<int> filter_ids;


//delaration
bool unique(int uid,ostream & out,ostream & sqlscript);
  
const string kTimeLineDB="feed_timeline";
const char* kA09FeedDB="feed_db";
const char* kB09FeedDB="feed_content_db";
const unsigned int kDBCluster = 100;
const unsigned int kDBHiCluster = 1000000000;
const Ice::Long kDBDividingLine = 18000000000;

void makeTimeStr(time_t t,string & s)
{
    char buf[100];
    tm* local_time = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local_time);
    s=buf;
}
inline const char* GetFeedDBInstance(bool after_2009)
{
    if(after_2009)
        return kA09FeedDB;//kA09FeedDB; comment it for protection.
    else
        return kB09FeedDB;
}

inline void GetMiniTable(int uid, string & name, bool after_2009) 
{
    ostringstream os;
    if( after_2009 ) {
        os << "feed_mini_"; 
    } else {
        os << "feed_mini_bc_";
    }
    os<< uid % kDBCluster;
    name=os.str();
}

inline void GetIndexTable(Ice::Long feed, string & name, bool after_2009) 
{
    ostringstream os;
    if( after_2009 ) {
        os << "feed_index_"; 
    } else {
        os << "feed_index_bc_";
    }
    os<< abs(feed) % 10;
    name=os.str();
}
/*
 string getZoneContentTab(int mod, int zone) {
    ostringstream tab;
    //tab << "feed_content_" << mod << "_" << zone;
    if (zone > 0) {
                     tab << "feed_content_" << mod << "_" << zone;
                } else {
                     tab << "feed_content_bc_" << abs(mod) << "_" << abs(zone);
                }
                MCE_DEBUG("DbHelper::getZoneContentTable --> mod:" << mod << " zone:" << zone << " tab:"<< tab.str());
    return tab.str();
  }
*/
inline void GetContentTable(Ice::Long feed,string & table)
{
    //negative id in feed_content_bc,else in feed_content
    //positive id < 18 billion  :   feed_content_(id%100)
    //positive id >= 18 billion :   feed_content_(feed/1billion)_(id%100)
    //negative id : feed_content_abs(id % kDBCluster)<<"_"<<(abs( feed / 1billion) );
    stringstream ss;
    if(feed<0)
    {
        ss<<"feed_content_bc_"<<abs(feed % kDBCluster)<<"_"<<(abs(feed / kDBHiCluster)%100);
    } else {
        if(feed >= kDBDividingLine) {
            ss<<"feed_content_"<<feed%kDBCluster<<"_"<<(feed/kDBHiCluster)%100;
        } else {
            ss<<"feed_content_"<<feed%kDBCluster;
        }
    }
    table=ss.str();
}


#define CHECK_SIZE(i) \
do{\
   if(args.size()!=i) { out<<"args size error.";return false;}\
}while(0)

#define GET(pos,name) \
do {\
  ss.clear();ss.str(args.at(pos));ss>>name;if(ss.fail()) {out<<"param error"<<endl; return false;}\
}while(0)

#define CLEAR()\
do {\
  sql.clear();ss.clear();ss.str("");\
}while(0)

#define CastTo(x,y,ok) \
do{\
  ss.clear();ss.str("");ss<<x;ss>>y;if(ss.fail()) {out<<"cast failed:"<<x<<endl;ok=false;}else{ok=true;}\
}while(0)



//usage:
//   del stype
//   del_mini id, (remove user mini_feed)
//   del_all id,  (remove user mini_feed, feed_content)
//   del_content feed (remove feed , user id, in feed_content)
//

//   tool  function

//deletes
bool deleteFeedMini(int uid, Ice::Long feed, ostream & out,ostream & sqlscript)
{
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    string mini_table;
    string mini_bc_db;
    GetMiniTable(uid,mini_table,false);
    mini_bc_db=GetFeedDBInstance(false);
    if(feed>0)  {
        out<<"not bc feed."<<endl;
        return true;
    }
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and feed="<<feed;
    out<<ss.str()<<endl;
    sqlscript<<mini_bc_db<<","<<mini_table<<","<<ss.str()<<endl;
    if(mode==TestDelete) {
        
        return true;
    }
    //  unique   we need delete mini when quick mode
    //if(mode==QuickDelete) {
    //    sqlscript<<mini_bc_db<<","<<mini_table<<","<<ss.str()<<endl;
        //return true;
    //}
    sql<<ss.str();
    try{
        simple_res = QueryRunner(mini_bc_db,CDbWServer,mini_table).execute(sql);
    }
    catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
    return true;
}
bool deleteFeedContent(Ice::Long feed, ostream & out,ostream & sqlscript)
{
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    string content_table;
    string content_db;
    GetContentTable(feed,content_table);
    content_db=GetFeedDBInstance(false);
    if(feed>0) {
        out<<"not bc feed."<<endl;
        return true;
    }
    ss<<"delete from "<<content_table<<" where feed="<<feed;
    out<<ss.str()<<endl;
    sqlscript<<content_db<<","<<content_table<<","<<ss.str()<<endl;
    if(mode==TestDelete) {
        //sqlscript<<content_db<<","<<content_table<<","<<ss.str()<<endl;
        return true;
    }
    if(mode==QuickDelete) {
        //sqlscript<<content_db<<","<<content_table<<","<<ss.str()<<endl;
        return true;
    }
    sql<<ss.str();
    try{
        simple_res = QueryRunner(content_db,CDbWServer,content_table).execute(sql);
    }
    catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
    return true;
}
bool deleteFeedIndex(Ice::Long feed, ostream & out,ostream & sqlscript)
{
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    string index_table;
    string bc_db;
    GetIndexTable(feed,index_table,false);
    bc_db=GetFeedDBInstance(false);
    if(feed>0) {
        out<<"not bc feed."<<endl;
        return true;
    }
    ss<<"delete from "<<index_table<<" where feed="<<feed;
    out<<ss.str()<<endl;
    sqlscript<<bc_db<<","<<index_table<<","<<ss.str()<<endl;
    if(mode==TestDelete) {
        //sqlscript<<bc_db<<","<<index_table<<","<<ss.str()<<endl;    
        return true;
    }
    if(mode==QuickDelete) {
        //sqlscript<<bc_db<<","<<index_table<<","<<ss.str()<<endl;
        return true;
    }
    sql<<ss.str();
    try{
        simple_res = QueryRunner(bc_db,CDbWServer,index_table).execute(sql);
    }
    catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
    return true;
}
bool deleteFeedMiniStype(int uid,int stype,ostream & out,ostream & sqlscript)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and small_type="<<stype<<" and feed<0";
    out<<ss.str()<<endl;
    if(mode==TestDelete)
        return true;
    sql<<ss.str();
    try{
        simple_res = QueryRunner(mini_bc_db,CDbWServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    } 
    return true;
}
bool deleteFeedMiniAll(int uid,ostream & out,ostream & sqlscript)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and feed<0";
    out<<ss.str()<<endl;
    if(mode==TestDelete)
        return true;
    sql<<ss.str();
    try{
        simple_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
    return true;
}
bool deleteFeedMiniFX(int uid,ostream & out,ostream & sqlscript)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and big_type=100 and feed<0";
    out<<ss.str()<<endl;
    if(mode==TestDelete)
        return true;
    sql<<ss.str();
    try{
        simple_res = QueryRunner(mini_bc_db,CDbWServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    } 
    return true;
}
bool deleteFeedMiniRZ(int uid,ostream & out,ostream & sqlscript)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and big_type=600 and feed<0";
    out<<ss.str()<<endl;
    if(mode==TestDelete)
        return true;
    sql<<ss.str(); 
    try{
        simple_res = QueryRunner(mini_bc_db,CDbWServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    } 
    return true;
}
bool deleteFeedMiniZT(int uid,ostream & out,ostream & sqlscript)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false); 
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and small_type=502 and feed<0";
    out<<ss.str()<<endl;
    if(mode==TestDelete)
        return true;
    sql<<ss.str();
    try{
        simple_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
    return true;
}
bool deleteFeedMiniXC(int uid,ostream & out,ostream & sqlscript)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false); 
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    stringstream ss;
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and big_type=700 and feed<0";
    out<<ss.str()<<endl;
    if(mode==TestDelete)
        goto NEXT;
    sql<<ss.str();
    try{
        simple_res = QueryRunner(mini_bc_db,CDbWServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
NEXT:
    CLEAR();
    ss<<"delete from "<<mini_table<<" where user="<<uid<<" and small_type=501 and feed<0";
    out<<ss.str()<<endl;
    if(mode==TestDelete)
        return true;
    sql<<ss.str();
    try{
        simple_res = QueryRunner(mini_bc_db,CDbWServer,mini_table).execute(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
    return true;
}
void getAllFeeds(int uid,vector<Ice::Long> & feeds,ostream &out)
{
    feeds.clear();
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    Statement sql;
    stringstream ss;
    ss<<"select feed from "<<mini_table<<" where user="<<uid<<" and feed<0";
    out<<ss.str()<<endl;
    sql<<ss.str();    
    try{
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return ;
    }
    out<<"size:"<<store_res.num_rows()<<endl;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        feeds.push_back( (Ice::Long)store_res[i]["feed"]);
    }
}
void getXCFeeds(int uid,vector<Ice::Long> & feeds,ostream &out)
{
    feeds.clear();
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"select feed from "<<mini_table<<" where user="<<uid<<" and big_type=700 and feed<0";
    sql<<ss.str();    out<<ss.str()<<endl;
    try{
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return ;
    }out<<"size:"<<store_res.num_rows()<<endl;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        feeds.push_back( (Ice::Long)store_res[i]["feed"]);
    }
    CLEAR();
    ss<<"select feed from "<<mini_table<<" where user="<<uid<<" and small_type=501 and feed<0";
     out<<ss.str()<<endl;
    sql<<ss.str();   
    try {
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    }catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return ;
    }
    out<<"size:"<<store_res.num_rows()<<endl;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        feeds.push_back( (Ice::Long)store_res[i]["feed"]);
    }
}
void getFeedsByStype(int uid,int stype,vector<Ice::Long> & feeds,ostream & out)
{
    feeds.clear();
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"select feed from "<<mini_table<<" where user="<<uid<<" and small_type="<<stype<<" and feed<0";
    sql<<ss.str();    out<<ss.str()<<endl;
    try{
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return ;
    }
    out<<"size:"<<store_res.num_rows()<<endl;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        feeds.push_back( (Ice::Long)store_res[i]["feed"]);
    }
}
void getZTFeeds(int uid,vector<Ice::Long> & feeds,ostream & out)
{
    feeds.clear();
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"select feed from "<<mini_table<<" where user="<<uid<<" and small_type=502 and feed<0";
    sql<<ss.str();    out<<ss.str()<<endl;
    try{
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return ;
    }
    out<<"size:"<<store_res.num_rows()<<endl;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        feeds.push_back( (Ice::Long)store_res[i]["feed"]);
    }
}
void getRZFeeds(int uid,vector<Ice::Long>& feeds,ostream &out)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"select feed from "<<mini_table<<" where user="<<uid<<" and big_type=600 and feed<0";
    out<<ss.str()<<endl;
    sql<<ss.str();    
    try{
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return ;
    }
    out<<"size:"<<store_res.num_rows()<<endl;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        feeds.push_back( (Ice::Long)store_res[i]["feed"]);
    }
}
void getFXFeeds(int uid,vector<Ice::Long>& feeds,ostream & out)
{
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    Statement sql;
    stringstream ss;
    CLEAR();
    ss<<"select feed from "<<mini_table<<" where user="<<uid<<" and big_type=100 and feed<0";
    sql<<ss.str();    out<<ss.str()<<endl;
    try{
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return ;
    }
    out<<"size:"<<store_res.num_rows()<<endl;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        feeds.push_back( (Ice::Long)store_res[i]["feed"]);
    }
}
bool deleteUserFeedsByStype(int uid, ostream & out,ostream & sqlscript,int stype)
{
    vector<Ice::Long> feeds;
    getFeedsByStype(uid,stype,feeds,out);
    //delete content
    foreach(Ice::Long &f,feeds)
    {
        deleteFeedIndex(f,out,sqlscript);
        deleteFeedContent(f,out,sqlscript);
    }
    return deleteFeedMiniStype(uid,stype,out,sqlscript);
}
bool deleteUserAllFeeds(int uid, ostream & out,ostream & sqlscript)
{
    vector<Ice::Long> feeds;
    getAllFeeds(uid,feeds,out);
    //delete content
    foreach(Ice::Long &f,feeds)
    {
        deleteFeedIndex(f,out,sqlscript);
        deleteFeedContent(f,out,sqlscript);
    }
    return deleteFeedMiniAll(uid,out,sqlscript);
}
bool deleteUserXCFeeds(int uid,ostream & out,ostream & sqlscript)
{
    vector<Ice::Long> feeds;
    getXCFeeds(uid,feeds,out);
    foreach(Ice::Long f,feeds)
    {
        deleteFeedIndex(f,out,sqlscript);
        deleteFeedContent(f,out,sqlscript);
    }
    return deleteFeedMiniXC(uid,out,sqlscript);
}
bool deleteUserZTFeeds(int uid,ostream & out,ostream & sqlscript)
{
    vector<Ice::Long> feeds;
    getZTFeeds(uid,feeds,out);
    foreach(Ice::Long f,feeds)
    {
        deleteFeedIndex(f,out,sqlscript);
        deleteFeedContent(f,out,sqlscript);
    }
    return deleteFeedMiniZT(uid,out,sqlscript);
}
bool deleteUserFXFeeds(int uid,ostream & out,ostream & sqlscript)
{
    out<<"FX"<<endl;
    vector<Ice::Long> feeds;
    getFXFeeds(uid,feeds,out);
    //delete content
    foreach(Ice::Long &f,feeds)
    {
        deleteFeedIndex(f,out,sqlscript);
        deleteFeedContent(f,out,sqlscript);
    }
    //delete fx mini
    return deleteFeedMiniFX(uid,out,sqlscript);
}
bool deleteUserRZFeeds(int uid,ostream & out,ostream & sqlscript)
{
    vector<Ice::Long> feeds;
    getRZFeeds(uid,feeds,out);
    foreach(Ice::Long f,feeds)
    {
        deleteFeedIndex(f,out,sqlscript);
        deleteFeedContent(f,out,sqlscript);
    }
    return deleteFeedMiniRZ(uid,out,sqlscript);
}



map<string, bool(*)(istream &is,ostream & os,const vector<string> &)> command_map;

//ndelbc,id1,num,fx
bool ndelbc(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid1=0,uid2=0,num=0;string type;
    GET(1,uid1); GET(2,num);GET(3,type);
    uid2=uid1+num-1;
    int i=uid1;
    ss.clear();ss.str("");
    ss<<uid1<<"_"<<uid2<<"_"<<type<<".sql";
    ofstream sqlout(ss.str().c_str() );
    if(sqlout.fail()) {
       cout<<" file Failed!!! "<<ss.str().c_str()<<endl;
        return false;
    }
    ss.clear();ss.str("");
    ss<<uid1<<"_"<<uid2<<"_"<<type<<".out";
    ofstream infoout(ss.str().c_str() );
    if(infoout.fail()) {
       cout<<" file Failed!!! "<<ss.str().c_str()<<endl;
        return false;
    }
    if(type=="xc")
    { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserXCFeeds(i,infoout,sqlout);++i;
         }
         //return deleteUserXCFeeds(uid,out,sqlout);
    } else if(type=="fx") { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserFXFeeds(i,infoout,sqlout);++i;
         }
    } else if(type=="rz") { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserRZFeeds(i,infoout,sqlout);++i;
         }
    } else if(type=="zt") {
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserZTFeeds(i,infoout,sqlout);++i;
         }
    } else if(type=="all") { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserAllFeeds(i,infoout,sqlout); ++i;
         }
    } else { 
        out<<"bad param."<<endl;
        return false;
    }
    return true;
}
//buniquebc,id1,id2,fx
bool bdelbc(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid1,uid2;string type;
    GET(1,uid1); GET(2,uid2);GET(3,type);
    int i=uid1;
    ss.clear();ss.str("");
    ss<<uid1<<"_"<<uid2<<"_"<<type<<".sql";
    ofstream sqlout(ss.str().c_str() );
    if (sqlout.fail() ) {
       cout<<" file Failed!!! "<<ss.str().c_str()<<endl;
        return false;
    }
    ss.clear();ss.str("");
    ss<<uid1<<"_"<<uid2<<"_"<<type<<".out";
    ofstream infoout(ss.str().c_str() );
    if( infoout.fail() ) {
       cout<<" file Failed!!! "<<ss.str().c_str()<<endl;
        return false;
    }
    if(type=="xc")
    { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserXCFeeds(i,infoout,sqlout);++i;
         }
         //return deleteUserXCFeeds(uid,out,sqlout);
    } else if(type=="fx") { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl; 
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserFXFeeds(i,infoout,sqlout);++i;
         }
    } else if(type=="rz") { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserRZFeeds(i,infoout,sqlout);++i;
         }
    } else if(type=="zt") {
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserZTFeeds(i,infoout,sqlout);++i;
         }
    } else if(type=="all") { 
         while(i<=uid2) {
            if(filter_ids.count(i)) {
                out<<"id filtered:"<<i<<endl;
                infoout<<"id filtered:"<<i<<endl;++i;
                continue;
            }
            deleteUserAllFeeds(i,infoout,sqlout); ++i;
         }
    } else { 
        out<<"bad param."<<endl;
        return false;
    }
    

   
    return true;
}

//fdelbcstype,123.xxx,2005
bool fdelbcstype(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(3);
    stringstream ss;
    string name;int stype;int uid;
    GET(1,name);GET(2,stype);
    if( stype <100 || stype >30000 )  //100~ 30000
        return false;
    fstream file(name.c_str());
    if(file.fail()) {
        cout<<"file failed:"<<name.c_str()<<endl;
        return false;
    }
    ofstream file_out ((name+".out").c_str());
    if(file_out.fail() ) {
        cout<<" file Failed!!! "<<(name+".out").c_str()<<endl;
        return false;
    }
    ofstream sql_out ((name+".sql").c_str());
    if(file.fail() )
    {
        out<<"bad file"<<name.c_str()<<endl;
        return false;
    }
    string line;
    while(getline(file,line,'\n') )
    {
        ss.clear();ss.str(line);ss>>uid;
        if(ss.fail() ) 
            continue;
        if(filter_ids.count(uid)) {
            out<<"id filtered:"<<uid<<endl;
            continue;
        }
        deleteUserFeedsByStype(uid,file_out,sql_out,stype);
    }
    return true;
}
bool fdelbc(istream &in, ostream & out,const vector<string> & args)
{ 
    CHECK_SIZE(3);
    stringstream ss;
    string name;string type;int uid;
    GET(1,name);GET(2,type);
    fstream file(name.c_str());
    if(file.fail()) {
        cout<<"file failed:"<<name.c_str()<<endl;
        return false;
    }
    ofstream file_out ((name+".out").c_str());
    if(file_out.fail() ) {
        cout<<" file Failed!!! "<<(name+".out").c_str()<<endl;
        return false;
    }
    ofstream sql_out ((name+".sql").c_str());
    if(file.fail() )
    {
        out<<"bad file"<<name.c_str()<<endl;
        return false;
    }
    string line;
    if(type=="xc")
    {
        while(getline(file,line,'\n') )
        {
            ss.clear();ss.str(line);ss>>uid;
            if(ss.fail() ) 
                continue;
            if(filter_ids.count(uid)) {
                out<<"id filtered:"<<uid<<endl;
                continue;
            }
            deleteUserXCFeeds(uid,file_out,sql_out);
        }
        return true;
    } else if(type=="fx") {
        while(getline(file,line,'\n') )
        {
            ss.clear();ss.str(line);ss>>uid;
            if(ss.fail() ) 
                continue;
            if(filter_ids.count(uid)) {
                out<<"id filtered:"<<uid<<endl;
                continue;
            }
            deleteUserFXFeeds(uid,file_out,sql_out);
        }
        return true;
    } else if(type=="rz") {
        while(getline(file,line,'\n') )
        {
            ss.clear();ss.str(line);ss>>uid;
            if(ss.fail() ) 
                continue;
            if(filter_ids.count(uid)) {
                out<<"id filtered:"<<uid<<endl;
                continue;
            }
            deleteUserRZFeeds(uid,file_out,sql_out);
        }
        return true;
    } else if(type=="zt") {
        while(getline(file,line,'\n') )
        {
            ss.clear();ss.str(line);ss>>uid;
            if(ss.fail() ) 
                continue;
            if(filter_ids.count(uid)) {
                out<<"id filtered:"<<uid<<endl;
                continue;
            }
            deleteUserZTFeeds(uid,file_out,sql_out);
        }
        return true;
    } else if(type=="all") {
        while(getline(file,line,'\n') )
        {
            ss.clear();ss.str(line);ss>>uid;
            if(ss.fail() ) 
                continue;
            if(filter_ids.count(uid)) {
                out<<"id filtered:"<<uid<<endl;
                continue;
            }
            deleteUserAllFeeds(uid,file_out,sql_out);
        }
        return true;
    } else {
        out<<"bad param."<<endl;
        return false;
    }
}
bool delbc(istream &in, ostream & out,const vector<string> & args)
{ 
    CHECK_SIZE(3); 
    stringstream ss;
    int uid;string type;
    GET(1,uid);GET(2,type); 
    if(filter_ids.count(uid)) {
       out<<"id filtered:"<<uid<<endl;
       return true;
    }
    ss.clear();ss.str("");
    ss<<uid<<".sql";
    ofstream sqlout(ss.str().c_str() );
    if(sqlout.fail() ) {
         cout<<"File failed"<<ss.str().c_str()<<endl;
         return false;
    }
    if(type=="xc")
    { 
        return deleteUserXCFeeds(uid,out,sqlout);
    } else if(type=="fx") { 
        return deleteUserFXFeeds(uid,out,sqlout);
    } else if(type=="rz") { 
        return deleteUserRZFeeds(uid,out,sqlout);
    } else if(type=="zt") {
        return deleteUserZTFeeds(uid,out,sqlout);
    } else if(type=="all") { 
        return deleteUserAllFeeds(uid,out,sqlout);
    } else { 
        out<<"bad param."<<endl;
        return false;
    }
}
// num,id,fx
bool num(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(3);
    stringstream ss;
    string type;int uid;
    GET(1,uid);GET(2,type);
    if(filter_ids.count(uid)) {
       out<<"id filtered:"<<uid<<endl;
    }
    vector<Ice::Long> feeds;
    if(type=="xc")
    {
        getXCFeeds(uid,feeds,out);
    } else if(type=="fx") { 
        getFXFeeds(uid,feeds,out);
    } else if(type=="rz") { 
        getRZFeeds(uid,feeds,out);
    } else if(type=="zt") {
        getZTFeeds(uid,feeds,out);
    } else if(type=="all") { 
        getAllFeeds(uid,feeds,out);
    } else { 
        out<<"bad param."<<endl;
        return false;
    }
    out<<feeds.size()<<endl;
    return true;
}



class feed_node{
public:
    feed_node();
    feed_node(time_t t,Ice::Long s,Ice::Long m,int type):time_(t),source_(s),merge_(m),stype_(type){};
    time_t time_;
    Ice::Long source_;
    Ice::Long merge_;
    int stype_;
};

class feed_node_lesser
{
public:
 bool operator()(const feed_node & t,const feed_node & f) 
{
     if(t.time_ < f.time_ )
            return true;
        if(t.time_ > f.time_ )
            return false;
        if(t.source_ < f.source_ )
            return true;
        if(t.source_ > f.source_ )
            return false;
        if(t.merge_ < f.merge_ )
            return true;
        if(t.merge_ > f.merge_ )
            return false;
        if(t.stype_ < f.stype_ )
            return true;
        return false;
}
};

ostream & operator << (ostream & out, feed_node & f) 
{
    return out<<"("<<f.time_<<","<<f.source_<<","<<f.merge_<<","<<f.stype_<<")";
}

//runsql,feed_db,table,sqlblablabla
//TODO  accomplish it !!!!
bool runsql(istream &in, ostream & out,const vector<string> & args)
{
    if(args.size()<4) { out<<"args size error.";return false;}
    stringstream ss;
    string db_name;
    string table_name;
    GET(1,db_name);GET(2,table_name);
    string sql_input;
    int i=0;
    foreach(const string& s,args)
    {
       ++i;
       if(i==1||i==2) continue;
       sql_input+=s;
       sql_input+=",";
    }
    //execute
    Statement sql;
    mysqlpp::StoreQueryResult store_res;
    //stringstream ss;
    //ss<<sql_input;
    sql<<sql_input;
    try {
        store_res=QueryRunner(db_name,CDbWServer,table_name).store(sql);    
    }catch(mysqlpp::Exception& e) {
        out<<" mysql exception :"<<e.what()<<endl;
        return false;
    }
    if(store_res.num_rows()==0) {
        out<<"none !!"<<endl;
        return false;
    }
    for(size_t i=0;i<store_res.num_rows();++i)    
    {
        for(mysqlpp::Row::iterator it=store_res[i].begin();it!=store_res[i].end();++it)
           cout<<*it<<" ";
        cout<<endl;
    }
    return false;
} 

bool GetFeedIndexTime(Ice::Long feed,string & time,ostream & out)
{
    string bc_db=GetFeedDBInstance(false);//no use
    string db=GetFeedDBInstance(true);
    string index_table;
    GetIndexTable(feed,index_table,true);
    Statement sql; mysqlpp::StoreQueryResult store_res;
    stringstream ss;
    ss<<"select time from "<<index_table<<" where feed="<<feed;
    out<<ss.str()<<endl;
    sql<<ss.str();
    try {
        store_res=QueryRunner(db,CDbRServer,index_table).store(sql);    
    }catch(mysqlpp::Exception& e) {
        out<<"feed:"<<feed<<" mysql exception :"<<e.what()<<endl;
        return false;
    }
    if(store_res.num_rows()==0) {
        out<<"no this feed in content !!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        return false;
    }
    time = (string)(store_res[0][0]);//time
    return true;
}
//dumpleTime,file,maxsource.
//format
//sourceid,feed_id,user_id,mini_time,send_time
//
bool dumpLeTime(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(3);
    stringstream ss;
    string fileName;
    Ice::Long maxSource;
    mysqlpp::StoreQueryResult store_res;
    
    string mini_bc_db=GetFeedDBInstance(false);
    string mini_db=GetFeedDBInstance(true);
    string mini_table,mini_bc_table;


    Statement sql;
    
    GET(1,fileName);GET(2,maxSource);
    ifstream file(fileName.c_str());
    ofstream outfile( (fileName+".out").c_str());
    ofstream userout("user.out");
    if(file.fail()) {
        cout<<"file failed: "<<fileName.c_str()<<endl;
        return false;
    }
    if(outfile.fail()) {
        cout<<"file failed: "<<fileName<<".out"<<endl;
        return false;
    }
    if(userout.fail()) {
        cout<<"file failed: "<<fileName<<".out"<<endl;
        return false;
    }
    int uid;string line;bool ok;
    //
    Ice::Long temp_feed, temp_source;
    string mini_time,send_time;
    int user_size=0;
    while(getline(file,line,'\n'))
    {
        ++user_size;
        int feed_nums=0;
        CastTo(line,uid,ok);
        if(!ok) {
            out<<"bad exception  bad id !!:"<<uid<<endl;
            continue;
        }
        userout<<uid<<endl;
        //search mini
        GetMiniTable(uid,mini_table,true);
        CLEAR();
        ss<<"select feed,source,time from "<<mini_table<<" where user= "<<uid<<" and small_type=1205 and source<="<<maxSource;
        out<<ss.str()<<endl;
        sql<<ss.str();
        try {
            store_res=QueryRunner(mini_db,CDbRServer,mini_table).store(sql);
        } catch(mysqlpp::Exception& e) {
            out<<"uid:"<<uid<<" bad mysql exception :"<<e.what()<<endl;
            continue;
        }
        if(store_res.num_rows()==0) {
            //ok
            out<<"no le in mini.";
            //we will search mini_bc;
        } else {
            feed_nums=store_res.num_rows();
            for(size_t i=0;i<store_res.num_rows();++i)    
            {
                //sourceid,feed_id,user_id,mini_time,send_time
                temp_feed = (::Ice::Long) store_res[i][0];//feed
                temp_source = (::Ice::Long) store_res[i][1];//source 
                //last_source=temp_source;

                mini_time =(string) store_res[0][2];//time
                if(!GetFeedIndexTime(temp_feed,send_time,out) ) {
                    out<<"feed index time bad !!!!!!!!!!!!!";
                    continue;
                }
                //makeTimeStr(temp_time,time_str);
                outfile<<temp_source<<","<<temp_feed<<","<<uid<<","<<mini_time<<","<<send_time<<endl;
            }
        }
        //search mini_bc
        GetMiniTable(uid,mini_bc_table,false);
        CLEAR();
        ss<<"select feed,source,time from "<<mini_bc_table<<" where user= "<<uid<<" and small_type=1205 and source<="<<maxSource;
        out<<ss.str()<<endl;
        sql<<ss.str();
        try {
            store_res=QueryRunner(mini_bc_db,CDbRServer,mini_bc_table).store(sql);
        } catch(mysqlpp::Exception& e) {
            out<<"uid:"<<uid<<"bad mysql exception :"<<e.what()<<endl;
            continue;
        }
        feed_nums+=store_res.num_rows();
        out<<"feed_nums:"<<feed_nums<<endl;
        if(feed_nums==0)
            out<<"NOLE !!"<<"uid:"<<uid<<endl;
        if(store_res.num_rows()==0) {
            //++user_size;
            continue;
        } else {
            for(size_t i=0;i<store_res.num_rows();++i)
            {
                //sourceid,feed_id,user_id,mini_time,send_time
                temp_feed = (::Ice::Long) store_res[i][0];//feed
                temp_source = (::Ice::Long)store_res[i][1];//source 
                mini_time = (string)store_res[0][2];//time
                if(!GetFeedIndexTime(temp_feed,send_time,out) ) {
                    out<<"feed index time bad !!!!!!!!!!!!!";
                    continue;
                }
                //GetFeedIndexTime(temp_feed,send_time,out);
                //makeTimeStr(temp_time,time_str);
                outfile<<temp_source<<","<<temp_feed<<","<<uid<<","<<mini_time<<","<<send_time<<endl;
            }
        }
    }
    out<<"user_size:"<<user_size<<endl;
    return true;
}

bool run(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(2);
    stringstream ss;
    string name;
    GET(1,name);string line;
    ifstream file(name.c_str());
    if(file.fail()) {
        cout<<"file failed: "<<name.c_str()<<endl;
        return false;
    }
    vector<string> parts;
    mysqlpp::SimpleResult simple_res;
    Statement sql;
    while(getline(file,line,'\n'))
    {
        parts.clear();
        split(parts,line,is_any_of(","),token_compress_on);
        sql.clear();
        sql<<parts[2];
        out<<parts[2]<<endl;
        try{
            simple_res = QueryRunner(parts[0],CDbWServer,parts[1]).execute(sql);
        }
        catch (mysqlpp::Exception& e) {
            out<<"mysql exception!!"<<e.what()<<" lines:"<<parts[0]<<","<<parts[1]<<","<<parts[2]<<endl;
        }
    }
    return true;
}

bool unique(int uid,ostream & out,ostream & sqlscript)
{
    stringstream ss;
    set<feed_node,feed_node_lesser> feed_set;
    string mini_table;string content_table;
    GetMiniTable(uid,mini_table,false);
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    
    Statement sql;
    ss.clear();ss.str("");
    ss<<"select feed, time, source, mini_merge, small_type from "<<mini_table<<" where user="<<uid<<" and feed<0";
    out<<ss.str()<<endl;
    sql<<ss.str();
    try{
        store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
    } catch (mysqlpp::Exception& e) {
        out<<"mysql exception!!"<<e.what()<<endl;
        return false;
    }
    out<<"size:"<<store_res.num_rows()<<endl;
    Ice::Long temp_feed; time_t temp_time;Ice::Long temp_source;Ice::Long temp_merge;int temp_stype;
    int del_size=0;
    for(size_t i=0;i<store_res.num_rows();++i)
    {
        temp_feed = (Ice::Long) store_res[i][0];
        temp_time = (time_t) mysqlpp::DateTime(store_res[i][1]);
        temp_source = (Ice::Long)store_res[i][2];
        temp_merge = (Ice::Long)store_res[i][3];
        temp_stype = (int)store_res[i][4];
        if( !( feed_set.insert(feed_node(temp_time,temp_source,temp_merge,temp_stype) ) ).second )
        { // bad data
             //delete content
             
             deleteFeedIndex(temp_feed,out,sqlscript);
             deleteFeedContent(temp_feed,out,sqlscript);
             //delete mini
             deleteFeedMini(uid,temp_feed,out,sqlscript);
             ++del_size;
        }
    }
    out<<"ID:"<<uid<<",delete size:"<<del_size<<endl;
    return true;
}

//uniquebc,id
bool uniquebc(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(2);
    stringstream ss;
    string name;int uid;
    GET(1,uid);string line;
    //get old bc feeds;
    ss.clear();ss.str("");
    ss<<uid<<".sql";
    ofstream sqlout(ss.str().c_str() );
    if(sqlout.fail() ) {
        cout<<"file failed."<<ss.str().c_str()<<endl;
        return false;
    }
    if(filter_ids.count(uid)) {
        out<<"id filtered:"<<uid<<endl;
    }
    return unique(uid,out,sqlout);
}
//buniquebc,id1,id2
bool buniquebc(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(3);
    stringstream ss;
    int uid1,uid2;
    GET(1,uid1); GET(2,uid2);
    int i=uid1;
    ss.clear();ss.str("");
    ss<<uid1<<"_"<<uid2<<".sql";
    ofstream sqlout(ss.str().c_str() );
    if(sqlout.fail() ) {
        cout<<"file failed."<<ss.str().c_str()<<endl;
        return false;
    }
    while(i<=uid2)
    {
        if(filter_ids.count(i)) {
            out<<"id filtered:"<<i<<endl;
        }
        unique(i,out,sqlout);
        ++i;
    }
    return true;
}
//nuniquebc,id1,num
bool nuniquebc(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(3);
    stringstream ss;
    int uid1=0,uid2=0,num=0;
    GET(1,uid1); GET(2,num);
    int i=uid1; uid2=uid1+num-1;
    ss.clear();ss.str("");
    ss<<uid1<<"_a_"<<num<<".sql";
    ofstream sqlout(ss.str().c_str() );
    if(sqlout.fail() ) {
        cout<<"file failed."<<ss.str().c_str()<<endl;
        return false;
    }
    while(i<=uid2)
    {
        if(filter_ids.count(i)) {
            out<<"id filtered:"<<i<<endl;
        }
        unique(i,out,sqlout);
        ++i;
    }
    return true;
}
//funiquebc,file
bool funiquebc(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(2);
    stringstream ss;
    string name;int uid;
    GET(1,name);string line;
    fstream file(name.c_str());
    ofstream file_out( (name+".out" ).c_str() );
    if(file_out.fail() ) {
        cout<<"file failed."<<(name+".out" ).c_str()<<endl;
        return false;
    }
    ofstream sql_out( (name+".sql" ).c_str() );
    if(sql_out.fail() ) {
        cout<<"file failed."<<(name+".sql" ).c_str()<<endl;
        return false;
    }
    mysqlpp::StoreQueryResult store_res;
    while(getline(file,line,'\n'))
    {
       ss.clear();ss.str(line);
       ss>>uid;
       if( filter_ids.count(uid) ) {
           file_out<<"id filtered:"<<uid<<endl;
       }
       if( !unique(uid,file_out,sql_out) )
           file_out<<"failed"<<endl;
    }
    file_out.flush();
    sql_out.flush();
    return true;
}
// checknum,file
bool check_feed_nums(istream &in, ostream & out,const vector<string> & args)
{
    CHECK_SIZE(2);
    stringstream ss;
    string name;int uid;size_t num;
    GET(1,name);string line;
    fstream file(name.c_str());
    if(file.fail() ) {
        cout<<"file failed:"<<name.c_str()<<endl;
        return false;
    }
    ofstream file_out( (name+".out" ).c_str() );
    if(file_out.fail() ) {
        cout<<"file failed:"<<(name+".out" ).c_str()<<endl;
        return false;
    }
    vector<string> cmds;Statement sql;
    string mini_table;string content_table;
    string mini_bc_db=GetFeedDBInstance(false);
    mysqlpp::StoreQueryResult store_res;
    while(getline(file,line,'\n') )
    {
         split(cmds,line,is_any_of(","),token_compress_on);
         if(cmds.size()!=2) {
             out<<"wrong size!"<<cmds.size()<<endl;
             continue;
         }
         ss.clear();ss.str(cmds[0]);
         ss>>uid;
         ss.clear();ss.str(cmds[1]);
         ss>>num;
         ss.clear();ss.str("");
         GetMiniTable(uid,mini_table,false);
         ss<<"select * from "<<mini_table<<" where user="<<uid<<" and big_type=100";
         out<<ss.str()<<endl;
         sql.clear();
         sql<<ss.str();
         try{
             store_res = QueryRunner(mini_bc_db,CDbRServer,mini_table).store(sql);
         } catch (mysqlpp::Exception& e) {
             out<<"mysql exception!!"<<e.what()<<endl;
             return false;
         }
         if( num != store_res.num_rows()) {
             out<<uid<<" error. db:"<<store_res.num_rows()<<" file:"<<num<<endl;
             file_out<<uid<<" error. db:"<<store_res.num_rows()<<" file:"<<num<<endl;
         }
    }
    return true;
}
void init_command()
{
    command_map["ndelbc"]=&ndelbc;
    command_map["buniquebc"]=&nuniquebc;
    command_map["bdelbc"]=&bdelbc;
    command_map["buniquebc"]=&buniquebc;
    command_map["delbc"]=&delbc;
    command_map["fdelbc"]=&fdelbc;
    command_map["checknum"]=&check_feed_nums;
    command_map["uniquebc"]=&uniquebc;
    command_map["funiquebc"]=&funiquebc;
    command_map["num"]=&num;
    command_map["run"]=&run;
    command_map["runsql"]=&runsql;
    command_map["fdelbcstype"]=&fdelbcstype;
    command_map["dump"]=&dumpLeTime;

}
int main(int argc,char * argv[])
{
    ifstream filter("filterID.txt");
    if(filter.fail())
    {
        cout<<"must have a file by the name of filterID.txt exist in ./"<<endl;
        cout<<"exit"<<endl;
        return 0;
    }
    string id_line;
    stringstream ss;int uid; int i=0;
    while(getline(filter,id_line,'\n') ) 
    {
        ss.clear();ss.str(id_line);
        ss>>uid;
        if(ss.fail()) {
            cout<<"a bad id in fiterID.txt line:"<<i<<endl;
        }
        filter_ids.insert(uid);
        ++i;
    }
    cout<<"filter size:"<<filter_ids.size()<<endl;
    ConnectionPoolManager::instance().initialize();
    init_command();
    cout<<"cmds: bdelbc, fdelbc , delbc, uniquebc, funiquebc,fdelbcstype, buniquebc, test, full, quick, exit "<<endl;
    cout<<"input command,currently in test mode, won't delete anything."<<endl;    
    string line;vector<string> cmds;
    ofstream fileout("out.out");
    if(fileout.fail()){
        cout<<" fail !!!! "<<endl;
    }
    while(1) 
    {
        cin>>line;
        split(cmds,line,is_any_of(","),token_compress_on);
        if(cmds[0]=="exit")
            return 0;
        if(cmds[0]=="test")
        {
            mode=TestDelete;
            cout<<"test mode"<<endl;
            continue;
        }
        if(cmds[0]=="full")
        {
            mode=FullDelete;
            cout<<"full delete mode"<<endl;
            continue;
        }
        if(cmds[0]=="quick")
        {
            mode=QuickDelete;
            cout<<"quick delete mode"<<endl;
            continue;
        }
        map<string, bool(*)(istream &is,ostream & os,const vector<string> &)>::iterator it=command_map.find(cmds[0]);
        if(it!=command_map.end() ) {
            if(cmds.size()==1) {//usage
                cout<<"param size error"<<endl;
                continue;
            } else {
                 try {
                    (*(it->second))(cin,fileout,cmds);
                    cout<<"done."<<endl;
                 } catch (std::exception &e ) {
                    cout<<"process catch a exception."<<e.what()<<endl;
                 }
            }
        } else {
            cout<<"bad command"<<endl;
        }
    }
    return 0;
}
 
