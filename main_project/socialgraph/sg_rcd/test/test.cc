/* Date:       2012-07-18
 * Developer:  junwu.xiong
 * EMail:      junwu.xiong@renren-inc.com
 * Function:   Validate the remove result of fof recommendation in tripod cache
 * */
#include "OceCxxAdapter/src/SocialGraphRcdMessageServiceAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "socialgraph/socialgraphproto/item.pb.h"

#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace MyUtil;
  using namespace com::xiaonei::xce;
  using namespace xce::serverstate;
  using namespace xce::socialgraph;
  using namespace xce::tripod;
  int hostId = 0, fofId = 0;
  std::cout << "Input the hostId: " << std::endl;
  std::cin >> hostId;
  std::cout << "Input the fofId that will be removed: " << std::endl;
  std::cin >> fofId;
  RcdMessageServiceAdapter::instance().AddSgBlockAndRemoveFoFCache(hostId, fofId, "FOF");
  typedef std::vector<char> BinaryData;
  BinaryData bin_data;
  std::string value;
  try {
 	 SocialGraphRcdTripodAdapter::instance().Get(boost::lexical_cast<std::string>(hostId), &value);
  } catch (boost::bad_lexical_cast& e) {
 	 MCE_WARN("boost::bad_lexical_cast" << e.what() << std::endl);
  }
  if (!value.empty()) {
 	 bin_data = BinaryData(value.begin(), value.end());
  } 

  Items items;
  //Items new_items;
  int num = 0;
  if (items.ParseFromArray(bin_data.data(), bin_data.size())) {
    BOOST_FOREACH(const Item& it, items.items()) {
    	//if (it.id() != fofId) {
    	//	Item* item = new_items.add_items();
    	//	*item = it;
    		++num;
      //}
      }
    std::cout << "The host's fof recommendation number after removed is: " << num << std::endl;
  }

}
















