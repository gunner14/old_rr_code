#ifndef XCE_SOCIALGRAPH_LAYOUT_H__
#define XCE_SOCIALGRAPH_LAYOUT_H__

#include <vector>
#include <string>
#include <map>

#include "socialgraph/socialgraphd/base_action.h"
#include "OceCxxAdapter/src/UserCacheAdapter.h"
#include "OceCxxAdapter/src/XceLoggerAdapter.h"
#include "OceCxxAdapter/src/RecommendationManangerAdapter.h"
#include "socialgraph/socialgraphproto/item.pb.h"

#include "base/logging.h"

namespace xce {
namespace socialgraph {

class Layout;

typedef std::map<std::string, std::vector<unsigned char> > RawDataMap;
typedef std::map<int, xce::usercache::UserCachePtr> UserCacheMap;
typedef std::vector<unsigned char> RawData;
typedef std::map<std::string, RawData> RawDataMap;
typedef boost::shared_ptr<Layout> LayoutPtr;

static const std::string FREINDS_CLUSTER = "FFUCR";		
static const std::string COMMON_FRIENDS = "FFW";
static const std::string NEW_COMMER = "FFNCW";
static const std::string RECONNECT = "FFWR";
static const std::string NEW_COMMON_FRIENDS = "FOF";
static const std::string PAGE = "PAGE";
static const std::string RBLOG = "RCD_BLOG";
static const std::string RVIDEO = "RCD_VIDEO";
static const std::string RSITE = "RCDSITE";
static const std::string RAPP = "RCDAPP";
static const std::string RDESK = "RCDDESK";
static const std::string RFORUM = "RCDFORUM";

static const std::string SHARE_FREINDS_CLUSTER = "SHARE_FFUCR";		
static const std::string SHARE_COMMON_FRIENDS = "SHARE_FFW";
static const std::string SHARE_PAGE = "SHARE_PAGE";

//-----------------------------------------------------------------------------------------------------------

class Layout {
  public:
    virtual std::string DoLayout(UserCacheMap*) = 0;
    virtual const std::string getBizType() = 0;
    virtual std::set<int> UserIdSet() {
      return std::set<int>();
    }
    virtual ~Layout() {}
};

//-----------------------------------------------------------------------------------------------------------

class RcdLayout : public Layout {
 public:
  RcdLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {

  }
 
  const std::string getBizType() {
    return std::string("RcdLayOut");
  }
  virtual ~RcdLayout() {}

  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id());
    BOOST_FOREACH(int i, item_data_.fields()) {
      id_set.insert(i);
    }
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);
 private:
  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------
class Rcd4FriendsLayout : public Layout {
 public:
  Rcd4FriendsLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {

  }
  
  const std::string getBizType() {
    return std::string("Rcd4FriendsLayout");
  }
  virtual ~Rcd4FriendsLayout() {}
  
  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id());
    BOOST_FOREACH(int i, item_data_.fields()) {
      id_set.insert(i);
    }
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);
 private:
  //const static int kDispNameLength = 12;
  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

class Rcd4NewFriendsLayout : public Layout {
 public:
  Rcd4NewFriendsLayout(int uid, const RecommendItem& item_data):host_(uid), item_data_(item_data) {
  }
  virtual ~Rcd4NewFriendsLayout() {}
  
  const std::string getBizType() {
    return std::string("Rcd4NewFriendsLayout");
  }
  
  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id);
    BOOST_FOREACH(int i, item_data_.commons) {
      id_set.insert(i);
    }
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);
 private:
  //const static int kDispNameLength = 12;
  int host_;
  RecommendItem item_data_;
};

//-----------------------------------------------------------------------------------------------------------

class Rcd3NewFriendsLayout : public Layout {
 public:
  Rcd3NewFriendsLayout(int uid, const RecommendItem& item_data):host_(uid), item_data_(item_data) {
  }
  virtual ~Rcd3NewFriendsLayout() {}
  
  const std::string getBizType() {
    return std::string("Rcd3NewFriendsLayout");
  }
  
  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id);
    BOOST_FOREACH(int i, item_data_.commons) {
      id_set.insert(i);
    }
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);
 private:
  int host_;
  RecommendItem item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为blog连续阅读提供展示 */
class BlogLayout : public Layout {
public:
	BlogLayout(const Item& blog_recommend, const std::string& type):
		blog_recommend_(blog_recommend), type_(type) {
	}

    const std::string getBizType() {
      return std::string("BlogLayOut");
    }
	
	virtual std::string DoLayout(UserCacheMap*);
	
private:
	/* @brief 获取blog url
   * @param hostid 哪个用户分享的
   * @param blogid 日志id
   */
	std::string GetBlogUrl(int hostid, long blogid) {
		std::ostringstream oss;
		oss << "http://blog.renren.com/share/" 
			<< hostid << "/" << blogid << "/";
		return oss.str();
	}

	Item blog_recommend_;
	std::string type_;
};

//-----------------------------------------------------------------------------------------------------------

class VideoLayout : public Layout {
 public:
  VideoLayout(const std::string& t_url, 
              const std::string& title,
              long sid, 
              int uid):thrum_url_(t_url), title_(title), share_id_(sid), user_id_(uid) { 

  }
  
  const std::string getBizType() {
    return std::string("VideoLayOut");
  }
  std::set<int> UserIdSet() {
    return std::set<int>();
  }

  virtual std::string DoLayout(UserCacheMap*);

  std::string GetShareUrl() const {
    std::ostringstream oss;
    oss << "http://share.renren.com/share/" << user_id_ << "/" << share_id_;
    return oss.str();
  }

 private:
  std::string thrum_url_;
  std::string title_;
  long share_id_;
  int user_id_;
};

//-----------------------------------------------------------------------------------------------------------

class PageLayout : public Layout {
 public:
  PageLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {
  }
  const std::string getBizType() {
    return std::string("PageLayOut");
  }
  
  virtual ~PageLayout() {}

  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id());
		int size = item_data_.fields_size();
		for (int i = 1; i < size; ++i) {				//as to real size of common friends stored in the 0 position of fields, 
			id_set.insert(item_data_.fields(i)); 	//start with position 1
		}
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);
 private:
  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为video首页推荐提供展示 */
class RcdVideoLayout : public Layout {
public:
  RcdVideoLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {
  }
  const std::string getBizType() {
    return std::string("RcdVideoLayOut");
  }
  virtual ~RcdVideoLayout() {}

  virtual std::string DoLayout(UserCacheMap*);

private:
	/* @brief 获取video url
   * @param hostid 哪个用户分享的
   * @param shareid 视频id
   */
  std::string GetShareUrl(int hostid, long shareid) const {
    std::ostringstream oss;
    oss << "http://share.renren.com/share/" << hostid << "/" << shareid;
    return oss.str();
  }

  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为blog首页推荐提供展示 */
class RcdBlogLayout : public Layout {
public:
  RcdBlogLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {
  }
  const std::string getBizType() {
    return std::string("RcdBlogLayOut");
  }
  virtual ~RcdBlogLayout() {}

  virtual std::string DoLayout(UserCacheMap*);

private:
	/* @brief 获取blog url
   * @param hostid 哪个用户分享的
   * @param blogid 日志id
   */
	std::string GetBlogUrl(int hostid, long blogid) const {
		std::ostringstream oss;
		oss << "http://blog.renren.com/share/" 
			<< hostid << "/" << blogid << "/";
		return oss.str();
	}

  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为site首页推荐提供展示 */
class RcdSiteLayout: public Layout {
public:
  RcdSiteLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {
  }
  const std::string getBizType() {
    return std::string("RcdSiteLayOut");
  }
  virtual ~RcdSiteLayout() {}

  std::set<int> UserIdSet() {		//return id of minisite
    std::set<int> id_set;
    id_set.insert(item_data_.id());
    return id_set;
  }

  virtual std::string DoLayout(UserCacheMap*);

private:
  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为app首页推荐提供展示 */
class RcdAppLayout : public Layout {
 public:
  RcdAppLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {

  }
  const std::string getBizType() {
    return std::string("RcdAppLayOut");
  }
  virtual ~RcdAppLayout() {}

  std::set<int> UserIdSet() {
    std::set<int> id_set;
    BOOST_FOREACH(int i, item_data_.fields()) {
      id_set.insert(i);
    }
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);

 private:
	std::string CutName(const std::string& name) {
		int limit = 9;					//每个中文三个字节, 最多支持三个字的名字
		if ((int)name.size() > limit) {
			std::string tmp(name);
			tmp.resize(limit);
			return tmp;
		}
		return name;
	}

  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为forum首页推荐提供展示 */
class RcdForumLayout : public Layout {
 public:
  RcdForumLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {

  }

  const std::string getBizType() {
    return std::string("RcdForumLayout");
  }

  virtual ~RcdForumLayout() {}

  std::set<int> UserIdSet() {
    std::set<int> id_set;
    BOOST_FOREACH(int i, item_data_.fields()) {
      id_set.insert(i);
    }
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);

 private:
	std::string CutName(const std::string& name) {
		int limit = 9;					//每个中文三个字节, 最多支持三个字的名字
		if ((int)name.size() > limit) {
			std::string tmp(name);
			tmp.resize(limit);
			return tmp;
		}
		return name;
	}

  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 为desk-top首页推荐提供展示 */
class RcdDeskLayout : public Layout {
 public:
  RcdDeskLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {

  }
  const std::string getBizType() {
    return std::string("RcdDeskLayOut");
  }
  virtual ~RcdDeskLayout() {}

  std::set<int> UserIdSet() {
    std::set<int> id_set;
    BOOST_FOREACH(int i, item_data_.fields()) {
      id_set.insert(i);
    }
    return id_set;
 }
  virtual std::string DoLayout(UserCacheMap*);

 private:
	std::string CutName(const std::string& name) {
		int limit = 9;					//每个中文三个字节, 最多支持三个字的名字
		if ((int)name.size() > limit) {
			std::string tmp(name);
			tmp.resize(limit);
			return tmp;
		}
		return name;
	}

  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 依据不同的数据源创建分享页的Layout逻辑 */
class ShareLayout : public Layout {
 public:
  ShareLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {

  }
  const std::string getBizType() {
    return std::string("ShareLayOut");
  }
  virtual ~ShareLayout() {}

  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id());
    BOOST_FOREACH(int i, item_data_.fields()) {
      id_set.insert(i);
    }
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);
 private:
  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 依据不同的数据源创建分享页的好友推荐Layout逻辑 */
class ShareFriendsLayout : public Layout {
 public:
  ShareFriendsLayout(int uid, const RecommendItem& item_data):host_(uid), item_data_(item_data) {
  }
  virtual ~ShareFriendsLayout() {}
  
  const std::string getBizType() {
    return std::string("ShareFriendsLayout");
  }
  
  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id);
    BOOST_FOREACH(int i, item_data_.commons) {
      id_set.insert(i);
    }
    return id_set;
  }

  virtual std::string DoLayout(UserCacheMap*);

 private:
  int host_;
  RecommendItem item_data_;
};

//-----------------------------------------------------------------------------------------------------------

class SharePageLayout : public Layout {
 public:
  SharePageLayout(int uid, const Item& item_data):host_(uid), item_data_(item_data) {
  }
  const std::string getBizType() {
    return std::string("SharePageLayOut");
  }
  virtual ~SharePageLayout() {}

  std::set<int> UserIdSet() {
    std::set<int> id_set;
    id_set.insert(item_data_.id());
		int size = item_data_.fields_size();
		for (int i = 1; i < size; ++i) {				//as to real size of common friends stored in the 0 position of fields, 
			id_set.insert(item_data_.fields(i)); 	//start with position 1
		}
    return id_set;
  }
  virtual std::string DoLayout(UserCacheMap*);
 private:
  int host_;
  Item item_data_;
};

//-----------------------------------------------------------------------------------------------------------

/* @brief 依据不同的数据源创建不同的Layout逻辑 */
class LayoutFactory {
public:
	static LayoutPtr Create(int uid, const Item& item_data);
	static LayoutPtr Create(int uid, const Item& item_data, const std::string& biz);
};

}}
#endif

