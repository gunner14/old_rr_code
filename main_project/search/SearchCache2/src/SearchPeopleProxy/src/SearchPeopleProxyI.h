/*
 * FeedQueueI.h
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#ifndef FeedLRUI_H_
#define FeedLRUI_H_

#include "SearchPeopleProxy.h"
#include "Singleton.h"
#include <IceUtil/IceUtil.h>

#include "ObjectCacheI.h"
#include "Util.h"
using namespace std;
using namespace MyUtil;

namespace xce {
namespace searchcache{

class SearchPeopleProxyI: public SearchPeopleProxy, public MyUtil::Singleton<SearchPeopleProxyI> {
public:
	virtual void  Send(const ::MyUtil::Int2StrMap& info, const ::Ice::Current& = ::Ice::Current());
};



}
}
#endif /* FEEDQUEUEI_H_ */
