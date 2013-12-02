/** 
 * 具体的生产者消费者
 * xiaofeng.liang@renren-inc.com
 */
#ifndef XCE_TRIPOD_CHECKER_PC_IMPL_H_
#define XCE_TRIPOD_CHECKER_PC_IMPL_H_

#include <vector>

#include "tripod2/cc/core/producer.h"
#include "tripod2/cc/core/tripod_client.h"
#include "DbCxxPool/src/QueryRunner.h"

#include "pc_model.h"

using namespace xce::util;
using namespace xce::tripod;
using namespace com::xiaonei::xce;

namespace xce {
namespace tripod {
namespace checker {

//FileProducer 从文件产生id，文件格式需要是一行一个id
//--------------------------------------------------------------
class FileProducer : public ProducerThread<int> {
public:
  FileProducer(Queue<int> *queue, int band, int batch, string file);
  virtual ~FileProducer();

private:
  virtual void produce(vector<int> *ids, int batch);

private:
  bool _finish;
  ifstream fin;
};

//StdinProducer 从标准输入获取id，输入格式需要是一行一个id
//--------------------------------------------------------------
class StdinProducer : public ProducerThread<int> {
public:
  StdinProducer(Queue<int> *queue, int band, int batch);

private:
  virtual void produce(vector<int> *ids, int batch);

private:
  bool _finish;
};


class CreateKeysResultHandler: virtual public ResultHandler {
public:
  CreateKeysResultHandler(const string &key, vector<int> *keyseq);

protected:
  virtual bool handle(mysqlpp::Row& row) const;

private:
  string _key;
  vector<int> *_keyseq;
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
};

//BatchChecker 批量从DB和Cache获取数据，对比，对不一致的Load
//--------------------------------------------------------------
class BatchChecker : public ConsumerThread<int> {
public:
  BatchChecker(Queue<int> *queue, int batch, string ns, string biz);

  virtual bool consum(vector<int> ids);

private:
  void load(string key);

private:
  string _ns;
  string _biz;
  Producer *_producer;
  boost::shared_ptr<TripodClient> _client;
};

//SimpleLoader 不Check，只做Load
//--------------------------------------------------------------
class SimpleLoader : public ConsumerThread<int> {
public:
  SimpleLoader(Queue<int> *queue, int batch, string ns, string biz);

  virtual bool consum(vector<int> ids);

private:
  string _ns;
  string _biz;
  boost::shared_ptr<TripodClient> _client;
};

}}}// namespace xce::tripod

#endif
