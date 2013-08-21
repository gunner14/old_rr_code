#ifndef XCE_CWF_SITE_XIAONEI_H__
#define XCE_CWF_SITE_XIAONEI_H__
#include <string>
#include <vector>
namespace xce {
namespace userbase {
const std::string TINYURL = "TINYURL";
const std::string FLEXURL = "FLEXURL";
const std::string HEADURL = "HEADURL";
const std::string LARGEURL = "LARGEURL";
}

bool GetFriendList(int hostid,int total_limit,std::vector<int>& friend_list);
int GetTicketId(const std::string& t);
int GetTransId(int ticket_uid);
bool CheckTicket(const std::string &t, int id);
std::string Query(int id);
bool Init();
std::string userUrl(int uid, const std::string& urltype);
bool setUserUrl(int uid, const std::string& urltype, const std::string& url);
}
#endif // XCE_CWF_SITE_XIAONEI_H__
