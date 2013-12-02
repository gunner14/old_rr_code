#ifndef XCE_SOCIALGRAPH_ACTION_H__
#define XCE_SOCIALGRAPH_ACTION_H__

#include <vector>
#include <string>
#include <map>
#include<stdlib.h>
#include <sstream>
#include<ctime>

#include "socialgraph/socialgraphd/base_action.h"
#include "OceCxxAdapter/src/UserCacheAdapter.h"
#include "OceCxxAdapter/src/XceLoggerAdapter.h"
#include "socialgraph/socialgraphproto/item.pb.h"
#include "socialgraph/socialgraphd/layout.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "socialgraph/socialgraphd/xlog/src/client/xlog_appender.h"
#include "socialgraph/socialgraphd/xlog/src/client/client.h"

namespace xce {
namespace socialgraph {

const static int LEVEL_LOG_STATS = 5;			//level of log stats

//-----------------------------------------------------------------------------------------------------------

/* @brief log输出统计类*/
class XLogInstance 
{
public:
	static XLogInstance* GetInstance(){
		if(m_pInstance == NULL)//判断是否第一次调用
			m_pInstance = new XLogInstance();
		return m_pInstance;
	}
public:


	XLogInstance(){
                xremote_logger_ = new xlog::XLogAppender(xlog_paths_, "xlog.conf");
	}
	static bool _init;
	static bool Init(){
		//logpaths:socialgraph/reconnect/homeReconnect
		xlog_paths_.push_back("socialgraph");
                xlog_paths_.push_back("reconnect");
                xlog_paths_.push_back("homeReconnect");
	}
	static XLogInstance* m_pInstance;
public:
 	xlog::XLogAppender *xremote_logger_;
  	static std::vector<std::string> xlog_paths_;

  	int userid_;
  	std::string param_;
};
class XLogTask : public MyUtil::Task {
 public:
        XLogTask(Ice::Int userid, const std::string& param, int level = LEVEL_LOG_STATS):
                Task(level), userid_(userid), param_(param) {
        }

  /* @brief 初始化输出log的路径*/
        void InitLogPath();

        virtual void handle();

 private:
  int userid_;
  std::string param_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief log输出统计类*/
class LogStatsTask : public MyUtil::Task {
 public:
	LogStatsTask(Ice::Int userid, const std::string& param, int level = LEVEL_LOG_STATS):
		Task(level), userid_(userid), param_(param) {
	}

  /* @brief 初始化输出log的路径*/
	static void InitLogPath();

	virtual void handle();

 private:
  static com::xiaonei::service::XceLoggerPtr remote_logger_; 
  static std::vector<std::string> log_paths_;

	int userid_;
	std::string param_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 首页推荐位获取操作 */
//extern int currentRcdFriendsNum;
class GetAction5 : public Base2Action {
 public:
  GetAction5() {
    currentRcdFriendsNum = 0;
		MyUtil::TaskManager::instance().config(LEVEL_LOG_STATS, MyUtil::ThreadPoolConfig(0, 100));
		LogStatsTask::InitLogPath();			//initialize path of log in hadoop
  }

  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_nget_home");
  }
  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* user) {
    return DoPost(req, res, user);
  }
  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);

 private:
  /* @brief 按产品输出统计log
   * @param userid 用户id
   * @param biz 产品标记
   * @param type2count 策略对应数据统计
   */
	void OutputLogStatsByProduct(int userid, 
															 const std::string& biz, 
															 const std::map<std::string, int>& type2count);

	void OutputShowLog(int uid, const RecommendItemSeq& item_seq);

  std::vector<LayoutPtr> PrepareFriendsLayout(int uid);
  std::vector<LayoutPtr> PrepareOtherLayout(int uid);
  std::vector<LayoutPtr> PrepareLayout(int uid);
  std::vector<int> GetUserIdSeq(std::vector<LayoutPtr>*);
  void FilterUserCache(const UserCacheMap& map1, UserCacheMap& map2);
  int currentRcdFriendsNum;
  static const int kNewRcdFriendsSize = 32; //new totaol recommended friends number displayed 4 ones in a row
}; 

//-----------------------------------------------------------------------------------------------------------

/* @brief 首页推荐位删除操作 */
class RemoveAction5 : public Base2Action {
 public:
  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_nremove_home");
  }

  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* u) {
    return DoPost(req, res, u);
  }

  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 首页推荐位的数据监控 */
class StatAction : public Base2Action {
 public:
  StatAction();
  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/ajaxHomeReconnectStat");
  }
  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* user) {
    return DoPost(req, res, user);
  }
  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);

 private:
  com::xiaonei::service::XceLoggerPtr remote_logger_; 
  std::vector<std::string> log_paths_;

}; 

//-----------------------------------------------------------------------------------------------------------
class GetPhotoFriendAction5 : public Base2Action {
 public:
  GetPhotoFriendAction5() {
    currentRcdFriendsNum = 0;
                MyUtil::TaskManager::instance().config(LEVEL_LOG_STATS, MyUtil::ThreadPoolConfig(0, 100));
                LogStatsTask::InitLogPath();                    //initialize path of log in hadoop
  }

  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_nget_photo");
  }
  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* user) {
    return DoPost(req, res, user);
  }
  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);

 private:
  /* @brief 按产品输出统计log
   * @param userid 用户id
   * @param biz 产品标记
   * @param type2count 策略对应数据统计
   */
void OutputLogStatsByProduct(int userid, const std::string& biz, const std::map<std::string, int>& type2count);
void OutputShowLog(int uid, const RecommendItemSeq& item_seq);

  std::vector<LayoutPtr> PrepareFriendsLayout(int uid);
  std::vector<LayoutPtr> PrepareLayout(int uid);
  std::vector<int> GetUserIdSeq(std::vector<LayoutPtr>*);
  void FilterUserCache(const UserCacheMap& map1, UserCacheMap& map2);
  int currentRcdFriendsNum;
  static const int kNewRcdFriendsSize = 32; //new totaol recommended friends number displayed 4 ones in a row
};
//-----------------------------------------------------------------------------------------------------------

/* @brief 为blog连续阅读提供服务 */
class BlogAction : public Base2Action {
public: 
  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_blog");
  }

  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* u) {
    return DoPost(req, res, u);  
  }

  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);
  
private:
  /* @brief 根据blogid获取日志的相关度推荐
   * @param blogid 日志id
   */
  xce::socialgraph::Items GetBlogDataByBlogTag(long blogid);

  /* @brief 根据userid的用户行为推荐日志
   * @param userid 用户id
   */
  xce::socialgraph::Items GetBlogDataByUseraction(int userid);
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为video连续观看提供服务 */
class VideoAction : public Base2Action {
 public:
  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_video");
  }
  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* u) {
    return DoPost(req, res, u);
  }
  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 其他页面推荐位获取操作 */
class GetOtherAction5: public Base2Action {
 public:
  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_nget") || boost::starts_with(url, "/cwf_get") || boost::starts_with(url, "/cwf_performance");
  }
  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* user) {
    return DoPost(req, res, user);
  }
  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);

 private:
  /* @brief 按产品输出统计log
   * @param userid 用户id
   * @param biz 产品标记
   * @param type2count 策略对应数据统计
   */
	void OutputLogStatsByProduct(int userid, 
															 const std::string& biz, 
															 const std::map<std::string, int>& type2count);
  std::vector<LayoutPtr> PrepareFriendsLayout(int uid);
  std::vector<LayoutPtr> PrepareOtherLayout(int uid);
  std::vector<LayoutPtr> PrepareLayout(int uid);
  std::vector<int> GetUserIdSeq(std::vector<LayoutPtr>*);
  void FilterUserCache(const UserCacheMap& map1, UserCacheMap& map2);
}; 

//-----------------------------------------------------------------------------------------------------------

/* @brief 相册页面好友推荐位删除操作 */
class RemovePhotoFriendAction5 : public Base2Action {
 public:
  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_nremove_photo");
  }

  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* u) {
    return DoPost(req, res, u);
  }

  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);
};
//-----------------------------------------------------------------------------------------------------------

/* @brief 其他页面推荐位删除操作 */
class RemoveOtherAction5 : public Base2Action {
 public:
  bool Match(const std::string& url) const {
    return boost::starts_with(url, "/cwf_nremove") || boost::starts_with(url, "/cwf_remove");
  }

  virtual cwf::HttpStatusCode DoGet(cwf::Request* req, cwf::Response* res, cwf::User* u) {
    return DoPost(req, res, u);
  }

  virtual cwf::HttpStatusCode DoPost(cwf::Request*, cwf::Response*, cwf::User*);
};
}}
#endif

