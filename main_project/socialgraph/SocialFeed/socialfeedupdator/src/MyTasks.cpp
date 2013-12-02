#include "item.pb.h"
#include "MyTasks.h"
//using namespace xce::offerfriends::adapter;
//using namespace xce::offerfriends;
using namespace xce::socialgraph;
#include "SocialGraphCommonTripodAdapter.h"
//#include "UserCacheAdapter.h"
//using namespace MyUtil;
//using namespace xce::usercache;

//exchange uid to name
/*
string Id2Name(int uid){

    vector<int> idseq;
    idseq.push_back(uid);
    map<int, UserCachePtr> resultmap;
    map<int, UserCachePtr>::iterator it;
    resultmap = UserCacheAdapter::instance().GetUserCache(idseq);
    it = resultmap.begin();
    return (it->second)->name();
}*/

//show feeds
void DisplayFeedTask::handle(){
/*
    vector<Feed> feedlist;
    feedlist = SocialFeedCacheAdapter::instance().get(_uid,-1);
    for (vector<Feed>::iterator iter = feedlist.begin();
        iter != feedlist.end(); ++iter){
        DisplayFeed(*iter);
    }*/
}

//the core to show feeds
void DisplayFeedTask::DisplayFeed(Feed& feed){
/*
    sort(feed.friendIds.begin(), feed.friendIds.end());
    sort(feed.newCommonIds.begin(), feed.newCommonIds.end());
    ostringstream feedOrderedIdList;
    for (IntSeq::iterator iter=feed.friendIds.begin();
        iter!=feed.friendIds.end(); ++iter){
        feedOrderedIdList << Id2Name(*iter);
        if (iter != feed.friendIds.end() - 1){
            feedOrderedIdList << ",";
        }
    }
    feedOrderedIdList << ";";
    for (IntSeq::iterator iter=feed.newCommonIds.begin();
        iter!=feed.newCommonIds.end(); ++iter) {
        feedOrderedIdList << Id2Name(*iter);
        if ( iter != feed.newCommonIds.end() - 1) {
            feedOrderedIdList << ",";
        }
    }*/
//    fprintf(stderr, "%s\t%s\n", Id2Name(_uid).c_str(),feedOrderedIdList.str().c_str());
   //  CLOG(INFO) << Id2Name(_uid) << feedOrderedIdList.str();
}


//2.set feeds to cache
int CreateSocailFeedTask::SetSocialFeedCache(int uid, const vector<SocialFeed>& feeds){

	const size_t MAX_FEED_NUM = 4;//2
//	vector<Feed> feedlist;
//	feedlist.reserve(MAX_FEED_NUM);
        Items  sf_items;
        int    counter = 0;
        ostringstream fo;
	for (vector<SocialFeed>::const_iterator iter = feeds.begin(); iter != feeds.end(); ++iter){
  	        Feed  feed;
		iter->Transfer(feed);//it's needed
                fo << "cachefeed:" << counter++ << "-uid:" << uid << " type:" << feed.type << " fs:";
                Item* it2 = sf_items.add_items();
                it2->set_id(uid);
                it2->set_message(feed.type);
                BOOST_FOREACH(int i, feed.friendIds) {
                    it2->add_fields(i);
                    fo << " " << i;
                }
                it2->add_fields(-1);//splited by -1
                fo << " ffs:"; 
                BOOST_FOREACH(int j, feed.newCommonIds) {
                    it2->add_fields(j);
                    fo <<" " << j;
                }
                it2->add_fields(-1);//splited by -1
                fo << " cns:";
                BOOST_FOREACH(int k, feed.commonNums) {
                    it2->add_fields(k);
                    fo << " " << k;
                }
                it2->add_messages(feed.recommendation);
                fo <<" mes:" << feed.recommendation;
                fo << "\n";
		if (sf_items.items_size() >= MAX_FEED_NUM) {
	    	    break;
		}
  	}
        CLOG(INFO) << fo.str();           
	try {
              if (sf_items.items_size() > 0){
                  std::string value;
                  const int expired = 3*24*60*60;
                  int     byte_size = sf_items.ByteSize();
                  if (byte_size > 0) {
                      sf_items.set_timestamp(0);
                      sf_items.SerializeToString(&value);
                      std::string  key = boost::lexical_cast<std::string>(uid);
                      key += "_sf";
                      if (!SocialGraphCommonTripodAdapter::instance().Set(key, value, expired)){
                          CLOG(INFO) << "CreateSocailFeedTask::SetSocialFeedCache set feed data cache for hostId: " << key << " failed!";
                      }
                   }
               }
	 } catch (Ice::Exception& e) {
                CLOG(ERROR) << "CreateSocailFeedTask::SetSocialFeedCache Set feed slice exception " << e.what() ;
         } catch (std::exception& e) {
		CLOG(ERROR) << "CreateSocailFeedTask::SetSocialFeedCache Set feeds std exception " << e.what() ;
	 } catch (...) {
		CLOG(ERROR) << "CreateSocailFeedTask::SetSocialFeedCache Set feeds unknow exception";
	 }
	
 	 return 0;
} 


//1.create feed:ok 
int CreateSocailFeedTask::CreateSocailFeed(vector<SocialFeed>& feeds, 
		                           USERID uid, const vector<USERID>& changeFriends,
		                           const map<USERID,set<USERID> >& secondLayer){
	map<USERID,set<USERID> >::const_iterator secondIter;
	map<USERID,vector<USERID> > degree2; // map<3_layer_user, vector<my friends> >
	
	//struct timeval start[6];

	//1.计算某个新二度好友刚被哪些朋友加为好友了（此时可能包含好友以及自身）
	//gettimeofday(&start[0], NULL);
       // printf("\ntid:%d fs:",uid);
	for (vector<USERID>::const_iterator cfIter = changeFriends.begin();
	     cfIter != changeFriends.end(); ++cfIter){//for every changed friends
	//	printf("%d ffs:",*cfIter);
                secondIter = secondLayer.find(*cfIter);
		assert(secondIter != secondLayer.end());
		for (set<USERID>::const_iterator thirdIter = secondIter->second.begin();//for every friend added
		     thirdIter != secondIter->second.end(); ++thirdIter){
			map<USERID,vector<USERID> >::iterator iter = degree2.find(*thirdIter);//key:new added; value:friends
			if (iter == degree2.end()){
			    pair<map<USERID,vector<USERID> >::iterator, bool> result;
                            result = degree2.insert(make_pair<USERID,vector<USERID> >(*thirdIter, vector<USERID>()));
			    iter   = result.first;
			}
			iter->second.push_back(*cfIter);
          //              printf(" %d ",*thirdIter);
		}
	}//get the FF and the friends who has added it.

	//2.过滤好友及自身，分组合并
	//gettimeofday(&start[1], NULL);
	vector<USERID> friendlist;
        FriendFetcher  friendFetcher;
        friendFetcher.Read(uid, friendlist);
	//friendFetcher->Read(uid, friendlist);
	set<USERID> setFriends(friendlist.begin(), friendlist.end());

	//gettimeofday(&start[2], NULL);
	map<vector<USERID>,vector<USERID> > mapfeeds;
        //degree2::key:the FF newly added,value:friends who added the FF.
	for (map<USERID,vector<USERID> >::const_iterator iter = degree2.begin(); iter != degree2.end(); ++iter){
                //newly added is "myself" or already "my" friend: no contribution for "me" to make new friends.
		if (iter->first == uid || setFriends.find(iter->first) != setFriends.end()){
		    continue;
                }
		map<vector<USERID>,vector<USERID> >::iterator revIter = mapfeeds.find(iter->second);
		if (revIter == mapfeeds.end()){
		    pair<map<vector<USERID>,vector<USERID> >::iterator, bool> result;
                    result  = mapfeeds.insert(make_pair<vector<USERID>,vector<USERID> >(iter->second, vector<USERID>()));
		    revIter = result.first;
		}
		revIter->second.push_back(iter->first);
	}//grouped by the value of (degree2:value) 

	//3.去除block feed, 按照offerfriend排序, 计算TopN（no use）
	//gettimeofday(&start[3], NULL);
	set<long> blockfeed;
	//socialFeedBlockFetcher->Read(uid, blockfeed);
	//fprintf(stderr, "%d: ", uid);
	//for (set<long>::const_iterator iter = blockfeed.begin();
	//		iter != blockfeed.end(); ++iter)
	//{
	//	fprintf(stderr, "%ld,", *iter);
	//}
	//fprintf(stderr, "\n");
	
	//gettimeofday(&start[4], NULL);
        //4.sort the friends in one feed
	map<int,float>  offerfriend_sort;
	GetOfferFriendSort(uid, 100, offerfriend_sort);

	//gettimeofday(&start[5], NULL);
        //5.make feeds
        string  type = "sf";
	for (map<vector<USERID>,vector<USERID> >::const_iterator iter = mapfeeds.begin();
	     iter != mapfeeds.end(); ++iter){
		SocialFeed  feed(uid, type, iter->first, iter->second, offerfriend_sort);
		//long  finger = feed.Finger();
		//fprintf(stderr, "<<%ld>>", finger);
		//if (blockfeed.find(finger) != blockfeed.end()){
		 //   continue;
               // }
                if (!feed._notNormal){
		    feeds.push_back(feed);
                }
	}
        //6.sort between feeds
	sort(feeds.begin(), feeds.end(), SocialFeedComp());
	//for (int i=0; i<5; ++i)
	//{
	//	fprintf(stdout, "[%d:%ld]", i,
	//			(start[i+1].tv_sec-start[i].tv_sec)*1000000+start[i+1].tv_usec-start[i].tv_usec);
	//}
	//fprintf(stderr, "\n");
	return  0;
} 


//sort the friends by some weight
int CreateSocailFeedTask::GetOfferFriendSort(int userId, int limit, map<int,float>& offerfriend_sort) 
{
	//vector<Offer> list;
	//try {
	//	//list = OfferFriendsReplicaAdapter::instance().getOffer(userId, limit);
	//}catch( Ice::Exception e){
	//	cout << "[CreateSocailFeedTask] GetOfferFriendSort Ice-> " << e.what() << endl;
	//}catch( std::exception e){
	//	cout << "[CreateSocailFeedTask] GetOfferFriendSort std-> " << e.what() << endl;
	//}catch( ... ){
	//	cout << "[CreateSocailFeedTask] GetOfferFriendSort unknown " << endl;
	//}

	//for (vector<Offer>::const_iterator iter = list.begin();
	//		iter != list.end(); ++iter)
	//{
	//	offerfriend_sort.insert(make_pair<int,float>(iter->userId, iter->weight));
	//}
	return 0;
} 

