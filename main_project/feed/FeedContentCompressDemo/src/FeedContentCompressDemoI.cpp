/*
 * =====================================================================================
 *
 *       Filename:  FeedContentCompressDemoI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月23日 14时40分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  guanghe.ge (ggh), guanghe.ge@opi-corp.com
 *        Company:  XiaoNei
 *
 * =====================================================================================
 */

#include "FeedContentCompressDemoI.h"
#include "RFeed.h"
#include "FeedContentAdapter.h"
#include "FeedMemcProxy/client/feed_memc_client.h"

using namespace MyUtil;
using namespace xce::feed;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&FeedContentCompressDemoI::instance(),service.createIdentity("M", ""));
}

void FeedContentCompressDemoI::compressAndDecompress(long feedid, const Ice::Current& current) {
	MCE_DEBUG("FeedContentCompressDemoI::compressAndDecompress, feedid: " << feedid);

	 //get dict
	 FeedContentDict dict;
	 MyUtil::LongSeq fids;
	 fids.push_back(feedid);
	 try {
		 dict = FeedContentAdapter::instance().getFeedDict(fids);
	 } catch (...) {
		 MCE_WARN("get dict error, feedid: " << feedid);
	 }

	 if (dict.empty()) return;
	 MCE_DEBUG("getFeedDice from Memcached, feedid: " << feedid);

	 //有时会返回空的feed
	 for (FeedContentDict::iterator it = dict.begin();
			 it != dict.end(); ++it) {
		 MCE_DEBUG("feedid of first: " << it->first);
		 if (it->second) {
		 	MCE_DEBUG("feedid of second: " << it->second->data->feed << " xml: " << it->second->data->xml);
		 }
	 }

	 //set 0 content

	 try {
		 if (!dict[feedid]) return;
		 FeedContentPtr content = new FeedContent();
		 content->data = dict[feedid]->data;
		 content->data->feed = 0;
		 
		 bool res = FeedMemcClient::instance().SetFeed(content);
		 if (!res) {
			 MCE_WARN("setFeed fail, feedid: " << content->data->feed);
		 }
		 
	 } catch (std::exception& e) {
		 MCE_WARN("set 0 dict error, feedid: " << feedid << " " << e.what());
	 } catch (...) {
		 MCE_WARN("set 0 dict error, feedid: " << feedid);
	 }

	 //get 0 dict
	 FeedContentDict dict_zero;
	 MyUtil::LongSeq fids_zero;
	 fids_zero.push_back(-1);
	 fids_zero.push_back(0);
	 fids_zero.push_back(2);
	 fids_zero.push_back(feedid);
	 fids_zero.push_back(3);
	 try {
		 dict_zero = FeedContentAdapter::instance().getFeedDict(fids_zero);
	 } catch (std::exception& e) {
		 MCE_WARN("get 0 dict error, feedid: " << feedid << " " << e.what());
	 } catch (...) {
		 MCE_WARN("get 0 dict error, feedid: " << feedid);
	 }

	 MCE_DEBUG("getFeedDict of zero from Memcached, feedid: " << feedid);

	 for (FeedContentDict::iterator it = dict_zero.begin();
			 it != dict_zero.end(); ++it) {
		 MCE_DEBUG("feedid of zero first: " << it->first);
		 if (it->second) {
		 	MCE_DEBUG("feedid of zero second: " << it->second->data->feed << " xml: " << it->second->data->xml);
		 }
	 }

	return;
}
