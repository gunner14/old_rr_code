#include "OnlineBitmapManagerI.h"
#include "ServiceI.h"
#include "OnlineCenterAdapter.h"
#include "XmppTools.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace talk::onlinebitmap;
using namespace com::xiaonei::talk::common;
using namespace ::talk::online;
using namespace MyUtil;
using namespace ::talk::online::adapter;
using namespace com::xiaonei::talk::util;

static int g_reload_count = 0;

class ReloadBitmapTimer : public MyUtil::Timer {
public:
  ReloadBitmapTimer(int delay, int interval) : Timer(delay), interval_(interval){}
  int interval_;
  static int day_;
  static const int sethour = 3;//24小时计时法，凌晨3点开始同步
  virtual void handle() {
    // TODO : 计数同步

    MCE_INFO("ReloadBitmapTimer::handle() started. interval_ : " << interval_);

    time_t now;
    struct tm * timenow = NULL;
    
    time(&now);
    timenow = localtime(&now);
    if(interval_ != 100)//间隔为100时不进行判断，进行强制同步
    {
    	if(timenow -> tm_hour != sethour)
    	{
		MCE_INFO("ReloadBitmapTimer::handle() sethour is not arrived return.");
		return;
	}
    	if(timenow -> tm_mday == day_)
	{
		MCE_INFO("ReloadBitmapTimer::handle() today we have synchronized return.");
		return;
	}
    }
    day_ = timenow -> tm_mday;//记录下同步的日期，今天不再进行同步了

    ++ g_reload_count;

    for (int i = 0; i < kUserPoolCount; ++i) {
      MCE_DEBUG("ReloadBitmapByUserPool : " << i);
			try{
      OnlineCenterAdapter::instance().ReloadBitmapByUserPool(i, false);
			}
			catch(Ice::Exception& e){
				MCE_WARN("OnliceBitmapManager::classReloadBitmapTimer-->OnlineCenterAdapter::ReloadBitmapByUserPool-->err" << e);
				}
      //if (i % 100 / 10 == 9) {
      //  MCE_INFO("ReloadBitmapByUserPool : " << i);
      //}
      if (interval_ > 0) {
        usleep(interval_);
      }
    }
    MCE_INFO("ReloadBitmapTimer::handle() complete.");
  }
};

int ReloadBitmapTimer::day_ = 0;

//为了查找服务调用highload的问题，加入输出调用者IP的log
string GetIp(const Ice::Current& current){
  if(!current.con){
    return "";
  }
  string ip = current.con->toString();
  size_t found = ip.find('\n');
  if (found == string::npos) {
    return ip;
  }
  string res ( ip.begin() + found + 1,ip.end());
  return res;
}



void MyUtil::initialize(){
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&OnlineBitmapManagerI::instance(), service.createIdentity("M", ""));
  OnlineBitmapManagerI::instance().init();

  // 每30分钟尝试重新加载一次bitmap，只有新的一天的3:00am才会进行同步
  MCE_INFO("MyUtil::initialize()::settask reload bitmap every 30min");
  TaskManager::instance().scheduleRepeated(new ReloadBitmapTimer(60 * 1000 * 30, 200));

  // 启动时加载一次bitmap
  MCE_INFO("MyUtil::initialize()::settask reload bitmap for the first time");
  TaskManager::instance().schedule(new ReloadBitmapTimer(5 * 1000, 100));

	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
	xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerOnlineBitmap", &OnlineBitmapManagerI::instance(), mod, interval, new XceFeedControllerChannel());
	MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

}
 
void OnlineBitmapManagerI::init(){
  MCE_DEBUG("OnlineBitmapManagerI::init");
  if (bitmap_) {
    delete bitmap_;
    bitmap_ = NULL;
  }

  bitmap_ = new char[BITMAP_SIZE];
  if (!bitmap_) {
    MCE_WARN("OnlineBitmapManagerI::init() new bitmap error!");
    return;
  }

  memset(bitmap_, 0, BITMAP_SIZE);
	int cluster = -1;
	try{
		cluster = OnlineCenterAdapter::instance().getCluster();
	}catch(Ice::Exception& e){
		MCE_WARN("OnlineBitmapManagerI::init-->OnlineCenterAdapter::getCluster-->err" << e);
	}
  if (cluster <= 0) {
    cluster = 1;
  }

  MCE_DEBUG("OnlineBitmapManagerI:: ---> OnlineCenter cluster = " << cluster);
}

void OnlineBitmapManagerI::Reload(const Ice::Current&) {
  MCE_INFO("OnlineBitmapManagerI::Reload()");
  TaskManager::instance().schedule(new ReloadBitmapTimer(5 * 1000, 1000));
}

void OnlineBitmapManagerI::SyncBitmap(const SyncBitmapDataPtr & sync_data, const Ice::Current& cur) {

  if (sync_data->userPoolIndex >= 0) {
    if (g_reload_count == 1) {
      // 第一次从 OnlineCenter 加载, 不需要置0
      UserOnlineTypePtr p = 0;
      for(size_t i = 0; i < sync_data->stateSeq.size(); ++i) {
        p = sync_data->stateSeq[i];
        bitmap_[p->userId] = p->onlineType;
      }
    } else if (g_reload_count > 1) {
      // 定时从 OnlineCenter 同步的情况
      int pos = sync_data->userPoolIndex;

      for(size_t i = 0; i < sync_data->stateSeq.size(); ++i) {
        UserOnlineTypePtr next = sync_data->stateSeq[i];
        while(pos < next->userId) {
          if (bitmap_[pos] != 0) {
            MCE_WARN("batch sync mismatch (should be 0) uid = " << pos);
          }
          //MCE_DEBUG("batch sync offline user = " << pos);
          bitmap_[pos] = 0;
          pos += kUserPoolCount;
        }
        //MCE_DEBUG("batch sync online user = " << next->userId);
        bitmap_[next->userId] = next->onlineType;
				//if(next->userId % 10 == 2 || next->userId % 10 == 3){
				//	MCE_INFO("batch sync user : " << next->userId << '-' << (unsigned short)next->onlineType);
				//}
        if (pos != next->userId) {
          MCE_WARN("batch sync error - pos != userId : " << pos << "," << next->userId);
        }
        pos += kUserPoolCount;
      }

      pos += kUserPoolCount;
      while(pos > 0) {
        if (bitmap_[pos] != 0) {
          MCE_WARN("2 batch sync mismatch (should be 0) uid = " << pos);
        }
        //MCE_DEBUG("2 batch sync offline user = " << pos);
        bitmap_[pos] = 0;
        pos += kUserPoolCount;
      }
    }
    if (sync_data->userPoolIndex % 100 / 10 == 9) {
      //MCE_INFO("batch sync user pool " << sync_data->userPoolIndex << " finish@" << g_reload_count);
    }
  } else {
    for(size_t i = 0; i < sync_data->stateSeq.size(); ++i) {
      UserOnlineTypePtr p = sync_data->stateSeq[i];
      if (p->userId > 0) {
        if (p->userId % 10 == 2 || p->userId % 10 == 3) {
          //MCE_INFO("single sync user : " << p->userId << '-' << (unsigned short)p->onlineType);
        }
        bitmap_[p->userId] = p->onlineType;
      }
    }
  }
}

void OnlineBitmapManagerI::setUserStatByOnlineType(const UserOnlineTypePtr& onlineTypePtr, const Ice::Current& cur){
  unsigned int userId = (unsigned int)(onlineTypePtr->userId);
  int onlineType = onlineTypePtr->onlineType;
  if (userId % 100 == 93) {
    //MCE_INFO("setUserStatByOnlineTyp : " << userId << " = " << (int)onlineType);
  }
  bitmap_[userId] = onlineType;
}

void OnlineBitmapManagerI::load(const UserOnlineTypeSeq& onlineTypeSeq, const Ice::Current&){
  for(UserOnlineTypeSeq::const_iterator it = onlineTypeSeq.begin(); it != onlineTypeSeq.end(); ++it){
    setUserStatByOnlineType((*it));
  }
}

UserOnlineTypePtr OnlineBitmapManagerI::getUserType(Ice::Int userId, const Ice::Current& ic){
  string address = com::xiaonei::talk::util::GetRemoteAdress(ic);
  string local = "0.0.0.0";
  TimeStatN ts;
  ostringstream os;
  if(address != local) {
    os << "OnlineBitmapManagerI::getUserType";
  } 
  UserOnlineTypePtr info;
  info = new UserOnlineType();
  info->userId = userId;
  info->onlineType = (bitmap_[(unsigned int)(userId)] & 14);
  if(userId % 100 == 9 && GetIp(ic)!="" ){
      MCE_INFO("OnlineBitmapManagerI::getUserType: " << userId << " = " << (int)info->onlineType << " invoke ip = " << GetIp(ic));
  }
  if (address != local) {
    FunStatManager::instance().StatPlus(ic,os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  }
  return info;
}

UserOnlineTypeSeq OnlineBitmapManagerI::getUserTypeByIdSeq(const MyUtil::IntSeq& ids, const Ice::Current& ic){
  TimeStatN ts;
  ostringstream os;
  os << "OnlineBitmapManagerI::getUserTypeByIdSeq";
  UserOnlineTypeSeq seq;
  if (ids.size() % 100 == 9 && GetIp(ic)!="") {
    MCE_INFO("getUserTypeByIdSeq : " << ids.size() <<  " invoke ip = " << GetIp(ic));
  }
  for(IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it){
    UserOnlineTypePtr info = getUserType(*it);
    seq.push_back(info);
  }
  FunStatManager::instance().StatPlus(ic,os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  return seq;
}

UserOnlineTypeSeq OnlineBitmapManagerI::getOnlineUsers(const MyUtil::IntSeq& ids, const Ice::Current& ic){
  TimeStatN ts;
  ostringstream os;
  os << "OnlineBitmapManagerI::getOnlineUsers";
  UserOnlineTypeSeq seq;

  if (ids.size() % 100 == 9) {
    if( GetIp(ic) ==""){
	MCE_INFO("getOnlineUsers : " << ids.size());}
    else
	MCE_INFO("getOnlineUsers : " << ids.size() << " invoke ip = " << GetIp(ic));
  }  

  for(IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it) {
    UserOnlineTypePtr info = getUserType(*it);
    if (info->onlineType) {
      seq.push_back(info);
    }
  }
  FunStatManager::instance().StatPlus(ic,os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  return seq;
}
UserOnlineTypePtr OnlineBitmapManagerI::getUserStat(Ice::Int userId, const Ice::Current& ic){
  TimeStatN ts;
  ostringstream os;
  os << "OnlineBitmapManagerI::getUserStat";
	UserOnlineTypePtr info;
	info = new UserOnlineType();
	info->userId = userId;
	info->onlineType = (bitmap_[(unsigned int)(userId)] >> 4);
	if(userId % 100 == 9 &&  GetIp(ic)!=""){
		MCE_INFO("getUserStat : " << userId << " = " << (int)info->onlineType << " invoke ip = " << GetIp(ic));
	}
  FunStatManager::instance().StatPlus(ic,os.str(), ts.getTime(), (ts.getTime() >= 250.0));
	return info;
}

UserOnlineTypeSeq OnlineBitmapManagerI::getUserStatByIdSeq(const MyUtil::IntSeq& ids, const Ice::Current& ic){
  TimeStatN ts;
  ostringstream os;
  os << "OnlineBitmapManagerI::getUserStatByIdSeq";
  UserOnlineTypeSeq seq;
  if (ids.size() % 100 == 9 && GetIp(ic) != "" ) {
    MCE_INFO("getUserStatByIdSeq : " << ids.size() << " invoke ip = " << GetIp(ic));
  }
  for(IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it){
    UserOnlineTypePtr info = getUserStat(*it);
    seq.push_back(info);
  }
  FunStatManager::instance().StatPlus(ic,os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  return seq;
}

Ice::Int OnlineBitmapManagerI::getOnlineCount(const MyUtil::IntSeq& ids, const Ice::Current& ic){
  TimeStatN ts;
  ostringstream os;
  os << "OnlineBitmapManagerI::getOnlineCount";
  Ice::Int count = 0;
  for(IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it) {
    if (bitmap_[(unsigned int)(*it)] & 14) {
      ++ count;
    }
  }

  if (ids.size() % 10 == 9 && GetIp(ic) != "" ) {
    MCE_INFO("getOnlineCount : " << count << '/' << ids.size() << " invoke ip = " << GetIp(ic));
  }
  FunStatManager::instance().StatPlus(ic,os.str(), ts.getTime(), (ts.getTime() >= 250.0));
  return count;
}

