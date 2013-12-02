///////////////////////////////////////////////////////////
//
//       Title: DbHelper.cpp
//       Athor: Feed
//  CreateTime: 08/28/2012
//       Email: zhi.lin@renren-inc.com
// Description:  
//              1. implement of TimeLineFeedI class 's Cache operation
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
//using namespace xce::feed::timeline;
using namespace xce::subtle;
using namespace Ice;


#define foreach BOOST_FOREACH

namespace xce { 
namespace feed { 
namespace timeline { 




/// \fn bool xce::feed::timeline::TimeLineFeedI::setFeedWide(TimeLineObjectPtr & ptr,int uid,int feed_year,::Ice::Long feed,bool wide)
/// \brief set style of a feed
/// \param[in] ptr cache pointer of the user
/// \param[in] uid id of the user
/// \param[in] feed_year year of the feed
/// \param[in] feed id of the feed
/// \param[in] wide style to set, <b>true</b> wide , <b>false</b> narrow
/// \return <b>true</b> if succeed, <b>false</b> if the feed doesn't exist or database exception.
bool TimeLineFeedI::setFeedWide(TimeLineObjectPtr & ptr,int uid,int feed_year,::Ice::Long feed,bool wide)
{
    //1st. update cache & get old_type 
    int old_type=0;time_t temp_time;Ice::Long merge;
    feed_index_ref index=ptr->feedMap_.get<MIC_FEED_INDEX>();
    feed_iterator it=index.find(feed);
    if( it!=index.end() ) {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
        old_type=it->type_;
        node n=*it;
        n.type_=makeWide(old_type,wide);
        FEEDI_DEBUG(uid," update cache:"<<bitset<32>(makeWide(old_type,wide)) );
        index.replace(it,n);
    } else {//feed not in cache
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
        if(!loadFeed(ptr,uid,feed_year,feed,old_type,temp_time,merge)) {
            return false;
        }
        it=index.find(feed);
        if(it!=index.end() ) {
            node n=*it;
            n.type_=makeWide(old_type,wide);
            FEEDI_DEBUG(uid," update cache:"<<bitset<32>(makeWide(old_type,wide)) );
            index.replace(it,n);
        }
    }
    //2nd. update DB
    return setFeedWideDB(uid,feed_year,feed,old_type,wide);
}


/// \brief remove all feedsin cache with the specified stype in a month
/// \param[in] ptr cache pointer of the user .
/// \param[in] uid id of the user.
/// \param[in] stype the stype to remove
/// \param[in] start begin of the month
/// \param[in] end end of the month
/// \param[out] feeds the feeds have been removed. 
/// \return only true currently.
bool TimeLineFeedI::removeFeedsByStypeCache(TimeLineObjectPtr& ptr,int uid,int stype,time_t start,time_t end,std::vector<node> & feeds)
{
    time_index_ref index =ptr->feedMap_.get<MIC_TIME_INDEX>();
    std::pair<time_iterator,time_iterator> p=index.range(lambda::_1<end,start<=lambda::_1);
    while(p.first!=p.second) {
        //if( p.first->stype_==stype ) {
        if( type2stype( p.first->type_) ==stype ) {      
            feeds.push_back(*(p.first));
            p.first=index.erase(p.first);
        } else {
            ++p.first;
        }
    }
    return true;
}
/// \brief delete a feed, in extra mode,will delete feeds with same stype if the stype is one need to merge .
/// \param[in] ptr cache pointer of the user 
/// \param[in] uid id of the user.
/// \param[in] feed_year year of the feed
/// \param[in] feed id of the feed
/// \param[in] extra indicate extra mode
/// \return  <b>true</b> if succeed, <b>false</b> if feed doesn't exist in database or database exception
bool TimeLineFeedI::deleteFeed(TimeLineObjectPtr& ptr,int uid,int feed_year,::Ice::Long feed,bool extra)
{
    /// 1 .  ensure month of this feed has been loaded.
    set<int> _merge_stypes;
    merge_stypes(uid,_merge_stypes);

    stringstream ss;
    time_t temp_time; int stype; ::Ice::Long merge;
    feed_index_ref index=ptr->feedMap_.get<MIC_FEED_INDEX>();
    feed_iterator it=index.find(feed);
    if(it!=index.end()) {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
        //stype=it->stype_;
        stype=type2stype(it->type_);
        //merge=it->merge_;
        temp_time=it->time_;
    } else {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
        int type;
        if(!loadFeed(ptr,uid,feed_year>2009,feed,type,temp_time,merge)) {
            FEEDI_INFO(uid, "no this feed.feed year:"<<feed_year<<" feed:"<<feed);
            return false;
        }
        //stype=it->stype_;
        stype=type2stype(it->type_);
        it=index.find(feed);
    }
    //we assume that all feeds of this month are in cache.code below depends on it
    time_t start,end;
    int year,month,day;
    getYearMonthDay(temp_time,year,month,day);
    getMonthRange(temp_time,start,end);
    /// 2 .  remove feed in cache and call \ref deleteFeedByIdDB. 
    /// if in extra mode, when stype is one need to merge we call \ref removeFeedsByStypeCache and then \ref deleteFeedByIdDB for each feed
    /// 3 .  remove month in timeline if need
    if(extra && _merge_stypes.count(stype) )
    {//find all this stype in this month.
        vector<node> feed_nodes;
        removeFeedsByStypeCache(ptr,uid,stype,start,end,feed_nodes);//remove cache
        size_t size=readFeedSizeCache(ptr,uid,start,end);//remove timeline if need
        if( size==0 ) {
            timeLineRemove(ptr,uid,year,month);
        }
        foreach(node & n,feed_nodes) {
            deleteFeedByIdDB(uid,feed_year,n.feed_);//remove DB
        }
    }
    else {
        index.erase(it);//remove cache
        size_t size=readFeedSizeCache(ptr,uid,start,end);//remove timeline if need
        if( size==0 ) {
            timeLineRemove(ptr,uid,year,month);
        }
        deleteFeedByIdDB(uid,feed_year,feed);//remove DB
    }
    return true;
}
/// \brief insert a feed into cache.
/// \param[in] ptr cache pointer of the user.
/// \param[in] uid id of the user
/// \param[in] feed id of the feed
/// \param[in] time time of the feed
/// \param[in] type type of the feed
/// \return only true currently
bool TimeLineFeedI::insertCache(TimeLineObjectPtr& ptr,int uid, Ice::Long feed,time_t time, int type)//,int stype, Ice::Long merge)
{
    //stype_merge_index_ref index = ptr->feedMap_.get<MIC_STYPE_MERGE_INDEX>();
    feed_index_ref index = ptr->feedMap_.get<MIC_FEED_INDEX>();
    //if( !(index.insert(node(feed,time,type,stype,merge)).second)  ) {
    if( !(index.insert(node(feed,time,type)).second)  ) {
        //stype_merge_iterator it=index.find(make_tuple<int,Ice::Long>(stype,merge));
        //if(it!=index.end()) {//check which one is newer.
        //    if(it->time_>time)
        //        return true;// the one in cache is newer.
        //    index.erase(it);
        //    //re-insert
        //    if( !(index.insert(node(feed,time,type,stype,merge)).second) ) {
                string str;makeTimeStr(time,str);
                FEEDI_DEBUG(uid," insert to cache failed.feed:"<<feed<<" stype:"<<type2stype(type)<<" time:"<<str);
        //    }
        //}
    }
    return true;
}


/// \brief get num of feeds in a month in cache
/// \param ptr cache pointer of the user
/// \param uid id of the user
/// \param start_time start of the month
/// \param end_time end of the month
/// \return num of feeds
size_t TimeLineFeedI::readFeedSizeCache(TimeLineObjectPtr & ptr,int uid,time_t start_time,time_t end_time)
{
    time_index_ref index =ptr->feedMap_.get<MIC_TIME_INDEX>();
    std::pair<time_iterator,time_iterator> p=index.range(lambda::_1<end_time,start_time<=lambda::_1);
    return distance(p.first,p.second);
}
/// \brief get num of feeds in a month filtered by stypes, will load feeds to cache when need
/// \param[in] ptr cache pointer of the user
/// \param[in] uid id of the user
/// \param[in] year year of the month
/// \param[in] month the month
/// \param[in] type_set the filter stypes
/// \param[out] merge_num num of feeds with merge-stype
/// \return num of feeds after filter
size_t TimeLineFeedI::readFeedSize(TimeLineObjectPtr & ptr,int uid, int year, int month,const std::set<int> & type_set,size_t & merge_num)
{
    /// 1 .  try retrieve from cache \n
    /// 2 .  call \ref loadFeedInRange if this month in cache is empty \n
    /// 3 .  try retrieve from cache again. 
    set<int> _merge_stypes;
    merge_stypes(uid,_merge_stypes);


    time_t begin,end;
    merge_num=0;
    int year_next,month_next;
    getNextMonth(year,month,year_next,month_next);
    begin=getTime(year,month,1);
    end=getTime(year_next,month_next,1);
    //Notice that time is ordered DESC , so (_1<end,begin<=_1) rather than (begin<=_1,_1<end)
    size_t size=0;
    time_index_ref index =ptr->feedMap_.get<MIC_TIME_INDEX>();
    std::pair<time_iterator,time_iterator> pair1=index.range(lambda::_1<end,begin<=lambda::_1);
    if(pair1.first!=pair1.second)  {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
        if(type_set.size()==0) {
            for(;pair1.first!=pair1.second && pair1.first!=index.end() ;++pair1.first)  {
                ++size;
                if(_merge_stypes.count(type2stype(pair1.first->type_) ))
                {
                    ++merge_num;
                }
            }
        } else {
            for(;pair1.first!=pair1.second && pair1.first!=index.end();++pair1.first) {
                //if(type_set.count( pair1.first->stype_ ) ) {
                if(type_set.count( type2stype(pair1.first->type_ ) ) ) {
                    ++size;
                    if(_merge_stypes.count(type2stype( pair1.first->type_) ) )
                    {
                        ++merge_num;
                    }
                }
            }
        }
        timeLineAdd(ptr,uid,year,month);
        return size;
    } else  {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
    }
    loadFeedInRange(ptr,uid,year,month,1,0,0,0,year_next,month_next,1,0,0,0);
    std::pair<time_iterator,time_iterator> pair2=index.range(lambda::_1<end,begin<=lambda::_1);
    if(pair2.first!=pair2.second)  {
        if(type_set.size()==0) {
            for(;pair2.first!=pair2.second && pair2.first!=index.end() ;++pair2.first)  {
                ++size;
                if(_merge_stypes.count(type2stype(pair1.first->type_) ) )
                {
                    ++merge_num;
                }
            }
        } else {
            for(;pair2.first!=pair2.second && pair2.first!=index.end();++pair2.first) {
                if(type_set.count( type2stype( pair2.first->type_ ) ) ) {
                    ++size;
                    if(_merge_stypes.count(type2stype( pair1.first->type_) ) )
                    {
                        ++merge_num;
                    }
                }
            }
        }
        timeLineAdd( ptr,uid,year,month);
        return size;
    }
    return 0;
}
bool TimeLineFeedI::getFeedsRangeMonth(TimeLineObjectPtr & ptr, int uid, int year, int month,time_iterator& begin_iter,time_iterator& end_iter)
{
    time_t begin,end;
    int year_next,month_next;
    getNextMonth(year,month,year_next,month_next);
    begin=getTime(year,month,1);
    end=getTime(year_next,month_next,1);
    time_index_ref index=ptr->feedMap_.get<MIC_TIME_INDEX>();
    //Notice that time is ordered DESC , so (_1<end,begin<=_1) rather than (begin<=_1,_1<end)
    std::pair<time_iterator,time_iterator> pair1=index.range(lambda::_1<end,begin<=lambda::_1);
    if(pair1.first!=pair1.second)  {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
        begin_iter=pair1.first;
        end_iter=pair1.second;
        //timeLineAdd(ptr,uid,year,month);
        return true;
    } else {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
    }
    //FEEDI_DEBUG(uid," load from DB to cache.");
    //load frome DB
    if(!loadFeedInRange(ptr,uid,year,month,1,0,0,0,year_next,month_next,1,0,0,0))
        return false;
    //read from cache
    std::pair<time_iterator,time_iterator> pair2=index.range(lambda::_1<end,begin<=lambda::_1);
    begin_iter=pair2.first;
    end_iter=pair2.second;
    if(pair2.first==pair2.second)  {
        timeLineRemove(ptr,uid,year,month);
    }
    return true;
}


/// \brief set style of a feed
/// \param[in] ptr cache pointer of the user
/// \param[in] uid id of the user
/// \param[in] feed_year year of the feed,can be -1, required by qun.liu
/// \param[in] feed id of the feed
/// \return <b>true</b> if feed is wide, <b>false</b> if the feed is narraw or the feed doesn't exist or database exception
bool TimeLineFeedI::isFeedWide(TimeLineObjectPtr & ptr,int uid,int feed_year,::Ice::Long feed)
{
    feed_index_ref index=ptr->feedMap_.get<MIC_FEED_INDEX>();
    feed_iterator it=index.find(feed);
    if( it!=index.end() ) {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthHit]),1);
        return  isWide(it->type_);
    } else {
        NoBarrier_AtomicIncrement(&(stat_.values_[kMonthMiss]),1);
    }
    int type; time_t time;Ice::Long merge;
    if(feed_year!=-1) { //if feed_year ==-1 , read mini / mini_bc both
        loadFeed(ptr,uid,feed_year>2009,feed,type,time,merge);
    } else {
        loadFeed(ptr,uid,true,feed,type,time,merge);
        loadFeed(ptr,uid,false,feed,type,time,merge);
    }
    it=index.find(feed);
    if( it!=index.end() ) {
        return isWide(it->type_);
    } else {
        return false;
    }
}

/// \brief load feeds to cache
/// \param[in] ptr cache ptr of a user
/// \param[in] uid id of the user
/// \param[in] year year of the month ,if year==0 , will load all feeds of the user
/// \param[in] month the month
/// \param[in] tml whether retrieve timeline from db too.
/// \return **true** if succeed, **false** if database exception.
bool TimeLineFeedI::loadCache(TimeLineObjectPtr& ptr,int uid,int year,int month,bool tml)
{
    if(year!=0) {
        int year_next;int month_next;
        getNextMonth(year,month,year_next,month_next);
        loadFeedInRange(ptr,uid,year,month,1,0,0,0,year_next,month_next,1,0,0,0);
        updateLru(ptr,uid,year,month,true);
        return true;
    }
    loadAllFeeds(ptr,uid);
    //update month lru.
    if(tml) {
        readTimeLineDB(uid,ptr->timeLine_);
    }
    //update lru by timeline
    YearMonthMap temp;
    timeLineToMap(ptr->timeLine_,temp);
    typedef YearMonthMap::value_type pair_t ;
    foreach(pair_t & p, temp) {
        foreach(int i,p.second) {
            updateLru(ptr,uid,p.first,i,false);
        }
    }
    updateLru(ptr,uid,0,0,true);//do evict
    return true;
}
/// \brief clear feeds of the month in cache
/// \param[in] ptr cache ptr of a user
/// \param[in] uid id of the user
/// \param[in] year year of the month
/// \param[in] month the month
void TimeLineFeedI::clearMonthCache(TimeLineObjectPtr & ptr,int uid,int year,int month)
{
    FEEDI_DEBUG(uid, " "<<year<<","<<month);
    time_t begin,end;
    int year_next,month_next;
    getNextMonth(year,month,year_next,month_next);
    begin=getTime(year,month,1);
    end=getTime(year_next,month_next,1);
    time_index_ref index=ptr->feedMap_.get<MIC_TIME_INDEX>();
    //Notice that time is ordered DESC , so (_1<end,begin<=_1) rather than (begin<=_1,_1<end)
    std::pair<time_iterator,time_iterator> pair1=index.range(lambda::_1<end,begin<=lambda::_1);
    index.erase(pair1.first,pair1.second);
}

/// \brief update LRU struct of a user
/// \param[in] ptr cache ptr of a user
/// \param[in] uid id of the user
/// \param[in] year year of the month
/// \param[in] month the month
/// \param[in] evict should evict timeout month
void TimeLineFeedI::updateLru(TimeLineObjectPtr & ptr,int uid,int year,int month,bool evict)
{
    
    map<int,timeline_lru::little> & map_ = ptr->lru_.map_;
    list<int> & list_ = ptr->lru_.list_;
    time_t current=time(NULL);
    int temp=year*100+month;
    if(temp !=0) {
        map<int,timeline_lru::little>::iterator it=map_.find(temp);
        if( it == map_.end() ) {
            timeline_lru::little lit;
            list_.push_front(temp);
            lit.last_time_ = current;
            lit.list_iter_ = list_.begin();
            map_.insert(make_pair<int,timeline_lru::little>(temp,lit));
            FEEDI_DEBUG(uid, " update:("<<year<<","<<month<<"), new , cur size:"<<list_.size() );
        } else {
            it->second.last_time_=current;
            list_.erase(it->second.list_iter_);
            list_.push_front(it->first);
            it->second.list_iter_=list_.begin();
            FEEDI_DEBUG(uid, " update:("<<year<<","<<month<<"), old , cur size:"<<list_.size() );
        }
    }
    if(!evict)
        return;
    list<int>::iterator list_it;
    map<int,timeline_lru::little>::iterator map_it;
    int time_out_ ;
    time_out_= lru_timeout(uid);
    while( list_.size()!=0 )
    {
        list_it=--list_.end();
        map_it= map_.find(*list_it);
        if( map_it == map_.end() )
        {//means bug !
            FEEDI_DEBUG(uid,"not found, evcit "<<*list_it/100<<","<<*list_it%100);
            list_.erase(list_it);
            continue;
        }
        if( current - map_it->second.last_time_ >= time_out_ )
        {
            FEEDI_DEBUG(uid,"timeout evcit "<<*list_it/100<<" "<<*list_it%100);
            clearMonthCache(ptr,uid,*list_it/100,*list_it%100);
            list_.erase(list_it);
            map_.erase(map_it);
            continue;
        } else {
            break;
        }
    }
    //size lru
    int evict_size;
    evict_size= lru_maxsize(uid);
    if(evict_size <= 0 )
        return ;
    evict_size = list_.size() - evict_size;
    if(evict_size<=0)
        return ;
    while( evict_size >0 && list_.size()!=0)
    {
        list_it= --list_.end();
        map_it= map_.find(*list_it);
        if( map_it == map_.end() )
        {
            FEEDI_DEBUG(uid,"not found, evcit "<<*list_it/100<<","<<*list_it%100);
            list_.erase(list_it);
            --evict_size;
            continue;
        }
        else{
            FEEDI_DEBUG(uid,"size evcit: "<<*list_it/100<<" "<<*list_it%100);
            clearMonthCache(ptr,uid,*list_it/100,*list_it%100);
            list_.erase(list_it);
            map_.erase(map_it);
            --evict_size;
            continue;
        }
    }
}
/// \brief clear LRU struct of a user
/// \param[in] ptr cache ptr of a user
/// \param[in] uid id of the user
void TimeLineFeedI::resetLru(TimeLineObjectPtr & ptr,int uid)
{
    ptr->lru_.map_.clear();
    ptr->lru_.list_.clear();
}

}
}
}
