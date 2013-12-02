#ifndef FEEDDBWRITERADAPTER_H_
#define FEEDDBWRITERADAPTER_H_

#include <vector>
#include "Singleton.h"
#include "AdapterI.h"
//#include "/data/home/shilong.li/code/oce/main_project/trunk/release-newarch/tmp/OceSlice/cpp/FeedDbWriter.h"
#include "RFeed.h"
namespace xce {
namespace feed{
using namespace MyUtil;
class FeedDbWriterAdapter: public MyUtil::AdapterI,
		public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedDbWriterAdapter> {
public:
	FeedDbWriterAdapter(){
		_managers.resize(cluster() > 0 ? cluster() : 1);
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
	}
  void FeedDispatch2DB(const FeedSeedPtr & ,const FeedReplyPtr & );
  void UpdateReply2DB(const MyUtil::LongSeq & ,const FeedReplyPtr &);
  void FeedDispatchEDM2DB(const FeedSeedPtr &);
  void AddFeedSchool2DB(int schoolid,const FeedSeedPtr & seed);
  void RemoveFeedFromDB(const FeedIndexSeq indexSeq); 
  void ReplaceFeedXML2DB(const MyUtil::LongSeq & feedIds,const string & xml);

protected:
	virtual string name() {
		return "FeedDbWriter";
	}
	virtual string endpoints() {
		return "@FeedDbWriter";
	}
	virtual size_t cluster() {
		return 1;
	}

	FeedDbWriterPrx getManager(Ice::Long id);
	FeedDbWriterPrx getManagerOneway(Ice::Long id);
	vector<FeedDbWriterPrx> _managersOneway;
	vector<FeedDbWriterPrx> _managers;
};

}
}

#endif /*BUDDYCOREADAPTER_H_*/
