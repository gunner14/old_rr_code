#ifndef __FEEDMARK_ADAPTER_H_
#define __FEEDMARK_ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "Channel.h"
#include "FeedMark.h"

namespace xce {
namespace feed {

using namespace MyUtil;

  class FeedMarkAdapter : public MyUtil::AdapterI,
      public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedMarkAdapter>
  {
    public:
      FeedMarkAdapter() {
        _managers.resize(cluster() > 0 ? cluster() : 1 );
        _managersOneway.resize(cluster() > 0 ? cluster() : 1);
      }
      void MarkFeed(int uid, long fid, long source, int stype, int actor, bool mark);
      MarkInfoPtr GetMarkInfo(int uid);
    
    protected:
      virtual string name() { return "FeedMark"; }
      virtual string endpoints() { return "@FeedMark"; }
      virtual size_t cluster() { return 10; }

      FeedMarkPrx GetManager(int uid) {
        return locate<FeedMarkPrx> (_managers, "M", uid, TWO_WAY);
      };

      FeedMarkPrx GetManagerOneWay(int uid) {
        return locate<FeedMarkPrx> (_managersOneway, "M", uid, ONE_WAY);
      };

      vector<FeedMarkPrx> _managers;
      vector<FeedMarkPrx> _managersOneway;
  };



}

}

#endif


