#include "UserLoginCountTripodAdapter.h"
#include "boost/lexical_cast.hpp"

namespace xce {
namespace adapter {
namespace userlogincount {

const int UserLoginCountTripodAdapter::expiration = 3600 * 24 * 30;

int UserLoginCountTripodAdapter::Get(int user_id) {
  int result = 0;
  string value;
  if(client_->Get(boost::lexical_cast<string>(user_id), &value, expiration)) {    
    result = boost::lexical_cast<int>(value);
  }
  return result;
}

bool UserLoginCountTripodAdapter::Load(int user_id, bool is_master) {
  return client_->Load(boost::lexical_cast<string>(user_id), expiration, is_master); 
}

bool UserLoginCountTripodAdapter::Remove(int user_id) {
  return client_->Remove(boost::lexical_cast<string>(user_id));
}

long UserLoginCountTripodAdapter::Inc(int user_id, int step) {
  long result = -1;
  std::string size_str = client_->Inc(boost::lexical_cast<string>(user_id), boost::lexical_cast<string>(step));
  if(size_str.size()) {
    result = boost::lexical_cast<long>(size_str);
  }
  return result;
}

UserLoginCountTripodAdapter::UserLoginCountTripodAdapter() {
  client_.reset(new TripodClient("xce_user", "user_logincount"));
}

UserLoginCountTripodAdapter::~UserLoginCountTripodAdapter() {
  client_.reset();
}

}}} //namespace xce::adapter::userlogincount
