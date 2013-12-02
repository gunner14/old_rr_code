//////////////////////////////////////////////////////////
//
//       Title: TimeLineFeedI.cpp
//       Athor: Feed
//  CreateTime: 05/01/2012
//       Email: zhi.lin@renren-inc.com
// Description:  
//              1. implement of TimeLineFeedI class 
//                 implement of tool function
//                 implement of cache data class: node, TimeLineObject, 
//
//      Notice: 1. time_t must be 8 bytes; our code & TimeLine design depends on this
//                 time_t(1970-3001) range:about 1000 year, so we maybe support(1600 ~ 2400)  
//                 nagetive time_t used to represent years before 1970.
//                 
//        Test: 1. feature test,  see '../test'
//              2. unit test, see '../unit_test'
//              3. timeline data tool, see'../tool'
//
//   ChangeLog: 06/04/2012  zhi.lin  add cache support
//   ChangeLog: 06/25/2012  zhi.lin  add support for feeds before 2010. 
//   ChangeLog: 07/15/2012  zhi.lin  add execute command feature
//   ChangeLog: 09/**/2012  zhi.lin  add month lru evict
//   ChangeLog: 10/25/2012  zhi.lin  add online real-time configuration support for various special ids.(for lru_time, merge_stypes currently)
//   ChangeLog: 10/31/2012  zhi.lin  because of quick mktime()'s bug, replace the quick mktime() back to system's mktime() (very very slow.1ms), 
//                                   maybe one day I will find a new mktime can pass strict unit_test to replace sys's mktime() again.
//   ChangeLog: 11/07/2012  zhi.lin  add month lru size evict
// 
//TODO 2. encapsule operations of cache , database.
//TODO 0. separate TimeLine string and month Feeds in ObjectCacheI to improve Feeds's month hit rate.
//
//#include "TimeLineFeedPch.h"//Precompile file created from preheader.h
#include "preheader.h"
#include "TimeLineFeedI.h"
#include "Task.h"

using namespace boost;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace xce::serverstate;
using namespace xce::feed::timeline;
using namespace xce::subtle;
using namespace Ice;

using namespace Json;

TimeLineSetting::info TimeLineSetting::default_config_;

#define CLEAR_SS() do{ ss.clear();ss.str("");}while(0)
#define CLEAR_SS_SQL() do{ ss.clear();ss.str("");sql.clear();}while(0)

/// \brief initialization of TimeLineFeed
void MyUtil::initialize()
{
    ///1 .  create service & register objectCache
    BOOST_STATIC_ASSERT(sizeof(time_t)==8);//TimeLine only support time_t with 8 bytes.
    ServiceI& server = ServiceI::instance();
    server.getAdapter()->add(&TimeLineFeedI::instance(),server.createIdentity("TimeLineFeed",""));
    server.registerObjectCache(TIMELINE_CATEGORY,"obj",new TimeLineObjectFactoryI(),true);
    ///2 .  get property to initialize ServerStateSubscriber
    int mod=server.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("TimeLineFeed.Mod",0);
    int interval=server.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("TimeLineFeed.Interval",5);
    string ctrEndpoint=server.getCommunicator()->getProperties()->getPropertyWithDefault("TimeLine.ControllerEndpoint", "ControllerTimeLineFeed");
    ServerStateSubscriber::instance().initialize(ctrEndpoint, &TimeLineFeedI::instance(), mod, interval, new XceFeedControllerChannel());
    ///3 .  get property to launch a task to preload user feeds info 
    string load_file=server.getCommunicator()->getProperties()->getPropertyWithDefault("TimeLineFeed.PreloadFileAbsPath","");
    string save_file;//=server.getCommunicator()->getProperties()->getPropertyWithDefault("TimeLineFeed.SaveStatFileAbsPath","");
    ///4 .  call Initialize.
    TimeLineFeedI::instance().Initialize(mod,load_file,save_file);
}


namespace xce { 
namespace feed { 
namespace timeline { 

#define foreach BOOST_FOREACH

/// for call string::reserve of timeline string to improve performance & reduce mem fragment,( 5 + 2 * 12m ) * 10y =290
#define BEST_TL_LEN 300

/// \brief function to record timecost \ref TimeHelper
enum callee{ InQuery, InQfbs, InAdd, InOthers};

void OthersTimeHook(TimeLineFeedI * feedI, TimeHelper * th)
{
    long elapse = th->elapsed();
    if(elapse<2*1000) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k0_2ms]),1);
    } else if(elapse < 50*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k2_50ms]),1);
    } else if(elapse < 500*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k50_300ms]),1);
    } else {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k300ms]),1);
    }
}
void QueryTimeHook(TimeLineFeedI * feedI, TimeHelper * th)
{
    long elapse = th->elapsed();
    if(elapse<2*1000) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k0_2ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kq0_2ms]),1);
    } else if(elapse < 50*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k2_50ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kq2_50ms]),1);
    } else if(elapse < 500*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k50_300ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kq50_300ms]),1);
    } else {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k300ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kq300ms]),1);
    }
}
void AddNotifyTimeHook(TimeLineFeedI * feedI, TimeHelper * th)
{
    long elapse = th->elapsed();
    if(elapse<2*1000) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k0_2ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kadd0_2ms]),1);
    } else if(elapse < 50*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k2_50ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kadd2_50ms]),1);
    } else if(elapse < 500*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k50_300ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kadd50_300ms]),1);
    } else {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k300ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kadd300ms]),1);
    }
}
void QfbsTimeHook(TimeLineFeedI * feedI, TimeHelper * th)
{
    long elapse = th->elapsed();
    if(elapse<2*1000) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k0_2ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kqfbs0_2ms]),1);
    } else if(elapse < 50*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k2_50ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kqfbs2_50ms]),1);
    } else if(elapse < 500*1000 ) {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k50_300ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kqfbs50_300ms]),1);
    } else {
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[k300ms]),1);
        NoBarrier_AtomicIncrement(&(feedI->stat_.values_[kqfbs300ms]),1);
    }
}
//
// Do Not store result returned.
// if reload interface invoked twice in a interval that shorter than the time service use the config returned.
// (after switch index, wait at least 1s to write the ilde setting.)
// service will crash !!!!!!!!!
//
const TimeLineSetting::info & TimeLineSetting::config(int uid)
{
    //SETTING_INFO("call config search once.");//heavy function so leave a log.
    try
    {
        if(no_special_setting_) {
             return default_config_;
        }
        //check white list.
        if( uid_map_.size()!=0 )  {
            //SETTING_INFO("try white lists.");
            std::map<int,int>::iterator it=uid_map_.find(uid);
            if( it != uid_map_.end() ) {
                SETTING_INFO("found in white lists. config:"<<it->second);
                return configs_.at(it->second);
            }
        }
        //check remainders_map
        if(remainders_vector_.size()!=0) {
            //SETTING_INFO("try remainders map.");
            for(vector<tuple<int ,int, int> >::iterator it=remainders_vector_.begin();
                it!=remainders_vector_.end();++it)  {
	        if(it->get<0>()==0) //dare to remove this ?
		    continue;
	        if( uid / (it->get<0>())==it->get<1>()) {
                    SETTING_INFO("found in remainders.config:"<<it->get<2>());
                    return configs_.at(it->get<2>());
                }
	    }
        }
        //check range_map
        if(range_vector_.size()!=0) {
            for(vector<tuple<int ,int, int> >::iterator it=range_vector_.begin();
                it!=range_vector_.end();++it)  {
	        if(  uid >= (it->get<0>() )   &&  uid <= (it->get<1>())  ) {
                    SETTING_INFO("found in ranges.config:"<<it->get<2>());
                    return configs_.at(it->get<2>());
                }
	    }
        }
    } catch (const std::exception & e) {
        FEEDI_WARN(uid," TimeLineFeed's bug!! exception when get lru_timeout :"<<e.what());
        return default_config_;
    }
    return default_config_;
}
//string json="{\"default\":{\"lru_timeout\":432000,\"merge_stypes\":[1201,9801,9802],\"stat_path\":\"\"} }";
void TimeLineSetting::resetDefault()
{
    clear();
    default_config_.lru_timeout_=432000;
    default_config_.lru_maxsize_=10;
    default_config_.merge_stypes_.insert(1201);default_config_.merge_stypes_.insert(9801);default_config_.merge_stypes_.insert(9802);
    json_="{\"default\":{\"lru_timeout\":432000,\"merge_stypes\":[1201,9801,9802],\"stat_path\":\"\"} }";
}
void TimeLineSetting::clear()
{
    default_config_.lru_timeout_=432000;//this two value is fatal, set safe values for defence.
    default_config_.lru_maxsize_=10;
    default_config_.merge_stypes_.clear();
    stat_path_="";
    no_special_setting_=true;
    uid_map_.clear();
    remainders_vector_.clear();
    range_vector_.clear();
    configs_.clear();
    json_="";
}

// 
//  TODO Maybe hand write a customed json-lex-parser is a good choice to avoid boring code below.
// 
//  NOTICE !
//    when it's unhappy, jsoncpp will directly crash (no null returned and no exception throwed) 
//    So we need check every value type carefully and code here looks ugly.
//  
//   //0. wrong type will return false.
//   //1. restricted property missing will return false.
//   //2. wrong index will return false.
//   //3. some propertys are optional 
//   //4. other propertys will be ignored.
//   //5. white_list is prior to remainders is prior to  range. If an id belongs to two configs, the former one will make effect 
//   Json format: 
//   {
//       "stat_path":"/data/xce/XFeed/log/",      
//       "default" : { "lruT":123456,"lruS":0,"mstypes":[1,2,3,4,5] } ,  //use for normal user.
//       "configs" :     //optional,but not allow 0-size. if any type in it error .return false;
//       [
//         { "lruT":123456 ,"lruS":0,"mstypes":[1,2,3,4] } , 
//         { "lruT":123456 ,"lruS":0,"mstypes":[1,2,3,4] } , 
//         { "lruT":123456 ,"lruS":0,"mstypes":[1,2,3,4] } , 
//         { "lruT":123456 ,"lruS":0,"mstypes":[1,2,3,4] } , 
//         { "lruT":123456 ,"lruS":0,"mstypes":[1,2,3,4] }  
//       ]
//       "white_lists" : [ { "ids":[1,2,3],"idx":0 }, {"ids":[4,5,6],"idx":1  }]
//       
//       //"ids":[1,2,3], "idx":0 //if id already listed before return false, if idx wrong, return false;
//       
//       "remainders" : [
//           [1,2,3] , [4,5,6] , [7,8,9]
//       ]
//       "range" ;[
//          [100,101,3],[30,200,6]
//       ]
//   }
bool TimeLineSetting::loadJson(const string & json)
{ 
    #define RETURN_RESET(x) { resetDefault(); SETTING_INFO(x<<" give up loading!!!");return false;} 
    
    FEEDI_INFO(0," parse json: "<<json);
    Reader _reader; Value _root; stringstream ss;
    if ( ! _reader.parse(json, _root ) ) 
    {
        FEEDI_INFO(0," parse error."); return false;
    }
    clear();
    //check root .
    if( _root.type() !=objectValue )  {
         RETURN_RESET(" bad root type.give up."<<_root.type());
    }
    //check for "stat_path":
    const Value _stat_path = _root["stat_path"];
    if( _stat_path.type() != stringValue ) {
         RETURN_RESET("bad stat_path type:"<<_stat_path.type());
    }
    ss<<_stat_path.asString()<<"TimeLineFeedStatics"<<TimeLineFeedI::instance().mod_;
    stat_path_ =ss.str();
    ss.clear();ss.str("");
    //check for "default" :
    const Value _default = _root["default"];
    if( _default.type() != objectValue ) { 
         RETURN_RESET(" bad default type or no default.give up."<<_default.type());
    }
    //check for "default.lruT"
    const Value _default_lruT = _default["lruT"];
    if( _default_lruT.type() != intValue ) { 
         RETURN_RESET("In default,bad lruT type or no lruT."<<_default_lruT.type()); 
    }
    //set default.lruT
    default_config_.lru_timeout_=_default_lruT.asInt();
    //at least one day,too short timeout will be a disaster
    if (default_config_.lru_timeout_ < 60*60*24 ) default_config_.lru_timeout_=60*60*24; 
    
    const Value _default_lruS = _default["lruS"];
    if( _default_lruS.type() != intValue ) { 
         RETURN_RESET("In default,bad lruS type or no lruS."<<_default_lruS.type()); 
    }
    //set default.lruS
    default_config_.lru_maxsize_=_default_lruS.asInt();
    //
    if (default_config_.lru_maxsize_>0 && default_config_.lru_maxsize_<5 ) default_config_.lru_maxsize_=5; 
    
    //check for "default.mstypes"
    const Value _default_mstypes = _default["mstypes"];
    if( _default_mstypes.type() != arrayValue ) { 
         RETURN_RESET("In default,bad mstypes type or no mstypes."<<_default_mstypes.type()); 
    } 
    //set default.mstypes
    for(Value::iterator it= _default_mstypes.begin();it!=_default_mstypes.end();++it) 
    {
         if( (*it).type() != intValue) {
             RETURN_RESET("In default.mystypes, id type error."<<(*it).type());
         }
         default_config_.merge_stypes_.insert((*it).asInt());
    }
    // check for configs
    const Value _configs = _root["configs"];
    if( _configs.type() == nullValue ) 
        return true;
    if( _configs.type() != arrayValue ) { 
        RETURN_RESET("bad configs type."<<_configs.type());
    }
    SETTING_INFO("configs found.");
    // check for config size
    if( _configs.size() ==0)  
	RETURN_RESET("define configs, but zero size.");
    info temp_info;
    for(Value::iterator it= _configs.begin();it!=_configs.end();++it) 
    {
        if ( (*it).type() != objectValue ) { RETURN_RESET("In configs, bad type."<<(*it).type());}
        //check for lruT,S and mstypes typs
        const Value _tmp_lruT = ( (*it)["lruT"] );
        const Value _tmp_lruS = ( (*it)["lruS"] );
        const Value _tmp_mstypes= ( (*it)["mstypes"] );
        if( _tmp_lruT.type() != intValue) { RETURN_RESET("in configs, lruT is bad type."<<_tmp_lruT.type());}
        if( _tmp_lruS.type() != intValue) { RETURN_RESET("in configs, lruS is bad type."<<_tmp_lruS.type());}
        if( _tmp_mstypes.type() != arrayValue ) { RETURN_RESET("in configs, mstypes is bad type."<<_tmp_mstypes.type()); }
        //if( _tmp_mstypes.size() ==0 ) { RETURN_RESET("in configs, mstypes is 0-size."); }
        //insert  lru and msytypes
        temp_info.lru_timeout_= _tmp_lruT.asInt(); 
        temp_info.lru_maxsize_= _tmp_lruS.asInt();
        //limit value
        if(temp_info.lru_timeout_ < 60*60*24 )   temp_info.lru_timeout_=60*60*24;
        if(temp_info.lru_maxsize_ >0 && temp_info.lru_maxsize_<5 )   temp_info.lru_maxsize_=5;        
   
        temp_info.merge_stypes_.clear();
        for(Value::iterator it2=_tmp_mstypes.begin();it2!=_tmp_mstypes.end();++it2) {
            if( (*it2).type()!=intValue ) { RETURN_RESET("in configs.mystypes, item is not int type."<<(*it).type());}
            temp_info.merge_stypes_.insert( (*it2).asInt());
        }
        configs_.push_back(temp_info);
    }
    // check for lists
    uid_map_.clear();
    const Value _lists = _root["white_lists"];
    if( _lists.type() != nullValue ) //has white_lists.
    {
        SETTING_INFO("white_list found.");
        no_special_setting_=false;
        if(_lists.type() != arrayValue || _lists.size() == 0 ) {
          RETURN_RESET("white_lists is of bad type, 0-size.");
        }
        for(Value::iterator it= _lists.begin();it!=_lists.end();++it) {
           if( (*it).type() !=objectValue ) {
               RETURN_RESET("In white_lists, bad type."<<(*it).type());
            }
            const Value _tmp_idx = (*it)["idx"];
           if ( _tmp_idx.type() != intValue ) { RETURN_RESET("In white_lists,idx is bad type:"<<_tmp_idx.type());}
           int tmpInt=_tmp_idx.asInt();
           if(tmpInt < 0 || (size_t)tmpInt >= configs_.size() ) { RETURN_RESET("In white_lists,idx is outrange or negative. ");}
            const Value _tmp_ary= (*it)["ids"];
            if ( _tmp_ary.type() != arrayValue || _tmp_ary.size() ==0 ) { RETURN_RESET("In white_lists, ids is bad type, 0-size or no ids.type:"<<_tmp_ary.type()); }
           for(Value::iterator it2= _tmp_ary.begin();it2!=_tmp_ary.end();++it2 ) {
               if( (*it2).type()!=intValue ) { RETURN_RESET("In white_lists_ids, has a not int type."<<(*it2).type());}
               int tmpInt2= (*it2).asInt();
               if(uid_map_.find(tmpInt2) != uid_map_.end() ) { RETURN_RESET("id in ids already exist.");}
               //now insert
               uid_map_[tmpInt2]=tmpInt;
           } 
        }
    }
    // check for remainders
    const Value _remainders = _root["remainders"];
    if( _remainders.type()!=nullValue)
    {
        SETTING_INFO("remainders found.");
        no_special_setting_=false;
        if( _remainders.type()!=arrayValue || _remainders.size()==0) { RETURN_RESET("remainders is bad type or 0-size.");}
        for(Value::iterator it= _remainders.begin();it!= _remainders.end();++it)
        {
            if( (*it).type()!=arrayValue || (*it).size()!=3 ) { RETURN_RESET("In remainders, bad type or size !=3.type:"<<(*it).type());}
            const Value e0=(*it)[UInt(0)];const Value e1=(*it)[UInt(1)];const Value e2=(*it)[UInt(2)];
            if(e0.type()!=intValue||e1.type()!=intValue||e2.type()!=intValue) { RETURN_RESET("In remainders [],a not int type."<<e0.type());}
            if(e1.asInt()==0) { RETURN_RESET("In remainders,dividian can not be zero."); }
            if(e2.asInt()<0 || (size_t)e2.asInt() >= configs_.size() ) { RETURN_RESET("in remainders,idx is outrange or negative"); }
            //ok insert
            remainders_vector_.push_back(make_tuple<int,int,int>(e0.asInt(),e1.asInt(),e2.asInt()));
        }
    }
    //check for range
    const Value _range = _root["ranges"];
    if( _range.type()!=nullValue)
    {
        SETTING_INFO("range found.");
        no_special_setting_=false;
        if( _range.type()!=arrayValue || _range.size()==0 ) {RETURN_RESET("ranges is bad type or 0-size.");}
        for(Value::iterator it = _range.begin();it!= _range.end();++it)
        {
            if( (*it).type()!=arrayValue || (*it).size()!=3 ) { RETURN_RESET("In ranges, bad type or size !=3.");}
            const Value e0=(*it)[UInt(0)];const Value e1=(*it)[UInt(1)];const Value e2=(*it)[UInt(2)];
            if(e0.type()!=intValue||e1.type()!=intValue||e2.type()!=intValue) { RETURN_RESET("In ranges [],a not int type."<<e0.type());}
            if(e0.asInt() > e1.asInt() ) { RETURN_RESET("In ranges,the 1st can not be larger than the 2nd ."); }
            if(e2.asInt() <0 || (size_t)e2.asInt()>=configs_.size() ) { RETURN_RESET("in ranges,idx is outrange or negative."); }
            //ok insert
            range_vector_.push_back(make_tuple<int,int,int>(e0.asInt(),e1.asInt(),e2.asInt()));
        }
    }
    //set json
    json_=json;
    return true;
}
Ice::ObjectPtr TimeLineObjectFactoryI::create(Ice::Int uid) 
{
    TimeHelper th;
    string result;
    stringstream ss;
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    TimeLineFeedI::instance().readTimeLineDB(uid,result);
    NoBarrier_AtomicIncrement(&(TimeLineFeedI::instance().stat_.values_[kUserMiss]),1);
    TimeLineObjectPtr obj=new TimeLineObject;
    obj->timeLine_=result;
    MCE_INFO("TimeLineObjectFactoryI::create --> uid:"<<uid<<" timeline:"<<result);
    //other obj datas left to add when need
    return obj;
}
TimeLineFeedI::TimeLineFeedI()
{
    ///1 .  initialize command handler map
    handler_map_["log"]=&CommandHandler::LogLevel;
    handler_map_["stat_file"]=&CommandHandler::StatFile;
    handler_map_["load"]=&CommandHandler::LoadCache;
    handler_map_["fload"]=&CommandHandler::LoadFile;
    handler_map_["clear"]=&CommandHandler::ClearUserCache;
    handler_map_["cfeed"]=&CommandHandler::QueryFeedInCache;
    handler_map_["cmonth"]=&CommandHandler::QueryMonthInCache;
    handler_map_["csize"]=&CommandHandler::QueryFeedSize;
    handler_map_["ltml"]=&CommandHandler::QueryTimeLineInCache;
    handler_map_["ctml"]=&CommandHandler::QueryTimeLineInCache;
    handler_map_["dfeed"]=&CommandHandler::QueryFeedInDB;
    handler_map_["dmonth"]=&CommandHandler::QueryMonthInDB;
    handler_map_["dtml"]=&CommandHandler::QueryTimeLineInDB;
    handler_map_["rbdtml"]=&CommandHandler::RebuildTimeLine;
    handler_map_["cache_size"]=&CommandHandler::QueryCacheSize;
    handler_map_["cache_set"]=&CommandHandler::SetCacheSize;
    handler_map_["show"]=&CommandHandler::Show;
    handler_map_["stat"]=&CommandHandler::Stat;
    handler_map_["help"]=&CommandHandler::Help;
    handler_map_["lru"]=&CommandHandler::Lru;
    //handler_map_["lru_timeout"]=&CommandHandler::LruTimeout;
    handler_map_["setting_reload"]=&CommandHandler::SettingReload;
    handler_map_["setting_show"]=&CommandHandler::SettingShow;
    handler_map_["setting"]=&CommandHandler::Setting;
    handler_map_["test"]=&CommandHandler::Test;

    TimeLineSetting::default_config_.lru_timeout_=432000;
    TimeLineSetting::default_config_.merge_stypes_.insert(1201);
    TimeLineSetting::default_config_.merge_stypes_.insert(9801);
    TimeLineSetting::default_config_.merge_stypes_.insert(9802);
}

bool TimeLineFeedI::Initialize(int mod,const string & load_file, const string & stat_file)
{
    ///1 .  set mod
    mod_=mod;
    FEEDI_INFO(0," preload_file path:"<<load_file<<" mod:"<<mod<<"stat_file path:"<<stat_file);
    ///2 .  check and start load file task; save stat_file path and start statics task.
    if(!load_file.empty())
    {
        TaskManager::instance().execute(new LoadCacheByFileTask(load_file,"task",10,mod));//we only load user with this mod.
    }
    TimeLineFeedI::instance().timerPtr_ = new IceUtil::Timer();
    TimeLineFeedI::instance().timerPtr_->scheduleRepeated(new TimeLineStaticsTask(),IceUtil::Time::seconds(600));//10 min
    ///3 .  set default setting value
    setting_[0].resetDefault();
    setting_[1].resetDefault();
    ///4 .  load data from DB to setting_0  and use it.
    NoBarrier_Store( & (setting_index_),0);
    if(!loadSetting(0))
        return false;
    return true;
}

//interface functions

// cache&DB access of same user should be exclusive operation for consistency of cache&DB 
// 0.
//    locateObject is not thread-safe. When many threads call
//    locate in the beginning(this uid isn't in cache), it may create many objs,and only the last one is in cache,
//    (see evict::add() ) and locate just return us the temporary wrong ptr which isn't in cache.
//    And this cause threads for this id run without synchronism at all, which may break consistent of DB & cache .
//    Luckly, this case rarely rarely happens.
// 1.
//   Notice the DB latentcy between CDbWServer & CDbRServer ( SetRead*, SetWide, AddFeedMini, Modify ) which has brought 2 bugs yet.
//
/// \brief set style of a feed.
/// \param[in] uid id of the user.
/// \param[in] feed_year year of the feed
/// \param[in] feed id of the feed
/// \param[in] wide **0**: narrow style; **1**: wide style
/// \return **true**: success; **false**: can't find this feed or database exception.
bool TimeLineFeedI::SetWideScreen(int uid,int feed_year,::Ice::Long feed, bool wide,const Ice::Current& current) 
{
    /// 1 . stat count.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kSwNum]),1);
    stringstream ss;
    ss << " uid:"<<uid<<" feed_year: " << feed_year << " feed: " << feed << " wide: " << wide;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    /// 2 . lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    //FEEDI_INFO(uid," feed:"<<feed<<" wide"<<wide);
    /// 3 .  call \ref setFeedWide to do the job.
    return setFeedWide(ptr,uid,feed_year,feed,wide);
}
/// \brief retrieve style of a feed
/// only this func support feed_year not specified, required by qun.liu
/// \param[in] uid id of the user.
/// \param[in] feed_year year of the feed. -1 means not specified. 
/// \param[in] feed id of the feed.
/// \return  **true**: wide style ; **false**: narrow style.
bool TimeLineFeedI::IsWideScreen(int uid,int feed_year,::Ice::Long feed,const Ice::Current& current) 
{
    ///1 .  stat count.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kIwNum]),1);

    stringstream ss;
    ss << " uid:"<<uid<<" feed_year:" << feed_year << " feed:" << feed;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    //TimeCost t=TimeCost::create(ss.str(),LOG_LEVEL_INFO);
    ///2 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    FEEDI_INFO(uid," feed:"<<feed);
    ///3 .  call \ref isFeedWide do the job.
    return isFeedWide(ptr,uid,feed_year,feed);
}
/// \brief Get year-month information of an user's timeline.
/// \param[in] uid user's id
/// \return year-month information result of UidYear struct type.
/// \see TimeLine.ice
UidYear TimeLineFeedI::Query(int uid,const Ice::Current& current)
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(QueryTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kQNum]),1);

    stringstream ss;
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    ss << " uid:"<<uid;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    //TimeCost t=TimeCost::create(ss.str(),LOG_LEVEL_INFO);
    TimeLineObjectPtr ptr ;
    /// 2 .  lock mutex of the user.
    ptr= ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    FEEDI_INFO(uid," timeline:"<<ptr->timeLine_);
    UidYear result;
    /// 3 .  call \ref timeLineToUidYear
    timeLineToUidYear(ptr->timeLine_,result);
    //show(result,uid);
    return result;
}

/// \brief get a certain number of feeds of an user in a month from offset. won't be invoked currently.
/// \param[in] uid id of the user.
/// \param[in] year 
/// \param[in] month
/// \param[in] start skip nums from front, if start < 0 will be treated as start = 0
/// \param[in] limit the max num of feeds, if limit < 0 means no num limit 
/// \return feeds information result of TimeLineFeedResult struct type.
/// \see TimeLine.ice
TimeLineFeedResult TimeLineFeedI::QueryFeedInfo(int uid,int year,int month,int start,int limit,const Ice::Current& current) 
{
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    stringstream ss;
    ss << " uid:"<<uid<<" year:" <<year<<" month:"<<month << " start:"<<start<<" limit:"<<limit;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    /// 1 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    //FEEDI_INFO(uid," year:"<<year<<" month:"<<month<<" begin:"<<start<<" limit:"<<limit);
    TimeLineFeedResult result;result.size=0;
    /// 2 .  error handling.
    if( year < kMinYearAllow || year >kMaxYearAllow || month > 12 || month < 1 || is_future(year,month) ) {
        FEEDI_INFO(uid," date error! uid:"<<uid);
        timeLineRemove(ptr,uid,year,month);
        return result;
    }
    if( limit ==0 )
        return result;
    if( start <0 )
        start=0;
   
    /// 3 .  update LRU
    updateLru(ptr,uid,year,month);
    /// 4 .  call \ref getFeedsRangeMonth ,\ref rearrangeFeedsNew and return result.
    TimeLineFeedInfoSeq temp;
    time_iterator begin; time_iterator end;
    if( ! getFeedsRangeMonth(ptr,uid,year,month,begin,end) )
        return result;
    rearrangeFeedsNew(uid,set<int>(),begin,end,temp);
    //skip 
    TimeLineFeedInfoSeq::iterator endIter;
    if( limit < 0 || (size_t)(start + limit) > temp.size() )
        endIter=temp.end();
    else
        endIter=temp.begin() + start + limit;
    if( (size_t)start > temp.size() ) // or else "temp.begin() + start" will crash.
        return result;
    result.size=temp.size();
    result.feeds.assign( temp.begin() + start , endIter );
    show(result,uid);
    return result;
}
/// \brief Get a certain number of feeds of an user in a month from offset, filtered by stypes.
/// \param[in] uid id of the user.
/// \param[in] year 
/// \param[in] month
/// \param[in] stypes the stype filter set. MyUtil::IntSeq is the same as vector<int>
/// \param[in] start skip nums from front, if start < 0 will be treated as start = 0
/// \param[in] limit the max num of feeds, if limit < 0 means no num limit 
/// \return feeds information result of TimeLineFeedResult struct type.
/// \see TimeLine.ice
TimeLineFeedResult TimeLineFeedI::QueryFeedInfoByStype(int uid,int year,int month,const MyUtil::IntSeq& stypes,int start,int limit,const Ice::Current&  current) 
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(QfbsTimeHook,this,_1));
    stringstream ss; 
   
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kQfbsNum]),1);   
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);  
    ss << " uid:"<<uid<<" year:"<<year<<" month:"<<month<<" start:"<<start<<" limit:"<<limit;
    
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    CLEAR_SS();
    ss<<"type nums:"<<stypes.size()<<";  ";

    /// 2 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    
    TimeLineFeedResult result;result.size=0;
    for(MyUtil::IntSeq::const_iterator it=stypes.begin();it!=stypes.end();++it)
        ss<<*it<<",";
    /// 3 .  error handling
    FEEDI_INFO(uid," year:"<<year<<" month:"<<month<<" Stypes:("<<ss.str()<<")"<<" begin"<<start<<" limit:"<<limit);
    if( year<kMinYearAllow || year>kMaxYearAllow || month>12 || month<1 || is_future(year,month)) {
        FEEDI_INFO(uid," date error!");
        timeLineRemove(ptr,uid,year,month);
        return result;
    }
    if( limit ==0 )
        return result;
    if( start <0 )
        start=0;
    /// 4 .  update LRU
        updateLru(ptr,uid,year,month);
    /// 5 .  call \ref getFeedsRangeMonth ,\ref rearrangeFeedsNew and return result.
    
    set<int>  type_set (stypes.begin(),stypes.end());
    TimeLineFeedInfoSeq temp;
    time_iterator begin; time_iterator end;
    if( ! getFeedsRangeMonth(ptr,uid,year,month,begin,end) )
        return result;
    rearrangeFeedsNew(uid,type_set,begin,end,temp);//result in temp
    //skip 
    TimeLineFeedInfoSeq::iterator endIter;
    if( limit < 0 || (size_t)(start + limit) > temp.size() )
        endIter=temp.end();
    else
        endIter=temp.begin() + start + limit;
    if( (size_t)start > temp.size() ) // or else "temp.begin() + start" will crash.
        return result;
    result.size=temp.size();
    result.feeds.assign( temp.begin() + start , endIter );
    show(result,uid);
    return result;
}
/// \brief Get size of feeds of an user in a month. won't be invoked currently.
/// \param[in] uid id of the user.
/// \param[in] year 
/// \param[in] month
/// \return the size of feeds of the month.
int TimeLineFeedI::QueryFeedSize(int uid,int year,int month,const Ice::Current& current) 
{
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    stringstream ss;
    ss << " uid:"<<uid<<" year:" << year << " month:" << month;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();

    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());//TimeCost t=TimeCost::create(ss.str(),LOG_LEVEL_INFO);
    /// 1 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    FEEDI_INFO(uid," year:"<<year<<" month:"<<month);
    /// 2 .  error handling.
    if( year < kMinYearAllow || year >kMaxYearAllow || month > 12 || month < 1|| is_future(year,month)) {
        FEEDI_INFO(uid," date error! uid:");
        timeLineRemove(ptr,uid,year,month);
        return 0;
    }
    /// 3 .  update LRU
    updateLru(ptr,uid,year,month);

    /// 4 .  call \ref readFeedSize to get the size.
    size_t merge_num=0;
    size_t size=readFeedSize(ptr,uid,year,month,set<int>(),merge_num);
    size -= merge_num+1;//front_end page want us give the exact size of feeds those will show up.
    FEEDI_DEBUG(uid," size:"<<size);
    return size;
}
/// \brief Get size of feeds of an user in a month , filtered by stypes
/// \param[in] uid id of the user.
/// \param[in] year 
/// \param[in] month
/// \param[in] stypes the stype filter set. MyUtil::IntSeq is the same as vector<int>
/// \return the size of the feeds after filtered
int TimeLineFeedI::QueryFeedSizeByStype(int uid,int year,int month,const MyUtil::IntSeq& stypes,const Ice::Current& current) 
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kQsbsNum]),1);

    stringstream ss;
    ss << " uid:"<<uid<<" year: "<<year<<" month:"<<month;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());//TimeCost t=TimeCost::create(ss.str(),LOG_LEVEL_INFO);
    /// 2 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    FEEDI_INFO(uid," year:"<<year<<" month:"<<month);
    /// 3 .  error handling
    if( year<kMinYearAllow || year>kMaxYearAllow || month>12 || month<1 || is_future(year,month)) {
        FEEDI_INFO(uid," date error!");
        timeLineRemove(ptr,uid,year,month);
        return 0;
    }
    /// 4 .  update LRU
    updateLru(ptr,uid,year,month);

    set<int>  type_set (stypes.begin(),stypes.end());
    /// 5 .  call \ref readFeedSize to get the size.
    size_t merge_num=0;
    size_t size=readFeedSize(ptr,uid,year,month,type_set,merge_num);
    size-=merge_num+1;//front_end page want us give the exact size of feeds those will show up.
    FEEDI_DEBUG(uid," size:"<<size);
    return size;
}
/// \brief load all feeds of an user to cache , won't be invoked currently.
/// \param[in] uid id of the user.
void TimeLineFeedI::LoadFeedsToCache(int uid,const Ice::Current & current)  
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    stringstream ss;
    ss << " uid:"<<uid;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kLoadNum]),1);
    /// 2 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    //FEEDI_INFO(uid," ");
    /// 3 .  call \ref loadCache to load.
    loadCache(ptr,uid,0,0,true);
}

/// \brief Insert a new-added feed into cache and update timeline.A notify handler function called by FeedAssistN when it adds a new feed.
/// \param[in] seed original information of a feed.
/// \see RFeed.ice
/// \param[in] type type of the feed , may have been modified by FeedAssistN
/// \param[in] time time of the feed , may have been modified by FeedAssistN
void TimeLineFeedI::AddFeedMiniNotify(const FeedSeedPtr & seed,int type,Ice::Long time,const Ice::Current & current)
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(AddNotifyTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kAddNum]),1);

    stringstream ss;string time_str;makeTimeStr(time,time_str);
    ss << " uid:"<<seed->actor<<" stype:" << type2stype(type) << " time:" << time_str;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<seed->actor;
    th.preString(ss.str());//TimeCost t=TimeCost::create(ss.str(),LOG_LEVEL_INFO);
    //string time_str;makeTimeStr(time,time_str);

    //if user is not in cache, we only check if we need update timeline string???  
    // it's better we separate timeline string and feeds's ObjectCacheI.

    /// 2 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,seed->actor);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    //FEEDI_INFO(seed->actor," feed:"<<seed->feed<<" stype:"<<type2stype(seed->type)<<" time:"<<time_str<<" time_t:"<<time);
    int year=9999,month=12,day=31;
    Str2StrMap::iterator it;
    
    if( type2stype(type)==kStypeLifeEvent ) {
        NoBarrier_AtomicIncrement(&(stat_.values_[kLeNum]),1);
    }
    
    /// 3 .  call \ref timeLineAdd to add month when need.
    getYearMonthDay(time,year,month,day);
    //  4 .  update timeline
    timeLineAdd(ptr,seed->actor,year,month);
    int year_next,month_next;
    getNextMonth(year,month,year_next,month_next);
    time_t time1=getTime(year,month,1);
    time_t time2=getTime(year_next,month_next,1);
    size_t size=readFeedSizeCache(ptr,seed->actor,time1,time2);

    //new added
    if(size==0) {
        FEEDI_DEBUG(seed->actor," only update timeline.");
        return;
    }

    /// 6.  update LRU
    updateLru(ptr,seed->actor,year,month);

    /// 7 .  call \ref insertCache to insert the feed to cache.
    //insertCache(ptr,seed->actor,seed->feed,time,type,type2stype(type),seed->miniMerge);
    insertCache(ptr,seed->actor,seed->feed,time,type);  
    //if we add a feed to cache ,we need add all this month,or else QueryFeedSize won't work right
    /// 8 .  check if feeds of this month loaded, if not ,call \ref loadFeedInRange load it
    
    // new commented
    //if(size==0) {
    //    NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
    //    loadFeedInRange(ptr,seed->actor,year,month,1,0,0,0,year_next,month_next,1,0,0,0);
    //} else {
    //    NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
    //}
}

/// \brief A nofity handler function called when user modify date of a feed
/// \param[in] uid id of the user
/// \param[in] feed_year year of the feed
/// \param[in] feed id of the feed
/// \param[in] new_year year of the new date, should be valid. 
/// \param[in] new_month month of the new date, -1 means not specified, we treat it as 12, max value. 
/// \param[in] new_day day of the new date,-1 means not specified,we treat it as the max day of this month.
void TimeLineFeedI::Modify(int uid,int feed_year,Ice::Long feed, int new_year,int new_month,int new_day, const Ice::Current& current)
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kModifyNum]),1);

    stringstream ss;
    ss << " uid:"<<" feed_year:"<<feed_year<<" feed:"<<feed<<" new_year:"<<new_year<<" new_month:"<<new_month<<" new_day:"<<new_day;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());//TimeCost t=TimeCost::create(ss.str(),LOG_LEVEL_INFO);
    /// 2 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    /// 3 .  error handling and call \ref fix_date to fix the date to be a valid day. 
    FEEDI_INFO(uid," feed:"<<feed<<"feed_year"<<feed_year<<" year:"<<new_year<<" month:"<<new_month<<" day:"<<new_day);
    if(new_month==-1)  {
        new_month=12;new_day=31;
    }
    else if(new_day==-1) {
        new_day=31;
    }
    if(new_year<kMinYearAllow||new_year>kMaxYearAllow||new_month>12||new_month<1||new_day>31||new_day<1) {
        FEEDI_INFO(uid," date error!");
        return ;
    }
    fix_date(new_year,new_month,new_day);//

    /// 4 .  ensure the feeds of old month and feeds of new month are all in cache and get sizes of the each month to update timeline.
    ///  if the feed doesn't exist,return 
    //assure old month & new month both in cache
    feed_index_ref index = ptr->feedMap_.get<MIC_FEED_INDEX>();
    feed_iterator it = index.find(feed);
    if( it == index.end() ) {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
        FEEDI_DEBUG(uid," not in cache.we will load. fid"<<feed);
        int type;time_t time;Ice::Long merge;
        loadFeed(ptr,uid,feed_year>2009,feed,type,time,merge);//will load this month
        it=index.find(feed);
        if(it == index.end() ) {
            FEEDI_INFO(uid," this feed doesn't exist. fid"<<feed);
            return ;
        }
    } else {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
    }
    time_t old_time=it->time_;
    //old_time
    int old_year,old_month,old_day;
    getYearMonthDay(old_time,old_year,old_month,old_day);
    int old_year_next,old_month_next;
    getNextMonth(old_year,old_month,old_year_next,old_month_next);
    time_t old_month_begin=getTime(old_year,old_month,1);
    time_t old_month_end=getTime(old_year_next,old_month_next,1);
    size_t old_month_size=readFeedSizeCache(ptr,uid,old_month_begin,old_month_end);

    updateLru(ptr,uid,new_year,new_month);
    updateLru(ptr,uid,old_year,old_month);

    /// 5 .  if the new date is today, we use current time instead to make this feed newer than any other feeds at today, accords with user's expectation.
    time_t new_time=getTime(new_year,new_month,new_day);
    int year_now ,month_now ,day_now;
    getYearMonthDay(time(NULL),year_now,month_now,day_now);
    if( new_year== year_now && new_month== month_now && new_day==day_now) {
        new_time=time(NULL);
    }
    if(new_time == old_time) {//if newtime==oldtime , no need to modify ( it could cause DB update failed below)
        FEEDI_DEBUG(uid," old_time == new_time, return directly.");
        return ;
    }
    //load new month if need
    int new_year_next,new_month_next;
    getNextMonth(new_year,new_month,new_year_next,new_month_next);
    time_t new_month_begin=getTime(new_year,new_month,1);
    time_t new_month_end=getTime(new_year_next,new_month_next,1);
    size_t new_month_size=readFeedSizeCache(ptr,uid,new_month_begin,new_month_end);
    if(new_month_size==0) {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
        loadFeedInRange(ptr,uid,new_year,new_month,1,0,0,0,new_year_next,new_month_next,1,0,0,0);
        new_month_size=readFeedSizeCache(ptr,uid,new_month_begin,new_month_end);
    } else {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
    }
    /// 6 .  modify info of the feed in cache.
    node n=*it;
    n.time_=new_time;
    index.replace(it,n);

    /// 7 .  modify info of the feed in database. call \ref moveFeedDB
    /// if the new_time & old_time doesn't across 2010/1/1, we simply use a update sql.\n
    /// if yes, we retrieve the entire info of the feed ,then delete the feed from old table and insert it to the new table.
    moveFeedDB(uid,feed_year,new_year,new_time,feed);

    /// 8 .  add/remove timeline if need.
    if(old_month_size==1 ||old_month_size==0)
        timeLineRemove(ptr,uid,old_year,old_month);
    timeLineAdd(ptr,uid,new_year,new_month);
}
/// \brief mark a feed as read by deleting it in feed_mini and cache, won't be invoked currently & full of bugs 
/// \param[in] uid id of the user
/// \param[in] feed_year yearof the feed
/// \param[in] stype stype of the feed
/// \param[in] merge merge id of the feed
void TimeLineFeedI::SetReadMini(int uid,int feed_year,int stype, ::Ice::Long merge, const Ice::Current&)
{
    //TODO remove/rewrite someday.
    stringstream ss;
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    /// 1 .  lock mutex of the user.
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    FEEDI_INFO(uid,"  stype:"<<stype<<" merge:"<<merge<<"  feed_year:"<<feed_year);
    Statement sql;ss.clear();ss.str("");
    string table; GetFeedMiniTable(uid,table,feed_year>2009);
    const char * feed_db=GetFeedDBInstance(feed_year>2009);
    /// 2 .  get time and id of the feed. \n
    /// time is used to check if need to modify timeline.
    /// feed_id is used to remove feed in cache.
    const int kTimeIdx=0;const int kFeedIdx=1;
    ss<<"select time,feed from "<<table<<" where user = "<<uid<<" and small_type = "<<stype<<" and mini_merge = "<<merge;
    sql<<ss.str();
    mysqlpp::StoreQueryResult res;
    try {
        res = QueryRunner(feed_db, CDbRServer,table).store(sql);
    }catch(mysqlpp::Exception& e) {
        FEEDI_WARN(uid, "  mysqlException: "<<e.what());
        return;
    }
    if(!res || res.num_rows()==0) {
        FEEDI_INFO(uid," can't find feed with merge:"<<merge<<" stype:"<<stype);
        feed_index_ref index = ptr->feedMap_.get<MIC_FEED_INDEX>();
        index.erase(merge);//for consistency
        return ;
    }
    int temp_time=(time_t) mysqlpp::DateTime(res.at(0)[kTimeIdx]);
    Ice::Long feed=res.at(0)[kFeedIdx];
    
    /// 3 .  modify timeline if need.
    int year,month,day;
    getYearMonthDay(temp_time,year,month,day);
    size_t merge_num=0;
    size_t size=readFeedSize(ptr,uid,year,month,set<int>(),merge_num);
    size -=merge_num+1;//  we must rewrite this func entirely. it's totally a mess.
    if( size==1 || size==0 ) {// removeTimeline
        //FEEDI_DEBUG(uid, " remove timeline");
        timeLineRemove(ptr,uid,year,month);
    } else {
        FEEDI_DEBUG(uid, " not none, no need to remove");
    }
    /// 4 .  remove feed in cache
    feed_index_ref index=ptr->feedMap_.get<MIC_FEED_INDEX>();
    FEEDI_DEBUG(uid," remove from cache.feed:"<<feed);
    if(index.erase(feed)==0) {
        FEEDI_INFO(uid," remove from cache failed!!");
    }
    /// 5 .  remove feed in database.
    CLEAR_SS_SQL();
    ss<<"delete from "<<table<<" where user="<<uid<<" and small_type="<<stype<<" and mini_merge="<<merge;
    FEEDI_DEBUG(uid, " sql"<<ss.str());
    sql<<ss.str();
    mysqlpp::SimpleResult simple_res = QueryRunner(feed_db, CDbWServer,table).execute(sql);
    if(simple_res.rows()==0) {
        FEEDI_INFO(uid, " delete failed.");
    }
}
/// \brief mark several feeds in same year as read by deleting it in DB and cache, won't be invoked currently.\n
/// if a feed will be removed along with removing another feed,don't put them all in the param feeds,
/// this will cause a side-effect query database operation once for each feed.see \ref deleteFeed.
/// \param[in] uid id of the user
/// \param[in] feed_year year of these feeds.
/// \param[in] feeds ids of these feeds.MyUtil::LongSeq is the same as vector<int64_t> 
void TimeLineFeedI::SetReadMiniByIds(int uid, int feed_year, const MyUtil::LongSeq & feeds, const Ice::Current &)
{
    /// 1 .  lock mutex of the user.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    stringstream ss;
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    CLEAR_SS();
    foreach(Ice::Long feed,feeds) {
        ss<<feed<<",";
    }
    ss<<")";
    FEEDI_INFO(uid, " feed year:"<<feed_year<<" feeds:"<<ss.str());
    /// 2 .  call \ref deleteFeed for each feed.
    foreach(Ice::Long feed,feeds)
    {
        deleteFeed(ptr,uid,feed_year,feed,true);
    }
}
/// \brief mark a feed as read by deleting it in DB and cache.
/// \param[in] uid id of the user.
/// \param[in] feed_year year of the feed.
/// \param[in] feed id of the feed.
void TimeLineFeedI::SetReadMiniById(int uid,int feed_year,::Ice::Long feed,const Ice::Current & current)
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kSrbiNum]),1);
    stringstream ss;
    ss << " uid:"<<uid<<" feed_year:"<<feed_year<<" feed:"<<feed;
    InvokeClient tm = InvokeClient::create(current, ss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    CLEAR_SS();
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());
    /// 2 .  lock mutex of the user
    TimeLineObjectPtr ptr = ServiceI::instance().locateObject<TimeLineObjectPtr>(TIMELINE_CATEGORY,uid);
    IceUtil::Mutex::Lock lock(ptr->mutex_);
    /// 3 .  call \ref deleteFeed 
    FEEDI_INFO(uid, " feed year:"<<feed_year<<" feed:"<<feed);
    deleteFeed(ptr,uid,feed_year,feed,true);
}
/// \brief execute a command
/// \param[in] uid id of the user.
/// \param[in] cmd command line
/// \return the result string
string TimeLineFeedI::Execute(int uid,const string & cmd, const Ice::Current&)
{
    /// 1 .  stat count.
    TimeHelper th(false);th.function(bind(OthersTimeHook,this,_1));
    NoBarrier_AtomicIncrement(&(stat_.values_[kInvokeNum]),1);
    NoBarrier_AtomicIncrement(&(stat_.values_[kExeNum]),1);
    stringstream ss;
    ss<<__FUNCTION__<<":"<<uid;
    th.preString(ss.str());//TimeCost t=TimeCost::create(ss.str(),LOG_LEVEL_INFO);
    FEEDI_INFO(uid, " args:"<<cmd);
    vector<string> args;
    /// 2 .  split the commandline and invoke command handler.
    /// we will not lock mutex here but inside each handler when need;
    split(args,cmd,is_any_of(","),token_compress_on);
    if(handler_map_.find(args[0])==handler_map_.end()) {
        return string("bad command.");
    }
    try {
        return ((&handler_)->*handler_map_[args[0]])(uid,args,cmd);
    } catch(const std::exception &e) {
        stringstream ss;
        ss<<"An std::exception raises:"<<e.what(); 
        return ss.str();
    } catch(...) {
        return "An unkown exception raises,pls check code.";
    }
}



/*
bool TimeLineFeedI::needMerge(int uid,int stype)
{
    return setting_used().config(uid).merge_stypes_.count(stype);
}*/

//heavy !
void TimeLineFeedI::merge_stypes(int uid, std::set<int> & stypes)
{
    stypes=setting_used().config(uid).merge_stypes_;
}
//heavy !
int TimeLineFeedI::lru_maxsize(int uid)
{
    return setting_used().config(uid).lru_maxsize_;
}
//heavy !
time_t TimeLineFeedI::lru_timeout(int uid)
{
    return setting_used().config(uid).lru_timeout_;
}
void TimeLineFeedI::stat_path(string &path)
{
    path=setting_used().stat_path_;
}
/// \brief load user's timeline to cache
void TimeLineFeedI::loadTimeLine(TimeLineObjectPtr & ptr,int uid)
{
    readTimeLineDB(uid,ptr->timeLine_);
}

// 
int TimeLineFeedI::lru_size(TimeLineObjectPtr & ptr,int uid)
{
    return ptr->lru_.list_.size(); 
}

/// \brief add a month to timeline for an user, if this month exists ,won't write database;
/// \param ptr cache pointer of the user
/// \param uid id of the user
/// \param year year of the month
/// \param month the month
void TimeLineFeedI::timeLineAdd(TimeLineObjectPtr & ptr,int uid,int year, int month)
{
    /// 1 .  call \ref timeLineToMap \n
    /// 2 .  call map[year].insert(month) \n
    /// 3 .  call \ref mapToTimeLine \n
    /// 4 .  if same return ,else call \ref writeTimeLineDB and update timeline in cache. 
    FEEDI_DEBUG(uid," year:"<<year<<" month:"<<month);
    string newStr;
    if(inTimeLine(ptr->timeLine_,year,month))
        return ;
    YearMonthMap temp_map;
    timeLineToMap(ptr->timeLine_,temp_map);
    temp_map[year].insert(month);
    mapToTimeLine(temp_map,newStr);
    if(ptr->timeLine_ == newStr){
        return ;
    }
    if(! writeTimeLineDB(uid,newStr) )//update DB
        return ;
    FEEDI_DEBUG(uid, " old  "<<ptr->timeLine_<<" new:"<<newStr);
    ptr->timeLine_=newStr;//update cache after DB sucessed
}
/// \brief remove a month from timeline for an user, if this month doesn't exist ,won't write database;
/// \param ptr cache pointer of the user
/// \param uid id of the user
/// \param year year of the month
/// \param month the month
void TimeLineFeedI::timeLineRemove(TimeLineObjectPtr & ptr,int uid,int year, int month)
{
    /// 1 .  call \ref timeLineToMap \n
    /// 2 .  call map[year].erase(month) \n
    /// 3 .  call \ref mapToTimeLine \n
    /// 4 .  if same return ,else call \ref writeTimeLineDB and update timeline in cache.
    FEEDI_DEBUG(uid, " year:"<<year<<" month:"<<month);
    string newStr;
    if(!inTimeLine(ptr->timeLine_,year,month))
        return ;
    YearMonthMap temp_map;
    timeLineToMap(ptr->timeLine_,temp_map);
    set<int> & temp_set= temp_map[year];
    if(temp_set.erase(month)==0){//none is remove
        return;
    }
    mapToTimeLine(temp_map,newStr);
    if(ptr->timeLine_==newStr) {
        return ;
    }
    if(! writeTimeLineDB(uid,newStr) )//update DB
        return ;
    FEEDI_DEBUG(uid, " old:"<<ptr->timeLine_<<" new:"<<newStr);
    ptr->timeLine_=newStr;//update cache after DB sucessed
}


/// \brief sort,filter and merge feeds
/// for merging stype, we store index of first element with this stype , and push_back later elems with this stype here     
/// for feed sorting, when we found feeds with the same time, we put them in a set(order by feed id DESC),
/// and dump back to vector when they ends.
/// we have to use twice loops currently, and use temp vector<iterator> instead of vector<node> to reduce copy-consumption,
/// 1st loop we do feed sorting and stype filter, 2nd loop we do stype merging
/// \param[in] stype_filter stypes we need
/// \param[in] begin start of the these feeds
/// \param[in] end end of the these feeds
/// \param[out] result timeline string
void TimeLineFeedI::rearrangeFeedsNew(int uid,const set<int> & stype_filter,time_iterator begin, time_iterator end,TimeLineFeedInfoSeq & result)
{
    set<int> _merge_stypes;
    merge_stypes(uid,_merge_stypes);

    map<int,int> merge_stype_map; //for storing first elem's index of every stype we need merge
    set<time_iterator,TimeIteratorFeedGreater> temp_set;//for feed sorting 
    vector<time_iterator> temp;
    time_iterator next_it;
    //next_it=begin;  ++next_it;//so urgly,in boost::multiIndex, it seems that  no next() method or begin+1  available;
    #define NEXT_IT(a,b) do{b=a;++b;}while(0)
   
    ///1 .  feed_sort & stype_filter
    while(begin!=end)
    {
        //if(! stype_filter.count(begin->stype_) ) {
        if(stype_filter.size()!=0 && ! stype_filter.count(  type2stype(begin->type_) ) ) {
            ++begin;
            continue;
        }
        NEXT_IT(begin,next_it);
        if( next_it==end || next_it->time_ != begin->time_ )//depends on short circuit evaluation,or else crash
        {
            temp.push_back(begin);
            ++begin;
            continue;
        } else {
            temp_set.clear();
            do
            {
                if(stype_filter.size()!=0 && ! stype_filter.count( type2stype( begin->type_) ) ) {
                   ++begin;
                   NEXT_IT(begin,next_it);
                } else {
                   temp_set.insert(begin);
                   ++begin;
                   NEXT_IT(begin,next_it);
                }
            }while( next_it != end && next_it ->time_ == begin->time_ );
            temp_set.insert(begin);
            ++begin;
        }
        copy(temp_set.begin(),temp_set.end(),back_inserter(temp));//dump back to vector
    }
    TimeLineFeedInfo info;//temp feeds containner
    ///2 .  then stype_merge
    foreach(time_iterator t, temp)
    {
        if(_merge_stypes.count(type2stype( t->type_) ) )
        {
            map<int,int>::iterator it=merge_stype_map.find(type2stype(t->type_));
            if(it==merge_stype_map.end())
            {
                //merge_stype_map[t->stype_]=result.size();
                merge_stype_map[type2stype(t->type_)]=result.size();   
                info.feeds.clear();info.flag=0; info.feeds.push_back(t->feed_);//if merged, it's narrow style.
                result.push_back(info);
            }else {
                int i=it->second;
                result[i].feeds.push_back(t->feed_);
            }
        } else {
            info.feeds.clear();info.flag=isWide(t->type_); info.feeds.push_back(t->feed_);
            result.push_back(info);
        }
    }
    #undef NEXT_IT
} 


/// \brief make time_t in beijing(+8) from a tm struct
/// system mktime is unbelievable slow,we use this function instead. 
/// \param[in] t the tm struct 
/// \return time_t value
time_t mktime_bj(struct tm *t)
{
    return mktime(t);
}
/// \brief get time value of 00:00:00 on a date 
/// \param[in] year year of the date
/// \param[in] month month of the date
/// \param[in] day day of the date
/// \return time_t value
time_t getTime(int year,int month,int day,int hour,int min,int sec) 
{
    struct tm t;
    t.tm_year=year-1900;
    t.tm_mon=month-1;//-1,0-based
    t.tm_mday=day;
    t.tm_hour=hour;t.tm_min=min;
    t.tm_sec=sec;t.tm_isdst=0;
    return mktime_bj(&t);
}
/// \brief get year, month ,day value of a time
/// \param[in] t the time.
/// \param[out] year store the year
/// \param[out] month store the month
/// \param[out] day store the day
void getYearMonthDay(time_t t,int & year,int & month, int &day)
{
    struct tm * temp=localtime(&t);
    year=temp->tm_year+1900;
    month=temp->tm_mon+1;
    day=temp->tm_mday;
}
/// \brief get year,month,day,hour, min,sec value of a time
/// \param[in] t the time.
/// \param[out] year store the year
/// \param[out] month store the month
/// \param[out] day store the day
/// \param[out] hour store the hour
/// \param[out] min store the minute
/// \param[out] sec store the second
void getDayTime(time_t t,int &year,int &month,int & day,int &hour,int &min,int &sec)
{
    struct tm * temp=localtime(&t);
    year=temp->tm_year+1900;
    month=temp->tm_mon+1;
    day=temp->tm_mday;
    hour=temp->tm_hour;
    min=temp->tm_min;
    sec=temp->tm_sec;
}
/// \brief get the range of the month of the time
/// \param[in] time the time
/// \param[out] start start of the month
/// \param[out] end end of the month
void getMonthRange(time_t time,time_t & start,time_t & end)
{
    int year,month,day,year_next,month_next;
    getYearMonthDay(time,year,month,day);
    getNextMonth(year,month,year_next,month_next);
    start=getTime(year,month,1);
    end=getTime(year_next,month_next,1);
}

/// \brief fix a date to be valid,and it will be limited to curren time if it's future
///e.g  02/31/2000 -> 02/29/2000 , 04/31/2000 -> 04/30/2000
/// \param[in,out] year year of the date
/// \param[in,out] month month of the date
/// \param[in,out] day day of the date
void fix_date(int & year,int & month,int &day)
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

/// \brief check if the month is future
/// \param[in] year  year of the month
/// \param[in] month the month
/// \return **true** if the month is future, **false** if not
bool is_future(int year,int month)
{
    int _year,_month,_day;
    getYearMonthDay(time(NULL),_year,_month,_day);
    if( year>_year) return true;
    if( year==_year && month > _month) return true;
    return false;
}
/// \brief build a readable string of the time_t
/// \param[in] t the time
/// \param[out] s store the output string
void makeTimeStr(time_t t,string & s)
{
    char buf[100];
    tm* local_time = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local_time);
    s=buf;
}
/// \brief check if part of string are digits
/// \param[in] str the string
/// \param[in] begin index we start from to check 
/// \param[in] len the num of char to check
/// \return **true** they are all digit,**false** some of them isn't digit
bool checkDigit(const string & str,size_t begin, size_t len)
{
    while(len) {
        --len;
        if( str[begin+len] < '0' || str[begin+len] > '9' ) {
            return false;
        }
    }
    return true;
}
////////////////////
// Timeline Format//
////////////////////
//
// format:   :Y1m1m2m3m4m5:Y2m1m2m3:Y3m1.....
// detail: 1. Y -->  year, 4 chars, 1900-2099
//                         ordered ASC, distinct
//         2. m -->  month, 2 chars, 01-12, ordered ASC, distinct in a year.
//
// UidYear: map<year ,vector<month> >  for return value in iterface 
// YearMonthMap: map<year, set<month> > for read/write timeline string
//
//  currently support :yy too
//

/// \brief check if a month already in timeline
/// \param[in] tml the timeline string
/// \param[in] year year of the month
/// \param[in] month the month
/// \return **true** if timeline has the month, else **false**
bool inTimeLine(const string &tml,int year, int month)
{
    stringstream ss;
    ss<<":"<<year;
    size_t pos=tml.find(ss.str());
    if(pos==string::npos) {
        return false;
    }
    pos+=5;//:yyyy
    while(pos<=tml.size()-2)
    {
        if(!checkDigit(tml,pos,2)) {
            return false;
        }
        if(month == ((tml[pos]-'0')*10+tml[pos+1]-'0') )
            return true;
        pos+=2;
    }
    return false;
}
/// \brief build  a UidYear from a timeline string
/// \param[in] str the timeline string
/// \param[out] result the result map
void timeLineToUidYear(string & str, UidYear& result)
{
    size_t pos=0;int year,month; set<int> month_set;
    size_t pos_limit_year2= str.size()-2;//need 3 char available :YY
    size_t pos_limit_month= str.size()-1;//need 2 char available
    size_t pos_limit_year4= str.size()-4;//need 4 char available :YYYY
    result.clear();
    while( pos < pos_limit_year2 &&  pos!=string::npos) {
        //1st. find year
        if( ( pos=str.find(':',pos) ) !=string::npos ) {
            if( !checkDigit(str,pos+1,2) ) { //:y1y2 check y1y2
                pos+=1;//skip :  e.g ::20120102 
                continue;
            }
            year = (str[pos+1]-'0')*10 + str[pos+2]-'0';
            if(year!=19 && year != 20) {//:yy format
                if(year<50)  
                    year += 2000;
                else  
                    year += 1900;
                pos+=3;//jumps to month
            } else { //:yyyy format
                if(pos>=pos_limit_year4)
                    return;//the remains not enough for a vaild :yyyy
                if(!checkDigit(str,pos+3,2) ) {//:y1y2y3y4, check y3y4
                    pos+=3;//e.g : :19:20120201  we skip :19
                    continue;
                }
                year =(str[pos+1]-'0')*1000;
                year +=(str[pos+2]-'0')*100;
                year +=(str[pos+3]-'0')*10;
                year +=(str[pos+4]-'0');
                pos+=5;//jumps to month
            }
            if( year < kMinYearAllow || year > kMaxYearAllow) {
                continue;//discard this damned year
            }
            //2nd. add month
            month_set.clear();
            while(pos < pos_limit_month && str[pos] != ':' && str[pos+1] !=':') { 
                if(!checkDigit(str,pos,2)) {
                     pos+=2;
                     continue;
                }
                month = (str[pos]-'0')*10 + str[pos+1]-'0';
                pos+=2; 
                if( month<1 || month>12 )
                    continue;
                month_set.insert(month);
            }
            if(month_set.size()!=0)
                result[year].assign(month_set.begin(),month_set.end());
        }
        else
           break;
    }
}
/// \brief build  a YearMonthMap from a timeline string
/// \param[in] str the timeline string
/// \param[out] result the result map
void timeLineToMap(string & str, YearMonthMap & result)
{
    size_t pos=0;int year,month;
    size_t pos_limit_year2= str.size()-2;//need 3 chars available :YY
    size_t pos_limit_month= str.size()-1;//need 2 chars available
    size_t pos_limit_year4= str.size()-5;//need 5 chars available :YYYY
    result.clear();
    while( pos < pos_limit_year2 && pos!=string::npos) {
        //1st. find year
        //cout<<"try find year"<<endl;
        if( ( pos=str.find(':',pos) ) !=string::npos ) {
            //cout<<"found year"<<endl;
            if( !checkDigit(str,pos+1,2) ) {
                //cout<<"check 2 digit fail. pos jumps to:"<<pos+3<<endl;
                //cout<<str[pos+1]<<str[pos+2]<<endl;
                pos+=1;//skip : , e.g ::20120102
                continue;
            }
            //cout<<"check 2 digit ok"<<endl;
            year = (str[pos+1]-'0')*10 + str[pos+2]-'0';
            //cout<<"2 digit:"<<year<<endl;
            if(year!=19 && year != 20) {//:yy format
                if(year<50)  
                    year += 2000;
                else  
                    year += 1900;
                pos+=3;//jump to month
                //cout<<"final year value:"<<year<<endl;
            } else { //:yyyy format
                //cout<<":yyyy format"<<endl;
                if(pos>=pos_limit_year4) {
                    //cout<<"not enough for a valid :yyyy"<<endl;
                    return;//the remains not enough for a vaild :yyyy
                }
                if(!checkDigit(str,pos+3,2) ) {//:y1y2y3y4, check y3y4
                    //cout<<"check y3y4 digit failed, pos jumps to:"<<pos+3;
                    pos+=3;//e.g  :19:20120201  we skip :19
                    continue;
                }
                year =(str[pos+1]-'0')*1000;
                year +=(str[pos+2]-'0')*100;
                year +=(str[pos+3]-'0')*10;
                year +=(str[pos+4]-'0');
                pos+=5;//jump to month
                //cout<<"final year value:"<<year<<endl;
            }
            if( year < kMinYearAllow || year > kMaxYearAllow) {
                //cout<<"year is bad value"<<endl;
                continue;//discard this damned year
            }
            set<int> & months=result[year];
            while(pos < pos_limit_month && str[pos] != ':' && str[pos+1] !=':') { 
                 if(!checkDigit(str,pos,2)) {
                     pos+=2;
                     continue;
                 }
                 month = (str[pos]-'0')*10 + str[pos+1]-'0';
                 pos+=2;
                 //cout<<"found a month value"<<month<<endl;
                 if( month<1 || month>12 )
                     continue;
                 //cout<<"in year:"<<year<<"insert a month"<<endl;
                 months.insert(month);
            }
            if(result[year].size()==0)//no month at all 
                 //cout<<"no month in this year:"<<year<<",remove it"<<endl;
                 result.erase(year);
        }
        else
           break;
    }
}
/// \brief build a timeline from a YearMonthMap
/// \param[in] year_month the map
/// \param[out] str store the result timeline
void mapToTimeLine(YearMonthMap & year_month, string & str)
{
    str.clear();str.reserve(BEST_TL_LEN);//default 15 is always too short;
    int year,month;bool has_month;
    for( YearMonthMap::iterator it = year_month.begin(); it != year_month.end(); ++it ) {
        year = it->first;
        if( year<kMinYearAllow || year>kMaxYearAllow)
            continue;
        str.push_back(':');
        str.push_back( year/1000 + '0');
        year=year%1000;
        str.push_back( year/100 + '0');
        year=year%100;
        str.push_back( year/10 + '0');
        str.push_back( year%10 + '0');
        has_month=false;
        //add month
        for(YearMonthMap::value_type::second_type::iterator it2 = it->second.begin(); it2!= it->second.end(); ++it2) {
            month=*it2;
            if( month >= 1 && month <= 12 ) {
                has_month=true;
                str.push_back( month/10 + '0');
                str.push_back( month%10 + '0');
            }
        }
        if(!has_month) {
            str.erase(str.size()-5,str.size());//remove :YYYY,  this year contains no valid month.
        }
    }
}
/// \brief output a TimeLineFeedResult with \ref FEEDI_DEBUG
/// \param[in] res 
/// \param[in] uid id of the user,for purpose we will grep it in log file 
void show(TimeLineFeedResult res,int uid)
{
    stringstream ss;
    ss<<"total size:("<<res.size<<") return size:("<<res.feeds.size()<<") (";
    foreach(TimeLineFeedInfo & f ,res.feeds) {
        ss<<"( ";
        foreach(Ice::Long feed,f.feeds){
            ss<<feed<<",";
        }
        ss<<" )";
        ss<<"["<<f.flag<<"]"<<",";
    }
    ss<<")";
    FEEDI_DEBUG(uid, " "<<ss.str());
}
/// \brief output a UidYear with \ref FEEDI_DEBUG
/// \param[in] m 
/// \param[in] uid id of the user,for purpose we will grep it in log file 
void show(UidYear & m,int uid) {
    typedef UidYear::value_type pair_t ;
    stringstream ss;
    ss<<"years:"<<m.size()<<" ";
    foreach(pair_t & p, m){
        ss<<p.first<<" ";
        ss<<"size: ("<<p.second.size()<<") (";
        foreach(int i,p.second) {
            ss<<i<<",";
        }
        ss<<") "<<" ; ";
    }
    FEEDI_DEBUG(uid, " "<<ss.str());
}
}
}
}

