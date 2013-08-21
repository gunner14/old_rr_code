#include "site_xiaonei.h"
#include "Account_diff/account_adapter.h"
#include "OceSlice/slice/UserBase.h"
#include "IdCacheReaderAdapter.h"
#include "BuddyByIdReplicaAdapter.h"
namespace xce {
using namespace idcache;
using namespace buddy::adapter;

bool GetFriendList(int hostid,int total_limit,std::vector<int>& friend_list)
{
    try {
      friend_list= BuddyByIdCacheAdapter::instance().getFriendListAsc(hostid, total_limit);
    } catch (...) {
      return false;
    }
  return true;
}

int GetTicketId(const std::string& t) {
  if (t.length() < 33)
    return -1;
  try {
    passport::TicketManagerPrx ta = CachedTicketAdapter(t[32] - '0');
    if (!ta)
      return -1;
    return ta->verifyTicket(t, "");
  } catch (std::exception& e) {
    return -1;
  }
}

int GetTransId(int ticket_uid) {
  if (ticket_uid < 0)
    return -1;
  try {
    if (IdCacheReaderAdapter::instance().isVirtualId(ticket_uid)) {
      return IdCacheReaderAdapter::instance().getRelatedTransId(ticket_uid);
    } else {
      return ticket_uid;
    }
  } catch (Ice::Exception& e) {
    return -1;
  }
}

std::string Query(int id) {
  try {
    passport::TicketManagerPrx ta = CachedTicketAdapter(id % 10);
    if (!ta)
      return "";
    return ta->queryTicket(id);
  } catch (std::exception& e) {
    return "";
  }
}

bool CheckTicket(const std::string &t, int id) {

  try {
    passport::TicketManagerPrx ta = CachedTicketAdapter(t[32] - '0');
    if (!ta)
      return false;

    if (t == ta->queryTicket(id)) {
      return true;
    } else {
      return false;
    }
  } catch (std::exception& e) {
    return false;
  }

  return false;
}

bool Init() {
  try {
    xce::Adapter::passport::ResetCached<passport::TicketManagerPrx>("M@Ticket",
                                                                    10);
    xce::Adapter::userbase::ResetCached<userbase::UserUrlManagerPrx>(
        "U@UserBase", 100);
  } catch (const std::exception& e) {
    printf("%s\n", e.what());
    return false;
  }
  return true;
}

std::string userUrl(int uid, const string& urltype) {
  try {
    xce::userbase::UserUrlManagerPrx url_manager =
        xce::Adapter::userbase::Cached<xce::userbase::UserUrlManagerPrx>(
            "U@UserBase", uid % 100, 100);
    if (!url_manager) {
      return std::string("");
    }
    xce::userbase::UserUrlDataNPtr url_data = url_manager->getUserUrlN(uid);
    if (!url_data) {
      return std::string("");
    }
    if (urltype == xce::userbase::TINYURL) {
      return url_data->tinyUrl;
    } else if (urltype == xce::userbase::HEADURL) {
      return url_data->headUrl;
    } else if (urltype == xce::userbase::LARGEURL) {
      return url_data->largeUrl;
    } else {
      return std::string("");
    }
  } catch (const std::exception& e) {
    printf("%s\n", e.what());
  }
  return std::string("");
}
bool setUserUrl(int uid, const string& urltype, const std::string& url) {
  try {
    xce::userbase::UserUrlManagerPrx url_manager =
        xce::Adapter::userbase::Cached<xce::userbase::UserUrlManagerPrx>(
            "U@UserBase", uid % 100, 100);
    if (!url_manager) {
      return false;
    }
    ::MyUtil::Str2StrMap properties;
    properties[urltype] = url;
    url_manager->setUserUrl(uid, properties);
    return true;
  } catch (const std::exception& e) {
    printf("%s\n", e.what());
  }
  return false;
}
}
