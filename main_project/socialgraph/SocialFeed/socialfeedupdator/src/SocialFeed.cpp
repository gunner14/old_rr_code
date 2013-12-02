#include "FriendFetcher.h"
#include "SocialFeed.h"
using namespace xce::socialgraph;
//using namespace MyUtil;


//now it's likely an random way  for friends and new FFs
SocialFeed::SocialFeed(USERID userid, const string type, const vector<USERID>& friends, const vector<USERID>& strangers,
		       const map<int,float>& offerfriend_sort){
        
        _type = type;
        _recommendation ="";
        _notNormal      = false;
	// sort _strangers by offerfriends
	// 对friends按照off排序会更好，效率考虑，暂不做
	for (vector<USERID>::const_iterator iter = friends.begin();
	     iter != friends.end(); ++iter){
	     _friends.push_back(make_pair<int,float>(*iter,0));
	}
	for (vector<USERID>::const_iterator iter = strangers.begin();
	     iter != strangers.end(); ++iter){
		float weight = 0;
		map<int,float>::const_iterator ofIter = offerfriend_sort.find(*iter);
		if (ofIter != offerfriend_sort.end()){
		    weight = ofIter->second;
                }
		_strangers.push_back(make_pair<int,float>(*iter,weight));
	}

        CommonNumbers(userid);

        sort(_strangers.begin(), _strangers.end(), FeedInnerComp());
}

//set the priority:type for feeds
char SocialFeed::GetType() const {
	if (_friends.size()>1 && _strangers.size()>1){
	    return 'c';
        }
	if (_friends.size()>1 && _strangers.size()==1){
	    return 'b';
        }
	return 'a';  // if (_friends.size()==1 && _strangers.size()>0)

}

//make md5 finger for one feed: A1,A2,A3;B1,B2,B3; it's ok
long SocialFeed::Finger() const {
	Feed feed;
	Transfer(feed);
	sort( feed.friendIds.begin(), feed.friendIds.end() );
	sort( feed.newCommonIds.begin(), feed.newCommonIds.end() );

	ostringstream feedOrderedIdList;
	for(vector<int>::iterator iter=feed.friendIds.begin();iter!=feed.friendIds.end(); ++iter) {
		feedOrderedIdList << *iter;
		if( iter != feed.friendIds.end() - 1) { 
			feedOrderedIdList << ","; 
		}
	}

	feedOrderedIdList << ";";

	for(vector<int>::iterator iter=feed.newCommonIds.begin();iter!=feed.newCommonIds.end(); ++iter) {
		feedOrderedIdList << *iter;
		if( iter != feed.newCommonIds.end() - 1 ) {
			feedOrderedIdList << ",";
		}
	}

	long   feedValue = xce::md5::MD5Util::stringToMD5( feedOrderedIdList.str() );
	return feedValue;
}
//transfer to cache data format:it's ok
int SocialFeed::Transfer(Feed& feed) const {
	const size_t MAXNUM_FRIEND = 8;//4
	const size_t MAXNUM_COMMON = 8;//6
	feed.friendIds.reserve(_friends.size() < MAXNUM_FRIEND ? _friends.size() : MAXNUM_FRIEND);
	feed.newCommonIds.reserve(_strangers.size() < MAXNUM_COMMON ? _strangers.size() : MAXNUM_COMMON);
        feed.commonNums.reserve(_common_numbers.size() < MAXNUM_COMMON ? _common_numbers.size() : MAXNUM_COMMON);
       
        feed.type           = _type;
        feed.recommendation = _recommendation;

	for (vector<pair<USERID,float> >::const_iterator iter = _friends.begin();
	     iter != _friends.end(); ++iter){
		feed.friendIds.push_back(iter->first);
		if (feed.friendIds.size() >= MAXNUM_FRIEND){
		    break;
                }
	}
	for (vector<pair<USERID,float> >::const_iterator iter = _strangers.begin();
	     iter != _strangers.end(); ++iter){
		feed.newCommonIds.push_back(iter->first);
		if (feed.newCommonIds.size() >= MAXNUM_COMMON){
		    break;
                }
	}
        for (vector<int>::const_iterator iter = _common_numbers.begin();
             iter != _common_numbers.end(); ++iter){
                feed.commonNums.push_back(*iter);
                if (feed.commonNums.size() >= MAXNUM_COMMON){
                    break;
                }
        }
        
	return 0;
}

void  SocialFeed::CommonNumbers(USERID userid){

      FriendFetcher fetcher;
      std::vector<USERID>  friends_list;
      std::vector<USERID>  temp_list;
      fetcher.Read(userid, friends_list);
      for (vector<pair<USERID,float> >::const_iterator iter = _strangers.begin(); iter != _strangers.end(); iter++){
		temp_list.clear();
		fetcher.Read(iter->first, temp_list);
                int  shares = 0;
                vector<USERID>::const_iterator friends_iter = friends_list.begin();
                vector<USERID>::const_iterator temp_iter    = temp_list.begin(); 
		if (!temp_list.empty()) {
			while (friends_iter != friends_list.end() &&
			       temp_iter != temp_list.end()) {
				if (*friends_iter == *temp_iter) {
				    shares++;
				    ++friends_iter;
				    ++temp_iter;
				} else if (*friends_iter < *temp_iter) {
			            ++friends_iter;
				} else {
				    ++temp_iter;
				}
			}
                 }else{
                      CLOG(WARN) << "computer common friends num:0 for uid " << userid;
                 }
                 _common_numbers.push_back(shares);
                 
                 if(shares <= 0){
                    _notNormal = true;
                 }
      }
}

//print the feed 
int SocialFeed::Print(USERID userid) const{
        
        ostringstream fo;
        fo << "socialfeed:" << userid << " fs:";
        for (vector<pair<USERID, float > >::const_iterator iter = _friends.begin();
             iter !=_friends.end(); iter++){
                fo << " " << iter->first;
        }
        fo << " ffs:";
        for (vector<pair<USERID,float> > ::const_iterator iter = _strangers.begin();
             iter != _strangers.end(); ++iter){
                fo << " " << iter->first;
        }
        fo <<" cns:";
        for (vector<USERID>::const_iterator iter = _common_numbers.begin(); iter!=_common_numbers.end(); iter++){
                fo <<" "<< *iter;
        }
        CLOG(INFO) << fo.str();

/*
        fo << "\ntransfeed:";        
	for (vector<pair<USERID,float> >::const_iterator iter = _friends.begin();
	     iter != _friends.end(); ++iter){
		if (iter != _friends.begin()){
		    fprintf(stderr, ",");
                }
		fprintf(stderr, "%d_%.2f", iter->first, iter->second);
	}

	fprintf(stderr, " + ");

	for (vector<pair<USERID,float> >::const_iterator iter = _strangers.begin();
	     iter != _strangers.end(); ++iter){

		if (iter != _strangers.begin()){
		    fprintf(stderr, ",");
                } 
		fprintf(stderr, "%d_%.2f", iter->first, iter->second);
	}
	fprintf(stderr, "\n");*/
	return 0;
}

