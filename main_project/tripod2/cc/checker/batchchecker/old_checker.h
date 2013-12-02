/** 
 * 具体的生产者消费者
 * xiaofeng.liang@renren-inc.com
 */
#ifndef XCE_TRIPOD_CHECKER_OLD_CHECKER_H_
#define XCE_TRIPOD_CHECKER_OLD_CHECKER_H_

#include <vector>

#include "userbase_executer.hpp"

using namespace xce::util;

namespace xce {
namespace tripod {
namespace checker {

class OldChecker : public ConsumerThread<int> {
public:
  OldChecker(Queue<int> *queue, int batch, const string &biz):ConsumerThread<int>(queue, batch) {
    //有时间最好加上switch
    if(biz == "user_passport") {
      _executer = new PassportExecuter();
    } else if(biz == "user_name") {
      _executer = new NameExecuter();
    } else if(biz == "user_doing") {
      _executer = new DoingExecuter();
    } else if(biz == "user_right") {
      _executer = new RightExecuter();
    } else if(biz == "user_state") {
      _executer = new StateExecuter();
    } else if(biz == "user_stage") {
      _executer = new StageExecuter();
    } else if(biz == "user_born") {
      _executer = new BornExecuter();
    } else if(biz == "user_url") {
      _executer = new UrlExecuter();
    } else {
      MCE_WARN("No surpport for biz : " << biz);  
    }
  }

  virtual bool consum(vector<int> ids) {_executer->consum(ids);}

private:
  string _biz;
  OldCheckExecuterIF *_executer;
};

}}}// namespace xce::tripod

#endif
