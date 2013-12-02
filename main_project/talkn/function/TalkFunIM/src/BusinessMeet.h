#ifndef __FUN_IM_BUSINESSMEET__
#define __FUN_IM_BUSINESSMEET__

#include "Singleton.h"
#include "TaskManager.h"

#include "MessageAnalyst.h"
#include "BadooServiceAdapter.h"
using namespace MyUtil;
using namespace com::renren::xoa2::sns::badoo;
namespace com {
namespace xiaonei {
namespace talk {
namespace function {
namespace im {
namespace business {
static const int RUN_TIME_OUT = 200;

class BusinessMeet : public Singleton<BusinessMeet> {
  enum {
    TASK_MAX_LEN_DEFAULT = 20, // 遇见业务 任务队列最大长度
  };

  public:
    void business(int level, const Str2StrMap &paras, int type);
};

// Task Define
class MeetTask : public Task {
public:
    enum {
      GET_PAY_LIST,
      MEET_MESSAGE,
      MEET_MESSAGE2,
      PRESENCE,
      OFFLINE,
      GET_SELF_INFO,
      GET_OTHER_INFO,

      TASK_COUNT, // 计数，必须放在最后
    };

    enum {
      BADOOS_LIMIT = 1000, // 列表上限

    };

    enum {
      IS_DEBUG_OUT = 1,
    };


  private:
    Str2StrMap paras_;
    int ques_;
    int rid_;
    int bid_;
    static void (MeetTask::*const queses_[TASK_COUNT])();


  void getPaylist();
  void message();
  void message2();
  void presence();
  void offline();
  void getSelfInfo();
  void getOtherInfo();


  bool getBadooIdByRId(const char *fun, const string &rId, int32_t &bid);
  bool getBadooIdByRId(const char *fun, const int32_t rId, int32_t &bid);
  bool getRIdByBId(const char *fun, const string &bId, int32_t &rid);
  bool getRIdByBId(const char *fun, const int32_t bId, int32_t &rid);
  bool sendGossip(const char *fun, const int32_t senderId, const int32_t receiverId, const std::string& message);
  bool getLikeUser(const char *fun, BadooLikeInfoList& rv, const int32_t rId, const int32_t limit);
  bool getBeLikeUser(const char *fun, BadooLikeInfoList& rv, const int32_t rId, const int32_t limit);
  bool getMutileLikeUser(const char *fun, BadooLikeInfoList& rv, const int32_t rId, const int32_t limit);
  bool getUserBaseInfoByRId(const char *fun, BadooUserInfoList& rv, const IdWrappers& rIds);
  bool getUserBaseInfoByBId(const char *fun, BadooUserInfoList& rv, const IdWrappers& bIds);
  bool getUserBaseInfoByBId(const char *fun, BadooUserInfoList& rv, const vector<string>& sbIds);
  bool getUserBaseInfoByRId2(const char *fun, BadooUserInfo& rv, const int rId);


  void getNotifyList(int rId, vector<int> &rIds);

  void notifyBadooList(string &from_jid,
                  const BadooLikeInfoList &pay_list,
                  const BadooLikeInfoList &payed_list,
                  const BadooLikeInfoList &both_list
                  );

  void notifyPresence(string &from_jid,
                  const BadooLikeInfoList &pay_list,
                  const BadooLikeInfoList &payed_list,
                  const BadooLikeInfoList &both_list
                  );

  void getBaseInfo(const BadooUserInfo &info, const char *pref, int idx, Str2StrMap &msg);
  void getListInfo(const BadooLikeUserInfo &info, const char *pref, int idx, Str2StrMap &msg);


  bool SpamFilterVerify(int from, const string &msg);

  public:
	MeetTask(int level, const Str2StrMap &paras, int ques) : Task(level), paras_(paras),
    ques_(ques), rid_(0), bid_(0)
      {
      }
	virtual void handle();


};



};
};
};
};
};
};




#endif
