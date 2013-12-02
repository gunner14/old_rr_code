/*
 * =====================================================================================
 *
 *       Filename:  user_score_producer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年05月09日 15时48分52秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef USER_SCORE_PRODUCER_H_
#define USER_SCORE_PRODUCER_H_


#include "tripod2/cc/core/producer.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {

class UserScoreVipProducer : public Producer {
  public:
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
};

class ScoreVipResultHandler : virtual public com::xiaonei::xce::ResultHandler {
  public:
    ScoreVipResultHandler(int& result) : result_(&result) {}
    virtual ~ScoreVipResultHandler() {}
  protected:
    virtual bool handle(mysqlpp::Row& res) const ;
  private:
    int* result_;
};


}} // xce::tripod



#endif
