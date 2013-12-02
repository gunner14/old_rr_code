#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "base/logging.h"
#include "base/ptime.h"
#include "base/threadpool.h"
#include "base/logrotate.h"

#include "OceSlice/slice/FriendClusterCache.h"
#include "OceCxxAdapter/src/FriendClusterCacheAdapter.h"
#include "OceCxxAdapter/src/BuddyByIdReplicaAdapter.h"
#include "OceCxxAdapter/src/BuddyFriendsAdapter.h"

#include "OceCxxAdapter/src/BuddyCoreAdapter.h"

#include "socialgraph/socialgraphutil/mc_adapter.h"
#include "socialgraph/socialgraphutil/algostuff.h"
#include "socialgraph/socialgraphproto/item.pb.h"
#include "socialgraph/socialgraphutil/dbutil.h"
#include "OceCxxAdapter/src/SocialGraphFFUCRTripodAdapter.h"



int main(int argc, char **argv) {
  using namespace xce::socialgraph;


	std::string key = "42321834";
	std::string value = "test";

	int exptime = 100;
	if (SocialGraphFFUCRTripodAdapter::instance().Set(key, value, exptime)) {
		std::string getvalue;

		if (SocialGraphFFUCRTripodAdapter::instance().Get(key, &getvalue)) {
			std::cout << getvalue << std::endl;
		}
	}

/*
	int64_t hostid = 42321834000000;
	int64_t id_test = -1002;
	Items items;
	Item* item = items.add_items();
	item->set_id(id_test);
	item->set_field(0);
	item->set_message("计算机学院2001级");
	item->add_messages("消息1");
	item->add_messages("消息2");
	item->add_messages("消息3");
	item->set_messages(0, "消息10");

	int byte_size = items.ByteSize();
  std::string s;
  items.SerializeToString(&s);
  std::vector<unsigned char> bin_data;
  bin_data.reserve(byte_size);
  bin_data.assign(s.data(), s.data() + byte_size);
  const static std::string kBiz = "FFUCR";
  BusinessCacheAdapter::GetInstance().Set(kBiz, hostid, bin_data);

	std::cout << "cache set!" << std::endl;
	
	std::vector<unsigned char> raw_data;
  int rank = 0;

  bool fetch = BusinessCacheAdapter::GetInstance().Get(kBiz, hostid, raw_data);
  if (!fetch) {
		std::cout << "cache not fetch!" << std::endl;
  }   

  Items raw_items;
  if (!raw_items.ParseFromArray(raw_data.data(), raw_data.size())) {
		std::cout << "parse failed!" << std::endl;
  }   

  BOOST_FOREACH(const Item& it, raw_items.items()) {
    std::cout << "id:\t" << it.id() << std::endl;
		std::cout << "id.field():\t" << it.field() << std::endl;
		std::cout << "id:message:\t" << it.message() << std::endl;
		std::cout << "id:messages size\t" << it.messages_size() << std::endl;
		std::cout << "id:messages 0\t" << it.messages(0) << std::endl;
		std::cout << "id:messages 2\t" << it.messages(2) << std::endl;

	}

*/
}
