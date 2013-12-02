////////////////////////////////////////////
//
//  unit_test:
//   1. timeLineToMap
//   2. timeLineToUidYear
//   3. mapToTimeLine_
//   4. timeLine Add  logic
//   5. timeLine Remove logic
//   6. 
//  TODO more test case for merging stype 
//

#include "../src/preheader.h"

#define private public
#include "../src/TimeLineFeedI.h"
#include "../src/TimeLineInternal.h"
#include "../src/Task.h"
#include "../src/CommandHandler.h"
#undef private

#include <boost/tuple/tuple.hpp>
//#include "../src/TimeLineFeedI.cpp" 
//#include "../src/CommandHandler.cpp"
//#include "../src/Task.cpp"

using namespace Json;

#define BOOST_TEST_MODULE timeline_test 
#include <boost/test/auto_unit_test.hpp>

#define foreach BOOST_FOREACH

using namespace std;
using namespace boost;
using namespace ::xce::feed::timeline;

time_t getTimeSys(int year,int month,int date)
{
   tm  timeinfo;
   timeinfo.tm_year = year-1900;
   timeinfo.tm_mon = month-1;
   timeinfo.tm_mday = date;
   timeinfo.tm_hour = 0;
   timeinfo.tm_min = 0;
   timeinfo.tm_sec = 0;
   return mktime(&timeinfo);
}


BOOST_AUTO_TEST_SUITE(timeline_test)

//tool function
void show(const YearMonthMap& result)
{
   cout<<"size: "<<result.size()<<endl;
   for(YearMonthMap::const_iterator it=result.begin();it!=result.end();++it)
   {
       cout<<"year:"<<it->first<<endl;
       const set<int> & months=it->second;
       for(YearMonthMap::value_type::second_type::iterator it2=months.begin();
             it2!=months.end();++it2)
       {
           cout<<" "<<*it2<<" ";
       }
       cout<<endl;
   }
}
void show(const UidYear & result)
{
   cout<<"size: "<<result.size()<<endl;
   for(UidYear::const_iterator it=result.begin();it!=result.end();++it)
   {
       cout<<"year:"<<it->first<<endl;
       const vector<int> & months=it->second;
       for(UidYear::value_type::second_type::const_iterator it2=months.begin();
             it2!=months.end();++it2)
       {
           cout<<" "<<*it2<<" ";
       }
       cout<<endl;
   }
}
void show(const TimeLineFeedInfoSeq & result)
{
   cout<<"size: "<<result.size()<<endl;
   foreach(const TimeLineFeedInfo& info, result)
   {
       cout<<" feeds: (";
       foreach(Ice::Long feed,info.feeds) {
           cout<<feed<<",";
       }
       cout<<" )";
   }
   cout<<endl;
}


// logic  use in TimeLineFeedI
void testTimeLineAdd(string &str, int year ,int month)
{
   YearMonthMap v;  timeLineToMap(str,v);
   v[year].insert(month);
   mapToTimeLine(v,str);
}
void testTimeLineRemove(string & str, int year,int  month)
{
   YearMonthMap v; timeLineToMap(str,v);
   v[year].erase(month);
   if(v[year].size()==0){//remove this year
      v.erase(year);
   }
   mapToTimeLine(v,str);
}
/*
time_t mkTime(int year,int month,int day,int hour=0,int min=0,int sec=0) 
{
    struct tm t;
    t.tm_year=year-1900;
    t.tm_mon=month-1;//-1,0-based
    t.tm_mday=day;
    t.tm_hour=0;t.tm_min=0;
    t.tm_sec=0;t.tm_isdst=0;
    return mktime_bj(&t);
}
time_t mkTimeSys(int year,int month,int day,int hour=0,int min=0,int sec=0) 
{
    tm  timeinfo;
    timeinfo.tm_year = year-1900;
    timeinfo.tm_mon = month-1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = min;
    timeinfo.tm_sec = sec;
    timeinfo.tm_isdst=0;
    return mktime(&timeinfo);
}*/ 

BOOST_AUTO_TEST_CASE( _test_json)
{
   cout<<"====================================="<<endl;

    cout<<"Run _test_json"<<endl;
    //
    Reader _reader; Value _root; stringstream ss; string json;
    //   {
    //     "default":
    //     {
    //        "lru":432000,
    //        "mstypes":[1205,9801,9802],
    //        "stat_path" : "/data/xce/XFeed/log/"
    //     },
    //     "configs":[
    //     {  
    //        "lru":432000,
    //        "mstypes":[1205,9801,9802]
    //     }
    //     ],
    //     "white_lists":[ 
    //        {"ids":[380504108],"idx":0} 
    //     ]
    //   }
    json="{\"default\":{\"lru\":432000,\"mstypes\":[1205,9801,9802],\
\"stat_path\":\"/data/xce/XFeed/log/\"},\"configs\":[{\"lru\":432000,\"mstypes\":[1205,9801,9802]}],\"white_lists\":[{\"ids\":[380504108],\"idx\":0}]}";
    bool res=_reader.parse(json, _root,false);
    BOOST_CHECK(res);
    BOOST_CHECK( _root.type()==objectValue);
    Value _dft = _root["default"];
    BOOST_CHECK( _dft.type()==objectValue);
    Value _path = _dft["stat_path"];
    cout<<_path.type()<<endl;
    cout<<stringValue<<endl;
    cout<<_path<<endl;
    BOOST_CHECK( _path.type()==stringValue);



    BOOST_CHECK( _root.type() == objectValue );
    const Value _default = _root["default"];
    BOOST_CHECK(_default.type() == objectValue );
    const Value _default_lru = _default["lru"];
    BOOST_CHECK(_default_lru.type() == intValue );
    const Value _default_mstypes = _default["mstypes"];
    BOOST_CHECK(_default_mstypes.type() == arrayValue );
    for(Value::iterator it= _default_mstypes.begin();it!=_default_mstypes.end();++it) 
    {
         BOOST_CHECK((*it).type() == intValue);
    }
    const Value _default_stat_path = _default["mstypes"];
    BOOST_CHECK( _default_stat_path.type() == stringValue ) ;
    const Value _configs = _root["configs"];
    BOOST_CHECK(_configs.type() == arrayValue );
    for(Value::iterator it= _configs.begin();it!=_configs.end();++it) 
    {
        SETTING_INFO("configs once.");
        BOOST_CHECK( (*it).type() == objectValue ) ;
        const Value _tmp_lru=(*it)["lru"];
        const Value _tmp_mstypes=(*it)["mstypes"];
        BOOST_CHECK( _tmp_lru.type() == intValue) ;
        BOOST_CHECK( _tmp_mstypes.type() == arrayValue ) ;
        for(Value::iterator it2=_tmp_mstypes.begin();it2!=_tmp_mstypes.end();++it2) {
            BOOST_CHECK( (*it).type()!=intValue );
        }
    }
    const Value _lists = _root["white_lists"];
    BOOST_CHECK( _lists.type() == arrayValue);
    for(Value::iterator it= _lists.begin();it!=_lists.end();++it) {
        SETTING_INFO("configs once.");
           BOOST_CHECK((*it).type() ==objectValue );
           const Value _tmp_idx = (*it)["idx"];
           BOOST_CHECK(_tmp_idx.type() == intValue );
           const Value _tmp_ary= (*it)["ids"];
           BOOST_CHECK(_tmp_ary.type() == arrayValue);
           for(Value::iterator it2= _tmp_ary.begin();it2!=_tmp_ary.end();++it2 ) {
               BOOST_CHECK( (*it2).type() ==intValue );
           } 
    }
/*
    // check for remainders
    const Value _remainders = _root["remainders"];
    if( _remainders.type()!=nullValue)
    {
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
    const Value _range = _root["range"];
    if( _range.type()!=nullValue)
    {
        if( _range.type()!=arrayValue || _range.size()==0 ) {RETURN_RESET("ranges is bad type or 0-size.");}
        for(Value::iterator it = _range.begin();it!= _range.end();++it)
        {
            if( (*it).type()!=arrayValue || (*it).size()!=3 ) { RETURN_RESET("In range, bad type or size !=3.");}
            const Value e0=(*it)[UInt(0)];const Value e1=(*it)[UInt(1)];const Value e2=(*it)[UInt(2)];
            if(e0.type()!=intValue||e1.type()!=intValue||e2.type()!=intValue) { RETURN_RESET("In remainders [],a not int type."<<e0.type());}
            if(e0.asInt() > e1.asInt() ) { RETURN_RESET("In range,the 1st can not be larger than the 2nd ."); }
            if(e2.asInt() <0 || (size_t)e2.asInt()>=configs_.size() ) { RETURN_RESET("in range,idx is outrange or negative."); }
            //ok insert
            range_vector_.push_back(make_tuple<int,int,int>(e0.asInt(),e1.asInt(),e2.asInt()));
        }
    }

*/
}



BOOST_AUTO_TEST_CASE( _mktime_bj_test_ )
{
   //  2008 
   //  00.00.00  00.00.01 12.00.00 23.59.59 23.59.00 5 
   //  1900 
   //  1700 ?
   //cout<<mkTime(1980,1,2,0,0,1)<<endl;
   //cout<<mkTimeSys(1980,1,2,0,0,1)<<endl;

   //cout<<"1980 1 1"<<endl;
   //c/out<<"sys   "<<getTimeSys(1980,1,1)<<endl;
   //cout<<"me   "<<getTime(1980,1,1)<<endl;
   time_t t=getTime(1980,1,1,0,0,0);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   //for convenience
   //BOOST_CHECK( make_tuple<int,int,int,int,int,int>(year,month,day,hour,min,sec) ==  make_tuple<int,int,int,int,int,int>(1980,1,1,0,0,0) );
   /*
   time_t t=getTime(1980,2,1);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   BOOST_CHECK(year==1980 && month==2&&day==1);
   
   time_t t=getTime(1980,2,1);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   BOOST_CHECK(year==1980 && month==2&&day==1);
   
   time_t t=getTime(1910,1,1);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   BOOST_CHECK(year==1980 && month==1&&day==1);
   
   time_t t=getTime(1980,1,1);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   BOOST_CHECK(year==1980 && month==1&&day==1);
   
   time_t t=getTime(1980,1,1);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   BOOST_CHECK(year==1980 && month==1&&day==1);
   
   time_t t=getTime(1980,1,1);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   BOOST_CHECK(year==1980 && month==1&&day==1);
   
   time_t t=getTime(1980,1,1);
   int year,month,day;string s;
   getYearMonthDay(t,year,month,day);
   BOOST_CHECK(year==1980 && month==1&&day==1);*/
   
}

BOOST_AUTO_TEST_CASE( _test_time_ )
{
   cout<<"Run _test_time"<<endl;
   int year, month, day,hour,min,sec;
   // 1600 - 2400
   
   //db support   
   getDayTime( (time_t)mysqlpp::DateTime("1930-1-1 00:00:00"),year,month,day,hour,min,sec);
   BOOST_CHECK(year==1930);
   BOOST_CHECK(month==1);
   BOOST_CHECK(day==1);
   BOOST_CHECK(hour==0);
   BOOST_CHECK(min==0);
   BOOST_CHECK(sec==0);

   getDayTime( (time_t)mysqlpp::DateTime("1970-1-1 00:00:00"),year,month,day,hour,min,sec);
   BOOST_CHECK(year==1970);
   BOOST_CHECK(month==1);
   BOOST_CHECK(day==1);
   BOOST_CHECK(hour==0);
   BOOST_CHECK(min==0);
   BOOST_CHECK(sec==0); 
   getDayTime( (time_t)mysqlpp::DateTime("1670-1-1 00:23:00"),year,month,day,hour,min,sec);
   BOOST_CHECK(year==1670);
   BOOST_CHECK(month==1);
   BOOST_CHECK(day==1);
   BOOST_CHECK(hour==0);
   BOOST_CHECK(min==23);
   BOOST_CHECK(sec==0);
   getDayTime( (time_t)mysqlpp::DateTime("2380-1-1 23:59:59"),year,month,day,hour,min,sec);
   BOOST_CHECK(year==2380);
   BOOST_CHECK(month==1);
   BOOST_CHECK(day==1);
   BOOST_CHECK(hour==23);
   BOOST_CHECK(min==59);
   BOOST_CHECK(sec==59);
   getDayTime( (time_t)mysqlpp::DateTime("2400-5-6 23:20:59"),year,month,day,hour,min,sec);
   BOOST_CHECK(year==2400);
   BOOST_CHECK(month==5);
   BOOST_CHECK(day==6);
   BOOST_CHECK(hour==23);
   BOOST_CHECK(min==20);
   BOOST_CHECK(sec==59);
   //basic  support

   getYearMonthDay(getTime(2038,12,2),year,month,day);
   BOOST_CHECK(2038==year);
   BOOST_CHECK(12==month);
   BOOST_CHECK(2==day); 

   getYearMonthDay(getTime(2020,12,2),year,month,day);
   BOOST_CHECK(2020==year);
   BOOST_CHECK(12==month);
   BOOST_CHECK(2==day); 
   //
   getDayTime(getTime(1900,1,1),year,month,day,hour,min,sec);
   BOOST_CHECK(1900==year);
   BOOST_CHECK(1==month);
   BOOST_CHECK(1==day);

   getYearMonthDay(getTime(2000,2,29),year,month,day);
   BOOST_CHECK(2000==year);
   BOOST_CHECK(2==month);
   BOOST_CHECK(29==day);

   getYearMonthDay(getTime(1900,2,28),year,month,day);
   BOOST_CHECK(1900==year);
   BOOST_CHECK(2==month);
   BOOST_CHECK(28==day);


   getYearMonthDay(getTime(1900,3,1),year,month,day);
   BOOST_CHECK(1900==year);
   BOOST_CHECK(3==month);
   BOOST_CHECK(1==day);


   getYearMonthDay(getTime(1850,12,2),year,month,day);
   BOOST_CHECK(1850==year);
   BOOST_CHECK(12==month);
   BOOST_CHECK(2==day);

   getYearMonthDay(getTime(1600,12,2),year,month,day);
   BOOST_CHECK(1600==year);
   BOOST_CHECK(12==month);
   BOOST_CHECK(2==day);
   
   getYearMonthDay(getTime(1600,12,2),year,month,day);
   BOOST_CHECK(1600==year);
   BOOST_CHECK(12==month);
   BOOST_CHECK(2==day);

   getYearMonthDay(getTime(2400,12,2),year,month,day);
   BOOST_CHECK(2400==year);
   BOOST_CHECK(12==month);
   BOOST_CHECK(2==day);

   getYearMonthDay(getTime(2360,12,2),year,month,day);
   BOOST_CHECK(2360==year);
   BOOST_CHECK(12==month);
   BOOST_CHECK(2==day);
}

// test object
TimeLineFeedI &feed=TimeLineFeedI::instance();


// test cases

BOOST_AUTO_TEST_CASE( _test_inTimeLine)
{
    cout<<"Run _test_inTimeLine_"<<endl;
    string line;
    line=":20000102";
    BOOST_CHECK(inTimeLine(line,2000,1));
    BOOST_CHECK(inTimeLine(line,2000,2));
    BOOST_CHECK(!inTimeLine(line,2000,0));
    BOOST_CHECK(!inTimeLine(line,2000,3));
    line=":200012021215:2012010105:::2304:201001";
    BOOST_CHECK(inTimeLine(line,2000,12));
    BOOST_CHECK(inTimeLine(line,2012,1));
    BOOST_CHECK(inTimeLine(line,2012,5));
    BOOST_CHECK(inTimeLine(line,2010,1));
    BOOST_CHECK(inTimeLine(line,2000,15));
    BOOST_CHECK(!inTimeLine(line,2012,2));
    line=":2000010305070911:1940020406081012";
    BOOST_CHECK(inTimeLine(line,2000,1));
    BOOST_CHECK(inTimeLine(line,2000,3));
    BOOST_CHECK(inTimeLine(line,2000,5));
    BOOST_CHECK(inTimeLine(line,2000,7));
    BOOST_CHECK(inTimeLine(line,2000,9));
    BOOST_CHECK(inTimeLine(line,2000,11));
    BOOST_CHECK(!inTimeLine(line,2000,2));
    BOOST_CHECK(!inTimeLine(line,2000,4));
    BOOST_CHECK(!inTimeLine(line,2000,6));
    BOOST_CHECK(!inTimeLine(line,2000,8));
    BOOST_CHECK(!inTimeLine(line,2000,10));
    BOOST_CHECK(!inTimeLine(line,2000,12));
    
    BOOST_CHECK(inTimeLine(line,1940,2));
    BOOST_CHECK(inTimeLine(line,1940,4));
    BOOST_CHECK(inTimeLine(line,1940,6));
    BOOST_CHECK(inTimeLine(line,1940,8));
    BOOST_CHECK(inTimeLine(line,1940,10));
    BOOST_CHECK(inTimeLine(line,1940,12));
    BOOST_CHECK(!inTimeLine(line,1940,1));
    BOOST_CHECK(!inTimeLine(line,1940,3));
    BOOST_CHECK(!inTimeLine(line,1940,5));
    BOOST_CHECK(!inTimeLine(line,1940,7));
    BOOST_CHECK(!inTimeLine(line,1940,9));
    BOOST_CHECK(!inTimeLine(line,1940,11));

}
BOOST_AUTO_TEST_CASE( _test_timeLineToMap_)
{
    cout<<"Run _test_timeLineToMap_"<<endl;
    YearMonthMap _map1;string _line1;
    YearMonthMap _map2;string _line2;
    // 1
    _line1=":1201020204";
    _map1.clear();_map1[2012].insert(1);_map1[2012].insert(2);_map1[2012].insert(4);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map1==_map2);
    // 2
    _line1=":12010203040506070809101112";
    _map1.clear();set<int> & _set=_map1[2012];_set.insert(1);_set.insert(2);
    _set.insert(3);_set.insert(4);_set.insert(5);_set.insert(6);_set.insert(7);
    _set.insert(8);_set.insert(9);_set.insert(10);_set.insert(11);_set.insert(12);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map1==_map2);
    // 3
    _line1=":120000000000000";
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2.size()==0);
    // 4
    _line1=":120";
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2.size()==0);
    // 5
    _line1=":0010";_map1.clear();
    _map1[2000].insert(10);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 6
    _line1=":9912131415";_map1.clear();
    _map1[1999].insert(12);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 7
    _line1=":5001";_map1.clear();
    _map1[1950].insert(1);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 8
    _line1=":100102:120304";_map1.clear();
    _map1[2010].insert(1); _map1[2010].insert(2);
    _map1[2012].insert(3);_map1[2012].insert(4);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 9
    _line1=":12:13:14:16:17";
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2.size()==0);    
    // 10
    _line1=":49010";_map1.clear();
    timeLineToMap(_line1,_map2);
    _map1[2049].insert(1);
    BOOST_CHECK(_map2==_map1);
    // 11
    _line1="awidy&^^RTIGWDIAGWDJGK";_map1.clear();
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map1==_map2);
    // 12
    _line1="aw:idy&^^R:TIGWDIA:GWDJGK";_map1.clear();
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map1==_map2);
    // 13
    _line1=":awidy&^^R:1201TIGWDIAGWDJGK";_map1.clear();
    _map1[2012].insert(1);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map1==_map2);

    //below for :yyyy
    //B1 
    _line1=":2012010203040506070809101112";_map1.clear();
    _map1[2012].insert(1);_map1[2012].insert(2);_map1[2012].insert(3);
    _map1[2012].insert(4);_map1[2012].insert(5);_map1[2012].insert(6);
    _map1[2012].insert(7);_map1[2012].insert(8);_map1[2012].insert(9);
    _map1[2012].insert(10);_map1[2012].insert(11);_map1[2012].insert(12);
    timeLineToMap(_line1, _map2);
    BOOST_CHECK(_map2==_map1);
    //B2 多个年
    _line1=":1911010203040506070809101112:000101020304070809101112:201001020304";_map1.clear();
    _map1[1911].insert(1);_map1[1911].insert(2);_map1[1911].insert(3);
    _map1[1911].insert(4);_map1[1911].insert(5);_map1[1911].insert(6);
    _map1[1911].insert(7);_map1[1911].insert(8);_map1[1911].insert(9);
    _map1[1911].insert(10);_map1[1911].insert(11);_map1[1911].insert(12);
    _map1[2000].insert(1);_map1[2000].insert(2);_map1[2000].insert(3);_map1[2000].insert(4);
    _map1[2000].insert(7);_map1[2000].insert(8);_map1[2000].insert(9);_map1[2000].insert(10);
    _map1[2000].insert(11);_map1[2000].insert(12);
    _map1[2010].insert(1);_map1[2010].insert(2);_map1[2010].insert(3);_map1[2010].insert(4);
    timeLineToMap(_line1,_map2);
    //show(_map1);
    //show(_map2);
    BOOST_CHECK(_map2==_map1);
    //B3  
    
    //B4  
    
    //B5  
    _line1=":19:192003:04:20:201205:060707";_map1.clear();
    _map1[1920].insert(3);_map1[2012].insert(5);_map1[2006].insert(7);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    //B6  
    _line1=":";
    
    //B7  
    _line1=":000102:xx01:x001:0x01:9901:/001:0/01://01::001:0:01:::01:0005";_map1.clear();
    _map1[2000].insert(1);_map1[2000].insert(2);_map1[2000].insert(5);
    _map1[1999].insert(1);
    timeLineToMap(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    
    //B8  
    _line1=":xx01020304:2090:2090010203:0x01x001:?*0102:20240506:)0:20190708.&^:%^:$:20080102";_map1.clear();
    _map1[2090].insert(1);_map1[2090].insert(2);_map1[2090].insert(3);
    _map1[2024].insert(5);_map1[2024].insert(6);_map1[2019].insert(7);_map1[2019].insert(8);
    _map1[2008].insert(1);_map1[2008].insert(2);
    timeLineToMap(_line1,_map2);
    //show(_map2);
    BOOST_CHECK(_map2==_map1);
}

BOOST_AUTO_TEST_CASE( _test_timeLineToUidYear_)
{
    cout<<"Run _test_timeLineToUidYear_"<<endl;
    UidYear _map1;string _line1;
    UidYear _map2;string _line2;
    // 1
    _line1=":1201020204";
    _map1.clear();_map1[2012].push_back(1);_map1[2012].push_back(2);_map1[2012].push_back(4);
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map1==_map2);
    // 2
    _line1=":12010203040506070809101112";_map1.clear();
    vector<int> & _v=_map1[2012];_v.push_back(1);_v.push_back(2);
    _v.push_back(3);_v.push_back(4);_v.push_back(5);_v.push_back(6);_v.push_back(7);
    _v.push_back(8);_v.push_back(9);_v.push_back(10);_v.push_back(11);_v.push_back(12);
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map1==_map2);
    // 3
    _line1=":120000000000000";
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map2.size()==0);
    // 4
    _line1=":120";
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map2.size()==0);
    // 5
    _line1=":0010";_map1.clear();
    _map1[2000].push_back(10);
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 6
    _line1=":9912131415";_map1.clear();
    _map1[1999].push_back(12);
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 7
    _line1=":5001";_map1.clear();
    _map1[1950].push_back(1);
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 8
    _line1=":100102:120304";_map1.clear();
    _map1[2010].push_back(1); _map1[2010].push_back(2);
    _map1[2012].push_back(3);_map1[2012].push_back(4);
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map2==_map1);
    // 9
    _line1=":12:13:14:16:17";
    timeLineToUidYear(_line1,_map2);
    BOOST_CHECK(_map2.size()==0);    
    // 10
    _line1=":49010";_map1.clear();
    timeLineToUidYear(_line1,_map2);
    _map1[2049].push_back(1);
    BOOST_CHECK(_map2==_map1);
    // 11
}
BOOST_AUTO_TEST_CASE( _test_mapToTimeLine_)
{
    cout<<"Run _test_mapToTimeLine_"<<endl;
    string _line1,_line2;
    YearMonthMap _map1,_map2;
    
    // 1
    _map1.clear(); _map1[11];
    mapToTimeLine(_map1,_line2);
    BOOST_CHECK( _line2.size()==0);
    // 2 
    _map1.clear(); _map1[2011];
    mapToTimeLine(_map1,_line2);
    BOOST_CHECK(_line2.size()==0);
    // 3
    _map1.clear(); _map1[2011].insert(1);_map1[2011].insert(2);
    mapToTimeLine(_map1,_line2);
    BOOST_CHECK(_line2==":20110102");
    // 4
    _map1.clear(); _map1[2012].insert(1);_map1[2013].insert(8);
    mapToTimeLine(_map1,_line2);
    BOOST_CHECK(_line2==":201201:201308");
    // 5
    _map1.clear();_map1[1923232].insert(1232);_map1[-11].insert(-221);
    _map1[2321].insert(12);_map1[0].insert(12);_map1[1987].insert(-221);
    _map1[1987].insert(13);_map1[1987].insert(0);_map1[1989].insert(6);
    _map1[1949].insert(10);_map1[2050].insert(1);
    mapToTimeLine(_map1,_line2);
    BOOST_CHECK(_line2==":194910:198906:205001:232112");
    // 6
    _map1.clear();
    _map1[1989].insert(11);_map1[1999].insert(10);_map1[2000].insert(1);_map1[2000].insert(2);
    _map1[2003].insert(12);_map1[2003].insert(1);_map1[2008].insert(13);_map1[2008].insert(12);
    _map1[2007].insert(2);_map1[2008].insert(2);_map1[2008].insert(4);_map1[2007].insert(1);
    _map1[2011].insert(1);_map1[2011].insert(9);_map1[2008].insert(3);_map1[2010].insert(2);
    _map1[2011].insert(10);_map1[2011].insert(2);_map1[2011].insert(5);_map1[2011].insert(11);
    _map1[2005].insert(5);_map1[2003].insert(8);_map1[2012].insert(1);_map1[2012].insert(12);
    _map1[2005].insert(6);_map1[2005].insert(1);_map1[2006].insert(6);_map1[1985].insert(12);
    mapToTimeLine(_map1,_line2);
    BOOST_CHECK(_line2==":198512:198911:199910:20000102:2003010812:2005010506:200606:20070102:200802030412:201002:2011010205091011:20120112");
}

BOOST_AUTO_TEST_CASE( _test_timeLineAdd_)
{
    cout<<"Run _test_timeLineAdd_"<<endl;
   string line;
   // 1
   line=":120101";
   testTimeLineAdd(line,2011,2);
   BOOST_CHECK(line==":201102:201201");
   // 2
   line="";
   testTimeLineAdd(line,2011,1);
   testTimeLineAdd(line,2011,12);
   BOOST_CHECK(line==":20110112");
   // 3
   line=":120101";
   testTimeLineAdd(line,2012,1);
   testTimeLineAdd(line,2012,2);
   testTimeLineAdd(line,2012,3);
   testTimeLineAdd(line,2012,4);
   testTimeLineAdd(line,2012,5);
   BOOST_CHECK(line==":20120102030405");
   // 4
   line="";
   testTimeLineAdd(line,1949,1);
   testTimeLineAdd(line,1950,12);
   testTimeLineAdd(line,1950,1);
   testTimeLineAdd(line,1950,0);
   testTimeLineAdd(line,1950,13);
   testTimeLineAdd(line,2049,1);
   testTimeLineAdd(line,2049,12);
   testTimeLineAdd(line,2049,0);
   testTimeLineAdd(line,2049,13);
   testTimeLineAdd(line,2050,1);
   testTimeLineAdd(line,2001,1);
   testTimeLineAdd(line,2002,2);
   testTimeLineAdd(line,2003,3);
   BOOST_CHECK(line==":194901:19500112:200101:200202:200303:20490112:205001");
   // 5
   line="::::::::1232124*&^@&%@(&$*@@!";
   testTimeLineAdd(line,2011,2);
   BOOST_CHECK(line==":201102:201212");
   // 6
   line="我是谁？？？";
   testTimeLineAdd(line,1999,13);
   testTimeLineAdd(line,1987,0);
   BOOST_CHECK(line=="");
   // 7
   line=":121315";
   testTimeLineAdd(line,-12,1);
   testTimeLineAdd(line,2011,11);
   //cout<<line<<endl;
   BOOST_CHECK(line==":201111");
   // 8
   line=":1201";
   testTimeLineAdd(line,2012,1);
   BOOST_CHECK(line==":201201");
   // 9
}
BOOST_AUTO_TEST_CASE( _test_timeLineRemove_)
{
    cout<<"Run _test_timeLineRemove_"<<endl;
   string line;
   // 1
   line=":10";
   testTimeLineRemove(line,2012,12);
   BOOST_CHECK(line=="");
   // 2
   line="$^%%&*^&*%^%*&^(*^&^";
   testTimeLineRemove(line,2012,12);
   BOOST_CHECK(line=="");
   // 3
   line=":1204";
   testTimeLineRemove(line,2012,0);
   BOOST_CHECK(line==":201204");
   // 4
   line=":12010203040506:990100050506";
   testTimeLineRemove(line,2012,0);
   testTimeLineRemove(line,2012,1);
   testTimeLineRemove(line,2012,2);
   testTimeLineRemove(line,2012,3);
   testTimeLineRemove(line,2012,4);
   testTimeLineRemove(line,1912,5);
   testTimeLineRemove(line,2012,6);
   BOOST_CHECK(line==":1999010506:201205");
   testTimeLineRemove(line,2012,5);
   BOOST_CHECK(line==":1999010506");
   testTimeLineRemove(line,1999,01);
   testTimeLineRemove(line,1999,05);
   BOOST_CHECK(line==":199906");
   testTimeLineRemove(line,1999,06);
   BOOST_CHECK(line=="");
} 
/*
BOOST_AUTO_TEST_CASE( _test_rearrangeFeeds)
{  
    cout<<"Run _test_rearrangeFeeds"<<endl;
    vector<node> v;
    TimeLineFeedInfoSeq res;
    TimeLineFeedInfoSeq seq;
    TimeLineFeedInfo info;
    info.flag=0;
    feed.setting_.merge_stypes.clear();
    //test stype merge
    //feed,time,stype  flag(wide)
    //  2   1    200    1             (2,5,13,23,-21) (0)
    //  3   1    100    0             (3) (0)
    // -1   2    300    1             (9) (1)
    //  5   2    200    0             (7) (1) 
    //  7   2    400    1             (-1) (1)
    //  9   2    500    1             (20) (0)
    // 20   3    100    0     (200)   (19) (0)
    // 13   3    200    1      =>     (17) (0)
    // 17   3    500    0             (13) (1)
    // 19   3    400    0             (66) (0)
    // 66   4    300    0             (50) (0)
    // 23   4    200    1             (14) (1)
    // 14   5    100    1             (12) (0)
    // 50   5    100    0             (-7) (1)
    // 12   6    100    0             (-9) (1)
    //-21   7    200    1             
    // -7   7    400    1
    // -9   9    500    1
    feed.setting_.merge_stypes.clear();feed.setting_.merge_stypes.insert(200);
    v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
    v.push_back(node( 2,1,makeWide(200,1),200,0));
    v.push_back(node( 3,1,makeWide(100,0),100,0));
    v.push_back(node(-1,2,makeWide(300,1),300,0));
    v.push_back(node( 5,2,makeWide(200,0),200,0));
    v.push_back(node( 7,2,makeWide(400,1),400,0));
    v.push_back(node( 9,2,makeWide(500,1),500,0));
    v.push_back(node(20,3,makeWide(100,0),100,0));
    v.push_back(node(13,3,makeWide(200,1),200,0));
    v.push_back(node(17,3,makeWide(500,0),500,0));
    v.push_back(node(19,3,makeWide(400,0),400,0));
    v.push_back(node(66,4,makeWide(300,0),300,0));
    v.push_back(node(23,4,makeWide(200,1),200,0));
    v.push_back(node(14,5,makeWide(100,1),100,0));
    v.push_back(node(50,5,makeWide(100,0),100,0));
    v.push_back(node(12,6,makeWide(100,0),100,0));
    v.push_back(node(-21,7,makeWide(200,1),200,0));
    v.push_back(node(-7,7,makeWide(400,1),400,0));
    v.push_back(node(-9,9,makeWide(500,1),500,0));
    
    info.feeds.clear();info.flag=0;
    info.feeds.push_back(2);
    info.feeds.push_back(5);
    info.feeds.push_back(13);
    info.feeds.push_back(23);
    info.feeds.push_back(-21);
    info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(3);info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(9);info.flag=1;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(7);info.flag=1;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(-1);info.flag=1;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(20);info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(19);info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(17);info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(66);info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(50);info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(14);info.flag=1;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(12);info.flag=0;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(-7);info.flag=1;seq.push_back(info);
    info.feeds.clear();info.feeds.push_back(-9);info.flag=1;seq.push_back(info);
    feed.rearrangeFeeds(v.begin(),v.end(),res);
    show(res);
    cout<<"========"<<endl;
    show(seq);
    BOOST_CHECK(res==seq);

    //test stype merge
    //feed,time,stype  flag(wide)
    //  2   1    200    1             (2,5,13,23,-21) (0)
    //  3   1    100    0             (3) (0)
    // -1   2    300    1             (9) (1)
    //  5   2    200    0             (7) (1) 
    //  7   2    400    1             (-1) (1)
    //  9   2    500    1             (20) (0)
    // 20   3    100    0     (200)   (19) (0)
    // 13   3    200    1      =>     (17) (0)
    // 17   3    500    0             (13) (1)
    // 19   3    400    0             (66) (0)
    // 66   4    300    0             (50) (0)
    // 23   4    200    1             (14) (1)
    // 14   5    100    1             (12) (0)

   // A1
   
   // time feed
   //  7  2           7   2
   //  6  3           6   3
   //  4  -1          4   5
   //  4  5           4  -1
   //  4  -3   ==>    4  -3
   //  3  7           3   7
   //  3  0           3   4
   //  3  4           3   0
   //  2  9           2   9
   //  1  6           1   6
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   v.push_back(node(2,7,0,0,0));v.push_back(node(3,6,0,0,0));
   v.push_back(node(-1,4,0,0,0));v.push_back(node(5,4,0,0,0));
   v.push_back(node(-3,4,0,0,0));v.push_back(node(7,3,0,0,0));
   v.push_back(node(0,3,0,0,0));v.push_back(node(4,3,0,0,0));
   v.push_back(node(9,2,0,0,0));v.push_back(node(6,1,0,0,0));
   
   info.feeds.clear();info.feeds.push_back(2);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(3);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(5);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-1);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-3);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(7);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(4);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(0);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(9);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(6);seq.push_back(info);
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   BOOST_CHECK(res==seq);
   // A2
   //  10  10         10  10
   //  9   9           9   9
   //  8   8           8   8
   //  7   7           7   7
   //  6   6   ==>     6   6
   //  5   5           5   5
   //  4   4           4   4
   //  3   3           3   3
   //  2   2           2   2
   //  1   1           1   1
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   v.push_back(node(10,10,0,0,0));v.push_back(node(9,9,0,0,0));
   v.push_back(node(8,8,0,0,0));v.push_back(node(7,7,0,0,0));
   v.push_back(node(6,6,0,0,0));v.push_back(node(5,5,0,0,0));
   v.push_back(node(4,4,0,0,0));v.push_back(node(3,3,0,0,0));
   v.push_back(node(2,2,0,0,0));v.push_back(node(1,1,0,0,0));
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   info.feeds.clear();info.feeds.push_back(10);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(9);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(8);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(7);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(6);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(5);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(4);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(3);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(2);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(1);seq.push_back(info);
   BOOST_CHECK(res==seq);
    
    //写的没什么章法.不细致全面 哎

   // A3
   //  10  -3         10   -1
   //  10  -1         10   -3   
   //  9   6           9   7    
   //  9   7           9   6    
   //  8   0           8   66     
   //  8   -2          8   12      
   //  8   66          8   4      
   //  8   12          8   2   
   //  8   4           8   0
   //  8   2           8  -2
   //  6   -10   ==>   6  -10
   //  5   5           5   8
   //  5   8           5   5
   //  4   99          4   99
   //  3   3           3   3
   //  1   9           1   10
   //  1   10          1   9
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   v.push_back(node(-3,10,0,0,0));v.push_back(node(-1,10,0,0,0));
   v.push_back(node(6,9,0,0,0));v.push_back(node(7,9,0,0,0));
   v.push_back(node(0,8,0,0,0));v.push_back(node(-2,8,0,0,0));
   v.push_back(node(66,8,0,0,0));v.push_back(node(12,8,0,0,0));
   v.push_back(node(4,8,0,0,0));v.push_back(node(2,8,0,0,0));
   v.push_back(node(-10,6,0,0,0));v.push_back(node(5,5,0,0,0));
   v.push_back(node(8,5,0,0,0));v.push_back(node(99,4,0,0,0));
   v.push_back(node(3,3,0,0,0));v.push_back(node(9,1,0,0,0));
   v.push_back(node(10,1,0,0,0));
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   info.feeds.clear();info.feeds.push_back(-1);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-3);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(7);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(6);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(66);seq.push_back(info);
   info.feeds.clear();;info.feeds.push_back(12);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(4);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(2);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(0);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-2);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-10);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(8);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(5);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(99);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(3);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(10);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(9);seq.push_back(info);
   BOOST_CHECK(res==seq);
   
   //A 4
   //
   // null  => null
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   BOOST_CHECK(res==seq);
   //A5
   //
   // 1 10  =>  1 10
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   v.push_back(node(10,1,0,0,0));
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   info.feeds.clear();info.feeds.push_back(10);seq.push_back(info);
   BOOST_CHECK(res==seq);
   //A6
   //
   // 1 3   =>  1 3
   // 2 4       2 4
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   v.push_back(node(3,1,0,0,0));v.push_back(node(4,2,0,0,0));
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   info.feeds.clear();info.feeds.push_back(3);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(4);seq.push_back(info);
   BOOST_CHECK(res==seq);
   //A7
   //
   //  1   3    =>  1  3   
   //  1   -1       1  0
   //  1   -2       1 -1
   //  1   0        1 -2
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   v.push_back(node(3,1,0,0,0));v.push_back(node(-1,1,0,0,0));
   v.push_back(node(-2,1,0,0,0));v.push_back(node(0,1,0,0,0));
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   info.feeds.clear();info.feeds.push_back(3);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(0);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-1);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-2);seq.push_back(info);
   BOOST_CHECK(res==seq);
   //A8
   //
   //  0  -10           0  2
   //  0  2    =>       0  -1 
   //  0  -1            0  -10
   //  2  3             2  3
   feed.setting_.merge_stypes.clear();
   v.clear();res.clear();seq.clear();info.feeds.clear();info.flag=0;
   v.push_back(node(-10,0,0,0,0));v.push_back(node(2,0,0,0,0));
   v.push_back(node(-1,0,0,0,0));v.push_back(node(3,2,0,0,0));
   feed.rearrangeFeeds(v.begin(),v.end(),res);
   info.feeds.clear();info.feeds.push_back(2);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-1);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(-10);seq.push_back(info);
   info.feeds.clear();info.feeds.push_back(3);seq.push_back(info);
   BOOST_CHECK(res==seq);
}*/

 
BOOST_AUTO_TEST_SUITE_END()


