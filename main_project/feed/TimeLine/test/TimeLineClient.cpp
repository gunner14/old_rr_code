
#include "TimeLineFeedAdapter.h"
#include "TimeLineFeedReplicaAdapter.h"
#include <map>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/tuple/tuple.hpp>
#include "QueryRunner.h"
#include "ServiceI.h"
#include <iterator>
#include "RFeed.h"

const string kTimeLineDB="feed_timeline";
const string kFeedDB="feed_db";
const int    kDBCluster = 100;

typedef std::map<int , std::set<int> > YearMonthMap;

using namespace boost;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace xce::serverstate;
using namespace xce::feed::timeline;
using namespace Ice;
using namespace xce::feed;
using namespace std;
using namespace boost;
using namespace MyUtil;
using namespace ::xce::feed::timeline;

#define foreach BOOST_FOREACH
class TimeLine;


void timeLineToMap(string & str, YearMonthMap & result);
void mapToTimeLine(YearMonthMap & year_month, string & str);
ostream& operator << (ostream& stream,const TimeLineFeedInfo & f);
void show(const UidYear & result,ostream & out=std::cout);
void show(const TimeLineFeedResult & result,ostream & out=std::cout);
void process(istream & in,ostream & out,TimeLine & timeline,bool script);
void usage(ostream & out=std::cout,bool name=false);
void run_script(const string &f1,const string & f2,TimeLine & timeline);
void init();

void getLastMonth(int year,int month,int &year_last,int &month_last);



//global
map<string, string> usage_map;
map< string, void(*)(vector<string>&,istream &,ostream &,TimeLine & ,bool)  > handler_map;
MyUtil::IntSeq types;


class TimeHelper
{
public:
    TimeHelper(bool log = true, ostream & _out=std::cout) : needLog_(log),out(_out)
    {
        startTime_ = current();
    }
    TimeHelper(const string & preLog, bool log = true, ostream & _out=std::cout) : preString_(preLog),needLog_(log),out(_out)
    {
        startTime_ = current();
    }
    ~TimeHelper()
    {
        if(func_)
           func_(this);
        if(!needLog_)
           return;
        stringstream ss;
        ss<<"TimeHelper: "<<preString_<<" Time cost:"<<elapsed()<<" us";
        log(ss.str());
    }
    void log(const string & str)
    {
        out<<str<<endl;
    }
    void function(const boost::function< void(TimeHelper*) > & f)
    {
        func_=f;
    }
    void preString(const string & preLog)
    {
        preString_=preLog;
    }
    long reset()
    {
        long result = abs( current() - startTime_ );
        startTime_ = current();
        return result;
    }
    long current(){
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000*1000 + tv.tv_usec;
    }
    long elapsed()
    {
        return abs( current() - startTime_ );
    }
    void markStone(int id)
    {
        time_map_[id]=current();
    }
    long elapsed(int id)
    {
        return abs(current()-time_map_[id]);
    }
    boost::function<void(TimeHelper*)> func_;
    std::map<int,long> time_map_;
    std::string preString_;
    bool needLog_;
    ostream & out;
    long startTime_;
};

class TimeLine
{
public:
    TimeLine(bool replica_mode,TimeLineFeedAdapter & _a, TimeLineFeedReplicaAdapter & _r):
    replica_(replica_mode),a(_a),r(_r) {};
    bool replica() { return replica_;}
    void replica(bool r) { replica_ = r;}
    bool replica_;
    //there seems noway to make code below pretty.
    UidYear Query(ostream & o,int uid) {
       if(replica_)
          return r.Query(uid);
       else
          return a.Query(uid);
    }
    TimeLineFeedResult QueryFeedInfoByStype(ostream & o,int uid,int year,int month,const MyUtil::IntSeq& types,int begin,int limit)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.QueryFeedInfoByStype(uid,year,month,types,begin,limit);
       else
          return a.QueryFeedInfoByStype(uid,year,month,types,begin,limit);
    }
    TimeLineFeedResult QueryFeedInfo(ostream & o,int uid,int year,int month,int begin,int limit)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.QueryFeedInfo(uid,year,month,begin,limit);
       else
          return a.QueryFeedInfo(uid,year,month,begin,limit);
    }
    int QueryFeedSize(ostream & o,int uid,int year,int month)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.QueryFeedSize(uid,year,month);
       else
          return a.QueryFeedSize(uid,year,month);
    }
    int QueryFeedSizeByStype(ostream & o,int uid,int year,int month,const MyUtil::IntSeq& types)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.QueryFeedSizeByStype(uid,year,month,types);
       else
          return a.QueryFeedSizeByStype(uid,year,month,types);
    }
    bool IsWideScreen(ostream & o,int uid,int feed_year, ::Ice::Long feed)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.IsWideScreen(uid,feed_year,feed);
       else
          return a.IsWideScreen(uid,feed_year,feed);
    }
    bool SetWideScreen(ostream & o,int uid,int feed_year, ::Ice::Long feed, bool wide)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.SetWideScreen(uid,feed_year,feed,wide);
       else
          return a.SetWideScreen(uid,feed_year,feed,wide);
    }
    void AddFeedMiniNotify(ostream & o,const FeedSeedPtr & s,int type, ::Ice::Long time)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.AddFeedMiniNotify(s,type,time);
       else
          return a.AddFeedMiniNotify(s,type,time);
    }
    void Modify(ostream & o,int uid,int feed_year, Ice::Long feed, int year,int month,int day)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.Modify(uid,feed_year,feed,year,month,day);
       else
          return a.Modify(uid,feed_year,feed,year,month,day);
    }
    void SetReadMini(ostream & o,int uid, int feed_year, int stype, ::Ice::Long merge)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.SetReadMini(uid,feed_year,stype,merge);
       else
          return a.SetReadMini(uid,feed_year,stype,merge);
    }
    void SetReadMiniById(ostream & o,int uid, int feed_year, ::Ice::Long feed)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.SetReadMiniById(uid,feed_year,feed);
       else
          return a.SetReadMiniById(uid,feed_year,feed);
    }
    void SetReadMiniByIds(ostream & o,int uid, int feed_year,const MyUtil::LongSeq & feeds)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.SetReadMiniByIds(uid,feed_year,feeds);
       else
          return a.SetReadMiniByIds(uid,feed_year,feeds);
    }
    void LoadFeedsToCache(ostream & o,int uid)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.LoadFeedsToCache(uid);
       else
          return a.LoadFeedsToCache(uid);
    }
    std::string Execute(ostream & o,int uid,const string & cmd)
    {
       TimeHelper t(true,o);
       if(replica_)
          return r.Execute(uid,cmd);
       else
          return a.Execute(uid,cmd);
    }
    TimeLineFeedAdapter & a;
    TimeLineFeedReplicaAdapter & r;
};

#define GET(pos,name) \
do {\
  ss.clear();ss.str(cmds.at(pos));ss>>name;if(ss.fail()) {out<<"param error"<<endl; return;}\
}while(0)

#define CHECK_SIZE(i)\
do {\
    if( cmds.size()!=i ) {\
        out<<__FUNCTION__<<": param size error."<<endl;\
        return;\
    }\
}while(0)

#define CHECK_SIZE_GE(i)\
do {\
    if( cmds.size()<i ) {\
        out<<__FUNCTION__<<": param size error."<<endl;\
        return;\
    }\
}while(0)

void replica(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss;
    int temp;
    GET(1,temp);
    if(temp) {
        timeline.replica(true);
        out<<"using replica adapter."<<endl;
    } else {
        timeline.replica(false);
        out<<"using adapter."<<endl;
    }
}
void q(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss;
    int uid;
    GET(1,uid);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"Query("<<uid<<")"<<endl;
    UidYear res= timeline.Query(out,uid);
    show(res,out);
}
void qfbs(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(6);
    stringstream ss;
    int uid,year,month,begin,limit;
    GET(1,uid);GET(2,year);GET(3,month);GET(4,begin);GET(5,limit);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"QueryFeedIdByStype("<<uid<<","<<year<<","<<month<<","<<begin<<","<<limit<<")"<<endl;
    TimeLineFeedResult res=timeline.QueryFeedInfoByStype(out,uid,year,month,types,begin,limit);
    show(res,out);
}
void qf(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(6);
    stringstream ss;
    int uid,year,month,begin,limit;
    GET(1,uid);GET(2,year);GET(3,month);GET(4,begin);GET(5,limit);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"QueryFeedId("<<uid<<","<<year<<","<<month<<","<<begin<<","<<limit<<")"<<endl;
    TimeLineFeedResult res=timeline.QueryFeedInfo(out,uid,year,month,begin,limit);
    show(res,out);
}
void qs(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year,month;
    GET(1,uid);GET(2,year);GET(3,month);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"QueryFeedSize("<<uid<<","<<year<<","<<month<<")"<<endl;
    out<<timeline.QueryFeedSize(out,uid,year,month)<<endl;
}
void qsbs(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year,month;
    GET(1,uid);GET(2,year);GET(3,month);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"QueryFeedSizeByStype("<<uid<<","<<year<<","<<month<<")"<<endl;
    out<<timeline.QueryFeedSizeByStype(out,uid,year,month,types)<<endl;
}
void iw(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year;Ice::Long feed;
    GET(1,uid);GET(2,year);GET(3,feed);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"IsWideScreen("<<uid<<","<<feed<<")"<<endl;
    if(timeline.IsWideScreen(out,uid,year,feed)) {
        out<<"wide"<<endl;
    } else {
        out<<"not wide"<<endl;
    }
}
void sw(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(5);
    stringstream ss;
    int uid,year,wide;Ice::Long feed;
    GET(1,uid);GET(2,year);GET(3,feed);GET(4,wide);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    if(wide!=0) {
         out<<"SetWideScreen("<<uid<<","<<feed<<","<<"true)"<<endl;
             timeline.SetWideScreen(out,uid,year,feed,true);
    } else {
         out<<"SetWideScreen("<<uid<<","<<feed<<","<<"false)"<<endl;
         timeline.SetWideScreen(out,uid,year,feed,false);
    }
}
void sr(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(5);
    stringstream ss;
    int uid,year,stype;Ice::Long merge;
    GET(1,uid);GET(2,year);GET(3,stype);GET(4,merge);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"SetReadMini("<<uid<<","<<stype<<","<<merge<<")"<<endl;
    timeline.SetReadMini(out,uid,year,stype,merge);
    out<<"done."<<endl;
}
void srbi(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year;Ice::Long feed;
    GET(1,uid);GET(2,year);GET(3,feed);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"SetReadMiniById("<<uid<<","<<feed<<")"<<endl;
    timeline.SetReadMiniById(out,uid,year,feed);
    out<<"done."<<endl;
}
void srbis(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE_GE(4);
    stringstream ss;
    int uid,year;Ice::Long feed;
    GET(1,uid);GET(2,year);
    int temp_skip=0;
    MyUtil::LongSeq v;
    stringstream temp;
    foreach(const string & s, cmds) 
    {
        if(temp_skip<3) {
            ++temp_skip;
            continue;
        }
        ss.clear();ss.str(s);ss>>feed;
        if(! ss.fail() ) {
            v.push_back(feed);
            temp<<s<<",";
        }
    }
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"SetReadMiniByIds("<<uid<<","<<temp.str()<<")"<<endl;
    timeline.SetReadMiniByIds(out,uid,year,v);
    out<<"done."<<endl;
}
void m(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(7);
    stringstream ss;
    int uid,feed_year,year,month,day;Ice::Long feed;
    GET(1,uid);GET(2,feed_year);GET(3,feed);GET(4,year);GET(5,month);GET(6,day);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"Modify("<<uid<<","<<feed_year<<","<<feed<<","<<year<<","<<month<<","<<day<<")"<<endl;
    timeline.Modify(out,uid,feed_year,feed,year,month,day);
}
void mf(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(6);
    stringstream ss;
    int uid,year,month,new_year,new_month;
    TimeLineFeedResult res; vector<Ice::Long> all_feeds;
    GET(1,uid);GET(2,year);GET(3,month);GET(4,new_year);GET(5,new_month);
    if(timeline.replica()) out<<"replica.";
    else out<<"adapter.";
    out<<"QueryFeedInfo("<<uid<<","<<year<<","<<month<<","<<")"<<endl;
    try {
        res=timeline.QueryFeedInfo(out,uid,year,month,0,-1);
    } catch(std::exception  &  e) {
            out<<"execute： "<<e.what()<<endl;
    }
    foreach(TimeLineFeedInfo & t,res.feeds)
    {
        foreach(Ice::Long f,t.feeds)
        {
            all_feeds.push_back(f);
        }
    }
    foreach(Ice::Long f, all_feeds)
    {
        if(timeline.replica()) out<<"replica.";
        else out<<"adapter.";
        out<<"Modify("<<uid<<","<<year<<","<<f<<","<<new_year<<","<<new_month<<","<<"1)"<<endl;
        try {
            timeline.Modify(out,uid,year,f,new_year,new_month,1);
        } catch(std::exception &  e) {
            out<<"exception："<<e.what()<<endl;
        }
    }
}
void bqf(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year1,year2;
    TimeLineFeedResult res;
    GET(1,uid);GET(2,year1);GET(3,year2);
    for(int i=year1;i<=year2;++i) {
        for(int j=1;j<13;++j) {
            if(timeline.replica()) out<<"replica.";
            else out<<"adapter.";
            out<<"QueryFeedInfo("<<uid<<","<<i<<","<<j<<",0,-1)"<<endl;
            res=timeline.QueryFeedInfo(out,uid,i,j,0,-1);
            out<<"("<<i<<"-"<<j<<")";show (res,out);
        }
    }
}
void bqs(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year1,year2;int s;
    GET(1,uid);GET(2,year1);GET(3,year2);
    for(int i=year1;i<=year2;++i) {
        for(int j=1;j<13;++j) {
            //out<<"timeline.QueryFeedSize("<<uid<<","<<i<<","<<j<<")"<<endl;
            s=timeline.QueryFeedSize(out,uid,i,j);
            out<<"("<<i<<"-"<<j<<") :"<<s<<endl;
        }
    }
}
void biw(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year1,year2;TimeLineFeedResult res;
    GET(1,uid);GET(2,year1);GET(3,year2);
    for(int i=year1;i<=year2;++i) {
        for(int j=1;j<13;++j) {
            res=timeline.QueryFeedInfo(out,uid,i,j,0,-1);
            if(res.feeds.size()!=0) {
                foreach(TimeLineFeedInfo & f,res.feeds) {
                    foreach(Ice::Long feed, f.feeds) {
                        if(timeline.replica()) out<<"replica.";
                        else out<<"adapter.";
                        out<<"IsWideScreen("<<uid<<","<<i<<","<<feed<<")"<<endl;
                        out<<feed<<"("<<i<<"-"<<j<<")  Wide:"<<timeline.IsWideScreen(out,uid,i,feed)<<endl;
                    }
                }
            }
        }
    }
}
void bsw(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(5);
    stringstream ss;
    int uid,year1,year2,wide;TimeLineFeedResult res;
    GET(1,uid);GET(2,year1);GET(3,year2);GET(4,wide);
    for(int i=year1;i<=year2;++i) {
        for(int j=1;j<13;++j) {
            res=timeline.QueryFeedInfo(out,uid,i,j,0,-1);
            if(res.feeds.size()!=0) {
                foreach(TimeLineFeedInfo & f,res.feeds) {
                    foreach(Ice::Long feed, f.feeds) {
                        if(timeline.replica()) out<<"replica.";
                        else out<<"adapter.";
                        out<<"SetWideScreen("<<uid<<","<<i<<","<<feed<<","<<wide<<")"<<endl;
                        out<<feed<<"("<<i<<"-"<<j<<")  SetWide:"<<wide<<endl;
                        timeline.SetWideScreen(out,uid,i,feed,wide);
                    }
                }
            }
        }
    }
}
void bsr(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int uid,year1,year2;TimeLineFeedResult res;
    GET(1,uid);GET(2,year1);GET(3,year2);
    for(int i=year1;i<=year2;++i) {
         for(int j=1;j<13;++j) {
             res=timeline.QueryFeedInfo(out,uid,i,j,0,-1);
             if(res.feeds.size()!=0) {
                 foreach(TimeLineFeedInfo& f,res.feeds) {
                     foreach(Ice::Long feed, f.feeds) {
                        if(timeline.replica()) out<<"replica.";
                        else out<<"adapter.";
                        out<<"SetReadMiniById("<<uid<<","<<i<<","<<feed<<")"<<endl;
                        out<<feed<<"("<<i<<"-"<<j<<") SetRead."<<endl;
                        timeline.SetReadMiniById(out,uid,i,feed);
                     }
                 }
             }
         }
    }
}
void bqsu(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(5);
    stringstream ss;
    int uid1,uid2,year1,year2;int temp;
    GET(1,uid1);GET(2,uid2);GET(3,year1);GET(4,year2);
    for(int i=uid1;i<=uid2;++i) {
        temp=0;
        for(int j=year1;j<=year2;++j) {  
            for(int k=1;k<13;++k) {
               //out<<"timeline.QueryFeedSize("<<i<<","<<j<<","<<k<<")"<<endl;
               temp+=timeline.QueryFeedSize(out,i,j,k);
            }
        }
        out<<"user: "<<i<<" nums:"<<temp<<endl;
    }
}
//
// temporarily we store uids in vector, 1kw (10MB) user is ok
//
void ftml(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss;
    ifstream file(cmds.at(1).c_str()); 
    if(file.fail()) {
        out<<"Bad file"<<endl;
        return;
    }
    //read line in file ,then call execute("uid,dtml")
    string name;  int temp; int i=0;
    while(getline(file,name,'\n')) {
        ++i;
        ss.clear();ss.str(name);ss>>temp;if(ss.fail()) {out<<"an error id"<<endl;continue;}
RETRY:
        try {
            out<<temp<<" == "<<timeline.Execute(out,temp,"dtml")<<endl;
        }catch(std::exception  &  e) {
            out<<"execute： "<<e.what()<<endl;
            usleep(10000);//10ms
            goto RETRY;
        }
    }
}
void frbd(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss;
    ifstream file(cmds.at(1).c_str()); 
    if(file.fail()) {
        out<<"Bad file"<<endl;
        return;
    }
    //read line in file ,then call execute("uid,rbdtml")
    string name;  int temp; int i=0;
    while(getline(file,name,'\n')) {
        ++i;
        ss.clear();ss.str(name);ss>>temp;if(ss.fail()) {out<<"an error id"<<endl;continue;}
RETRY:        
        try {
            timeline.Execute(out,temp,"rbdtml");
        }catch(std::exception  & e) {
            out<<"execute： "<<e.what()<<endl;
            usleep(10000);//10ms
            goto RETRY;
        }
    }
    out<<"update user nums:"<<i<<endl;
}
void fcmd(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    if( cmds.size()<=2 ) {
        out<<__FUNCTION__<<": param size error."<<endl;
        return;
    }
    //fcmd,file,load,09,10
    stringstream ss;
    ifstream file(cmds.at(1).c_str());
    string subcmd;
    int t=0;
    for(vector<string>::iterator it=cmds.begin();it!=cmds.end();++it) {
        ++t;
        if( t==1 || t==2 ) {
            continue;
        }
        if( t!=3)
            subcmd+=",";
        subcmd+=*it;
    }
    if(file.fail()) {
        out<<"Bad file"<<endl;
        return;
    }
    //read line in file ,then call execute("uid,rbdtml")
    string name;  int temp; int i=0;
    while(getline(file,name,'\n')) {
        ++i;
        ss.clear();ss.str(name);ss>>temp;if(ss.fail()) {out<<"an error id"<<endl;continue;}
RETRY:        
        try {
            timeline.Execute(out,temp,subcmd);
            usleep(1000);//1ms
        }catch(std::exception  & e) {
            out<<"execute： "<<e.what()<<endl;
            usleep(10000);//10ms
            goto RETRY;
        }
    }
    out<<"update user nums:"<<i<<endl;
}
//fq,file
void fq(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss;
    ifstream file(cmds.at(1).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    string name;  int id;
    while(getline(file,name,'\n')) {
         ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
         try {
             if(timeline.replica()) out<<"replica.";
             else out<<"adapter.";
             out<<"Query("<<id<<");"<<endl;
             timeline.Query(out,id);
         } catch(std::exception  & e) {
             out<<"exception： "<<e.what()<<endl;
         }
    }
}
void fqs(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int year1,year2;
    GET(1,year1);GET(2,year2);
    ifstream file(cmds.at(3).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    string name;  int id; int temp;
    while(getline(file,name,'\n')) {
         ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
         temp=0;
         for(int j=year1;j<=year2;++j) {
              for(int k=1;k<13;++k) {
                   try {
                       if(timeline.replica()) out<<"replica.";
                       else out<<"adapter.";
                       out<<"QueryFeedSize("<<id<<","<<j<<","<<k<<");"<<endl;
                       temp+=timeline.QueryFeedSize(out,id,j,k);
                   } catch(std::exception  & e) {
                       out<<"exception： "<<e.what()<<endl;
                       --k;
                   }
              }
         }
         out<<"user: "<<id<<"nums: "<<temp<<endl;
    }
}
void fqsbs(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int year1,year2;
    GET(1,year1);GET(2,year2);
    ifstream file(cmds.at(3).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    string name;  int id; int temp;
    while(getline(file,name,'\n')) {
         ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
         temp=0;
         for(int j=year1;j<=year2;++j) {
              for(int k=1;k<13;++k) {
                   try {
                       if(timeline.replica()) out<<"replica.";
                       else out<<"adapter.";
                       out<<"QueryFeedSizeByStype("<<id<<","<<j<<","<<k<<",...);"<<endl;
                       temp+=timeline.QueryFeedSizeByStype(out,id,j,k,types);
                   } catch(std::exception  & e) {
                       out<<"exception： "<<e.what()<<endl;
                       --k;
                   }
              }
         }
         out<<"user: "<<id<<"nums: "<<temp<<endl;
    }
}
void fqfbstml(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss;
    ifstream file(cmds.at(1).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    //vector<int> ids; ids.reserve(2000);//2k ?
    string name;  int temp;int id;
    while(getline(file,name,'\n')) {
         ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
         temp=0;
         if(timeline.replica()) out<<"replica.";
         else out<<"adapter.";  
         out<<"Query("<<id<<")"<<endl;
         UidYear res;
         int trys=0;
QUERY:
         try {
             res=timeline.Query(out,id);
         } catch(std::exception  & e) {
             out<<"exception： "<<e.what()<<endl;
             ++trys;
             if(trys>5) 
                continue;
             goto QUERY;
             //continue; //
         }
         typedef const pair<int,vector<int> > CPP;
         foreach(CPP & p, res)
         {
             foreach(int m,p.second)
             {
                  try {
                      if(timeline.replica()) out<<"replica.";
                      else out<<"adapter.";
                      out<<"QueryFeedInfoByStype("<<id<<","<<p.first<<","<<m<<",....,0,30);"<<endl;
                      TimeLineFeedResult res=timeline.QueryFeedInfoByStype(out,id,p.first,m,types,0,30);
                      if(res.size >30) {
                          if(timeline.replica()) out<<"replica.";
                          else out<<"adapter.";
                          out<<"QueryFeedInfoByStype("<<id<<","<<p.first<<","<<m<<",....,30,"<< res.size-30<<") "<<endl;
                          timeline.QueryFeedInfoByStype(out,id,p.first,m,types,30,res.size-30);
                      }
                  } catch(std::exception  & e) {
                       out<<"exception： "<<e.what()<<endl;
                  }
             }
         }
    }
}

void fqfbs(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int year1,year2;
    GET(1,year1);GET(2,year2);
    ifstream file(cmds.at(3).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    //vector<int> ids; ids.reserve(2000);//2k ?
    string name;  int temp;int id;
    while(getline(file,name,'\n')) {
         ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
         temp=0;
         for(int j=year1;j<=year2;++j) {
              for(int k=1;k<13;++k) {
                   try {
                       if(timeline.replica()) out<<"replica.";
                       else out<<"adapter.";
                       out<<"QueryFeedInfoByStype("<<id<<","<<j<<","<<k<<",....,0,30);"<<endl;
                       timeline.QueryFeedInfoByStype(out,id,j,k,types,0,30);
                       if(timeline.replica()) out<<"replica.";
                       else out<<"adapter.";
                       out<<"QueryFeedInfoByStype("<<id<<","<<j<<","<<k<<",....,30,60);"<<endl;
                       timeline.QueryFeedInfoByStype(out,id,j,k,types,30,60);
                   } catch(std::exception  & e) {
                       out<<"exception： "<<e.what()<<endl;
                       --k;
                   }
              }
         }
    }
}
void fqf(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int year1,year2;
    GET(1,year1);GET(2,year2);
    ifstream file(cmds.at(3).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    string name; int id;
    while(getline(file,name,'\n')) {
         ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
         for(int j=year1;j<=year2;++j) {
              for(int k=1;k<13;++k) {
                   try {
                       if(timeline.replica()) out<<"replica.";
                       else out<<"adapter.";
                       out<<"QueryFeedInfo("<<id<<","<<j<<","<<k<<");"<<endl;
                       timeline.QueryFeedInfo(out,id,j,k,0,-1);
                   } catch(std::exception  & e) {
                       out<<"exception： "<<e.what()<<endl;
                       --k;
                   }
              }
         }
    }
}


// TODO change way of fiw 
// fiw,2012,2012,file
// fsw,2012,2012,1,file
// fsr,2102,2012,file
void fiw(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(4);
    stringstream ss;
    int year1,year2;
    GET(1,year1);GET(2,year2);
    ifstream file(cmds.at(3).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    vector<Ice::Long> all_feeds;TimeLineFeedResult res;
    string name; int id;
    while(getline(file,name,'\n')) {
        ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
        for(int j=year1;j<=year2;++j) { 
            all_feeds.clear();//year by year
            for(int k=1;k<13;++k) {
                  try {
                      //if(timeline.replica()) out<<"replica.";
                      //else out<<"adapter.";
                      //out<<"QueryFeedInfo("<<id<<","<<j<<","<<k<<");"<<endl;
                      res=timeline.QueryFeedInfo(out,id,j,k,0,-1);
                      foreach(TimeLineFeedInfo & t,res.feeds)
                      {
                          foreach(Ice::Long f,t.feeds)
                          {
                              all_feeds.push_back(f);
                          }
                      }
                      //usleep(10000);//10ms
                  } catch(std::exception  & e) {
                      out<<"exception： "<<e.what()<<endl;
                      --k;
                  }
            }
            foreach(Ice::Long f, all_feeds)
            {
                if(timeline.replica()) out<<"replica.";
                else out<<"adapter.";
                out<<"IsWideScreen("<<id<<","<<j<<","<<f<<")   ";
                try {
                    if(timeline.IsWideScreen(out,id,j,f))
                    out<<" wide"<<endl;
                else
                    out<<" not wide"<<endl;
                } catch(std::exception  & e) {
                    out<<"exception： "<<e.what()<<endl;
                }
            //usleep(100);//we can't be too quick; 
            }
        }
    }
}

// fsw,2012,2012,1,file
void fsw(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(5);
    stringstream ss;
    int year1,year2,wide;
    GET(1,year1);GET(2,year2);GET(3,wide);
    ifstream file(cmds.at(4).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file
    string name;int id;    vector<Ice::Long> all_feeds;
    TimeLineFeedResult res;
    while(getline(file,name,'\n')) {
        ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
        for(int j=year1;j<=year2;++j) {
            all_feeds.clear();
            for(int k=1;k<13;++k) {
                try {
                    //if(timeline.replica()) out<<"replica.";
                    //else out<<"adapter.";
                    //out<<"QueryFeedInfo("<<id<<","<<j<<","<<k<<");"<<endl;
                    res=timeline.QueryFeedInfo(out,id,j,k,0,-1);
                    foreach(TimeLineFeedInfo & t,res.feeds)
                    {
                        foreach(Ice::Long f,t.feeds)
                        {
                            all_feeds.push_back(f);
                        }
                    }
                } catch(std::exception  & e) {
                    out<<"exception："<<e.what()<<endl;
                    --k;
                }
            }
            foreach(Ice::Long f, all_feeds)
            {
                if(timeline.replica()) out<<"replica.";
                else out<<"adapter.";
                out<<"SetWideScreen("<<id<<","<<j<<","<<f<<","<<wide<<")"<<endl;
                timeline.SetWideScreen(out,id,j,f,wide);  
            }
        }
    }
}

// fsr,2012,2012,file
void fsr(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    return ; //this func is  dangerous.
    //
    CHECK_SIZE(4);
    stringstream ss;
    int year1,year2;
    GET(1,year1);GET(2,year2);
    ifstream file(cmds.at(3).c_str());
    if(file.fail()) {
        out<<"Bad file"<<endl;
        return;
    }
    string name;int id;
    vector<Ice::Long> all_feeds;TimeLineFeedResult res;
    while(getline(file,name,'\n')) {
        ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
        for(int j=year1;j<=year2;++j) {
            all_feeds.clear();
            for(int k=1;k<13;++k) {
                try {
                    //if(timeline.replica()) out<<"replica.";
                    //else out<<"adapter.";
                    //out<<"QueryFeedInfo("<<id<<","<<j<<","<<k<<");"<<endl;
                    res=timeline.QueryFeedInfo(out,id,j,k,0,-1);
                    foreach(TimeLineFeedInfo & t,res.feeds) {
                        foreach(Ice::Long f,t.feeds) {
                            all_feeds.push_back(f);
                        }
                    }
                } catch(std::exception  & e) {
                    out<<"exception："<<e.what()<<endl;
                    --k;
                }
            }
            foreach(Ice::Long f,all_feeds)
            {
                if(timeline.replica()) out<<"replica.";
                else out<<"adapter.";
                out<<"SetReadMiniById("<<id<<","<<j<<","<<f<<");"<<endl;
                try {
                    timeline.SetReadMiniById(out,id,j,f);
                } catch(std::exception  & e) {
                    out<<"exception："<<e.what()<<endl;
                }
            }
        }
    }
}
void file(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss; 
    ifstream file(cmds.at(1).c_str()); 
    if(file.fail()) {
         out<<"Bad file"<<endl;
         return;
    }
    //read file 
    string name; int id;
    while(getline(file,name,'\n')) {
         ss.clear();ss.str(name);ss>>id;if(ss.fail()) {out<<"an error id"<<endl;continue;}
            try {
                 if(timeline.replica()) out<<"replica.";
                 else out<<"adapter.";
                 out<<"QueryFeedInfoByStype("<<id<<","<<2012<<","<<10<<",....);"<<endl;
                 timeline.QueryFeedInfoByStype(out,id,2012,10,types,0,30);
            } catch(std::exception  & e) {
                 out<<"exception： "<<e.what()<<endl;
            }
    }
}
void slp(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(2);
    stringstream ss;
    int ms;
    ss.str(cmds.at(1));ss>>ms;if(ss.fail()) {out<<"param error"<<endl; return;}
    usleep(1000*ms);
}
void cmd(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    if( cmds.size()<2 ) {
        out<<"param size error"<<endl;
        return;
    }
    stringstream ss;
    int uid;
    ss.clear();ss.str(cmds.at(1));ss>>uid;if(ss.fail()) {out<<"param error"<<endl; return;}
    string args;int i=0;
    foreach(string & s,cmds) {
        if(i==0||i==1) {
            ++i;continue; 
        }
        args+=s;args+=",";
    }
    args.erase(args.size()-1,1);// remove last ','
    out<<"Execute result:"<<endl;
    out<<timeline.Execute(out,uid,args)<<endl;
}
void run(vector<string> & cmds,istream & in,ostream & out,TimeLine & timeline,bool script)
{
    CHECK_SIZE(3);
    out<<"script"<<endl;
    if( !script ) {
         out<<"start script"<<endl;
         run_script(cmds.at(1),cmds.at(2),timeline);
         out<<"script ends"<<endl;
    }
}
void process(istream & in,ostream & out,TimeLine & timeline,bool script) 
{
    string line;vector<string> cmds;stringstream ss;
    while(getline(in,line,'\n'))
    {
        split(cmds,line,is_any_of(","),token_compress_on);
        if(cmds.size()<1)
            continue;
        if(cmds[0]=="\n" ||cmds[0]==" " ||cmds[0]=="")
            continue;
        if(cmds.size()==1) {
            if(cmds[0]=="exit" && !script) {
                exit(0);
            }
            if(cmds[0]=="help" || cmds[0]=="h") {
                usage();
                continue;
            }
        }
        map< string, void(*)(vector<string>&,istream &,ostream &,TimeLine & ,bool)  >::iterator it=handler_map.find(cmds[0]);
        if(it!=handler_map.end() ) {
            if(cmds.size()==1)
            {//usage
                out<<usage_map[cmds[0]]; 
                continue;
            } else {
              try {
                  (*(it->second))(cmds,in,out,timeline,script);
              } catch (std::exception &e ) {
                  out<<"process catch a exception."<<e.what()<<endl;
              }
            }
        } else {
            out<<"bad command."<<endl;
        }
    }
}
int main(int argc, char * argv[])
{
    TimeLine timeline(true,TimeLineFeedAdapter::instance(),TimeLineFeedReplicaAdapter::instance());
    init();
    //ConnectionPoolManager::instance().initialize();
    cout<<"type cmd name or help to see example. format: bsw\n";
    usage(cout,true);
    cout<<"Using replica adapter"<<endl;
    //MCE_INIT("/dev/null","Debug");
    //usage();
    if(argc==1) {
        process(cin,cout,timeline,false);
    }
    if(argc!=1)
    {
        vector<shared_ptr<boost::thread> > vv;
        cout<<"run script";
        int i=0;
        foreach(char * f,argv) {
            ++i;
            if(i==1) continue;
            shared_ptr<boost::thread> ptr=make_shared<boost::thread>(run_script,f,string(f)+".out",ref(timeline));
            vv.push_back(ptr);
        }
        foreach(shared_ptr<boost::thread> & p,vv) {
            p->join();
        }
    }
    return 0;
}
void usage(ostream & out,bool name)
{
    typedef pair<string,string> pUsage_t;
    if(name) {
        foreach(const pUsage_t &s,usage_map) {
            out<<s.first<<" ";
        }
        out<<endl;
    } else {
        foreach(const pUsage_t &s,usage_map) {
            out<<s.second;
        }
    }
}
void init()
{
//21;102,103,104,107,110,502,601,701,702,709,801,808,1201,1205,1206,8210,8211,8301,9801,9802,9803
    types.push_back(20);
    types.push_back(102);
    types.push_back(103);
    types.push_back(104);
    types.push_back(107);
    types.push_back(110);
    types.push_back(502);
    types.push_back(601);
    types.push_back(701);
    types.push_back(702);
    types.push_back(709);
    types.push_back(801);
    types.push_back(808);
    types.push_back(1201);
    types.push_back(1205);
    types.push_back(1206);
    types.push_back(2002);
    types.push_back(2401);
    types.push_back(3707);
    types.push_back(8210);
    types.push_back(8211);
    types.push_back(8301);
    types.push_back(9801);
    types.push_back(9802);
    types.push_back(9803);

    handler_map["q"]=q;
    handler_map["qfbs"]=qfbs;
    handler_map["qf"]=qf;
    handler_map["qs"]=qs;
    handler_map["qsbs"]=qsbs;
    handler_map["iw"]=iw;
    handler_map["sw"]=sw;
    handler_map["sr"]=sr;
    handler_map["srbi"]=srbi;
    handler_map["srbis"]=srbis;
    handler_map["m"]=m;
    handler_map["bqf"]=bqf;
    handler_map["biw"]=biw;
    handler_map["bqs"]=bqs;
    handler_map["bsw"]=bsw;
    handler_map["biw"]=biw;
    handler_map["bsr"]=bsr;
    handler_map["bqsu"]=bqsu;
    handler_map["fq"]=fq;
    handler_map["fqs"]=fqs;
    handler_map["fqf"]=fqf;
    handler_map["fqsbs"]=fqsbs;
    handler_map["fqfbs"]=fqfbs;
    handler_map["fqfbstml"]=fqfbstml;
    handler_map["fiw"]=fiw;
    handler_map["fsw"]=fsw;
    handler_map["ftml"]=ftml;
    handler_map["frbd"]=frbd;
    handler_map["fcmd"]=fcmd;
    handler_map["file"]=file;

    handler_map["mf"]=mf;//move all user feeds to last month 
    handler_map["cmd"]=cmd;
    handler_map["slp"]=slp;
    handler_map["run"]=run;
    handler_map["q"]=q;
    handler_map["replica"]=replica;
    
    usage_map["q"]="\
q\n\
  UidYear Query(int);\n\
  format: q,(uid)\n\
  example:q,380504108\n";
    usage_map["qfbs"]="\
qfbs\n\
  FeedIdSeq QueryFeedIdByStype(int,int,int,TypeSeq&,int,int);\n\
  format: qfbs,uid,year,month,begin,limit\n\
  example:qfbs,380504108,2011,4,0,30\n";
    usage_map["qf"]="\
qf\n\
  FeedIdSeq QueryFeedId(int,int,int,int,int);\n\
  format: qf,uid,year,month,begin,limit\n\
  example:qf,380504108,2011,4,0,30\n";
    usage_map["qs"]="\
qs\n\
  int QueryFeedSize(int,int,int);\n\
  format: qs,uid,year,month\n\
  example:qs,380504108,2011,4\n";
    usage_map["qsbs"]="\
qsbs\n\
  int QueryFeedSizeByStype(int,int,int,TypeSeq&);\n\
  format: qsbs,uid,year,month\n\
  example:qsbs,380504108,2012,5\n";
    usage_map["iw"]="\
iw\n\
  bool IsWideScreen(int,int year,Long);\n\
  format: iw,uid,feed\n\
  example:iw,380504108,2010,17171717171\n";
    usage_map["sw"]="\
sw\n\
  void SetWideScreen(int,int year,Long,bool wide);\n\
  format: sw,uid,feed,(0/!0)\n\
  example:sw,380504108,2008,17171717,1\n";
    usage_map["sr"]="\
sr\n\
  void SetReadMini(int uid,int year, int stype, ::Ice::Long merge);\n\
  format: sr,uid,stype,merge\n\
  example:sr,380504108,2008,1233,12312312321\n";
    usage_map["srbi"]="\
srbi\n\
  void SetReadMiniById(int uid,int year,::Ice::Long feed);\n\
  format: srbi,uid,feed\n\
  example:srbi,380504108,2008,171171771\n";
    usage_map["srbis"]="\
srbis\n\
  void SetReadMiniByIds(int uid,int year,LongSeq feed);\n\
  format: srbis,uid,year,feed1,feed2,feed3,feed4,\n\
  example:srbis,380504108,2008,171171771,171171772,171171773\n";
    usage_map["m"]="\
m\n\
  void Modify(int uid,int year,::Ice::Long feed,int year,int month,int day);\n\
  format: m,380504108,2009,171171771,2012,5,30\n";
    usage_map["mf"]="\
mf\n\
  move user feeds to last month of original time.eg,feeds in May will be in April.\n\
  format:mf,uid,year,month,new_year,new_month\n\
  example:mf,380504108,2012,7,2009,2\n";
    usage_map["bqf"]="\
bqf\n\
  batch query feed ids.\n\
  format: bqf,uid,start_year,end_year\n\
  example:bqf,380504108,2000,2013\n";
    usage_map["biw"]="\
biw\n\
  batch isWide\n\
  format: biw,uid,start_year,end_year\n\
  example:biw,380504108,2010,2012\n";
    usage_map["bqs"]="\
bqs\n\
  batch query size\n\
  format: bqs,uid,start_year,end_year\n\
  example:bqs,380504108,2010,2012\n";
    usage_map["bsw"]="\
bsw\n\
  batch setWide\n\
  format: bsw,uid,start_year,end_year,bool\n\
  example:bsw,380504108,2010,2012,1\n";
    usage_map["biw"]="\
biw\n\
  batch query wide\n\
  format: 15,uid,start_year,end_year\n\
  example:biw,380504108,2010,2012\n";
    usage_map["bsr"]="\
bsr\n\
  batch set read\n\
  format: bsr,uid,start_year,end_year\n\
  example:bsr,380504108,2010,2012\n";
    usage_map["bqsu"]="\
bqsu\n\
  batch query size for users\n\
  format: bqsu,start_uid,end_uid,year,start_year,end_year\n\
  example:bqsu,380504100,380504199,2012,2012\n";
    usage_map["fqs"]="\
fbqs\n\
  batch query size for users in file\n\
  format: bqsf,start_year,end_year,file_name\n\
  example:bqsf,2012,2012,id.txt\n";

    usage_map["cmd"]="\
cmd\n\
  execute command.\n\
  example:cmd,****\n";
    usage_map["slp"]="\
slp\n\
  sleep for ms\n\
  example:slp 100\n";
    usage_map["run"]="\
run\n\
  use file param1 as input,file param2 as ouput.\n\
  format: run,in.txt, out.txt\n";
    usage_map["exit"]="\
exit\n\
  exit to quit\n";
    usage_map["ftml"]="\
ftml\n\
  get timeline for users in file.\n\
  format: ftml,filename\n\
  example: ftml,1.txt\n";
  
    usage_map["frbd"]="\
frbd\n\
  rbuild timeline for users in file.\n\
  format: frbd,file_name\n\
  example: frbd,1.txt\n";

    usage_map["fqf"]="\
fqf\n\
  qf for every users in file.\n\
  format: fq,year1,year2,file\n\
  example: fq,2012,2012,1.txt\n";

    usage_map["fqfbstml"]="\
fqfbstml\n\
  qfbs for users in file.\n\
  format: fq,year1,year2,file\n\
  example: fqfbstml,1.txt\n";

    usage_map["fqsbs"]="\
fqsbs\n\
  qsbs for every users in file.\n\
  format: fqsbs,year1,year2,file\n\
  example: fqsbs,2012,2012,1.txt\n";

    usage_map["fqfbs"]="\
fqfbs\n\
  qfbs for  every users in file.\n\
  format: fqfbs,year1,year2,file\n\
  example: fqfbs,2012,2012,1.txt\n";

    usage_map["fq"]="\
fq\n\
  query every users in file.\n\
  format: fq,year1,year2,file\n\
  example: fq,2012,2012,1.txt\n";

    usage_map["fiw"]="\
fiw\n\
  query every feeds wide between years for users in file.\n\
  format: fiw,year1,year2,file\n\
  example: fiw,2012,2012,1.txt\n";

    usage_map["fsw"]="\
fsw\n\
  set every feeds wide between years for users in file.\n\
  format: fsw,year1,year2,file\n\
  example: fsw,2012,2012,1.txt\n";

    usage_map["fsr"]="\
fsr\n\
  DANGEROURS! delete every feeds wide between years for users in file.\n\
  format: fsr,year1,year2,file\n\
  example: fsr,2012,2012,1.txt\n";
    usage_map["fcmd"]="\
fcmd\n\
  format: fcmd,file,cmd,arg1....\n\
  example: fcmd,1.txt,load,2012,9\n";

    usage_map["replica"]="\
replica\n\
  replica,0\n";
}

void timeLineToMap(string & str, YearMonthMap & result)
{
    size_t pos=0;int year,month;
    size_t pos_limit1= str.size()-2;//need 3 char available because ':'
    size_t pos_limit2= str.size()-1;//need 2 char available
    result.clear();
    while( pos < pos_limit1 && pos!=string::npos) {
        if( ( pos=str.find(':',pos) ) !=string::npos ){//find year
            ++pos;
        year = (str[pos]-'0')*10 + str[pos+1]-'0';
        pos+=2;
        if( year < 0 || year > 99) {
           continue;//discard this damned year
        }
        //add month
        if(year<50)  year += 2000;
        else  year += 1900;
        set<int> & months=result[year];
        while(pos < pos_limit2 && str[pos] != ':' && str[pos+1] !=':') { 
           month = (str[pos]-'0')*10 + str[pos+1]-'0';
           pos+=2; 
           if( month<1 || month>12 )
               continue;
           months.insert(month);
        }
        if(result[year].size()==0)//no month at all
           result.erase(year);
        }
        else
           break;
    } 
}
void mapToTimeLine(YearMonthMap & year_month, string & str)
{
    str.clear();str.reserve(300);//default 15 is always too short;
    int year,month;bool has_month;
    for( YearMonthMap::iterator it = year_month.begin(); it != year_month.end(); ++it ) {
        year = it->first;
        if( year<1950 || year >2049)
            continue;
        str.push_back(':');
        if( year>2000)   year-=2000;
        else   year-=1900;
        year%=100;
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
            str.erase(str.size()-3,str.size());//remove :YY,  this year contains no valid month.
        }
    }   
}

ostream& operator << (ostream& stream,const TimeLineFeedInfo & f)
{
    stream<<"(";
    foreach(Ice::Long feed,f.feeds) {
        stream<<feed<<",";
    }
    return stream<<")"<<"["<<f.flag<<"]";
}

//tool function
inline void getNextMonth(int year,int month,int & year_n,int & month_n) 
{
    if(month!=12) {
        year_n=year;month_n=month+1;
    } else {
        year_n=year+1;month_n=1;
    }
}
void getLastMonth(int year,int month,int &year_last,int &month_last)
{
    if(month!=1) {
        year_last=year; month_last=month-1;
    } else {
        year_last=year-1; month_last=12;
    }
}
void show(const UidYear & result,ostream & out)
{
   out<<"size: "<<result.size()<<endl;
   for(UidYear::const_iterator it=result.begin();it!=result.end();++it)
   {
       out<<"year: "<<it->first<<"  (";
       const vector<int> & months=it->second;
       for(UidYear::value_type::second_type::const_iterator it2=months.begin();
             it2!=months.end();++it2)
       {
           out<<*it2<<", ";
       }
       out<<")"<<endl;
   }
}
void show(const TimeLineFeedResult & result,ostream & out)
{
    out<<"all size:"<<result.size<<endl;
    out<<"ret size: "<<result.feeds.size()<<" (";
    foreach(const TimeLineFeedInfo & f, result.feeds)
    {
        out<<f<<",";
    }
    out<<")"<<endl;
}

void run_script(const string &f1,const string & f2,TimeLine & timeline)
{   
    ifstream ifs(f1.c_str());
    if(ifs.fail()) {
        cout<<"file error! stop"<<endl;
        return;
    }
    ofstream ofs(f2.c_str());
    if(ofs.fail())  {
        cout<<"file error! stop"<<endl;
        return;
    }
    cout<<"start: in: "<<f1<<"out: "<<f2<<endl;
    process(ifs,ofs,timeline,true);
    cout<<"done."<<endl;
}
