#include <IceLogger.h>
#include "SSDFeedDBTAdapter.h"


using namespace xce::feed;


/////////////////////////////////////////////////////////////////
FeedDBDataPrx SSDFeedDBDataTAdapter::getManager(int id, int timeout) {
	return locate<FeedDBDataPrx>(_managers, "M", id, TWO_WAY, timeout);
}

FeedDBDataPrx SSDFeedDBDataTAdapter::getManagerOneway(int id) {
	return locate<FeedDBDataPrx>(_managersOneway, "M", id, ONE_WAY);
}

void SSDFeedDBDataTAdapter::put(const FeedItem& feed, const BlockInfoPtr& blkInfo){
	getManagerOneway(blkInfo->serverId)->put(feed, blkInfo);
}

void SSDFeedDBDataTAdapter::putBatch(const FeedItem& feed, const BlockInfoSeq& blkInfoSeq){
	if(blkInfoSeq.empty()){
		return;
	}
	//getManagerOneway(blkInfoSeq.at(0)->serverId)->putBatch(feed, blkInfoSeq);
	getManager(blkInfoSeq.at(0)->serverId, 500)->putBatch(feed, blkInfoSeq);
	//getManager(1)->putBatch(feed, blkInfoSeq);
}

string SSDFeedDBDataTAdapter::encodeData(const BlockInfoPtr& blkInfo){
	string data;
	char tmp[1024] = {0};
	char* p = tmp;
	*(int*)p = blkInfo->serverId;
	*(int*)(p+=sizeof(int)) = blkInfo->userId;		
	*(int*)(p+=sizeof(int)) = blkInfo->totalCount;		
	*(int*)(p+=sizeof(int)) = blkInfo->curCount;		
	*(int*)(p+=sizeof(int)) = blkInfo->fileId;		
	*(int*)(p+=sizeof(int)) = blkInfo->blockIndex;		
	*(int*)(p+=sizeof(int)) = blkInfo->preFileId;
	*(int*)(p+=sizeof(int)) = blkInfo->preBlockIndex;
	*(int*)(p+=sizeof(int)) = blkInfo->weight;
	data.insert(data.end(), tmp, p+sizeof(int)); 
	return data;
}

BlockInfoPtr SSDFeedDBDataTAdapter::decodeData(const string& data){
	const char* p = data.c_str();
	BlockInfoPtr blkInfo = new BlockInfo;
	blkInfo->serverId 	= *(int*)p;
	blkInfo->userId 	= *(int*)(p+=sizeof(int));
	blkInfo->totalCount 	= *(int*)(p+=sizeof(int));
	blkInfo->curCount 	= *(int*)(p+=sizeof(int));
	blkInfo->fileId 	= *(int*)(p+=sizeof(int));
	blkInfo->blockIndex 	= *(int*)(p+=sizeof(int));
	blkInfo->preFileId 	= *(int*)(p+=sizeof(int));
	blkInfo->preBlockIndex 	= *(int*)(p+=sizeof(int));
	blkInfo->weight  	= *(int*)(p+=sizeof(int));
	return blkInfo;
}

string SSDFeedDBDataTAdapter::encodeDataSeq(const BlockInfoSeq& seq){
	string data;
	for(int i=0; i<seq.size(); i++){
		string tmp = encodeData(seq.at(i));
		data.insert(data.end(), tmp.begin(), tmp.end()); 
	}
	return data;
}

BlockInfoSeq SSDFeedDBDataTAdapter::decodeDataSeq(const string& data){
	int pos = 0;
        int UNIT = 36;
        BlockInfoSeq seq;
        for(;;){
                string tmp = data.substr(pos, UNIT);
                if(tmp.size() != UNIT){
                        break;
                }
                BlockInfoPtr blk = decodeData(tmp);
                seq.push_back(blk);
                pos+=UNIT;
        }
	return seq;
}

void SSDFeedDBDataTAdapter::putBatchBinary(const FeedItem& feed, const BlockInfoSeq& blkInfoSeq){
	if(blkInfoSeq.empty()){
		return;
	}
	string data = encodeDataSeq(blkInfoSeq);	
	getManager(blkInfoSeq.at(0)->serverId, 500)->putBatchBinary(feed, data);
	//getManager(1)->putBatch(feed, blkInfoSeq);
}

FeedItemSeq SSDFeedDBDataTAdapter::get(const BlockInfoPtr& blkInfo, int begin, int count){
	return getManager(blkInfo->serverId, 1000)->get(blkInfo, begin, count);
}

void SSDFeedDBDataTAdapter::removeFeed(const BlockInfoPtr& blkInfo, Ice::Long feedId){
	getManagerOneway(blkInfo->serverId)->removeFeed(blkInfo, feedId);
}

bool SSDFeedDBDataTAdapter::moveBlock(const BlockInfoPtr& blkInfoFrom, const BlockInfoPtr& blkInfoTo){
	return getManagerOneway(blkInfoFrom->serverId)->moveBlock(blkInfoFrom, blkInfoTo);
}

//////////////////////////////////////////////////////////////////////////////
FeedDBMasterPrx SSDFeedDBMasterTAdapter::getManager(int id, int timeout) {
        return locate<FeedDBMasterPrx>(_managers, "M", id, TWO_WAY, timeout);
}

FeedDBMasterPrx SSDFeedDBMasterTAdapter::getManagerOneway(int id) {
        return locate<FeedDBMasterPrx>(_managersOneway, "M", id, ONE_WAY);
}

BlockInfoPtr SSDFeedDBMasterTAdapter::getBlock(int userid, bool write){
	return getManager(userid)->getBlock(userid, write);
}

void SSDFeedDBMasterTAdapter::putBatch(const FeedItem& feed, const Int2IntMap& userWeight){
	if(userWeight.empty()){
		return;
	}
	//getManagerOneway(0)->putBatch(feed, userWeight);
	getManagerOneway(0)->putBatch(feed, userWeight);
}

BlockInfoPtr  SSDFeedDBMasterTAdapter::newBlock(int userid){
	return getManager(0)->newBlock(userid); 
}

void SSDFeedDBMasterTAdapter::removeBlock(int userid){
	return getManager(0)->removeBlock(userid); 
}

BlockInfoSeq  SSDFeedDBMasterTAdapter::getBlockBatch(const IntSeq& ids, bool write){
	if(ids.empty()){
		return BlockInfoSeq();
	}
	return getManager(ids.at(0), 1000)->getBlockBatch(ids, write); 
}

BlockInfoSeq  SSDFeedDBMasterTAdapter::newBlockBatch(const IntSeq& ids){
	if(ids.empty()){
		return BlockInfoSeq();
	}
	return getManager(ids.at(0), 1000)->newBlockBatch(ids); 
}

void SSDFeedDBMasterTAdapter::decTotalCount(const UserFeedCountSeq& feedCounts){
	if(feedCounts.empty()){
		return;
	}
	return getManager(0)->decTotalCount(feedCounts); 
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SSDFeedDBTAdapter::put(const FeedItem& feed, const Int2IntMap& userWeight){
	SSDFeedDBMasterTAdapter::instance().putBatch(feed, userWeight);
}

FeedItemSeq SSDFeedDBTAdapter::get(int userId, int begin, int count){
	BlockInfoPtr blk;
	try{
		blk = SSDFeedDBMasterTAdapter::instance().getBlock(userId, false);
	}catch(Ice::Exception& e){
		MCE_WARN("SSDFeedDBTAdapter::get --> call master err, "  << userId << ", " << e);
	}
	if(blk){
		try{
			return SSDFeedDBDataTAdapter::instance().get(blk, begin, count);
		}catch(Ice::Exception& e){
			MCE_WARN("SSDFeedDBTAdapter::get --> call data err, " << userId << ", " << e)
		}
	}
	return FeedItemSeq();
}

void SSDFeedDBTAdapter::remove(int userId, Ice::Long feedId){
	BlockInfoPtr blk;
	try{
		blk = SSDFeedDBMasterTAdapter::instance().getBlock(userId, false);
	}catch(Ice::Exception& e){
		MCE_WARN("SSDFeedDBTAdapter::remove --> call master err, "  << userId << " " << feedId << ", " << e);
	}
	if(blk){
		try{
			SSDFeedDBDataTAdapter::instance().removeFeed(blk, feedId);
		}catch(Ice::Exception& e){
			MCE_WARN("SSDFeedDBTAdapter::remove --> call data err, " << userId << " " << feedId << ", " << e)
		}
	}
}

void  SSDFeedDBTAdapter::removeAll(int userId){
	SSDFeedDBMasterTAdapter::instance().removeBlock(userId);
}
