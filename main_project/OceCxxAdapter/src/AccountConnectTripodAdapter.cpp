#include "AccountConnectTripodAdapter.h"

#include <sstream>

#include "OceProto/proto/account_connect.pb.h"

namespace xce {
namespace adapter {
namespace account {

using namespace xce::tripod;
using namespace xce::tripoddata::account;

const int AccountConnectTripodAdapter::expiration = 3600 * 24 * 30;

int AccountConnectTripodAdapter::Get(const string& cid, int type, bool use_master) {
  int result = -1;
  string key = boost::lexical_cast<string>(type) + "_" + cid;
  string value;
  if(client_->Get(key, &value, expiration)) {
    std::istringstream in(value); 
    PbAccountConnect pbInfo;
    pbInfo.ParseFromIstream(&in);
    result = pbInfo.uid();
  }
  return result;
}

bool AccountConnectTripodAdapter::Load(const string& cid, int type, bool use_master) {
  string key = boost::lexical_cast<string>(type) + "_" + cid;
  return client_->Load(key, expiration, use_master); 
}

AccountConnectTripodAdapter::AccountConnectTripodAdapter() {
  client_.reset(new TripodClient("xce_user", "account_connect"));
}

AccountConnectTripodAdapter::~AccountConnectTripodAdapter() {
  client_.reset();
}


}}} //namespace xce::adapter::userstage
