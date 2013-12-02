#ifndef __I_LIKE_IT_UTIL_H__
#define __I_LIKE_IT_UTIL_H__

#include <math.h>
#include <ILikeIt.h>
#include "IceExt/src/ServiceI.h"
//#include "WEventAdapter.h"
#include "boost/lexical_cast.hpp"
#include "NotifyDispatcherAdapter.h"
#include "xiaonei-commons-gid/util/c/gid.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace ilikeit {

const static std::string DB_DESC_ILIKEIT = "ilikeit";
const static std::string TRIPOD_LIKE_TABLE_NAME = "like_ship";
const static std::string TABLE_LIKESHIP = "like_ship_";
const static std::string FIELD_GlobalId = "gid";
const static std::string FIELD_UID = "uid";

const static size_t TABLE_NUM = 100;

const static int FRIENDS_LIMIT = 1000;

// const static int64_t TYPE_MASK = 0x100000;
// const static int64_t OBJECT_MASK = 0x80000000000;
const static int64_t TYPE_MASK = 1000000LL;
const static int64_t OBJECT_MASK = 8000000000000LL;

class GlobalIdTrans { 
public:
	static GlobalIdPtr Str2GlobalId(const std::string& str) {
		GlobalIdPtr gid = new GlobalId;
		gid->type = -1;
		gid->id = -1;
		try {
			size_t pos = str.find("_");
			if ( pos != std::string::npos ) {
				string seg0 = str.substr(0, pos);
				string seg1 = str.substr(pos+1, str.length()-pos-1);
				gid->type = TypeStr2Int(seg0);
				gid->id = atol(seg1.c_str());
			}
		} catch (std::exception& e) {
			MCE_WARN("[GlobalIdTrans::Str2GlobalId] invalid Str_Gid [" << str << "] " << e.what());
		}
		return gid;
	}


	// 将string类型的gid计算为一个可以进行模运算的long类型
	static inline long GlobalId2Long(const GlobalIdPtr& gid) {
		return (gid->type % TYPE_MASK) * OBJECT_MASK + (gid->id % OBJECT_MASK);
	}

	// 取得类型
	static int TypeStr2Int(const std::string& str) {
		if ( str == "profile" ) {
			return 0;
	   	} else if ( str == "blog") {
			return 1;
		} else if ( str == "photo" ) {
			return 2;
		} else if ( str == "album" ) {
			return 3;
		} else if ( str == "share" ) {
			return 4;
		} else if ( str == "status" ) {
			return 5;
		} else if ( str == "edm" ) {
			return 6;
		} else if ( str == "love" ) {
			return 7;
		} else if ( str == "connect" ) {
			return 8;
		} else if ( str == "ads" ) {
			return 9;
		} else if ( str == "game" ) {
			return 10;
		} else if ( str == "minisite" ) {
			return 11;
		} else if ( str == "page" ) {
			return 12;
		} else if ( str == "video" ) {
			return 13;
		} else if ( str == "app" ) {
			return 14;
		} else if ( str == "blogcomment" ) {
			return 15;
		} else if ( str == "photocomment" ) {
			return 16;
		} else if ( str == "albumcomment" ) {
			return 17;
		} else if ( str == "sharecomment" ) {
			return 18;
		} else if ( str == "statuscomment" ) {
			return 19;
		} else if ( str == "appreciation" ) {
			return 20;
		} else if ( str == "client" ) {
			return 21;
		} else if ( str == "reply" ) {
			return 22;
    } else if ( str ==  "school" ) {
      return 23;
    } else if ( str == "pagedoing" ) {
      return 24;
    } else if ( str == "pagedoingcomment" ) {
      return 25;
    } else if ( str == "pagephoto" ) {
      return 26;
    } else if ( str == "pagephotocomment" ) {
      return 27;
    } else if ( str == "pagenote" ) {
      return 28;
    } else if ( str == "pagenotecomment" ) {
      return 29;
    } else if ( str == "pageshare" ) {
      return 30;
    } else if ( str == "pagesharecomment" ) {
      return 31;
    } else if ( str == "pagealbum" ) {
      return 32;
    } else if ( str == "pagealbumcomment" ) {
      return 33;
    } else if ( str == "pagegossip" ) {
      return 34;
    } else if ( str == "pagegossipcomment" ) {
      return 35;
    } else if (str == "socialwiki") {
      return 36;
    } else if (str == "socialwikidoing") {
      return 37;
    } else if (str == "socialwikidone") {
      return 38;
    } else if (str == "socialwikiwilldo") {
      return 39;
    } else if (str == "mini") {
      return 40;
    } else if (str == "videocomment") {
      return 41;
    } else if (str == "zuji") {
      return 42;
    } else if (str == "zphoto") {
      return 43;
    } else if (str == "zvisit") {
      return 44;
    } else if (str == "zshared") {
      return 45;
    } else if (str == "zplantodo") {
      return 46;
    } else if (str == "zhantxt") {
      return 47;
    } else if (str == "zhanpic") {
      return 48;
    } else if (str == "zhanvid") {
      return 49;
    } else if (str == "zhanlink") {
      return 50;
    } else {
			return -1;
		}
	}
};


class ILikeItUtil  : public MyUtil::Singleton<ILikeItUtil>{

private:
	friend class MyUtil::Singleton<ILikeItUtil>; 
public:
	map<int,string> id2Name;
	map<int,string> id2Url;

	void init(){
		id2Name[1] = "日志";
		id2Name[2] = "相片"; 
		id2Name[3] = "相册";
		id2Name[4] = "分享";
		id2Name[13] = "视频";

		id2Url[1] = "http://blog.renren.com/GetEntry.do?";
		id2Url[2] = "http://photo.renren.com/getphoto.do?";
		id2Url[3] = "http://photo.renren.com/getalbum.do?";
		id2Url[4] = "http://share.renren.com/share/ShareList.do?";
		id2Url[13] = "http://video.renren.com/video/";
//		id2Url[13] = "http://video.renren.com/uid/video-";
//	MyUtil::init_logger("Mce", "oce_log_addlike", "INFO");

  }
	static string G2g(const string& G){
		GlobalIdPtr gp = GlobalIdTrans::Str2GlobalId(G);
		if(gp->type == 13){
			ostringstream oss;
			oss<<"video_";
			oss<<gid::G::ExtractX(gp->id);
			MCE_INFO(G<<" convert "<<oss.str());
			return oss.str();
		}else{
			return G;
		}	
	}
	static int g2G(const string& g){
		GlobalIdPtr gp = GlobalIdTrans::Str2GlobalId(g);
		if(gp->type == 13){
			int id = gid::G::Id(gp->id,gid::kVedio);	
			MCE_INFO(g<<" convert2 "<<id);
			return id;
		}else{
			return gp->id;
		}
	}
	static LikeInfoPtr emptyLikeInfo(int detailLimit = 0, bool showStrangers=true, bool withFriendList=false) {
		LikeInfoPtr result = new LikeInfo;
		result->selfCount = 0;
		result->friendCount = 0;
		result->totalCount = 0;
		result->detailLimit = detailLimit;
		result->showStrangers = showStrangers;
		result->withFriendList = withFriendList;
		return result;
	}
	static LikeInfo2Ptr emptyLikeInfo2(int detailLimit = 0, bool showStrangers=true, bool withFriendList=false) {
		LikeInfo2Ptr result = new LikeInfo2;
		result->selfCount = 0;
		result->friendCount = 0;
		result->totalCount = 0;
		result->detailLimit = detailLimit;
		result->showStrangers = showStrangers;
		result->withFriendList = withFriendList;
		return result;
	}
        static long getGidNum(const string& gid){
            int pos=gid.find('_');
            long num=-1;
            try{
                num = boost::lexical_cast<long>(gid.substr(pos+1));
            }catch(...){
            }
            return num;
        }

        static  short hexChar2dec(char c) {
            if ( '0'<=c && c<='9' ) {
                return short(c-'0');
            } else if ( 'a'<=c && c<='f' ) {
                return ( short(c-'a') + 10 );
            } else if ( 'A'<=c && c<='F' ) {
                return ( short(c-'A') + 10 );
            } else {
                return -1; 
            }
        }

        static string deescapeURL(const std::string &URL) {
            string result = "";
            for ( unsigned int i=0; i<URL.size(); i++ ) {
                char c = URL[i];
                if ( c != '%' ) {
                    result += c;
                } else {
                    char c1 = URL[++i];
                    char c0 = URL[++i];
                    int num = 0;
                    num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
                    result += char(num);
                }
            }
            return result;
        }

        void sendWebPager(int ownerId, int guestId, int totalCount, const string& gid, const string& name){
                try{
                        if((totalCount<=5 || totalCount%30==1)){
				MyUtil::Str2StrMap infos;
				infos["schema_id"]=infos["type"]="60";
			  int typeId;
				{
					GlobalIdPtr gip = GlobalIdTrans::Str2GlobalId(gid);
				  typeId = gip->type;
					ostringstream oss;
					oss<<GlobalIdTrans::GlobalId2Long(gip);
					infos["source"]=oss.str();
				}
				{
					ostringstream oss;
					oss<<guestId;
					infos["from"] = oss.str();
				}
				{
					ostringstream oss;
					oss<<ownerId;
					infos["to"] = infos["owner"] = oss.str();
				}
				infos["from_name"] = deescapeURL(name);
				infos["time"]="0";	
				string other_liker_desc;
/*
		
                                if(totalCount<=5)
                                        other_liker_desc = " 觉得";
                                else{
					ostringstream oss;
					oss<<" 和"<<(totalCount-1)<<"个人觉得";
					other_liker_desc = oss.str();
				}
*/
				infos["other_liker_desc"]=other_liker_desc;	
				ostringstream msg;
				if(typeId<4)
								msg<<id2Url[typeId]<<"id="<<getGidNum(gid)<<"&owner="<<ownerId;
				else if(typeId == 4)
								msg<<id2Url[typeId]<<"id="<<ownerId<<"&share_id="<<getGidNum(gid);
				else if(typeId == 13)
								msg<<id2Url[typeId]<<ownerId<<"/video-"<<getGidNum(gid);
				else
						return;
//				else if(typeId == 13)

				infos["like_link"] = msg.str();
				infos["content"] = id2Name[typeId];
//				MCE_INFO("[NotifyDispatcherAdapter] invoke from:"<<guestId<<" to: "<<ownerId<<" "<<name);	
				xce::notify::NotifyDispatcherAdapter::instance().dispatch(infos);
                        }
		}catch(std::exception& ex){
			MCE_WARN("send ex:"<<ex.what());
                }catch(...){
			MCE_WARN("send webpager notify fail ..");
                }
        }

};

class LikeDataI : public LikeData {
public:
  LikeDataI() {
  }

  int getSize() const {
    IceUtil::Mutex::Lock lock(_mutex);
    return users.size();
  }

  bool empty() const {
    IceUtil::Mutex::Lock lock(_mutex);
    return users.empty();
  }

  LikeDataI(const LikeDataPtr& ptr) {
    users.insert(users.begin(), ptr->users.begin(), ptr->users.end());
  }

  void add(int userId) {
    IceUtil::Mutex::Lock lock(_mutex);
    vector<int>::iterator it = lower_bound(users.begin(), users.end(), userId);
    if (it == users.end() || *it != userId) {
      users.insert(it, userId);
    }
  }

  void sortUsers() {
    IceUtil::Mutex::Lock lock(_mutex);
    sort(users.begin(), users.end());
  }

  bool isUserLiked(int uid) {
    IceUtil::Mutex::Lock lock(_mutex);
    return binary_search(users.begin(), users.end(), uid);
  }

  void addDirectly(int userId) {
    IceUtil::Mutex::Lock lock(_mutex);
    users.push_back(userId);
  }

  void remove(int userId) {
    IceUtil::Mutex::Lock lock(_mutex);
    vector<int>::iterator it = find(users.begin(), users.end(), userId);
    if (it != users.end()) {
      users.erase(it);
    }
  }

  LikeDataPtr getLikeData() const {
    LikeDataPtr ptr = new LikeData;
    IceUtil::Mutex::Lock lock(_mutex);
    ptr->users.insert(ptr->users.begin(), users.begin(), users.end());
    return ptr;
  }

  vector<int> getUsers() const {
    IceUtil::Mutex::Lock lock(_mutex);
    return users;
  }

private:
  IceUtil::Mutex _mutex; 
};

typedef IceUtil::Handle<LikeDataI> LikeDataIPtr;
// -------------- BatchLikeshipHandler ----------------------------
class BatchLikeshipHandler : public com::xiaonei::xce::ResultHandler {
public:
  BatchLikeshipHandler(map<long, LikeDataIPtr>& result) : _result(result) {
  }
  virtual bool handle(mysqlpp::Row& row) const {
    int uid = (int) row[FIELD_UID.c_str()];
    long gidLong = (long) row[FIELD_GlobalId.c_str()];

    map<long, LikeDataIPtr>::iterator it = _result.find(gidLong);
    if ( it != _result.end() ) {
      (*it).second->addDirectly(uid);
    } else {
      LikeDataIPtr ld = new LikeDataI();
      ld->addDirectly(uid);
      _result.insert( make_pair<long, LikeDataIPtr>(gidLong, ld) );
    }
    return true;
  }
private:
  map<long, LikeDataIPtr>& _result;
};


}
}

#endif
