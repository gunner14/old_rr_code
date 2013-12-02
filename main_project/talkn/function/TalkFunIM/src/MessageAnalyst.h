#ifndef __Talk_Fun_IM_MESSAGE_ANALYST__
#define __Talk_Fun_IM_MESSAGE_ANALYST__

#include "Singleton.h"
#include "TaskManager.h"
#include <string>
#include <map>
#include "Util.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace im {

using namespace std;
using namespace MyUtil;

enum Jobs{
  CLIENT_ADD_BUDDY_REQUEST,
  CLIENT_ADD_BUDDY_ACCEPT,
  CLIENT_ADD_BUDDY_REJECT,
  GET_VCARD_REQUEST,
  PING_REQUEST,
  // 遇见业务相关
  GET_MEET_PAY_LIST, // 获取遇见的关注列表
  MEET_MESSAGE,
  MEET_MESSAGE2,
  GET_MEET_INFO,
  GET_MEET_SELFINFO,

  DESK_OFFLINE,

  CLIENT_USER_STATE_CHANGED, // always here
  UNKNOW
};


class AnalystTableItem {
public:
  AnalystTableItem(const string& key, const string& value, const Jobs type) : key_(key),
    value_(value), type_(type) {}
  string key_;
  string value_;
  Jobs type_;
};

class MessageAnalyst : public Singleton<MessageAnalyst> {
public:
  void Analyse(const Str2StrMap &paras);
  void InitAnalystTable();
private:
  Jobs GetJobType(const Str2StrMap &parars);
  vector<AnalystTableItem> analyst_table_;
};

};
};
};
};
};

#endif
