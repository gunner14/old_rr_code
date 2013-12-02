#ifndef _FEED_NEW_MEMC_PROXY_UTIL_
#define _FEED_NEW_MEMC_PROXY_UTIL_

namespace xce {
namespace feedprx {

int GetAndRemoveCmdEnd(char * buf, size_t len);


char * GetLineEnd(char * buf, size_t len);

}
}

#endif // _MEMC_PROXY_UTIL_

