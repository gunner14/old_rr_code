#include "user_passport_client.h"

#include "OceCxxAdapter/src/UserPassportAdapter.h"

namespace xce {
namespace client {

using xce::adapter::userpassport::UserPassportInfoPtr;
using xce::adapter::userpassport::UserPassportAdapter;

int UserPassportClient::GetUserSafeStatus(int user_id) {
  UserPassportInfoPtr ppassport = UserPassportAdapter::instance().getUserPassport(user_id);
  if(ppassport) {
    return ppassport->safestatus();
  } else {
    return -1;
  }
}

int UserPassportClient::GetUserStatus(int user_id) {
  UserPassportInfoPtr ppassport = UserPassportAdapter::instance().getUserPassport(user_id);
  if(ppassport) {
    return ppassport->status();
  } else {
    return -1;
  }
}

}} //namespace xce::client
