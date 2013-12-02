#ifndef _FEED_NEW_MEMC_PROXY_KEY_MAPPING_
#define _FEED_NEW_MEMC_PROXY_KEY_MAPPING_

#include <string>
#include <map>
#include <boost/asio.hpp>

namespace xce {
namespace feedprx {
using namespace std;

void LoadCluster(std::string, std::string);
bool InitCacheClusters();
boost::asio::ip::tcp::endpoint GetEndpointByKey(const char * key, size_t len);

}
}

#endif //_FEED_NEW_MEMC_PROXY_KEY_MAPPING_
