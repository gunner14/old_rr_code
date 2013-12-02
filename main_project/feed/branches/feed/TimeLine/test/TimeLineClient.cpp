#include "TimeLineFeedReplicaAdapter.h"
#include <map>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace ::xce::feed::timeline;
int main() {
  //map<int,vector<int> >result;
  //vector<long long>result; 
  vector<int>tmp;
  TimeLineFeedReplicaAdapter& timeline=TimeLineFeedReplicaAdapter::instance();
  int uid,year,month,mark;
  long long feed;
  while(1) {
    printf("0:插入数据到数据库\n");
    printf("1:查看数据库count\n");
    printf("2:查看uid数据\n");
    printf("3:查看uid的一个月feedid,bg,limit\n");
    printf("4:查询uid的一个月feedid的size\n");
    printf("5:SetNormal\n");
    printf("6:SetWideScreen\n");
    printf("7:judge wide\n");
    printf("8:查看uid的一个月feedid,bg,limit,types\n");
    printf("9:test\n");
    printf("10:quit\n");
    scanf("%d",&mark);
    if(mark==0) {
       printf("输入uid,str\n");
       string str;
       cin>>uid>>str;
       timeline.insert(uid,str);
    } else if(mark==1) {
      printf("输入uid\n");
      cin>>uid;
      timeline.Count(uid);
    } else if(mark==2){
      printf("输入uid\n");
      cin>>uid;
      UidYear result=timeline.query(uid);
      cout<<"size:"<<result.size()<<endl;
      for(map<int,vector<int> >::reverse_iterator it=result.rbegin();it!=result.rend();++it) {
        cout<<"year:"<<it->first<<"month:";
        tmp=it->second;
        for(int j=0;j<tmp.size();j++) {
          cout<<tmp[j]<<" ";
         }
        cout<<"\n";
      } 
    } else if(mark==3) {  
      printf("输入uid,year,month,bg,limit\n");
      int bg,limit;
      cin>>uid>>year>>month>>bg>>limit;
      FeedIdseq result;
      result=timeline.QueryFeedId(uid,year,month,bg,limit);
      cout<<"size:"<<result.size()<<"\n";
       for(int i=0;i<result.size();i++) {
        cout<<result[i]<<" ";
      } 
      cout<<"\n";
    } else if(mark==4) {
      printf("输入uid,year,month\n");
      cin>>uid>>year>>month; 
      cout<<timeline.QueryFeedSize(uid,year,month)<<endl; 
    } else if(mark==5) {
      printf("输入uid,feed\n");
      cin>>uid>>feed;
      timeline.CancelWideScreen(uid,feed);
    } else if(mark==6) {
      printf("输入uid,feed\n");
      cin>>uid>>feed;
      timeline.SetWideScreen(uid,feed);
    } else if(mark==7) {
      printf("输入uid,feed\n");
      cin>>uid>>feed;
      cout<<timeline.isWideScreen(uid,feed)<<endl;
    } else if(mark==8) {
      printf("输入uid,year,month,bg,limit,types\n");
      vector<int >types;
      int type,bg,limit;
      cin>>uid>>year>>month>>bg>>limit>>type;
      types.push_back(type);
      FeedIdseq result;
      result=timeline.QueryFeedIdBystype(uid,year,month,types,bg,limit);
      cout<<"size:"<<result.size()<<"\n";
       for(int i=0;i<result.size();i++) {
        cout<<result[i]<<" ";
      }  
      cout<<"\n";

    } else if(mark==9) {
      printf("输入feed\n");
      cin>>feed;
      timeline.test(feed);
    } else break;
  }
  return 0;
}
