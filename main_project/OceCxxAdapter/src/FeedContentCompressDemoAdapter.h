/*
 * =====================================================================================
 *
 *       Filename:  FeedContentCompressDemoAdapter.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月23日 16时18分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#ifndef __FEED_CONTENT_COMPRESS_DEMO_ADAPTER_H__
#define __FEED_CONTENT_COMPRESS_DEMO_ADAPTER_H__

#include "FeedContentCompressDemo.h"
#include "AdapterI.h"
#include "Singleton.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedContentCompressDemoAdapter : public AdapterI,
	public AdapterISingleton<XceFeedChannel, FeedContentCompressDemoAdapter> {
	
protected:
  virtual string name() {
    return "FeedContentCompressDemo";
  }
  virtual string endpoints() {
    return "@FeedContentCompressDemo";
  }
  virtual size_t cluster() {
    return 1;
  }

public:
  FeedContentCompressDemoAdapter() {
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
    _managers.resize(cluster() > 0 ? cluster() : 1);
  }

  void compressAndDecompress(long feedid) {
	  try {
		  getManager(feedid)->compressAndDecompress(feedid);
	  } catch (Ice::Exception& e) {
		  std::cout << "compressAndDecompress, feedid: " << feedid << e << std::endl;
	  }
	  return;
  }

private:
  FeedContentCompressDemoPrx getManagerOneway(int id){
    return locate<FeedContentCompressDemoPrx>(_managersOneway, "M", id, ONE_WAY);
  }

  FeedContentCompressDemoPrx getManager(int id){
    return locate<FeedContentCompressDemoPrx>(_managers, "M", id, TWO_WAY);
  }

  vector<FeedContentCompressDemoPrx> _managersOneway;
  vector<FeedContentCompressDemoPrx> _managers;

};

}
}

#endif
