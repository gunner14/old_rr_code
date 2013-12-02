#include <iostream>
#include <string>
#include "ServiceI.h"
#include "../SingleTableProducerNew.h"
#include "user_network.pb.h"
#include <QueryRunner.h>
#include <string>


namespace com
{
namespace renren
{
namespace tripod
{

using namespace xce::tripoddata::usernetwork;
using namespace com::xiaonei::xce;

const std::string DB_TRIPOD_NETWORK = "network_history"; 
const std::string primary_key = "userid";
const std::string DB_MASTER = "fb_master";
const std::string DB_SLAVE = "fb_slave";
const std::string TABLE_NETWORK = "network_history";


class BatchUserNetworkResultHandler:public com::xiaonei::xce::ResultHandler
{
public:

  BatchUserNetworkResultHandler(NetworkHistory& message):parent_message_(message){}

  virtual bool handle(mysqlpp::Row& row) const
{
        NetworkHistory_Network* network = parent_message_.add_networks();
        SetField(network, row);

    return true;
}

private:
  xce::tripoddata::usernetwork::NetworkHistory& parent_message_;
  void SetField(NetworkHistory_Network * network_data, mysqlpp::Row& row) const {
     network_data->set_network_id((int) (row["network_id"]));
     network_data->set_stage((int) (row["stage"]));
     network_data->set_info_id((int) (row["info_id"]));
     network_data->set_join_time((time_t)mysqlpp::DateTime(row["join_time"]));
     network_data->set_network_name((std::string)row["network_name"]);
     network_data->set_status((int) (row["status"]));

  }

};


class NetworkProducer {
  public:
  std::string create(int userid) {
    NetworkHistory message;
    message.set_userid(userid);
    BatchUserNetworkResultHandler handler(message);
    Statement sql;
    sql << "SELECT * FROM network_history  WHERE userId = " << userid ;
    QueryRunner("common", CDbWServer ).query(sql, handler);
    std::string data;
    message.SerializeToString(&data);
    return data;

        }
};
  
class Test {
  public:
    static std::string old_func() {
      NetworkProducer producer;			
      return producer.create(265929833);
    }

    static std::string new_func() { 
      string str("265929833");
      KeySeq keys;
      keys.push_back(str);

      SingleTableProducer<NetworkHistory> network_producer;
      DataMap result_ = network_producer.create(keys,false);
      return VectorChar2String(result_[str]);
    }

    static string VectorChar2String(std::vector<unsigned char>& value) {
      string resultstr_;
      for(std::vector<unsigned char>::iterator it = value.begin();it != value.end();it++) {
        resultstr_.push_back(*it);
      }
      return resultstr_;
    }

};

}
}
}


int main() {

  using namespace com::xiaonei::xce;
  using namespace com::renren::tripod;
  ConnectionPoolManager::instance().initialize();
  MyUtil::init_logger("Mce", "test.log", "DEBUG");	
  std::string s1=Test::old_func();
  std::string s2=Test::new_func();
  std::cout << s1.size() <<" " << s2.size() << std::endl;
  assert(Test::old_func()==Test::new_func());
  return 0;
}

