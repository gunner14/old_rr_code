#include "user_name_client.h"

#include "OceCxxAdapter/src/UserNameAdapter.h"

namespace xce {
namespace client {

using xce::adapter::username::UserNameInfoPtr;
using xce::adapter::username::UserNameAdapter;

string UserNameClient::GetUserName(int user_id) {
  UserNameInfoPtr pname = UserNameAdapter::instance().getUserName(user_id);
  if(pname) {
    return pname->name();
  } else {
    return "";
  }
}

}} //namespace xce::client
