/*
 * =====================================================================================
 *
 *       Filename:  FeedContentCompressDemoI.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月23日 14时22分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#ifndef __FEEDCONTENTCOMPRESSDEMO_I_H__
#define __FEEDCONTENTCOMPRESSDEMO_I_H__

#include "FeedContentCompressDemo.h"
#include "ServiceI.h"
#include "Singleton.h"

namespace xce {
namespace feed {

using namespace std;
using namespace MyUtil;
using namespace xce::feed;

class FeedContentCompressDemoI : public FeedContentCompressDemo,
	public MyUtil::Singleton<FeedContentCompressDemoI> {
public:
	FeedContentCompressDemoI(){}
	~FeedContentCompressDemoI(){}

	virtual void compressAndDecompress(long feedid, const Ice::Current& current = Ice::Current());
};

}
}

#endif


