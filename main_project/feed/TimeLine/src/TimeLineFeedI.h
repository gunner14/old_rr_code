///////////////////////////////////////////////////////////
//
//       Title: TimeLineFeedI.h
//       Athor: Feed
//  CreateTime: 05/01/2012
//       Email: zhi.lin@renren-inc.com
// Description: declare of TimeLineFeedI class 
//              declare of cache data class: node, TimeLineObject, 
//
//
//
#ifndef _TIMELINEFEEDI_H_
#define _TIMELINEFEEDI_H_
#include <sstream>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/tuple/tuple.hpp>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "TimeLineFeed.h"
#include "feed/base_feed/atomicops.h"
#include "CommandHandler.h"
#include <string>
#include <vector>
#include <set>
#include "feed/base_feed/atomicops.h"  //for Atomic64 method in inline func
#include "TimeLineInternal.h"

namespace xce { 
namespace feed { 
namespace timeline { 

// 5 days, ok ?    
#define TIMELINE_LRU_EXPIRE_DEFAULT 60*60*24*5

/// \brief TimeLineFeed service setting 
class TimeLineSetting
{
public:
    struct info {
        /// \var lru_timeout_
        /// \brief timeout value used in evicting month 
        time_t lru_timeout_;
        /// \var lru_size_
        /// \brief max size used in evicting month
        int lru_maxsize_;
        /// \var merge_stypes_
        /// \brief the same stype in the month will be merged.
        std::set<int> merge_stypes_;

        friend class TimeLineFeedI;
    };
    // operations of timelinesetting.
    bool loadJson(const string & json);
    const info& config(int uid);
    void resetDefault();
    void clear();

    /// \var default_config_
    /// \brief "default" in json.
    static info default_config_;

    /// \var no_special_setting_
    /// \brief whether setting has no special process,we use this to accelerate when there are no special setting.
    bool no_special_setting_;
    /// \var uid_map_
    /// \brief store the id need special process
    std::map<int,int> uid_map_; // <uid , the config value index>
    /// \var ramainders_map_
    /// \brief store the rmainder need special process 
    std::vector<boost::tuple<int ,int, int > > remainders_vector_;//<divider_remainder, the config value index>
    /// \var range_map_
    /// \brief store the range need special process
    std::vector<boost::tuple<int ,int, int > > range_vector_;

    //store the setting value   
    std::vector<info> configs_;

    /// \var statics_path_ 
    /// \brief statics task will save data in this file
    std::string stat_path_;

    /// \var json
    /// \brief json string 
    std::string json_;
};


/// \brief month LRU struct for an user
class timeline_lru
{
public:
    void update_month(int year,int month);
    class little
    {
    public:
        time_t last_time_;
        list<int>::iterator list_iter_;
    };
    std::map<int,little> map_;
    list<int> list_;
};

/// \brief data struct represent a feed in cache
class node
{
public:
    node(::Ice::Long f,int time,int type):
        feed_(f),time_(time),type_(type){}
    ::Ice::Long feed_;
    time_t time_;/// order by DESC
    int type_;
};

/// \brief searching container for feeds of an user
typedef boost::multi_index_container<
    node,
    boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::member<node,::Ice::Long,&node::feed_> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::member<node,time_t,&node::time_>,std::greater<time_t> >
                                  >
    > FeedMap;

#define MIC_FEED_INDEX  0
#define MIC_TIME_INDEX  1

typedef FeedMap::nth_index<MIC_FEED_INDEX>::type & feed_index_ref ;
typedef FeedMap::nth_index<MIC_TIME_INDEX>::type & time_index_ref ;
typedef FeedMap::nth_index<MIC_FEED_INDEX>::type::iterator feed_iterator;
typedef FeedMap::nth_index<MIC_TIME_INDEX>::type::iterator time_iterator;

class TimeIteratorFeedGreater
{
public:
    bool operator()(const xce::feed::timeline::time_iterator & it1,const xce::feed::timeline::time_iterator & it2)
    {
        return it1->feed_ > it2->feed_;
    }
};



#define TIMELINE_CATEGORY 1

/// \brief ObjectCacheI object represent an user
class TimeLineObject : public ::Ice::Object
{
public:
    timeline_lru lru_;
    std::string timeLine_;
    FeedMap feedMap_;
    ::IceUtil::Mutex mutex_;
    friend class TimeLineObjectFactoryI;
    friend class TimeLineFeedI;
    friend class CommandHandler;
};
typedef IceUtil::Handle<TimeLineObject> TimeLineObjectPtr;

/// \brief ObjectCache object factory
class TimeLineObjectFactoryI : public MyUtil::ServantFactory 
{
public:
    TimeLineObjectFactoryI() {}
    virtual ~TimeLineObjectFactoryI() {}
    virtual Ice::ObjectPtr create(Ice::Int userid);
};
/// \class TimeLineFeedI 
/// \brief TimeLineFeed implement class
class TimeLineFeedI : public TimeLineFeed,public MyUtil::Singleton<TimeLineFeedI>
{
public:
    TimeLineFeedI();
    bool Initialize(int mod,const string & load_file, const string & stat_file);
	/// @name ICE interface 
	/// the implement of <timeline.ice>'s interface  provided by TimeLineFeedI.
	/// @{
    UidYear Query(int uid,const Ice::Current&);
    TimeLineFeedResult QueryFeedInfoByStype(int uid,int year,int month,const MyUtil::IntSeq& types,int begin,int limit,const Ice::Current&);
    TimeLineFeedResult QueryFeedInfo(int uid,int year,int month,int begin,int limit,const Ice::Current&);
    int QueryFeedSize(int uid,int year,int month,const Ice::Current&);
    int QueryFeedSizeByStype(int uid,int year,int month,const MyUtil::IntSeq& types,const Ice::Current&);
    bool IsWideScreen(int uid,int feed_year, ::Ice::Long feed, const Ice::Current&);
    bool SetWideScreen(int uid,int feed_year, ::Ice::Long feed, bool wide, const Ice::Current&);
    //void AddFeedMini(const FeedSeedPtr &,const Ice::Current &);
    void AddFeedMiniNotify(const FeedSeedPtr & seed ,int type, ::Ice::Long time, const Ice::Current & );
    void Modify(int uid,int feed_year, Ice::Long feed, int year,int month,int day, const Ice::Current& );
    void SetReadMini(int uid, int feed_year, int stype, ::Ice::Long merge, const Ice::Current&);
    void SetReadMiniByIds(int uid, int feed_year, const MyUtil::LongSeq & feeds, const Ice::Current &);
    void SetReadMiniById(int uid, int feed_year, ::Ice::Long feed, const Ice::Current &);
    void LoadFeedsToCache(int uid,const Ice::Current &);
    std::string Execute(int uid,const string & cmd,const Ice::Current &);
	/// @}
        /// \var stat_
	/// \brief statistics data of TimeLineFeedI
    TimeLineStatistics stat_;
private:
	/// @name Member variable 
	/// data fileds of TimeLineFeedI
	/// @{

        /// \var mod
	/// \brief the mod (0-9) of the server.
    size_t mod_;	
	/// \var handler_
	/// \brief a method class handle commands in \ref Execute, no data member currently
    CommandHandler handler_; 
        /// \var setting_index_
        /// \brief a pingpong switch to choose which setting_ to use. 
    xce::subtle::Atomic64 setting_index_;
        /// \var setting_reload_mutex_
        /// \brief use it when you 1. need reload  2 acess ilde settings store 3. use a store for a long time. 
    ::IceUtil::Mutex setting_reload_mutex_;
	/// \var setting_
	/// \brief setting of TimeLineFeedI
    TimeLineSetting setting_[2];

	/// \var timerPtr_
	/// \brief TimerPtr of TimeLineStaticsTask
    IceUtil::TimerPtr timerPtr_;
	/// \var handler_map_
	/// \brief store the relationship of command and it's handler function.
    std::map<string,string (CommandHandler::*)(int uid,const std::vector<std::string> & args,const std::string &)> handler_map_;
	/// @}

private:
        /// @name Cache function
	/// cache related function
	/// @{
    void rearrangeFeedsNew(int uid,const set<int> &, time_iterator begin, time_iterator end,TimeLineFeedInfoSeq & result);
    void timeLineAdd(TimeLineObjectPtr & ptr, int uid,int year,int month);
    void timeLineRemove(TimeLineObjectPtr & ptr, int uid,int year, int month);
    void loadTimeLine(TimeLineObjectPtr & ptr,int uid);

    bool removeFeedsByStypeCache(TimeLineObjectPtr& ptr,int uid,int stype,time_t start,time_t end,std::vector<node> & feed_nodes);
    bool deleteFeed(TimeLineObjectPtr& ptr,int uid,int feed_year,::Ice::Long feed,bool extra);
    bool isFeedWide(TimeLineObjectPtr & ptr,int uid,int feed_year,::Ice::Long feed);
    bool setFeedWide(TimeLineObjectPtr & ptr,int uid,int feed_year,::Ice::Long feed, bool wide);
    bool insertCache(TimeLineObjectPtr& ptr,int uid, Ice::Long feed,time_t time,int type);
    bool loadCache(TimeLineObjectPtr& ptr,int uid,int year=0,int month=0,bool tml=false);
    void clearMonthCache(TimeLineObjectPtr & ptr,int uid,int year,int month);
    bool readFeedInfo(TimeLineObjectPtr & ptr,int uid, int year, int month,const std::set<int> & s,vector<node> & v,size_t & all_size,size_t & merge_num);
    size_t readFeedSizeCache(TimeLineObjectPtr & ptr,int uid,time_t start_time,time_t end_time);
    size_t readFeedSize(TimeLineObjectPtr & ptr,int uid, int year, int month,const std::set<int> & type_set,size_t & merge_num);
    bool getFeedsRangeMonth(TimeLineObjectPtr & ptr, int uid, int year, int month,time_iterator& begin_iter,time_iterator& end_iter);
    
    
    ///@}

    //DB operation
    /// @name DB function
    ///  database related function
    /// @{
    bool loadSetting(int index);
    bool reloadSetting();
    bool moveFeedDB(int uid,int old_year,int new_year,time_t new_time,::Ice::Long feed);
    bool setFeedWideDB(int uid,int feed_year,::Ice::Long feed,int old_type,bool wide);
    bool deleteFeedByIdDB(int uid,int feed_year,::Ice::Long feed);
    bool deleteFeedByMergeDB(int uid,int feed_year,int stype,::Ice::Long merge);
    bool rebuildTimeLine(TimeLineObjectPtr& ptr,int uid,std::string & timeline);
    bool readTimeLineDB(int uid, std::string & result);
    bool writeTimeLineDB(int uid,const std::string & timeline);
    bool loadFeed(TimeLineObjectPtr & ptr,int uid,bool after2009,::Ice::Long feed,int &type,time_t & time,::Ice::Long &merge);
    //doesn't support for range across 2010/01/01. only use to load month data now
    bool loadFeedInRange(TimeLineObjectPtr & ptr,int uid,int start_year,int start_month,int start_day,int start_hour,int start_min,int start_sec,
         int end_year,int end_month,int end_day,int end_hour,int end_min,int end_sec);
    bool loadAllFeeds(TimeLineObjectPtr& ptr,int uid);
    /// @}

    /// @name other function
    ///   other helper function
    /// @{
    string JsonToSetting(int index,const string & str);
    void updateLru(TimeLineObjectPtr & ptr,int uid,int year,int month,bool evict=true);
    void resetLru(TimeLineObjectPtr & ptr,int uid);
    inline void switch_setting_idx() {//not completely thread-safe,we will protect it in interface
        xce::subtle::NoBarrier_Store(&setting_index_,setting_idx_ilde());
    }
    // setting function
    inline TimeLineSetting & setting_used() { return setting_[xce::subtle::NoBarrier_Load(&setting_index_)];}
    inline TimeLineSetting & setting_ilde() { return setting_[(1-(int)xce::subtle::NoBarrier_Load(&setting_index_))];}
    inline int setting_idx_used() { return xce::subtle::NoBarrier_Load(&setting_index_); }
    inline int setting_idx_ilde() { return (1-(int)xce::subtle::NoBarrier_Load(&setting_index_)) ; }
    //bool needMerge(int uid,int stype);//heavy ,use merge_stypes when frequent.
    time_t lru_timeout(int uid);//heavy 
    int lru_maxsize(int uid);//heavy 
    int lru_size(TimeLineObjectPtr& ptr,int uid);
    void merge_stypes(int uid, std::set<int> & );
    void  stat_path(std::string & path);
    /// @}




    friend class TimeLineObjectFactoryI;
    friend class CommandHandler;
    friend class LoadCacheTask;
    friend class LoadCacheByFileTask;
    friend class TimeLineStaticsTask;
    friend class TimeLineSetting;
};
}
}
}
#endif
