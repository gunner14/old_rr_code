#ifndef COMMONDBHELPER_H_
#define COMMONDBHELPER_H_

#include <vector>

#include "DbCxxPool/src/QueryRunner.h"
#include "util/cpp/IntSeqSerialize.h"

namespace xce {
namespace socialgraph {

struct CommonFriend {
  CommonFriend() {       
    userId = 0;
    weight = 0;
    sharesCount = 0;     
  }

  int userId;
  int weight;            
  int sharesCount;
  std::vector<int> shares;  
};

typedef std::vector<CommonFriend> CommonFriendSeq;

//---------------------------------------------------------------------------------------------------------------

const static int EMPTY_ID = -999;		//如果要删除指点的推荐好友，则将该好友的friend_id设置为该值
const static int CF_SAVE_THRESHOLD_DB = 100;		//db中存储100条记录
const static size_t SAVE_DATA_LENGTH = 16;   //数据库shares字段长度为255,所以这里最多存63个int

//---------------------------------------------------------------------------------------------------------------

class CommonDbHelper {
public:
	/* @brief 从db中获取commons friends数据
   * @param hostid 用户id 
   * @param comms 返回的结果值
   */
	static void GetCommonsFromDb(int hostid, CommonFriendSeq& comms);
	
	/* @brief 向db中插入commons friends数据
   * @param hostid 用户id 
   * @param comms 插入的数据集合
   */
	static void SetCommonsToDb(int hostid, const CommonFriendSeq& comms);

	/* @brief 从db中删除commons friends数据,并加入block表
   * @param hostid 用户id 
   * @param friendid 删除id
   */
	static void RemoveCommonsFromDb(int hostid, int friendid);

	/* @brief 获取最近一天登录的用户id
   * @param userid_list 存储最近一天登录的用户id
   */
	static void GetRecentlyloginUserId(std::vector<int>& userid_list);

	/* @brief 获取用户数据的创建时间
   * @param hostid 用户id
   */
	static long GetCreateDataTime(int hostid);

	/* @brief 获取用户关注的小站
   * @param hostid 用户id
   * @param limit 小站的个数
   */
	static std::vector<int> GetUserFollowSites(int hostid, int limit);

	/* @brief 获取用户关注的小站的url
   * @param siteids 小站id
   */
	static std::map<int, std::string> GetSiteUrls(const std::vector<int>& siteids);

	/* @brief 获取用户关注的公共主页
   * @param hostid 用户id
   * @param limit 小站的个数
   */
	static std::vector<int> GetUserFollowPages(int hostid, int limit);

};

//---------------------------------------------------------------------------------------------------------------

/* @brief common friends 数据库handler类 */
class CommonFriendSetResultHandler : public com::xiaonei::xce::ResultHandler {
public:
	CommonFriendSetResultHandler(CommonFriendSeq& results) : results_(results) { }
	virtual ~CommonFriendSetResultHandler() {}

protected:
	virtual bool handle(mysqlpp::Row& row) const;	

private:
	CommonFriendSeq& results_;
};

//---------------------------------------------------------------------------------------------------------------

/* @brief 获取最近一天登录的用户id 数据库handler类*/
class UsertimeBatchReadHandler : public com::xiaonei::xce::ResultHandler {
public:
	UsertimeBatchReadHandler(int& last, std::vector<int>& idseq): last_(last), idseq_(idseq) {}
	virtual bool handle(mysqlpp::Row& row) const;

private:
	int& last_;
	std::vector<int>& idseq_; 
};

//---------------------------------------------------------------------------------------------------------------

/* @brief 获取用户hostid关注的小站列表 数据库handler类*/
class FollowSitesReadHandler : public com::xiaonei::xce::ResultHandler {
public:
	FollowSitesReadHandler(int hostid, std::vector<int>& site_seq): hostid_(hostid), site_seq_(site_seq) {}
	virtual bool handle(mysqlpp::Row& row) const;

private:
	int hostid_;
	std::vector<int>& site_seq_;
};

//---------------------------------------------------------------------------------------------------------------

/* @brief 获取用户hostid关注的小站列表 数据库handler类*/
class FollowSitesUrlHandler : public com::xiaonei::xce::ResultHandler {
public:
	FollowSitesUrlHandler(std::map<int, std::string>& site_url_map): site_url_map_(site_url_map) {}
	virtual bool handle(mysqlpp::Row& row) const;

private:
	std::map<int, std::string>& site_url_map_;
};

//---------------------------------------------------------------------------------------------------------------

/* @brief 获取用户hostid关注的公共主页列表 数据库handler类*/
class FollowPagesReadHandler : public com::xiaonei::xce::ResultHandler {
public:
	FollowPagesReadHandler(int hostid, std::vector<int>& page_seq): hostid_(hostid), page_seq_(page_seq) {}
	virtual bool handle(mysqlpp::Row& row) const;

private:
	int hostid_;
	std::vector<int>& page_seq_;
};

//---------------------------------------------------------------------------------------------------------------

class TableName{
public:
  static std::string common_friends_table(int id) {
    ostringstream oss;
    oss << "common_friends_" << (id % 100);
    return oss.str();
  }
  static std::string common_friends_pattern(int id) {
    ostringstream oss;
    oss << "common_friends_" << (id % 100);
    return oss.str();
  }
  static std::string common_friends_block_table(int id) {
    ostringstream oss;
    oss << "common_friends_block_" << (id % 100);
    return oss.str();
  }
  static std::string common_friends_block_pattern(int id) {
    ostringstream oss;
    oss << "common_friends_block_" << (id % 100);
    return oss.str();
  }

  static std::string buddyapply_friendfinder_applicant(int id) {
    ostringstream oss;
    oss << "buddyapply_applicant_" << (id % 100);
    return oss.str();
  }

  static std::string buddyapply_friendfinder_applicant_pattern(int id) {
    ostringstream oss;
    oss << "buddyapply_applicant_" << (id % 100);
    return oss.str();
  }

	static std::string minisite_follower_site_table(int id) {
		ostringstream oss;
		oss << "minisite_follower_site_" << (id % 100);
		return oss.str();
	}

	static std::string page_fans_table(int id) {
		ostringstream oss;
		oss << "fans_pages_" << (id % 100);
		return oss.str();
	}
};

}
}

#endif
