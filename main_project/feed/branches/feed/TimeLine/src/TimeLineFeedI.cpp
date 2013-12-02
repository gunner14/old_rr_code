#include <time.h>
#include <ServiceI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <string.h>
#include <algorithm>
#include "util/cpp/InvokeMonitor.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "ObjectCache.h"
#include "QueryRunner.h"
#include "TimeLineFeedI.h"

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace xce::serverstate;
using namespace xce::feed::timeline;

void MyUtil::initialize() {
  ServiceI& server = ServiceI::instance();
  server.getAdapter()->add(&TimeLineFeedI::instance(),server.createIdentity("M",""));
  ServantFactoryPtr factory=new ServantFactory;  
  for(int i=0;i<100;i++) {
    ostringstream os;
    os<<"TimeLine"<<i;
    server.registerObjectCache(i,os.str(),factory,true);
  }
  int mod=server.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("TimeLineFeed.Mod",0);
  int interval=server.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("TimeLineFeed.Interval",5);
  string ctrEndpoint=server.getCommunicator()->getProperties()->getPropertyWithDefault("TimeLine.ControllerEndpoint", "ControllerTimeLineFeed");
  ServerStateSubscriber::instance().initialize(ctrEndpoint, &TimeLineFeedI::instance(), mod, interval, new XceFeedControllerChannel());
}


UidYear TimeLineFeedI::query(int uid,const Ice::Current&) {
  string str=queryDB(uid);
  return translate(str);
}
string TimeLineFeedI::queryDB(int uid) {
  string table_name = GetTableName(uid);
  Statement sql;
  if(uid < 0 ) {
    MCE_WARN("TimeLineFeedI uid error!uid:"<<uid);
    return string();
  }
  sql << "select timeline from "<< table_name << " where uid = "<<uid;
  mysqlpp::StoreQueryResult res;
  try  {    
    res = QueryRunner(FeedDBInstance,1,table_name).store(sql);
  } catch (mysqlpp::Exception& e) {
    MCE_FATAL("mysql error!uid:"<<uid<<e.what());
    return string();
  } 
  if(!res  ) {
    MCE_FATAL("mysql error!uid:"<<uid);
    return string();
  } else if (res.num_rows()==0) {
    MCE_INFO("query function no data!uid:"<<uid);
    return string();
  } else {
     for (unsigned  int i=0; i < res.num_rows();i++) {
      mysqlpp::Row row = res.at(i);
      string str=(mysqlpp::Null<string, mysqlpp::NullIsBlank>) (row["timeline"]);
      return str;
     }
  } 
}
int TimeLineFeedI::string2int(string str) {
  int result=0;
  for(int  i=0;i<str.size();i++) {
    result*=10;
    result+=str[i]-'0';
  }   
  return result;
}
UidYear TimeLineFeedI::translate(string str) { 
  UidYear result;
  string strtmp;
  int tmp;
  if(str.size()==0) return result;
  for(int i=0;i<str.size();) { 
    if(str[i]==':') {  
      strtmp.assign(str,i+1,2);
      tmp=string2int(strtmp);
      tmp+=2000;
      result[tmp]=vector<int>();
      i+=3;
      for(;i<str.size() &&str[i]!=':';i+=2) {
        strtmp.assign(str,i,2);
        result[tmp].push_back(string2int(strtmp));
        } 
     }
     sort(result[tmp].begin(),result[tmp].end(),greater<int>()); 
  }  
  return result;
}
string TimeLineFeedI::int2string(int t) {
  if(t>13) {
    t-=2000;
  }
  char result[5];
  int bit[5]={0};
  int i=0;
  while(t) {
    bit[i++]=t%10;
    t/=10;
  }
  for(i=1;i>=0;i--) {
    result[1-i]=char(bit[i]+48);
  }
  result[2]='\0';
  string tmp(result);
  return tmp;
}
void TimeLineFeedI::modify_month(int uid,int preyear,int premonth,int newyear,int newmonth,int mark) {
  //mark=0 删除原来的，增加新的
  //mark=1 删除原来的，不动新的
  //mark=2 不动原来的，增加新的
  //mark=3 不动原来的，不动新的
  if(mark==3)return ;
  string str=queryDB(uid);
  MCE_INFO("TimeLineFeedI modify_month original str"<<str);
  string strtmp,result;
  int tmp,pos;
  if(mark==0||mark==1) {
    pos=-1;
    for(int i=0;i<str.size()&&pos==-1;) {
      if(str[i]==':') {
        strtmp.assign(str,i+1,2);
        tmp=string2int(strtmp);
        tmp+=2000;
        if(tmp==preyear) {
          int bg=i+2,end;
          i+=3;
          for(;i<str.size()&&str[i]!=':';i+=2) {
            if(pos==-1) {
              strtmp.assign(str,i,2);
              tmp=string2int(strtmp);
              if(tmp==premonth) {
                pos=i;
              }
            }
          }
          end=i;
          if(end-bg==3) {
            result=str.substr(0,bg-2)+str.substr(end,str.size()-end); 
          } else {
            result=str.substr(0,pos)+str.substr(pos+2,str.size()-pos-2);
          }
        } else i++;
      } else i++;
    }
    str=result;
  } 
  if(mark==0||mark==2) {
    pos=-1;
    bool flag=false;
    for(int i=0;i<str.size()&&pos==-1;) {
      if(str[i]==':') {
        strtmp.assign(str,i+1,2);
        tmp=string2int(strtmp);
        tmp+=2000;
        if(tmp==newyear) {
          i+=3;
          flag=true;
          for(;i<str.size()&&str[i]!=':';i+=2) {
            if(pos==-1) {
              strtmp.assign(str,i,2);
              tmp=string2int(strtmp);
              if(tmp<newmonth)continue;
              else {
                pos=i;
                break;
              }
            }
          }

        } else {
          if(tmp>newyear) {
            pos=i;
            break;
          } 
          i++;
        }
      } else i++;
    }
    if(!flag) {
      string res=":";
      res=res+int2string(newyear)+int2string(newmonth);
      if(pos==-1) {
        str=str+res; 
      } else {
        str=str.substr(0,pos)+res+str.substr(pos,str.size()-pos);
      }
    } else {
      string res=int2string(newmonth);
      str=str.substr(0,pos)+res+str.substr(pos,str.size()-pos);
    }
  }
  MCE_INFO("TimeLineFeedI modify_month after modify str"<<str); 
  //Statement sql;
  string table_name=GetTableName(uid);
  ostringstream sql;
  sql << "update "<<table_name<<" set timeline="<<str<<" where uid="<<uid;
  MCE_INFO(sql.str());
  //QueryRunner(FeedDBInstance,3,table_name).execute(sql);
}

void TimeLineFeedI::insert(int uid,const string& name,const Ice::Current& current) {
  FILE* file;
  string path="/data/xce/XFeed/bin/maoyoux_data/";  
  path=path+name; 
  file=fopen(path.c_str(),"r");
  if(file==NULL) {
     MCE_FATAL("open file error!");
     return ;
  }  
  string table_name = GetTableName(uid);
  char str[1000];
  int i=0,len,num=0;
  Statement sql;
  ostringstream os;
  os << "mysql replace time test";
  int line=0;
  InvokeClient tm = InvokeClient::create(current,os.str(),InvokeClient::INVOKE_LEVEL_INFO);
  
  std::string user[100],timeline[100];
  while(fgets(str ,999,file)) {
    num=0;
    do  { 
      i=0;
      len=strlen(str);
      if(len<5)continue;
      while(str[i]!=' '&&i<len)i++;
      user[num].assign(str,0,i);
      timeline[num++].assign(str,i+1,len-i-2);
    } while(num<100&&fgets(str,999,file));
    sql.clear(); 
    sql << "replace into "<<table_name<<" (uid,timeline) VALUES ";
    for(int j=0;j<num;j++) {
      sql <<"('"<<user[j]<<"','"<<timeline[j]<<"')";
      if(j<num-1)sql<<",";
    }  
    try  { 
      QueryRunner(FeedDBInstance,3,table_name).execute(sql);
    } catch (mysqlpp::Exception& e) {
      MCE_FATAL("load fail mysql error!"<<e.what());
    } 
    line+=num;
  }
  MCE_INFO("line:"<<line);
  fclose(file);
  //sql << "LOAD DATA INFILE '/data/xce/XFeed/bin/maoyoux_data/data' INTO TABLE "<<table_name<<" FIELDS TERMINATED BY ' ' LINES TERMINATED BY '\n' ";
} 

int TimeLineFeedI::QueryFeedSize(int uid,int year,int month,const Ice::Current&) {
  FeedIdseq result;
  ServiceI& server = ServiceI::instance();
  NodePtr node=new Node;
  if(uid < 0 || year < 1900 || month > 12 || month < 1) {
    MCE_WARN("TimeLineFeedI QueryFeedSize uid time error! uid:"<<uid);
    return 0;
  }
  if (server.hasObject(uid%100,uid)) {
    node=server.findObject<NodePtr>(uid%100,uid);
    if(node->feedtime==year*100+month) {
      MCE_INFO("test find");
      return node->feeds.size();
    } else {
      node->feeds=QueryFeedIdFromDB(uid,year,month);
      node->feedtime=year*100+month;
      MCE_INFO("test modify");
      return node->feeds.size();
    }
  } else {
    result=QueryFeedIdFromDB(uid,year,month);
    node->feedtime=year*100+month;
    node->feeds=result;
    server.addObject(node,uid%100,uid);
    MCE_INFO("test not find");
    return node->feeds.size();
  }
}
FeedIdseq TimeLineFeedI::QueryFeedId(int uid,int year,int month,int bg,int limit,const Ice::Current&) {
  FeedIdseq result;
  ServiceI& server = ServiceI::instance();
  NodePtr node=new Node;
  
  if(uid < 0 || year < 1900 || month > 12 || month < 1) {
    MCE_WARN("TimeLineFeedI QueryFeedId uid time error! uid:"<<uid);
    return result;
  }
  if(server.hasObject(uid%100,uid)) {
    node=server.findObject<NodePtr>(uid%100,uid);
    if(node->feedtime==year*100+month) {
      MCE_INFO("test find");
      //return node->feeds;
    } else {
      node->feeds=QueryFeedIdFromDB(uid,year,month);
      node->feedtime=year*100+month;
      MCE_INFO("test modify");
      //return node->feeds;
     }
  } else {
    result=QueryFeedIdFromDB(uid,year,month);
    node->feedtime=year*100+month;
    node->feeds=result;
    server.addObject(node,uid%100,uid);
    MCE_INFO("test not find");
    //return node->feeds;
   }
  vector<Ice::Long>::iterator bgit,endit;
  int i;
  for(i=0,bgit=node->feeds.begin();bgit!=node->feeds.end()&&i<bg;++i,++bgit);
  endit=bgit;
  if(limit==-1)endit=node->feeds.end();
  else for(i=0;endit!=node->feeds.end()&&i<limit;++i,++endit);
  vector<Ice::Long>res(bgit,endit);
  return res;
}
FeedIdseq TimeLineFeedI::QueryFeedIdFromDB(int uid,int year,int month) {
  string dbname,table_name;
  get_feed_mini_tablename(uid,table_name,dbname);
  FeedIdseq result;
   if(year<1900||month>12||month<1) {
    MCE_WARN("TimeLineFeedI QueryFeedId time error!");
    return result;
   } 
  if(uid < 0 ) {
    MCE_WARN("TimeLineFeedI QueryFeedIdFromDB uid  error! uid:"<<uid);
    return result;
  }
  time_t t_of_day_bg,t_of_day_end;
  t_of_day_bg=getTime(year,month,1);
  t_of_day_end=getTime(year,month+1,1);
  Statement sql;
  sql << "select feed  from "<<table_name<<" where user="<<uid<<" and time >=FROM_UNIXTIME("<<t_of_day_bg<<") and time < FROM_UNIXTIME("<<t_of_day_end<<")";
  mysqlpp::StoreQueryResult res=QueryRunner(dbname,1,table_name).store(sql);
  if(!res) {
    MCE_FATAL("TimeLineFeedI QueryFeedId fail uid:"<<uid);
    return result;
  } else if(res.num_rows()==0){
    MCE_FATAL("TimeLineFeedI QueryFeedId no data uid:"<<uid<<"year:"<<year<<"month:"<<month);
    return result;
  } else {
    for(unsigned int i=0;i<res.num_rows();i++) {
      mysqlpp::Row row = res.at(i);
      result.push_back(row["feed"]);
      //MCE_INFO(row["small_type"]);
    }
    sort(result.begin(),result.end(),greater<Ice::Long>());
    return result;
  }  
} 

void TimeLineFeedI::Count(int uid,const Ice::Current&) {
  Statement sql;
  string table_name=GetTableName(uid);
  sql<<"select count(*) as num from "<<table_name;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(FeedDBInstance,1,table_name).store(sql);
  } catch(...) {
    MCE_WARN("QueryRunner error!");
    return ;
  }
  mysqlpp::Row row = res.at(0);
  MCE_INFO(row["num"]);
}

void TimeLineFeedI::CancelWideScreen(int uid,::Ice::Long feed,const Ice::Current&) {
  
  if(uid < 0 || feed < 0) {
    MCE_WARN("TimeLineFeedI CancelWideScreen uid feed error! uid:"<<uid<<"feed:"<<feed);
    return ;
  }
  Statement sql;
  string dbname,tablename;
  get_feed_content_tablename(feed,tablename,dbname);
  unsigned int tmp=0xfeffffff;
  sql << "select type from "<<tablename<<" where  feed="<<feed;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(dbname,1,tablename).store(sql);
  } catch (mysqlpp::Exception& e) {
    MCE_FATAL("SetNormal mysql select error!"<<e.what());
    return ;
  } catch (...) {
    MCE_FATAL("SetNomal select error");
    return ;
  }
  int type;
  if(res.num_rows()>0) {
    mysqlpp::Row row = res.at(0);
    type=row["type"];
    MCE_INFO("Original type:"<<type);

    type = ((unsigned int)(type) & tmp);
    sql.clear();
    sql << "update "<<tablename<<" set type = "<<type<<" where actor="<<uid<<" and feed="<<feed;
  } else {
    MCE_INFO("no find!");
    return ;
  }
  try  { 
    QueryRunner(dbname,3,tablename).execute(sql);
    MCE_INFO("update type:"<<type);
  } catch (mysqlpp::Exception& e) {
    MCE_FATAL("SetNormal mysql update error!"<<e.what());
  } catch (...) {
    MCE_FATAL("SetNormal  fail!");
  }
}
void TimeLineFeedI::SetWideScreen(int uid,::Ice::Long feed,const Ice::Current&) {
  Statement sql;
  string dbname,tablename;
  get_feed_content_tablename(feed,tablename,dbname);
  unsigned int tmp=1<<24;
  if(uid < 0 || feed < 0) {
    MCE_WARN("TimeLineFeedI SetWideScreen uid feed error! uid:"<<uid<<"feed:"<<feed);
    return ;
  }
  sql << "select type from "<<tablename<<" where feed="<<feed;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(dbname,1,tablename).store(sql);
  } catch (mysqlpp::Exception& e) {
    MCE_FATAL("TimeLineFeedI SetWideScreen mysql select error!"<<e.what());
    return ;
  } catch (...) {
    MCE_FATAL("TimeLineFeedI SetWideScreen select error");
    return ;
  }
  int type;
  if(res.num_rows()>0) {
    mysqlpp::Row row = res.at(0);
    type=row["type"];
    MCE_INFO("Original type:"<<type);
    type = ((unsigned int)(type) | tmp);
    sql.clear();
    sql << "update "<<tablename<<" set type = "<<type<<" where actor="<<uid<<" and feed="<<feed;
  } else {
    MCE_WARN("TimeFeedI SetWideScreen no data!uid:"<<uid<<"feed:"<<feed);
    return ;
  }
  try { 
    QueryRunner(dbname,3,tablename).execute(sql);
    MCE_INFO("update type:"<<type);
  } catch (mysqlpp::Exception& e) {
    MCE_FATAL("TimeLineFeedI SetWideScreen mysql update error!"<<e.what()<<"uid:"<<uid<<"feed:"<<feed);
  } catch (...) {
    MCE_FATAL("TimeLineFeedI SetWideScreen  fail!uid:"<<uid<<"feed:"<<feed);
  }
}
bool TimeLineFeedI::isWideScreen(int uid,::Ice::Long feed,const Ice::Current&) {
  Statement sql;
  string dbname,tablename;
  get_feed_content_tablename(feed,tablename,dbname);
  
  if(uid < 0 || feed < 0) {
    MCE_WARN("TimeLineFeedI isWideScreen uid feed error! uid:"<<uid<<"feed:"<<feed);
    return false;
  }
  
  unsigned int tmp=1<<24;
  sql << "select type from "<<tablename<<" where  feed="<<feed;
  mysqlpp::StoreQueryResult res;
  try  { 
    res = QueryRunner(dbname,1,tablename).store(sql);
    
  } catch (mysqlpp::Exception& e) {
    MCE_FATAL("TimeLineFeedI isWideScreen mysql select error!"<<e.what()<<"uid:"<<uid<<"feed:"<<feed);
    return false;
  } catch (...) {
    MCE_FATAL("TimeLineFeedI is WideScreen error!uid:"<<uid<<"feed:"<<feed);
    return false;
  }
  if(res.num_rows()!=0) { 
    mysqlpp::Row row = res.at(0);
    int type=row["type"];
    MCE_INFO("Judge type:"<<type);
    if((unsigned int)type & tmp)return true;
    else return false;
  }  
}
int TimeLineFeedI::QueryFeedSizeByStype(int uid,int year,int month,const IntSeq& types,const Ice::Current&) {
  set<int>myset(types.begin(),types.end());
  int result=0;
  string table_name,dbname;
  get_feed_mini_tablename(uid,table_name,dbname);
  if(year<1900||month>12||month<1) {
    MCE_WARN("TimeLineFeedI QueryFeedId time error!");
    return result;
  }

  if(uid<0) {
    MCE_WARN("TimeLineFeedI QueryFeedIdBystype uid feed error!");
    return result;
  }
  time_t t_of_day_bg,t_of_day_end;
  t_of_day_bg=getTime(year,month,1);
  t_of_day_end=getTime(year,month+1,1);
  Statement sql;
  sql << "select feed,small_type from "<<table_name<<" where user="<<uid<<" and time >=FROM_UNIXTIME("<<t_of_day_bg<<") and time < FROM_UNIXTIME("<<t_of_day_end<<")";
  mysqlpp::StoreQueryResult res=QueryRunner(dbname,1,table_name).store(sql);
  if(!res) {
    MCE_FATAL("TimeLineFeedI QueryFeedId fail uid:"<<uid);
    return result;
  } else if(res.num_rows()==0){
    MCE_FATAL("TimeLineFeedI QueryFeedId no data uid:"<<uid<<"year:"<<year<<"month:"<<month);
    return result;
  } else {
    
    for(unsigned int i=0;i<res.num_rows();i++) {
      mysqlpp::Row row = res.at(i);
      int tmp=row["small_type"];
      //tmp=((unsigned int)tmp)&0xffff;
      MCE_INFO("stype:"<<tmp<<"feed:"<<row["feed"]);
      if(myset.count(tmp))result++;
    }

    return result;
  }
}
FeedIdseq TimeLineFeedI::QueryFeedIdBystype(int uid,int year,int month,const IntSeq& types,int bg,int limit,const Ice::Current& ) {
  set<int>myset(types.begin(),types.end());
  FeedIdseq result;
  string table_name,dbname;
  get_feed_mini_tablename(uid,table_name,dbname);
  if(year<1900||month>12||month<1) {
    MCE_WARN("TimeLineFeedI QueryFeedId time error!");
    return result;
  }  
  if(uid<0) {
    MCE_WARN("TimeLineFeedI QueryFeedIdBystype uid feed error!");
    return result;
  }
  time_t t_of_day_bg,t_of_day_end;
  t_of_day_bg=getTime(year,month,1);
  t_of_day_end=getTime(year,month+1,1);
  Statement sql;
  sql << "select feed,small_type from "<<table_name<<" where user="<<uid<<" and time >=FROM_UNIXTIME("<<t_of_day_bg<<") and time < FROM_UNIXTIME("<<t_of_day_end<<")";
  mysqlpp::StoreQueryResult res=QueryRunner(dbname,1,table_name).store(sql);
  if(!res) {
    MCE_FATAL("TimeLineFeedI QueryFeedId fail uid:"<<uid);
    return result;
  } else if(res.num_rows()==0){
    MCE_FATAL("TimeLineFeedI QueryFeedId no data uid:"<<uid<<"year:"<<year<<"month:"<<month);
    return result;
  } else {
    
    for(unsigned int i=0;i<res.num_rows();i++) {
      mysqlpp::Row row = res.at(i);
      int tmp=row["small_type"];
      //tmp=((unsigned int)tmp)&0xffff;
      MCE_INFO("stype:"<<tmp<<"feed:"<<row["feed"]);
      if(myset.count(tmp))result.push_back(row["feed"]);
    }
    sort(result.begin(),result.end(),greater<Ice::Long>());
    vector<Ice::Long>::iterator bgit,endit;
    int k=0;
    for(k=0,bgit=result.begin();bgit!=result.end()&&k<bg;++k,++bgit);
    endit=bgit;
    if(limit==-1)endit=result.end();
    else for(k=0;endit!=result.end()&&k<limit;++k,++endit);
    vector<Ice::Long>res(bgit,endit);

    return res;
  }
}

void TimeLineFeedI::test(::Ice::Long feed,const Ice::Current&) {
  string tablename,dbname;
  get_feed_content_tablename(feed,tablename,dbname);
  Statement sql;
  sql << "select small_type,big_type,type from " <<tablename<<" where feed="<<feed;
  mysqlpp::StoreQueryResult res;
  try {
    res = QueryRunner(dbname,1,tablename).store(sql);
    if(res.num_rows()>0) {
      mysqlpp::Row row=res.at(0);
      MCE_INFO("small_type:"<<row["small_type"]<<"big_type:"<<row["big_type"]<<"type:"<<row["type"]);
    } else {
      MCE_WARN("no data!");
    }
  } catch(mysqlpp::Exception& e) {
    MCE_WARN("mysql error!"<<e.what());
  } catch(...) {
    MCE_WARN("test wrong!");
  }

}

void TimeLineFeedI::modify(int uid, Ice::Long feed, int year, int month, int day, const Ice::Current& current) {
  if(year<1900||month>12||month<1||day>31||day<1) {
    MCE_WARN("TimeLineFeedI modify uid:"<<uid<<"feed:"<<feed<<"time error!");
    return ;
  }
  if(uid<0 || feed <0) {
    MCE_WARN("TimeLineFeedI modify uid feed error! uid:"<<uid<<"feed:"<<feed);
    return ;
  }
  string table_name,dbname;
  get_feed_mini_tablename(uid,table_name,dbname);
  Statement sql;
  mysqlpp::StoreQueryResult res;
  time_t pretime;
  struct tm *p;
  sql << "select time from "<<table_name<<" where user="<<uid<<" and feed="<<feed;
  try {
    res=QueryRunner(dbname,1,table_name).store(sql);
    if(res.num_rows()>0) {
      mysqlpp::Row row=res.at(0);
      pretime=row["time"];
      p=localtime(&pretime);
      p->tm_year+=1900;
      p->tm_mon+=1;
    } else {
      MCE_WARN("TimeLineFeedI modify uid:"<<uid<<"feed:"<<feed<<"no data !");
      return ;
    }
  } catch(mysqlpp::Exception& e) {
    MCE_WARN("TimeLineFeedI modify "<<"uid:"<<uid<<"feed:"<<feed<<"mysql error!"<<e.what());
    return ;
  } catch(...) {
    MCE_WARN("TimeLineFeedI modify error!");
    return ;
  }
  if(p->tm_year==year&&p->tm_mon==month) {
    if(day==31) {
      day=1;
      month++;
      if(month>12) {
        month=1;
        year++;
      }
    }
    time_t modify_time=getTime(year,month,day)-1;
    //Statement sql1;
    ostringstream sql1;
    sql1 << "update "<<table_name<<" set time = "<<modify_time<<" where user="<<uid<<" and feed="<<feed;
    MCE_INFO(sql1.str()); 
    //QueryRunner(dbname,3,table_name).execute(sql1);
    return ;
  } else {
    int size=QueryFeedSize(uid,p->tm_year,p->tm_mon,current);
    int size1=QueryFeedSize(uid,year,month,current);
    if(size==1&&size1==0) {
      modify_month(uid,p->tm_year,p->tm_mon,year,month,0); 
    } else if(size==1&&size1!=0){
      modify_month(uid,p->tm_year,p->tm_mon,year,month,1); 
    } else if(size!=1&&size1==0) {
      modify_month(uid,p->tm_year,p->tm_mon,year,month,2); 
    } else {
      modify_month(uid,p->tm_year,p->tm_mon,year,month,3); 
    }
    removeCache(uid,year,month);
    removeCache(uid,p->tm_year,p->tm_mon);
    if(day==31) {
      day=1;
      month++;
      if(month>12) {
        month=1;
        year++;
      }
    }
    time_t modify_time=getTime(year,month,day)-1;
    //Statement sql1;
    ostringstream sql1;
    sql1 << "update "<<table_name<<" set time = "<<modify_time<<" where user="<<uid<<" and feed="<<feed;
    MCE_INFO(sql1.str());
    //QueryRunner(dbname,3,table_name).execute(sql1);
    return ;
    
  }
}

void TimeLineFeedI::removeCache(int uid,int year,int month) {
  ServiceI& server = ServiceI::instance();
  NodePtr node=new Node;
  if(server.hasObject(uid%100,uid)) {
    node=server.findObject<NodePtr>(uid%100,uid);
    if(node->feedtime==year*100+month) {
      server.removeObject(uid%100,uid);
    }
  }
}

