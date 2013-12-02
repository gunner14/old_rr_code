#ifndef CountFeedI_h_
#define CountFeedI_h_ 


#include "TimeLineFeed.h"
#include <Ice/Ice.h>
#include <string.h>
#include <sstream>
#include <cstring>
#include <set>
#include "util/cpp/InvokeMonitor.h"

using namespace std;
using namespace ::xce::feed::timeline;
namespace xce{
namespace feed{
namespace timeline{

const string FeedDBInstance = "feed_timeline";
const int DBCluster = 100;

class TimeLineFeedI : public TimeLineFeed,public MyUtil::Singleton<TimeLineFeedI> {
  public:
    void test(::Ice::Long feed,const Ice::Current&);//不对外部使用
    bool isWideScreen(int uid,::Ice::Long feed,const Ice::Current&);//判断是否宽屏
    void CancelWideScreen(int uid,::Ice::Long feed,const Ice::Current&);//取消宽屏
    void SetWideScreen(int uid,::Ice::Long feed,const Ice::Current&);//设置宽屏显示
    UidYear query(int uid,const Ice::Current&); //返回是map,key是年,value是多个月份
    void insert(int uid,const string& file,const Ice::Current&);//不对外部使用
    void Count(int uid,const Ice::Current&);//不对外部使用
    
    //查询uid,year(2012),month(5)
    FeedIdseq QueryFeedId(int uid,int year,int month,int begin,int limit,const Ice::Current&);
    int QueryFeedSize(int uid,int year,int month,const Ice::Current&);//查询uid，某年(2011),某月(12)有多少条feed
    int QueryFeedSizeByStype(int uid,int year,int month,const IntSeq& types,const Ice::Current&);//查询uid，某年(2011),某月(12)stype有多少条feed
    void modify(int uid,::Ice::Long feed,int year,int month = 12, int day = 31, const Ice::Current& current = ::Ice::Current());//还没有写好不要使用 
    //void del(int uid,long long feedid,const Ice::Current&);
    //void add(int uid,long long feedid,const Ice::Current&);
    //查询uid,year(2012),month(5),stype
    FeedIdseq QueryFeedIdBystype(int uid,int year,int month,const IntSeq& types,int begin,int limit,const Ice::Current&);
  private:
    void get_feed_content_tablename(::Ice::Long feed,string &tablename,string& dbname) {
      ostringstream os;
      os << "feed_content_" << feed%DBCluster;
      tablename=os.str();
      dbname="feed_db";
    }
    void get_feed_mini_tablename(int uid,string &tablename,string& dbname) {
      ostringstream os;
      os << "feed_mini_" << uid%DBCluster;
      tablename=os.str();
      dbname="feed_db";
    }
    time_t getTime(int year,int month,int day) {
      struct tm t;
      time_t t_of_day;
      t.tm_year=year-1900;
      t.tm_mon=month-1;   //-1 
      t.tm_mday=day;
      t.tm_hour=0;
      t.tm_min=0;
      t.tm_sec=0;
      t.tm_isdst=8;  //cst
      t_of_day=mktime(&t);
      return t_of_day;    
    }
    void removeCache(int uid,int year,int month);
    void modify_month(int uid,int preyear,int premonth,int newyear,int newmonth,int mark);
    FeedIdseq QueryFeedIdFromDB(int uid,int year,int month);
    string queryDB(int uid);
    UidYear translate(string str);
    int string2int(string str);
    string int2string(int t);
    string GetTableName(int id) { 
      ostringstream os;
      os << "feed_timeline_" << id % DBCluster;
      return os.str();
    }  
};
/* 
 * cache
struct Node{
  int uid;
  std::string str;
};

class Lru_list:public MyUtil::Singleton<Lru_list>
{
  typedef multi_index_container<
    Node,
    indexed_by<
      sequenced<>,
      hashed_unique<member<Node,int,&Node::uid> >
    >
  > item_list;

public:
  typedef item_list::nth_index<0>::type SeqIndex;
  typedef item_list::nth_index<1>::type UidIndex;
  typedef typename item_list::iterator iterator;


  string locate(int uid) {
    IceUtil::Mutex::Lock lock(_mutex);
    UidIndex& uid_index=ilist.get<1>();
    UidIndex::iterator it = uid_index.find(uid);
    if(it!=uid_index.end()) {
      iterator it1 = ilist.project<0>(it);
      seq_index.relocate(seq_index.begin(),it1);
      return it->str;
    } else {
      return string();
    }
  }
  
  void insert(const Node& item)
  {
    IceUtil::Mutex::Lock lock(_mutex);
    SeqIndex& seq_index=ilist.get<0>();
    
    if(seq_index.size()>max_num_items){  
      seq_index.pop_back();
    }
  }
  void modify(int uid,string str) {
    IceUtil::Mutex::Lock lock(_mutex);
    UidIndex& uid_index=ilist.get<1>();
    UidIndex::iterator it=uid_index.find(uid);
    it->str=str;
    iterator it1 = ilist.project<0>(it);
    seq_index.relocate(seq_index.begin(),it1);
  }
//  void scan() {
//    SeqIndex& seq_index=ilist.get<0>();
//    SeqIndex::iterator it;
//    for(it=seq_index.begin();it!=seq_index.end();++it) {
//      cout<<it->uid<<" "<<it->str<<endl;
//    }
//  }
private:
  item_list   ilist;
  IceUtil::Mutex    _mutex;
  const std::size_t max_num_items=10000;
};

*/
}
}
}
#endif
