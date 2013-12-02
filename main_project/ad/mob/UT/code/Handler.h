/*
 *  Handler.h
 *
 *   Created on: 2012-3-30
 *       Author: yongjun.yin
 *  Description: 定义各种业务逻辑细节及业务逻辑流程控制
 */

#ifndef __AD_MOB_HANDLER__
#define __AD_MOB_HANDLER__

#include "AdMob.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "FeedMemcProxy/client/user_profile_client.h"
#include "AdUserCacheLoaderAdapter.h"
#include <map>

#include "Struct.h"

namespace xce{
namespace ad{

//全局参数
struct HandlerPara {
  enum HandleFlagType { UNHANDLED=0,HANDLED_BY_USERBIND=1,HANDLED_BY_ROTATE=2,HANDLED_BY_BRAND=3,HANDLED_BY_SELF=4 };
  explicit HandlerPara( const AdMobReqPara& request ) : request_(request), selected_group_type_(UNKNOWN_AD), handle_flag_( UNHANDLED ) {
    response_.adgroupId = -1;
		selected_creative_id_ = -1;
  }
  explicit HandlerPara() : selected_group_type_(UNKNOWN_AD), handle_flag_( UNHANDLED ) {
    response_.adgroupId = -1;
		selected_creative_id_ = -1;
  }
  inline void set_seleted_group(Ice::Long group_id, AdGroupPtr group, AdType type, HandleFlagType handle_flag);
  
  UserProfile userProfile_;	  //用户信息
  AdMobReqPara request_;	  //3G请求
  AdMobResPara response_;	  //3G返回
  
  //选取结果相关成员变量
  AdGroupPtr selected_group_ptr_;	//选到的广告指针
  Ice::Long selected_creative_id_;           //选到的creative_id
  AdType selected_group_type_;		//广告的类型 0品牌 1自助 -1未知
  HandleFlagType handle_flag_;		//处理标记

  //Log相关成员变量
  string pv_log_;		   //pv log
  string click_log_;	   //click log
  string cpm_charge_log_;  //cpm charge
  string cpc_charge_log_;  //cpc charge
};

//Handler接口
class BaseHandler : public IceUtil::Shared {
public:
  virtual void handle(HandlerPara& para) {}
  virtual ~BaseHandler() {}
};

typedef IceUtil::Handle<BaseHandler> HandlerPtr;

//参数检查模块 负责参数有效性检查 用户信息获取
class ParaCheckHandler : public BaseHandler {
public:
  void handle(HandlerPara& para);
private:
  inline void CheckRequestPara(AdMobReqPara& request);
  inline void PrintPara(AdMobReqPara& request);
  inline void PrintUserProfile(UserProfile& profile);
	static bool IsToReplace(char ch);
};

//客户截屏模块
class UserBindHandler : public BaseHandler {
public:
  void handle(HandlerPara& para);
};

//广告选取接口
class Selector {
public:
  virtual ~Selector(){}
  virtual set<AdGroupPtr> GetAds(HandlerPara& para, InvertedIndexType type);
  virtual void RandomSelect(const set<AdGroupPtr>& groups, AdGroupPtr& result);
  virtual void FilterGroups(set<AdGroupPtr>& selected, const vector<Ice::Long>& last_selected);
  virtual void FilterGroups(set<AdGroupPtr>& selected, const set<Ice::Long>& last_selected);
};

//轮播广告选取
class BrandRotateHandler : public BaseHandler, public Selector {
public:
  void handle(HandlerPara& para);
};

//品牌Cpm、Cpc广告选取模块
class BrandCpmCpcHandler : public BaseHandler, public Selector {
public:
  void handle(HandlerPara& para);
};

//中小广告选取模块
class SelfHandler : public BaseHandler, public Selector {
public:
  void handle(HandlerPara& para);
};

//日志打印模块 负责PV_Log的打印
class LogHandler : public BaseHandler {
public:
  void handle(HandlerPara& para);
  inline void BuildPvLog(HandlerPara& para);
  inline void BuildClickLog(HandlerPara& para);
  inline void BuildCpmChargeLog(HandlerPara& para);
  inline void BuildCpcChargeLog(HandlerPara& para);
private:
  inline void BuildDefault3GLog(HandlerPara& para, string& log);
};

//格式化模块 负责将一些后续的结构体数据填充工作
class FormatHandler : public BaseHandler {
public:
	FormatHandler();
	void handle(HandlerPara& para);
  bool ParseWidget(const string& json_str, map<string, string>& json_map);
  void UrlEncode(const string& input, string& output);
	string ClickUrlHead;
};

//测试模块 负责将候选广告格式化为应用测试回传结构
class TestHandler : public BaseHandler {
public:
  void handle(HandlerPara& para);
  TestHandler() {
    imageWidthSeq_.push_back(960);
    imageWidthSeq_.push_back(640);
    imageWidthSeq_.push_back(540);
    imageWidthSeq_.push_back(480);
    imageWidthSeq_.push_back(320);
    imageWidthSeq_.push_back(0);
    sort(imageWidthSeq_.begin(), imageWidthSeq_.end());  
  }
private:
  inline void ImageSelect(const AdMobReqPara& request, int& imageSize, int& imageRealSize, bool& needCompress);
  vector<int> imageWidthSeq_;  //图片大小seq
  map<int, int> test_map_;	   //3G测试需要
};

//广告Handler 集成特定广告位的处理逻辑
class ZoneHandler {
public:
  ZoneHandler();
  virtual ~ZoneHandler(){}
  virtual void handle( HandlerPara& para );
protected:
  vector<HandlerPtr> handler_list_;
};

//typedef IceUtil::Handle<ZoneHandler> ZoneHandlerPtr;
typedef ZoneHandler* ZoneHandlerPtr;

class ZoneTopHandler : public ZoneHandler , public MyUtil::Singleton<ZoneTopHandler> {
public:
  //void handle( HandlerPara& para );
};

class ZoneBottomHandler : public ZoneHandler , public MyUtil::Singleton<ZoneBottomHandler> {
public:
  //void handle( HandlerPara& para );
};

class ZoneTestHandler : public ZoneHandler , public MyUtil::Singleton<ZoneTestHandler> {
public:
  ZoneTestHandler();
  //void handle( HandlerPara& para );
};

class ZoneHandlerFactory {
public:
	static ZoneHandlerPtr GetZoneHandler( const Ice::Long zone_id ) {
		if ( MOB_ZONE_TOP == zone_id ) {
			return &ZoneTopHandler::instance();
		} else if ( MOB_ZONE_BOTTOM == zone_id ) {
			return &ZoneBottomHandler::instance();
		} else if ( MOB_ZONE_TEST == zone_id ) {
			return &ZoneTestHandler::instance();
		} else {
			return &ZoneTopHandler::instance();
		}
	}
};



}
}
#endif
