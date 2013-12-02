
#include "feed_index_memc_client.h"

#include <boost/lexical_cast.hpp>
#include "compress_wrap.h"
// #include "feedcontent.pb.h"
//#include "FeedIndex.pb.h"
//#include "FeedContent.pb.h"

#include "ServiceI.h"
#include "LogWrapper.h"
#include <string>

#include <iostream>
#include <set>

namespace xce {
namespace feed {

static char * kEmptyFeed = "EMPTY_FEED";
const size_t FIndexMemcClient::VALUE_ITEM_SIZE = sizeof(MemcValueItem);
const size_t FIndexMemcClient::KEY_SIZE = 50;
const size_t FIndexMemcClient::VALUE_LIMIT = VALUE_ITEM_SIZE * 50;
//const size_t FIndexMemcClient::MUTEX_COUNT = 100;
FIndexMemcClient::FIndexMemcClient() {
  pool_.Init(250);
}

FIndexMemcClient::~FIndexMemcClient() {
}

string  FIndexMemcClient::GetKey(Ice::Long source, Ice::Int stype) {
  string key_str;
  char key_arr[KEY_SIZE] = {0};
  int prt_ret = snprintf(key_arr, KEY_SIZE, "FINDEX#%ld,%d", source, stype);
  if(prt_ret < 0) {
    MCE_WARN("FIndexMemcClient::GetFIndex --> output error. source:" << source << " stype:" << stype);
    //cout << "FIndexMemcClient::GetFIndex --> output error. source:" << source << " stype:" << stype << endl;
    return key_str; 
  } else if((size_t)prt_ret >= KEY_SIZE) {
    MCE_WARN("FIndexMemcClient::GetFIndex --> key error. source:" << source << " stype:" << stype);
    //cout << "FIndexMemcClient::GetFIndex --> key error. source:" << source << " stype:" << stype << endl;
    return key_str; 
  }
  key_str = key_arr;
  return key_str;
}

void FIndexMemcClient::SetBuffer(void * buf, size_t off, MemcValueItem v) {
  //cout << "value size:" << VALUE_ITEM_SIZE << " " << &(v.actor) << " " << &(v.feedid) << endl;
  
  *(static_cast<MemcValueItem*>(buf) + off) = v;
/*
  int i =0;
  char * ptr = (char*)buf;
  for(;i<15;i++) {
    printf("%hhx ",*(ptr+i));
  } 
*/
}

bool FIndexMemcClient::ParseBuffer(const void * buf, size_t len, vector<MemcValueItem> & v) {
  if(!buf || len <= 0) {
    MCE_WARN("FIndexMemcClient::ParseBuffer --> buf is null or len error. len:" << len);
    //cout << "FIndexMemcClient::ParseBuffer --> buf is null or len error. len:" << len  << endl;
    return false;
  }
  size_t item_num = len /  VALUE_ITEM_SIZE ; 

  if(len % VALUE_ITEM_SIZE) {
    MCE_WARN("FIndexMemcClient::ParseBuffer --> len error. len:" << len);
    //cout << "FIndexMemcClient::ParseBuffer --> len error. len:" << len << endl;
    return false;
  }

  for(size_t i = 0; i < item_num; i++) {
    v.push_back(*(static_cast<const MemcValueItem*>(buf) + i));
  }

  return true;
  
}

bool FIndexMemcClient::AddFIndex(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Long feedid, Ice::Long miniMerge) {
  MCE_INFO("FIndexMemcClient::AddFIndex --> source:" << source << " stype:" << stype << " actor:" << actor << " feedid:" << feedid << " miniMerge:" << miniMerge);
  //cout << "FIndexMemcClient::AddFIndex --> source:" << source << " stype:" << stype << " actor:" << actor << " feedid:" << feedid << endl;

  string key_str = GetKey(source, stype);
  if(key_str.empty()) {
     MCE_WARN("FIndexMemcClient::AddFIndex --> source:" << source << " stype:" << stype);
     //cout << "FIndexMemcClient::AddFIndex --> source:" << source << " stype:" << stype << endl;
     return false;
  }

  pair<int, MemcachedClient*> cli = pool_.GetClient(source); 
  if(!cli.second){
    MCE_WARN("FIndexMemcClient::AddFIndex --> MemcachedClient ptr is NULL!  source:" << source);
    //cout << "FIndexMemcClient::AddFIndex --> MemcachedClient ptr is NULL!  source:" << source << endl;
    return false;
  }

  char value_arr[VALUE_ITEM_SIZE];
  MemcValueItem mv; 
  mv.actor = actor;
  mv.feedid = feedid;
  mv.miniMerge = miniMerge;
  SetBuffer(value_arr,0,mv); 

  TimeStatFIndex ts;
  float cost_mem = 0.0;
  int r = cli.second->Set(key_str, value_arr, sizeof(MemcValueItem), 0);

  cost_mem = ts.getTime();
  ts.reset();

  MCE_INFO("FIndexMemcClient::AddFIndex -->  key:" << key_str << " r:" << r << " set time:" << cost_mem); 
  //cout << "FIndexMemcClient::AddFIndex -->  key:" << key_str << " r:" << r << " set time:" << cost_mem << endl; 

  pool_.ReleaseClient(cli.first, cli.second);
  return r;
}

bool FIndexMemcClient::AddFIndex(string key, char * value_arr,  MemcachedClient * mcc) {
  TimeStatFIndex ts;
  float cost_mem = 0.0;
 
  int r = mcc->Set(key, value_arr, sizeof(MemcValueItem), 0);
  cost_mem = ts.getTime(); 

  MCE_INFO("FIndexMemcClient::AddFIndex(append) -->  key:" << key << " r:" << r << " set time:" << cost_mem); 
  //cout << "FIndexMemcClient::AddFIndex -->  key:" << key << " r:" << r << " set time:" << cost_mem << endl; 
  return r;
}


bool FIndexMemcClient::AppendFIndex(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Long feedid, Ice::Long miniMerge) {
  MCE_INFO("FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype << " actor:" << actor << " feedid:" << feedid << " miniMerge:" << miniMerge);
  //cout << "FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype << " actor:" << actor << " feedid:" << feedid << endl;

  string key_str = GetKey(source, stype);
  if(key_str.empty()) {
     MCE_WARN("FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype);
     //cout << "FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype << endl;
     return false;
  }

  pair<int, MemcachedClient*> cli = pool_.GetClient(source); 
  if(!cli.second){
    MCE_WARN("FIndexMemcClient::AppendFIndex --> MemcachedClient ptr is NULL!  source:" << source);
    //cout << "FIndexMemcClient::AppendFIndex --> MemcachedClient ptr is NULL!  source:" << source << endl;
    return false;
  }

  char value_arr[VALUE_ITEM_SIZE];
  MemcValueItem mv; 
  mv.actor = actor;
  mv.feedid = feedid;
  mv.miniMerge = miniMerge;
  SetBuffer(value_arr,0,mv); 


 
  TimeStatFIndex ts;
  float cost_mem = 0.0;


  int r = cli.second->Append(key_str, value_arr, sizeof(MemcValueItem), 0);
  cost_mem = ts.getTime();

  MCE_INFO("FIndexMemcClient::AppendFIndex -->  ret:" << r << " key_str:" << key_str << "append time:" << cost_mem); 
  //cout << "res:" << r  << " key_str:" << key_str << std::endl;
  pool_.ReleaseClient(cli.first, cli.second);
  return r;
}
/*
bool FIndexMemcClient::AppendFIndex(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Long feedid, Ice::Long miniMerge) {
  MCE_INFO("FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype << " actor:" << actor << " feedid:" << feedid << " miniMerge:" << miniMerge);
  //cout << "FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype << " actor:" << actor << " feedid:" << feedid << endl;

  string key_str = GetKey(source, stype);
  if(key_str.empty()) {
     MCE_WARN("FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype);
     //cout << "FIndexMemcClient::AppendFIndex --> source:" << source << " stype:" << stype << endl;
     return false;
  }
  vector<string> keys;
  keys.push_back(key_str);

  pair<int, MemcachedClient*> cli = pool_.GetClient(source); 
  if(!cli.second){
    MCE_WARN("FIndexMemcClient::AppendFIndex --> MemcachedClient ptr is NULL!  source:" << source);
    //cout << "FIndexMemcClient::AppendFIndex --> MemcachedClient ptr is NULL!  source:" << source << endl;
    return false;
  }

  char value_arr[VALUE_ITEM_SIZE];
  MemcValueItem mv; 
  mv.actor=actor;
  mv.feedid=feedid;
  SetBuffer(value_arr,0,mv); 


 
  map<string, string> res;
  map<string, int> flags;
  TimeStatFIndex ts;

  {
    IceUtil::Mutex::Lock lock(mutexs_[source % MUTEX_COUNT]); 

    float cost_mem = 0.0;
    bool succ = cli.second->Get(keys, res, flags);

    cost_mem = ts.getTime();
    ts.reset();
    MCE_INFO("FIndexMemcClient::AppendFIndex --> key:" << key_str << "get time:" << cost_mem);
    //cout << "FIndexMemcClient::AppendFIndex --> key:" << key_str << "get time:" << cost_mem << endl;

    if(succ) {
      if(!res.size()) {
        MCE_INFO("FIndexMemcClient::AppendFIndex -->  Get NULL.  key:" << key_str); //这里没有判断没有取到的原因
        //cout << "FIndexMemcClient::AppendFIndex -->  Get NULL.  key:" << key_str << endl; 
        bool ret = AddFIndex(key_str, value_arr, cli.second);  
        pool_.ReleaseClient(cli.first, cli.second);
        return ret;
      } 
    } else {
      MCE_WARN("FIndexMemcClient::AppendFIndex -->  Get failed!  key:" << key_str);
      //cout << "FIndexMemcClient::AppendFIndex -->  Get failed!  key:" << key_str << endl;
      bool ret = AddFIndex(key_str, value_arr, cli.second);  
      pool_.ReleaseClient(cli.first, cli.second);
      return ret;
    }

    map<string, string>::iterator mit = res.find(key_str);
    if(mit == res.end()) {
      MCE_WARN("FIndexMemcClient::AppendFIndex -->  res has not key!  key:" << key_str);
      pool_.ReleaseClient(cli.first, cli.second);
      return false;
    } else {
      size_t len = mit->second.size();
      if(len >= VALUE_LIMIT) {  // 超过50条，全部删除。 也可以改成LRU
        MCE_WARN("FIndexMemcClient::AppendFIndex -->  value too long!  len:" << len);
        bool ret = AddFIndex(key_str, value_arr, cli.second);  
        pool_.ReleaseClient(cli.first, cli.second);
        return ret;
      }
    }

    int r = cli.second->Append(key_str, value_arr, sizeof(MemcValueItem), 0);
    cost_mem = ts.getTime();

    MCE_INFO("FIndexMemcClient::AppendFIndex -->  ret:" << r << " key_str:" << key_str << "append time:" << cost_mem); 
  }
  //cout << "res:" << r  << " key_str:" << key_str << std::endl;
  pool_.ReleaseClient(cli.first, cli.second);
  return 1;
}
*/
FIndexMemcClient::FIValue FIndexMemcClient::GetFIndex(Ice::Long source, Ice::Int stype) {
  FIValue value;

  vector<string> keys;
  string key_str = GetKey(source, stype);
  if(key_str.empty()) {
    MCE_WARN("FIndexMemcClient::GetFIndex --> key_str is empty!  source:" << source << " stype:" << stype);
    //cout << "FIndexMemcClient::GetFIndex --> key_str is empty!  source:" << source << " stype:" << stype << endl;
  }
  keys.push_back(key_str);

  pair<int, MemcachedClient*> cli = pool_.GetClient(source); 
  if(!cli.second){
    MCE_WARN("FIndexMemcClient::GetFIndex --> MemcachedClient ptr is NULL!  source:" << source);
    //cout << "FIndexMemcClient::GetFIndex --> MemcachedClient ptr is NULL!  source:" << source << endl;
    return value ;
  }

  map<string, string> res;
  map<string, int> flags;
  TimeStatFIndex ts;
  float cost_mem = 0.0;
  bool succ = cli.second->Get(keys, res, flags);
  if(succ) {
     if(!res.size()) {
       MCE_INFO("FIndexMemcClient::GetFIndex -->  Get NULL.  key:" << key_str); 
       //cout << "FIndexMemcClient::GetFIndex -->  Get NULL.  key:" << key_str << endl; 
       pool_.ReleaseClient(cli.first, cli.second);
       return value;  
     } 
  } else {
    MCE_WARN("FIndexMemcClient::GetFIndex -->  Get failed!  key:" << key_str);
    //cout << "FIndexMemcClient::GetFIndex -->  Get failed!  key:" << key_str << endl;
    pool_.ReleaseClient(cli.first, cli.second);
    return value ;
  }
  cost_mem = ts.getTime();
  ts.reset();
  MCE_INFO("FIndexMemcClient::GetFIndex --> key:" << key_str << " time:" << cost_mem);
  //cout << "FIndexMemcClient::GetFIndex --> key:" << key_str << " time:" << cost_mem << endl;
  map<string, string>::iterator it = res.find(key_str);
  if(it != res.end()) {
    MCE_INFO("FIndexMemcClient::GetFIndex --> key:" << it->first << " value size:" <<  it->second.size());
    //cout <<"FIndexMemcClient::GetFIndex --> key:" << it->first << " value size:" <<  it->second.size() << endl;
    const char * ptr =  it->second.c_str();
    size_t value_size = it->second.size();  
    bool succ = ParseBuffer(ptr, value_size, value); 
    if(succ) {
      MCE_INFO("FIndexMemcClient::GetFIndex --> key:" << it->first << " vector size:" <<  value.size());
      //cout << "FIndexMemcClient::GetFIndex --> key:" << it->first << " vector size:" <<  value.size() << endl;
    } else {
      MCE_WARN("FIndexMemcClient::GetFIndex -->  ParseBuffer error.  key:" << key_str);
      //cout << "FIndexMemcClient::GetFIndex -->  ParseBuffer error.  key:" << key_str << endl;
    } 
  } else {
    MCE_WARN("FIndexMemcClient::GetFIndex -->  res has not found key:" << key_str);
    //cout << "FIndexMemcClient::GetFIndex -->  res has not found key:" << key_str << endl;
  }

  //string ip = cli.second->GetIp();
  //cout << "ip:" << ip << endl;
  pool_.ReleaseClient(cli.first, cli.second);
  return value;
}

bool FIndexMemcClient::RemoveFIndex(Ice::Long source, Ice::Int stype) {
  MCE_INFO("FIndexMemcClient::RemoveFIndex --> source:" << source << " stype:" << stype);
  string key_str = GetKey(source, stype);
  if(key_str.empty()) {
    MCE_WARN("FIndexMemcClient::RemoveFIndex --> key_str is empty!  source:" << source << " stype:" << stype);
    //cout << "FIndexMemcClient::RemoveFIndex --> key_str is empty!  source:" << source << " stype:" << stype << endl;
  }

  pair<int, MemcachedClient*> cli = pool_.GetClient(source); 
  if(!cli.second){
    MCE_WARN("FIndexMemcClient::RemoveFIndex --> MemcachedClient ptr is NULL!  source:" << source);
    //cout << "FIndexMemcClient::RemoveFIndex --> MemcachedClient ptr is NULL!  source:" << source << endl;
    return false;
  }

  map<string, string> res;
  map<string, int> flags;
  TimeStatFIndex ts;
  float cost_mem = 0.0;
  if(!cli.second->Delete(key_str)) {
    MCE_WARN("FIndexMemcClient::RemoveFIndex -->  remove failed!  key:" << key_str);
    //cout << "FIndexMemcClient::RemoveFIndex -->  remove failed!  key:" << key_str << endl;
    pool_.ReleaseClient(cli.first, cli.second);
    return false;
  }

  cost_mem = ts.getTime();
  ts.reset();
  MCE_INFO("FIndexMemcClient::RemoveFIndex --> key:" << key_str << " time:" << cost_mem);
  //cout << "FIndexMemcClient::RemoveFIndex --> key:" << key_str << " time:" << cost_mem << endl;
  pool_.ReleaseClient(cli.first, cli.second);
  return true;
  
}
 
}
}

/*

int main(int argc, char* argv[]) {
//  cout << "main..." << endl;
  //xce::feed::FeedContentPtr content = xce::feed::FeedMemcClient::instance().GetFeed(15428536907);
  //cout << ":" << content->data->feed << endl;
//  int r =  xce::feed::FIndexMemcClient::instance().AddFIndex(22, 23, 9, 10);
 //cout << "r:" << r << endl;
  //  xce::feed::FIndexMemcClient::FIValue value =  xce::feed::FIndexMemcClient::instance().GetFIndex(22, 23);
    //xce::feed::FindexMemeClient::MemcValueItem item = value[0];
//    if(value.size()) {
//      cout << "actor:" << value[0].actor << " feedid:" << value[0].feedid << endl;
////      cout << "actor:" << value[1].actor << " feedid:" << value[1].feedid << endl;
//    }else {
//      cout << "value 0." << endl;
//    }
//    bool b =  xce::feed::FIndexMemcClient::instance().RemoveFIndex(22, 23);
 //   cout << "b:" << b << endl;

  int r =  xce::feed::FIndexMemcClient::instance().AppendFIndex(1,2,6,7,8);
 cout << "r:" << r << endl;
//   r =  xce::feed::FIndexMemcClient::instance().AddFIndex(1,2,3,4,5);
 //cout << "r2:" << r << endl;
  xce::feed::FIndexMemcClient::FIValue value =  xce::feed::FIndexMemcClient::instance().GetFIndex(1, 2);
    //xce::feed::FindexMemeClient::MemcValueItem item = value[0];
int i =0;
    for(; i< value.size(); i++) {
      cout << "actor:" << value[i].actor << " feedid:" << value[i].feedid <<  " miniMerge:" << value[i].miniMerge  << " size:" << value.size()<< endl;
//      cout << "actor:" << value[1].actor << " feedid:" << value[1].feedid << endl;
    }
}

*/
