#ifndef __FRIEND_INPUT_MANAGER_H__
#define __FRIEND_INPUT_MANAGER_H__

#include "FriendInputDataI.h"

namespace com {
namespace xiaonei { 
namespace services {
namespace FriendInput {

  using namespace com::renren::finputdata;

	class FriendInputManagerI :public FriendInputManager,
	public MyUtil::Singleton<FriendInputManagerI> {		
		public:
			FriendInputManagerI();
			virtual ~FriendInputManagerI(){MCE_INFO("~FriendInput");};
			virtual MyUtil::IntSeq getFriendInput(Ice::Int hostID, const std::string& condition, Ice::Int limit, const Ice::Current& crt=Ice::Current());
			virtual void reLoadFriend(Ice::Int hostID, const Ice::Current& crt=Ice::Current());//在客户端无调用，兼容老接口
      void init(string& zk,string& loaderpath); 

      const int FRIENDINPUT_DATA;
		private:
      void createObject(int hostID,FriendInputDataIPtr&);

      const string DB_FRIENDINPUT_PINYIN;
      const int TIMEOUT_CACHE;
      std::tr1::unordered_map<string,vector<string> > dict_;
      RelationLoaderAdapter adapter;
      string zk_;
      string loaderpath_;

			friend class MyUtil::Singleton<FriendInputManagerI>;
	};
	typedef IceUtil::Handle<FriendInputManagerI> FriendInputManagerIPtr;
};
};
};
};
#endif
