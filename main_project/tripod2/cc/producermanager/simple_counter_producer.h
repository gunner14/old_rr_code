/**
 * 简单计数器
 */
#ifndef XCE_TRIPOD_SIMPLE_COUNTER_PRODUCER_H_
#define XCE_TRIPOD_SIMPLE_COUNTER_PRODUCER_H_

#include "tripod2/cc/core/producer.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "boost/lexical_cast.hpp"

namespace xce {
namespace tripod {

class SimpleCounterProducer : public Producer {
public:
//可以是多个主键确定一个计数器，多主键时key由各主键加逗号分隔符连接而成
  SimpleCounterProducer(const std::string& db, const std::string& table, 
      const std::string& key_filed, const std::string& value_field);
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
private:
  const std::string db_;
  const std::string table_;
  const std::string key_field_;
  const std::string value_field_;
  std::vector<std::string> key_fields_;
};

class SimpleCounterResultHandler : virtual public com::xiaonei::xce::ResultHandler {
public:
  SimpleCounterResultHandler(const std::string field, std::string& result) :
    field_(field), result_(result) {}
  virtual ~SimpleCounterResultHandler() {}
protected:
  virtual bool handle(mysqlpp::Row& row) const {
    result_ = boost::lexical_cast<string>(row[field_.c_str()]);
    return true;
  }
private:
  const std::string field_;
  std::string& result_;
};

}} // namespace xce::tripod

#endif
