#include "JourneyLogicAdapter.h"
#include "boost/lexical_cast.hpp"
#include "Journey/IntSerialize.h"

#include <iostream>

using namespace std;
using namespace MyUtil;
using namespace xce::tripod;
using namespace boost;
using namespace xce::journey::util; 

void JourneyLogicAdapter::invote(long key, int guestId) {
  xce::tempmutex::Lock lock(tempMutexManagerPtr_, key);
  string incCount = count_client_->Inc(lexical_cast<string>(key), "1");
  if(incCount == "" || incCount.size() == 0) {
    if (getCount(key) == 0) {
      count_client_->Set(lexical_cast<string>(key), "1", kJourneyExpire);
    }
  }
  //
  map<string, string> set_with_scores;

  //Negative value is for the sort!!
  set_with_scores.insert(make_pair(lexical_cast<string>(-time_adapter_->get_now()), IntSerialize::serialize(guestId))); 
  list_client_->ZAdd(lexical_cast<string>(key), set_with_scores);
  list_client_->ZRemRangeByRank(lexical_cast<string>(key), 19, -1);
  
//  xce::tempmutex::Lock lock(tempMutexManagerPtr_, key);
//  //cout << "It is in the JourneyLogicAdapter::invote()" << endl;
//  if(count_client_->Inc(lexical_cast<string>(key), "1") == "") {
//    count_client_->Set(lexical_cast<string>(key), "1", kJourneyExpire);
//  }
//
//  map<string, string> set_with_scores;
//
//  //Negative value is for the sort!!
//  set_with_scores.insert(make_pair(lexical_cast<string>(-time_adapter_->get_now()), lexical_cast<string>(guestId)));
//  list_client_->ZAdd(lexical_cast<string>(key), set_with_scores);
//  if (JourneyLogicAdapter::getCount(key) > 0) {
//    list_client_->ZRemRangeByRank(lexical_cast<string>(key), 20, -1);
//  }
}

IntSeq JourneyLogicAdapter::getList(long key) {
  StringListType resultStr;
  list_client_->ZRange(lexical_cast<string>(key), &resultStr, 0, 19);
  IntSeq result;
  for (StringListType::const_iterator iter = resultStr.begin(); iter != resultStr.end(); iter++) {
    result.push_back(IntSerialize::unserialize(*iter));
  }
  return result;
//  IntSeq result;
//  StringListType strResult;
//  if (list_client_->ZRange(lexical_cast<string>(key), &strResult, 0, 19)) {
//    result.reserve(strResult.size());
//    for(StringListType::const_iterator iter = strResult.begin(); iter != strResult.end(); iter++) {
//      result.push_back(lexical_cast<int>(*iter));
//    }
//  }
//  return result;
}

int JourneyLogicAdapter::getCount(long key) {
  int result = 0;
  string strResult;
  if (count_client_->GetAndIgnoreMiss(lexical_cast<string>(key), &strResult)) {
    result = lexical_cast<int>(strResult);
  }
  return result;
}

void JourneyLogicAdapter::reset(long key) {
  xce::tempmutex::Lock lock(tempMutexManagerPtr_, key);
  count_client_->Set(lexical_cast<string>(key), "0", kJourneyExpire);
  list_client_->ZRemRangeByRank(lexical_cast<string>(key), 0, -1);
}

void JourneyLogicAdapter::fillRedis(long key) {
  map<string, string> set_with_scores;
  set_with_scores.insert(make_pair( "352000000000001", IntSerialize::serialize(123456770)));
  set_with_scores.insert(make_pair( "352000000000002", IntSerialize::serialize(123456771)));
  set_with_scores.insert(make_pair( "352000000000003", IntSerialize::serialize(123456772)));
  set_with_scores.insert(make_pair( "352000000000004", IntSerialize::serialize(123456773)));
  set_with_scores.insert(make_pair( "352000000000005", IntSerialize::serialize(123456774)));
  set_with_scores.insert(make_pair( "352000000000006", IntSerialize::serialize(123456775)));
  set_with_scores.insert(make_pair( "352000000000007", IntSerialize::serialize(123456776)));
  set_with_scores.insert(make_pair( "352000000000008", IntSerialize::serialize(123456777)));
  set_with_scores.insert(make_pair( "352000000000009", IntSerialize::serialize(123456778)));
  set_with_scores.insert(make_pair( "352000000000010", IntSerialize::serialize(123456779)));
  set_with_scores.insert(make_pair( "352000000000011", IntSerialize::serialize(123456780)));
  set_with_scores.insert(make_pair( "352000000000012", IntSerialize::serialize(123456781)));
  set_with_scores.insert(make_pair( "352000000000013", IntSerialize::serialize(123456782)));
  set_with_scores.insert(make_pair( "352000000000014", IntSerialize::serialize(123456783)));
  set_with_scores.insert(make_pair( "352000000000015", IntSerialize::serialize(123456784)));
  set_with_scores.insert(make_pair( "352000000000016", IntSerialize::serialize(123456785)));
  set_with_scores.insert(make_pair( "352000000000017", IntSerialize::serialize(123456786)));
  set_with_scores.insert(make_pair( "352000000000018", IntSerialize::serialize(123456787)));
  set_with_scores.insert(make_pair( "352000000000019", IntSerialize::serialize(123456788)));
  set_with_scores.insert(make_pair( "352000000000020", IntSerialize::serialize(123456789)));
  set_with_scores.insert(make_pair( "352000000000021", IntSerialize::serialize(123456790)));
  list_client_->ZAdd(lexical_cast<string>(key), set_with_scores);
}


int main(int argc, char** argv) {
  cout << "please input the input key" << endl;
  long key;
  cin >> key;
  IntSeq result = JourneyLogicAdapter::instance().getList(key);
  cout << "[ ";
  for (IntSeq::const_iterator iter = result.begin(); iter != result.end(); iter++) {
    cout << *iter << " ";
  }
  cout << "]";
  int count = JourneyLogicAdapter::instance().getCount(key);
  cout << " key : " << key << " count : " << count << endl;
} 
