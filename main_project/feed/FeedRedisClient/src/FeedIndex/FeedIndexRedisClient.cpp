
#include "FeedIndexRedisClient.h"

#include <boost/lexical_cast.hpp>
// #include "feedcontent.pb.h"
//#include "FeedIndex.pb.h"
//#include "FeedContent.pb.h"
#include "RFeed.h"

#include <string>
#include <iostream>
#include "LogWrapper.h"

namespace xce {
namespace feed {

const size_t FeedIndexRedisClient::LIST_ITEM_SIZE = sizeof(IndexItem);
const size_t FeedIndexRedisClient::KEY_SIZE = 50;
const size_t FeedIndexRedisClient::LIST_LIMIT = 100;

FeedIndexRedisClient::FeedIndexRedisClient() {
}

FeedIndexRedisClient::~FeedIndexRedisClient() {
}

string  FeedIndexRedisClient::GetKey(Ice::Long source, Ice::Int stype) {
  string key_str;
  char key_arr[KEY_SIZE] = {0};
  int prt_ret = snprintf(key_arr, KEY_SIZE, "FEEDINDEX#%ld,%d", source, stype);
  if (prt_ret < 0) {
    MCE_WARN("FeedIndexRedisClient::GetKey --> output error. source:" << source << " stype:" << stype);
    //cout << "FeedIndexRedisClient::GetKey --> output error. source:" << source << " stype:" << stype << endl;
    return key_str; 
  } else if ((size_t)prt_ret >= KEY_SIZE) {
    MCE_WARN("FeedIndexRedisClient::GetKey --> key error. source:" << source << " stype:" << stype);
    //cout << "FeedIndexRedisClient::GetKey --> key error. source:" << source << " stype:" << stype << endl;
    return key_str; 
  }
  key_str = key_arr;
  return key_str;
}

void FeedIndexRedisClient::Encode(void * buf, const IndexItem& item) {
  *(static_cast< IndexItem * >(buf)) = item;
}

bool FeedIndexRedisClient::AddIndex(FeedIndexPtr index) {
  if (!index) {
    MCE_WARN("FeedIndexRedisClient::AddIndex -- index is NULL!");
    return false;
  }

  std::ostringstream oss;
  oss << "FeedIndexRedisClient::AddIndex --> source:" << index->source << " stype:" << index->stype << " actor:" << index->actor << " feedid:" << index->feed;
  MCE_INFO(oss.str());
  TimeLog tl(oss.str());

  string key_str = GetKey(index->source, index->stype);
  if(key_str.empty()) {
     MCE_WARN(oss.str());
     //cout << "FeedIndexRedisClient::AddIndex --> source:" << source << " stype:" << stype << endl;
     return false;
  }


  char value_arr[LIST_ITEM_SIZE];
  IndexItem index_item; 
  index_item.actor = index->actor;
  index_item.time = index->time;
  index_item.stype = index->stype;
  index_item.ptype = index->ptype;
  index_item.feedid = index->feed;
  index_item.miniMerge = index->miniMerge;
  index_item.newsMerge = index->newsMerge;
  index_item.source= index->source;
  index_item.psource= index->psource;

  Encode(value_arr, index_item); 
  
  xce::feed::RedisBinArgv arg(key_str);
  arg.PushArg(value_arr, LIST_ITEM_SIZE);


  
  return Push2List(key_str, arg);
  
}


bool FeedIndexRedisClient::GetIndex(Ice::Long source, Ice::Int stype, std::vector<FeedIndexRedisClient::IndexItem>& vec) {

  std::ostringstream oss;
  oss << "FeedIndexRedisClient::GetIndex --> source:" << source << " stype:" << stype;
  TimeLog tl(oss.str());
  MCE_INFO(oss.str());


  string key_str = GetKey(source, stype);
  if(key_str.empty()) {
    MCE_WARN("FeedIndexRedisClient::GetIndex --> key_str is empty!  source:" << source << " stype:" << stype);
    return false; 
  }


  return GetList<FeedIndexRedisClient::IndexItem>(key_str, vec, 0, LIST_LIMIT); 
}

bool FeedIndexRedisClient::RemoveIndex(Ice::Long source, Ice::Int stype) {
  MCE_INFO("FeedIndexRedisClient::RemoveIndex --> source:" << source << " stype:" << stype);
  string key_str = GetKey(source, stype);
  if(key_str.empty()) {
    MCE_WARN("FeedIndexRedisClient::RemoveIndex --> key_str is empty!  source:" << source << " stype:" << stype);
    return false;
  }

  return Del(key_str);
  
}
 
}
}

/*

using namespace xce::feed;

int main(int argc, char* argv[]) {
  xce::feed::FeedIndexPtr index = new xce::feed::FeedIndex; 
  index->feed = 999999;
  index->newsMerge = 123;
  index->miniMerge = 123;
  index->source = 1234;
  index->psource = 123;
  index->actor = 456;
  index->time = 555;
  index->stype = 666;
  index->ptype = 600;

  int r =  xce::feed::FeedIndexRedisClient::instance().AddIndex(index);
 cout << "r:" << r << endl;
//
//  xce::feed::FeedIndexPtr index2 = new xce::feed::FeedIndex; 
//  index2->feed = 999998;
//  index2->newsMerge = 11;
//  index2->miniMerge = 12;
//  index2->source = 123;
//  index2->psource = 123;
//  index2->actor = 678;
//  index2->time = 555;
//  index2->stype = 666;
//  index2->ptype = 600;
//   r =  xce::feed::FeedIndexRedisClient::instance().AddIndex(index2);
// cout << "r:" << r << endl;

  
//  int r3 =  xce::feed::FeedIndexRedisClient::instance().RemoveIndex(123, 666);
//  cout << " r3:" << r3 << endl;
//
 std::vector<FeedIndexRedisClient::IndexItem> value;
int   r2 =  xce::feed::FeedIndexRedisClient::instance().GetIndex(1234,666, value);
   cout << "r2:" << r2 << endl;
//    //xce::feed::FindexMemeClient::IndexItem item = value[0];
int i =0;
//
    for(; i< value.size(); i++) {
      cout << "actor:" << value[i].actor << " feedid:" << value[i].feedid <<  " miniMerge:" << value[i].miniMerge  << " size:" << value.size()<< endl;
//      cout << "actor:" << value[1].actor << " feedid:" << value[1].feedid << endl;
    }
}
*/


