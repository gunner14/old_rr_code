/*
 * =====================================================================================
 *
 *       Filename:  user_score_login_producer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月16日 11时45分39秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef USER_SCORE_LOGIN_PRODUCER_H_
#define USER_SCORE_LOGIN_PRODUCER_H_

#include "tripod2/cc/core/producer.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {

const int table_mod = 100;

class UserScoreLoginProducer : public Producer {
  public:
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
};

class ScoreLogin {
  public:
  int history_login_days_;
  int continue_login_days_;
  int active_days_;
  long last_login_time_;
  int login_type_;
};

class ScoreLoginResultHandler : virtual public com::xiaonei::xce::ResultHandler {
  public:
    ScoreLoginResultHandler(ScoreLogin& result) : result_(&result) {}
    virtual ~ScoreLoginResultHandler() {}
  protected:
    virtual bool handle(mysqlpp::Row& res) const;
  private:
    ScoreLogin * result_;
};

}} //xce::tirpod

#endif
