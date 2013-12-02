///////////////////////////////////////////////////////////
//
//       Title: CommandHanlder.cpp
//       Athor: Feed
//  CreateTime: 07/15/2012
//       Email: zhi.lin@renren-inc.com
// Description:  
//              1. implement of TimeLineFeedI command handler
//
//
#include "preheader.h"
#include "CommandHandler.h"
#include "TimeLineFeedI.h"
#include "TimeLineInternal.h"
#include "Task.h"

using namespace boost;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace xce::serverstate;
using namespace xce::subtle;
using namespace xce::feed::timeline;
using namespace Ice;

#define foreach BOOST_FOREACH

TimeLineStatistics::TimeLineStatistics()
{
    int year,month,day,hour,min,sec;
    //int year_now ,month_now ,day_now;
    //getYearMonthDay(time(NULL),year_now,month_now,day_now);
    getDayTime(time(NULL),year,month,day,hour,min,sec);
    time_=year*1000000;  time_+=month*10000; time_+=day*100; time_+=hour;  
    for(int i=0;i<kStatMax;++i)
    {
        NoBarrier_Store(&values_[i],0);
    }
    //used in output & xml , so forbid < > e.g.
    names_[kLeNum]="le_counts";names_[kInvokeNum]="invoke_counts";names_[kQNum]="query_counts";names_[kQsbsNum]="qsbs_counts";
    names_[kQfbsNum]="qfbs_counts";names_[kIwNum]="iw_counts";names_[kSwNum]="sw_counts";names_[kAddNum]="add_counts";
    names_[kModifyNum]="modify_counts";names_[kSrbiNum]="srbi_counts";names_[kLoadNum]="load_counts";names_[kExeNum]="exe_counts";
    names_[kUserMiss]="user_miss";names_[kMonthMiss]="month_miss";names_[kMonthHit]="month_hit";
    names_[k0_2ms]="all0_2ms";names_[k2_50ms]="all2_50ms";names_[k50_300ms]="all50_300ms";names_[k300ms]="all300_ms";
    names_[kq0_2ms]="query0_2ms";names_[kq2_50ms]="query2_50ms";names_[kq50_300ms]="query50_300ms";names_[kq300ms]="query300_ms";
    names_[kqfbs0_2ms]="qfbs0_2ms";names_[kqfbs2_50ms]="qfbs2_50ms";names_[kqfbs50_300ms]="qfbs50_300ms";names_[kqfbs300ms]="qfbs300_ms";
    names_[kadd0_2ms]="add0_2ms";names_[kadd2_50ms]="add2_50ms";names_[kadd50_300ms]="add50_300ms";names_[kadd300ms]="add300_ms";
    names_[ktmlDB0_50ms]="tmldb0_50ms";names_[ktmlDB50_300ms]="tmldb50_300ms";names_[ktmlDB300ms]="tmldb300_ms";names_[ktmlDBExcepn]="tmldbexceptions";
    names_[kminiDB0_50ms]="minidb0_50ms";names_[kminiDB50_300ms]="minidb50_300ms";names_[kminiDB300ms]="minidb300_ms";names_[kminiDBExcepn]="minidbexceptions";
    names_[kotherDBExcepn]="otherdbexceptions";
}
static void ShowSettingInfo(const TimeLineSetting::info & config, ostream & ss,const string & prefix)
{
    ss<<prefix<<"lru_timeout:"<<config.lru_timeout_<<endl;
    ss<<prefix<<"lru_maxsize:"<<config.lru_maxsize_<<endl;
    ss<<prefix<<"merge_stypes:( ";
    foreach(int stype, config.merge_stypes_)
    {
        ss<<stype<<" ";
    }
    ss<<")";
}
static void ShowSetting(const TimeLineSetting & setting, ostream & ss)
{
    ss<<"default:"<<endl;
    ShowSettingInfo(setting.default_config_,ss,"    ");
    ss<<endl;
    ss<<"no_special_setting_:"<<setting.no_special_setting_<<endl;
    // uid_map
    if(setting.uid_map_.size()!=0)
        ss<<"white_list:"<<endl;
    for(map<int,int>::const_iterator it0 = setting.uid_map_.begin();it0!=setting.uid_map_.end();++it0)
    {
        ss<<"    "<<it0->first<<" , "<<it0->second<<endl;
    }
    // remainders
    if(setting.remainders_vector_.size()!=0)
        ss<<"remainders:"<<endl;
    for(vector<tuple<int ,int, int> >::const_iterator it1=setting.remainders_vector_.begin(); it1!=setting.remainders_vector_.end();++it1)  
    {
        ss<<"    "<<it1->get<0>()<<" , "<<it1->get<1>()<<" , "<<it1->get<2>()<<" , "<<endl;
    }
    //range_vector_
    if(setting.range_vector_.size()!=0)
        ss<<"ranges:"<<endl;
    for(vector<tuple<int ,int, int> >::const_iterator it2=setting.range_vector_.begin(); it2!=setting.range_vector_.end();++it2)  
    {
        ss<<"    "<<it2->get<0>()<<" , "<<it2->get<1>()<<" , "<<it2->get<2>()<<" , "<<endl;
    }
    if(setting.configs_.size()!=0)
        ss<<"configs:"<<endl;
    int temp=0;
    for(vector<TimeLineSetting::info>::const_iterator it3 = setting.configs_.begin();it3!=setting.configs_.end();++it3)
    {
        ss<<temp<<":"<<endl;
        ShowSettingInfo(*it3,ss,"    ");
        ss<<endl;
        ++temp;
    }
    ss<<"stat_path:"<<setting.stat_path_<<endl;
    ss<<"json:"<<endl<<setting.json_<<endl;
}


extern const char *usage;

#define CHECK_SIZE(i) \
do{\
   if(args.size()!=i) return "param size error.";\
}while(0)\

#define GET(pos,name) \
do {\
  ss.clear();ss.str(args.at(pos));ss>>name;if(ss.fail()) { return "param error.";}\
}while(0)


std::string CommandHandler::Test(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    return "Test";
}

// command
std::string CommandHandler::SettingReload(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    if(TimeLineFeedI::instance().reloadSetting())
        return "ok";
    else
        return "failed.(maybe DB failed or bad DB data format)";
}
std::string CommandHandler::SettingShow(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    stringstream ss;
    // string result;
    
    //we need lock it,  because we will read both settings and doesn't allow any write operation, or else might crash
    ::IceUtil::Mutex::Lock lock(TimeLineFeedI::instance().setting_reload_mutex_);
    ss<<"currently using setting: "<<feedI.setting_idx_used()<<endl;
    ss<<"==========setting 0=========="<<endl;
    ShowSetting(feedI.setting_[0],ss);
    ss<<"==========setting 1=========="<<endl;
    ShowSetting(feedI.setting_[1],ss);
    return ss.str();
}
std::string CommandHandler::Setting(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    TimeLineFeedI &  feedI=TimeLineFeedI::instance();
    stringstream ss;
    //we will the config return for a long time, so we need lock reload, else we will crash
    ::IceUtil::Mutex::Lock lock(TimeLineFeedI::instance().setting_reload_mutex_);
    const TimeLineSetting::info & config=feedI.setting_used().config(uid);
    ShowSettingInfo(config,ss,"");
    return ss.str();
}
std::string CommandHandler::StatFile(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    string filepath;
    if(args.size()==1) {
        //IceUtil::Mutex::Lock lock(feedI.stat_.mutex_);
        feedI.stat_path(filepath);
        if( filepath == "")
            return "stat file is empty path.";
        else
            return filepath;
    //} else if(args.size()==2){
    //    IceUtil::Mutex::Lock lock(feedI.stat_.mutex_);
    //    feedI.stat_.savePath_=args[1];
    //    return "change save stat file to:\""+feedI.stat_.savePath_+"\"";
    } else {
        return "param size error";
    }
}
std::string CommandHandler::Stat(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    stringstream ss;
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    ss<<"{";
    for(int i=0;i<kStatMax;++i)  {
        ss<<"\n\""<<feedI.stat_.names_[i]<<"\": "<<NoBarrier_Load(&(feedI.stat_.values_[i]))<<",";
    }
    ss<<"\n}";
    return ss.str();
}
std::string CommandHandler::Show(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    if(args.size()==1) {
        return "show all not support yet.";
    }
    else if(args.size()==2) {
        TimeLineFeedI & feedI=TimeLineFeedI::instance();
        IceUtil::Mutex::Lock lock(feedI.stat_.mutex_);
        map<std::string, std::string>::iterator it=feedI.stat_.table_.find(args[1]);
        if(it==feedI.stat_.table_.end()) {
            return "no this item.";
        }
        else {
            string res=args[1];
            res+=" : ";
            res+=it->second;
            return res;
        }
    }
    return "param size error.";
}

//lru
std::string CommandHandler::Lru(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    stringstream ss; string time_str;
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    time_t interval=feedI.lru_timeout(uid);
    time_t current=time(NULL);
    time_t temp;
    ss<<"size: "<<ptr->lru_.list_.size()<<"\n";
    ss<<"year\tmonth\ttime\t\tdate\t\t\ttimeout";
    foreach(int m, ptr->lru_.list_) {
        temp=ptr->lru_.map_[m].last_time_;
        makeTimeStr(temp,time_str);
        ss<<"\n"<<m/100<<"\t"<<m%100<<"\t"<<temp<<"\t"<<time_str<<"\t"<<interval-(current-temp);
    }
    return ss.str();
}
std::string CommandHandler::RebuildTimeLine(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    string temp;
    feedI.rebuildTimeLine(ptr,uid,temp);
    return "ok.";
}
std::string CommandHandler::QueryTimeLineInCache(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    return ptr->timeLine_;
}
std::string CommandHandler::ClearUserCache(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    if(args.size()==1)
    {
        TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
        IceUtil::Mutex::Lock lock(ptr->mutex_);
        ptr->feedMap_.clear();
        return "ok.";
    } else if(args.size()==3){
        int year=0,month=0;
        stringstream ss;
        ss<<args[1];
        ss>>year;
        if( ss.fail()) 
           return "year param error.";
        ss.clear();ss.str("");
        ss<<args[2];
        ss>>month;
        if( ss.fail() )
           return "month param error.";
        TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
        IceUtil::Mutex::Lock lock(ptr->mutex_);
        TimeLineFeedI & feedI=TimeLineFeedI::instance();
        feedI.clearMonthCache(ptr,uid,year,month);
        return "ok";
    }
    return "param size error.";
}
std::string CommandHandler::QueryCacheSize(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    size_t size=ServiceI::instance().getObjectCache()->locateCategory(TIMELINE_CATEGORY)->size();
    stringstream ss;
    ss<<"cache size:"<<size;
    return ss.str();
}
std::string CommandHandler::SetCacheSize(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(2);
    size_t new_size=1000000;
    stringstream ss;
    ss<<args[1];
    ss>>new_size;
    if(new_size<100000) { //too small is not allowed.
        ss.clear();ss.str("");ss<<"new size <10w. "<<new_size;
        return ss.str();
    }
    ServiceI::instance().getObjectCache()->locateCategory(TIMELINE_CATEGORY)->size(new_size);
    ss<<"cache size:"<<new_size;
    return ss.str();
}
std::string CommandHandler::QueryFeedSize(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    stringstream ss;
    ss<<"cache size:"<<ptr->feedMap_.size();
    return ss.str();
}
std::string CommandHandler::QueryFeedInCache(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(2);
    stringstream ss;
    string time_str;
    ss.str(args[1]);
    Ice::Long feed;
    ss>>feed;
    TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    feed_index_ref index=ptr->feedMap_.get<MIC_FEED_INDEX>();
    feed_iterator it=index.find(feed);
    ss.clear();ss.str("");
    if( it!=index.end() ) {
        ss<<"\tfeed\ttime_t\t\tdate\t\t\ttype\t\twide\tstype\n";//merge\n";
        makeTimeStr(it->time_,time_str);
        ss<<it->feed_<<"\t"<<it->time_<<"\t"<<time_str<<"\t"<<it->type_<<"\t"<<isWide(it->type_)<<"\t"
        <<type2stype(it->type_);//<<"\t"<<it->merge_;
    } else {
        ss<<feed<<" not found.";
    }
    return ss.str();
}

std::string CommandHandler::QueryMonthInCache(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(3);
    stringstream ss; int year,month,year_next,month_next;
    string time_str;
    ss.str(args[1]);
    ss>>year;ss.clear();
    if(year<kMinYearAllow || year>kMaxYearAllow )
        return "bad year param";
    ss.str(args[2]);
    ss>>month;ss.clear();ss.str("");
    if(month<1|| month >12)
        return "bad month param";
    time_t begin=getTime(year,month,1);
    MCE_INFO("gettime:"<<year<<" "<<month<<" "<<1<<"   === "<<begin);
    getNextMonth(year,month,year_next,month_next);
    time_t end=getTime(year_next,month_next,1);
    MCE_INFO("gettime:"<<year_next<<" "<<month_next<<" "<<1<<"   === "<<end);
    TimeLineObjectPtr ptr=ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    time_index_ref index=ptr->feedMap_.get<MIC_TIME_INDEX>();
    size_t size=0;
    //Notice that time is ordered DESC , so (_1<end,begin<=_1) rather than (begin<=_1,_1<end)
    ss<<"\tfeed\ttime_t\t\tdate\t\t\ttype\t\twide\tstype";//\tmerge";
    std::pair<time_iterator,time_iterator> pair1=index.range(lambda::_1<end,begin<=lambda::_1);
    if(pair1.first!=pair1.second)  {
        for(;pair1.first!=pair1.second && pair1.first!=index.end() ;++pair1.first) {
            ++size;
            makeTimeStr(pair1.first->time_,time_str);
            ss<<"\n"<<pair1.first->feed_<<"\t"<<pair1.first->time_<<"\t"<<time_str<<"\t"<<pair1.first->type_<<"\t"
            //<<isWide(pair1.first->type_)<<"\t"<<type2stype(pair1.first->type_)<<"\t"<<pair1.first->merge_;
            <<isWide(pair1.first->type_)<<"\t"<<type2stype(pair1.first->type_);//<<"\t"<<pair1.first->merge_;
        }
    } else {
        ss<<"no data.";
    }
    ss<<"\ntotal size: "<<size;
    return ss.str();
}
//dfeed,(feed_year),(feed)
std::string CommandHandler::QueryFeedInDB(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(3);
    stringstream ss; int feed_year;Ice::Long feed;
    ss.str(args[1]);
    ss>>feed_year;ss.clear();
    if(feed_year<kMinYearAllow || feed_year>kMaxYearAllow )
        return "bad year param";
    ss.str(args[2]);
    ss>>feed;ss.clear();
    string table;
    GetFeedMiniTable(uid,table,feed_year>2009);
    const char * db=GetFeedDBInstance(feed_year>2009);
    Statement sql;
    mysqlpp::StoreQueryResult store_res;
    //feed,time,type,mini_merge
    ::Ice::Long temp_feed; time_t temp_time; int temp_type; ::Ice::Long temp_source; ::Ice::Long temp_merge;string time_str;
    const int kFeedIdx=0;  const int kTimeIdx=1;   const int kTypeIdx=2;const int kSourceIdx=3;   const int kMergeIdx=4;
    ss.clear();ss.str();
    ss<<"select feed,time,type,source,mini_merge from "<<table<<" where user="<<uid<<" and feed="<<feed;
    sql<<ss.str();
    try {
        store_res=QueryRunner(db,CDbRServer,table).store(sql);
    } catch (mysqlpp::Exception& e) {
        FEEDI_INFO(uid," mysql exception :"<<e.what()<<endl);
        ss<<"\n mysql exception:"<<e.what();
        return ss.str();
    }
    if(store_res.num_rows()==0) {
        ss<<"\n no this feed";
        return ss.str();
    } else {
        temp_feed = (::Ice::Long) store_res[0][kFeedIdx];
        temp_time = (time_t) mysqlpp::DateTime(store_res[0][kTimeIdx]); 
        temp_type =(int) store_res[0][kTypeIdx]; 
        temp_source = (::Ice::Long) store_res[0][kSourceIdx];
        temp_merge = (::Ice::Long) store_res[0][kMergeIdx];
        makeTimeStr(temp_time,time_str);
        ss<<"\n";
        ss<<"\tfeed\ttime_t\t\tdate\t\t\ttype\t\twide\tstype\tsource\tmerge\n";
        ss<<temp_feed<<"\t"<<temp_time<<"\t"<<time_str<<"\t"<<temp_type<<"\t"<<isWide(temp_type)<<"\t"
        <<type2stype(temp_type)<<"\t"<<temp_source<<"\t"<<temp_merge;
    }

    return ss.str();
}
//dmonth,(year),(month)
std::string CommandHandler::QueryMonthInDB(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(3);
    stringstream ss; int year,month,year_next,month_next;
    ss.str(args[1]);
    ss>>year;ss.clear();
    if(year<kMinYearAllow || year>kMaxYearAllow )
        return "bad year param";
    ss.str(args[2]);
    ss>>month;ss.clear();ss.str("");
    if(month<1|| month >12)
        return "bad month param";
    getNextMonth(year,month,year_next,month_next);
    string table;
    GetFeedMiniTable(uid,table,year>2009);
    const char * db=GetFeedDBInstance(year>2009);
    Statement sql;
    mysqlpp::StoreQueryResult store_res;
    //feed,time,type,mini_merge
    ::Ice::Long temp_feed; time_t temp_time; int temp_type; ::Ice::Long temp_source; ::Ice::Long temp_merge; string time_str;
    const int kFeedIdx=0;
    const int kTimeIdx=1;
    const int kTypeIdx=2;
    const int kSourceIdx=3;
    const int kMergeIdx=4;
    ss.clear();ss.str();
    ss<<"select feed,time,type,source,mini_merge from "<<table<<" where user="<<uid<<" and time>='"<<year<<"-"<<month<<"-01 00:00:00"
    <<"' and time < '"<<year_next<<"-"<<month_next<<"-01 00:00:00'";
    sql<<ss.str();
    try {
        store_res=QueryRunner(db,CDbRServer,table).store(sql);
    } catch (mysqlpp::Exception& e) {
        FEEDI_INFO(uid," mysql exception :"<<e.what()<<endl);
        ss<<" mysql exception :"<<e.what();
        return ss.str();
    }
    ss<<"\n\n";
    ss<<"\tfeed\ttime_t\t\tdate\t\t\ttype\t\twide\tstype\tsource\tmerge";
    size_t i=0;
    for(;i<store_res.num_rows();++i) {
        temp_feed = (::Ice::Long) store_res[i][kFeedIdx];
        temp_time = (time_t) mysqlpp::DateTime(store_res[i][kTimeIdx]); 
        temp_type =(int) store_res[i][kTypeIdx];
        temp_source = (int) store_res[i][kSourceIdx]; 
        temp_merge = (::Ice::Long) store_res[i][kMergeIdx];
        makeTimeStr(temp_time,time_str);
        ss<<"\n";
        ss<<temp_feed<<"\t"<<temp_time<<"\t"<<time_str<<"\t"<<temp_type<<"\t"<<isWide(temp_type)<<"\t"
        <<type2stype(temp_type)<<"\t"<<temp_source<<"\t"<<temp_merge;
    }
    ss<<"\ntotal size: "<<i;
    return ss.str();
}
std::string CommandHandler::QueryTimeLineInDB(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    CHECK_SIZE(1);
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    string tml;
    if(!feedI.readTimeLineDB(uid,tml)){
        return "DB truble.";
    } else {
        return tml;  
    }
}
std::string CommandHandler::LoadTimeLine(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    feedI.loadTimeLine(ptr,uid);
    return "ok";
}


string CommandHandler::LoadFile(int uid,const std::vector<std::string> & args,const std::string & cmd)
{//load
    CHECK_SIZE(2);
    stringstream ss;
    ss<<"load file:"<<args[1];
    TimeLineFeedI & feedI=TimeLineFeedI::instance();
    IceUtil::Mutex::Lock lock(feedI.stat_.mutex_);
    map<std::string, std::string>::iterator it=feedI.stat_.table_.find("task");
    if( it != feedI.stat_.table_.end() ) {
        ss<<"\nprevious task is still running,pls run after it ends";
        return ss.str();
    }
    TaskManager::instance().execute(new LoadCacheByFileTask(args[1],"task",10,uid%10));
    ss<<"\n show,task to see progress"; 
    return ss.str();
} 
string CommandHandler::LoadCache(int uid,const std::vector<std::string> & args,const std::string & cmd)
{//load
    stringstream ss;
    if(args.size()==3) {
        int year=0;int month=0;
        GET(1,year);GET(2,month);
        TimeLineFeedI & feedI=TimeLineFeedI::instance();
        TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
        feedI.loadCache(ptr,uid,year,month,false);
        return "ok";
    }
    if(args.size()==1) {
        vector<int> v;v.push_back(uid);
        TaskManager::instance().execute(new LoadCacheTask(v));
    }
    return "ok.";
} 
string CommandHandler::LogLevel(int uid,const vector<string> & args,const string & cmd)
{
    CHECK_SIZE(2);
    if(args[1]=="DEBUG"||args[1]=="debug") {
        MCE_LEVEL("DEBUG");
        return string("ok. loglevel: DEBUG. ");
    }
    if(args[1]=="INFO"||args[1]=="info") {
        MCE_LEVEL("INFO");
        return string("ok. loglevel: INFO. ");
    }
    if(args[1]=="WARN"||args[1]=="warn") {
        MCE_LEVEL("WARN");
        return string("ok. loglevel: WARN. ");
    }
    if(args[1]=="FATAL"||args[1]=="fatal") {
        MCE_LEVEL("FATAL");
        return string("ok. loglevel: FATAL. ");
    }
    return string("bad loglevel.");

}
std::string CommandHandler::Help(int uid,const std::vector<std::string> & args,const std::string & cmd)
{
    return string(usage);
}
const char * usage="\
help  \
log  \
load  \
clear  \
cfeed  \
cmonth  \
csize  \
ctml  \
dfeed  \
dmonth  \
dtml  \
rbdtml  \
call  \
show  \
";
