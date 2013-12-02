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
class FeedDbWriterReplicaAdapter: public MyUtil::ReplicatedClusterAdapterI<FeedDbWriterPrx>,
		public MyUtil::Singleton<FeedDbWriterReplicaAdapter> {
public:
	FeedDbWriterReplicaAdapter():MyUtil::ReplicatedClusterAdapterI<FeedDbWriterPrx> ("ControllerFeedDbWriterR",120,300,new XceFeedControllerChannel) {
	}
  void FeedDispatch2DB(const FeedSeedPtr & ,const FeedReplyPtr & );
  void UpdateReply2DB(const MyUtil::LongSeq & ,const FeedReplyPtr &);
  void FeedDispatchEDM2DB(const FeedSeedPtr &);
  void AddFeedSchool2DB(int schoolid,const FeedSeedPtr & seed);
  void RemoveFeedFromDB(const FeedIndexSeq indexSeq); 
  void ReplaceFeedXML2DB(const MyUtil::LongSeq & feedIds,const string & xml);

protected:
  FeedDbWriterPrx getManager(Ice::Long id);
	FeedDbWriterPrx getManagerOneway(Ice::Long id);
};

}
}

#endif /*BUDDYCOREADAPTER_H_*/
