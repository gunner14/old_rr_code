/** 
 * 具体的生产者消费者
 * xiaofeng.liang@renren-inc.com
 */
#ifndef XCE_SCORE_PC_IMPL_H_
#define XCE_SCORE_PC_IMPL_H_

#include <vector>
#include <ctime>
#include "DbCxxPool/src/QueryRunner.h"
#include "pc_model.h"

using namespace xce::util;
using namespace com::xiaonei::xce;

namespace xce {
namespace score {

class CreateKeysResultHandler: virtual public ResultHandler {
public:
  CreateKeysResultHandler(const string &key, vector<int> *keyseq);

protected:
  virtual bool handle(mysqlpp::Row& row) const;

private:
  string _key;
  vector<int> *_keyseq;
};

class LoginTimeHandler: virtual public ResultHandler {
public:
  LoginTimeHandler(time_t &time);
protected:
  virtual bool handle(mysqlpp::Row& row) const;
  time_t &_time;
};

//DBProducer 从数据库获取ID，可以指定范围
//--------------------------------------------------------------
class DbProducer : public ProducerThread<int> {
public:
  DbProducer(Queue<int> *queue, int band, int batch, int min, int max);

private:
  virtual void produce(vector<int> *ids, int batch);

private:
  int _min;
  int _max;
  string _table;
};

//ScoreGuarder
//--------------------------------------------------------------
class ScoreGuarder : public ConsumerThread<int> {
public:
  ScoreGuarder(Queue<int> *queue, int batch, int& count);
  virtual bool consum(vector<int> ids);
private:
  time_t _min;
  int* _pCount; // for statistic
};

}}// namespace

#endif
