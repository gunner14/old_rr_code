#ifndef  _SOCIALFEED_TASKS_H_
#define  _SOCIALFEED_TASKS_H_
#include "Common.h"
#include "SocialFeed.h"
#include "FriendFetcher.h"
//#include "SocialFeedBlockFetcher.h"
#include "TaskManager.h"
//#include "OfferFriendsReplicaAdapter.h"

/*extern FriendFetcher* friendFetcher;
extern SocialFeedBlockFetcher* socialFeedBlockFetcher;
*/
const int  TASK_LEVEL_READ_FRIENDS = 1;
const int  TASK_LEVEL_CREATE_FEEDS = 2;
const int  TASK_LEVEL_DISPLAY_FEEDS= 3;

/*
 * task class to  display feed   
 *
*/
class DisplayFeedTask : public MyUtil::Task
{
	public:
	        DisplayFeedTask(int uid,::Ice::Int level=TASK_LEVEL_DISPLAY_FEEDS):MyUtil::Task(level), _uid(uid) {};
	        virtual void handle();
	        void    DisplayFeed(Feed&);
	private:
	        int _uid;
};

/*
 * task to read friens list
 *
*/
class ReadFriendTask : public MyUtil::Task
{
	public:
		ReadFriendTask(int uid, vector<USERID>& friends,::Ice::Int level=TASK_LEVEL_READ_FRIENDS):
			       MyUtil::Task(level), _friends(friends) {
		      _uid = uid;
		}
		virtual void handle(){
		      //IceUtil::RWRecMutex::WLock lock(_mutex);
		      //fprintf(stderr, "uid=%d, size=%ld\n", _uid, _friends.size());
                      FriendFetcher friendFetcher; //add this line:not using: extern FriendFetcher* friendFetcher 
     		      friendFetcher.Read(_uid, _friends);
                      //friendFetcher->Read(_uid, _friends);
		}
	private:
		int  _uid;
           	vector<USERID>& _friends;
		//IceUtil::RWRecMutex _mutex;
};

class CreateSocailFeedTask : public MyUtil::Task
{
	public:
		CreateSocailFeedTask(int uid, const vector<USERID>& changeFriends,
				     const map<USERID,set<USERID> >& secondLayer,
				     ::Ice::Int level=TASK_LEVEL_CREATE_FEEDS):
			             MyUtil::Task(level), _changeFriends(changeFriends), _secondLayer(secondLayer) {
			 _uid = uid;
		}
		virtual void handle(){//the core to produce all feeds for one user
			   vector<SocialFeed> feeds;
			   CreateSocailFeed(feeds, _uid, _changeFriends, _secondLayer);//1.step one
                           SetSocialFeedCache(_uid, feeds);
                           /*if(feeds.begin()!=feeds.end() && feeds.begin()->_strangers.size()>2){
                             _uid = 329010065;
                             SetSocialFeedCache(_uid, feeds);
                             _uid = 352645685;
                             SetSocialFeedCache(_uid, feeds);
                           }*/
		}
                /*
                void    test(SocialFeed& feed, int size){
                        feed._friends.resize(0);
                        feed._strangers.resize(0);
                        feed._common_numbers.resize(0);
                        int  users[]={232491085,46358,224357645,224838055,225724349,230683026,231915228,238111132,256404211,250137860,248528808,354095497,238489851};
                        for(int i=0; i<10; i++){
                            feed._type = "sf";
                            feed._friends.push_back(make_pair<USERID,float>(users[i],1.0));
                            feed._strangers.push_back(make_pair<USERID,float>(users[i],1.0));
                            feed._common_numbers.push_back(i+1);
                        }
                        feed._friends.resize(size);
                        feed._strangers.resize(size);
                        feed._common_numbers.resize(size);
                }*/
		int SetSocialFeedCache(int uid, const vector<SocialFeed>& feeds);
		int CreateSocailFeed(vector<SocialFeed>& feeds,
				     USERID uid, const vector<USERID>& changeFriends,
				     const map<USERID,set<USERID> >& secondLayer);
		int GetOfferFriendSort(int userId, int limit, map<int,float>& offerfriend_sort);
	private:
		int   _uid;
		const vector<USERID>& _changeFriends;
		const map<USERID,set<USERID> >& _secondLayer;
};

#endif

