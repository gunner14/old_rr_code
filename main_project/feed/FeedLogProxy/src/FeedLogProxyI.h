/*
 * FeedLogProxyI.h
 *
 *   Create on: 2012-11-13
 *      Author: rui.qiu
 */
#ifndef __FEED_LOG_PROXY_I_H__
#define __FEED_LOG_PROXY_I_H__

#include "Singleton.h"
#include "FeedLogProxy.h"

namespace xce {
namespace feed {

class FeedLogProxyI : public FeedLogProxy, public MyUtil::Singleton<FeedLogProxyI> {
public:
  void Initialize();
};

}
}

#endif

