/*
 * =====================================================================================
 *
 *       Filename:  index.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年04月11日 14时25分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "indexManager.h"
#include "indexFactory.h"
#include "ServiceI.h"
#include "IceExt/src/Channel.h"
#include "indexTimer.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "freshGroup.h"
#include "UserDemographicCache.h"
#include "indexConfig.h"
#include "db.h"
#include "indexLog.h"
#include "indexConfig.h"
#include "UserDemographicCache.h"
#include "indexUtil.h"
#include "ad/indexService/TIndex/src/targetingIndex.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include "client/linux/handler/exception_handler.h"
#include "ad/Degrade/src/Degrade.h"
#include "ad/indexService/lower_ad_send_speed/include/ad_send_speed_filter.h"
#include "DataService.h"

using namespace xce::ad;

bool MyCallback(const char *dump_path, const char *minidump_id, void *context, bool succeeded) {
	printf("got crash %s %s\n", dump_path, minidump_id);
	return true;
}

void MyUtil::initialize() 
{
	int ret = 0;
	MCE_INFO("BEGIN MyUtil::initialize");
	srand(time(NULL));
	static google_breakpad::ExceptionHandler eh(".", 0, MyCallback, 0, true);
	ServiceI& service = ServiceI::instance();
	TriggerLog::init("trigger", "../log/" + ServiceI::instance().getName() + "/trigger", "INFO");
	std::string source_db = service.getCommunicator()->getProperties()->getPropertyWithDefault("DbInstance", "ade_db");
	std::string config_db_source = service.getCommunicator()->getProperties()->getPropertyWithDefault("ConfigSource", "ad_st");
	DBSelector::instance().setAdDBSource(source_db);
	DBSelector::instance().setConfigDBSource(config_db_source);
	DBSelector::instance().outputDBConfig();

	ret = IndexConfig::instance().loadConfig();
	if(ret != 0)
	{
		MCE_WARN("IndexConfig::instance().init() FAILED");
		return;
	}

	AdSendSpeedLower::instance().Init(IndexConfig::instance().GetLowerDB()); 
	DegradeManager::instance().Register("interest");
	DegradeManager::instance().Register("ad_send_speed_lower");
	DegradeManager::instance().SetDBSource(config_db_source);
	ret = DegradeManager::instance().Init();
	if(ret < 0){
		MCE_INFO("DegradeManager Init Failed\n");
		return;
	}
	service.getAdapter()->add(&IndexManager::instance(), service.createIdentity("IdxSrv", ""));
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("IndexService.Mod");
	int idx = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("ServerIndex", -1);
	int interval = service.getCommunicator()->getProperties()->
		getPropertyAsIntWithDefault("IndexService.Interval",10);
	IndexConfig::instance().setServerIndex(idx);
	xce::serverstate::ServerStateSubscriber::instance().
		initialize("ControllerIndexService",&IndexManager::instance(),mod,interval,new EdmChannel());

	ret = IndexManager::instance().init();
	if(ret < 0)
	{
		MCE_WARN("IndexManager::instance().init FAILID");
	}
	MCE_INFO("FINISH MyUtil::initialize");
}

int IndexManager::init()
{
	int ret = 0;

	std::map<std::string, int> triggerInfo = IndexConfig::instance().getTriggerInfo();
	for(std::map<std::string, int>::iterator it = triggerInfo.begin(); it != triggerInfo.end(); ++it)
	{
		IndexPtr index = IndexFactory::createIndex(it->first);
		if(!index)
		{
			MCE_WARN("IndexFactory::createIndex return NULL, index name: " << it->first);
			return -1;
		} 

		index->init(it->first, it->second);
		addIndex(index);
	}

	MCE_INFO("Load AdPool from Data Server Start!");

	DataService::instance().Initialize("10.3.17.203",8033,50);
	DataService::instance().start();
	MCE_INFO("Load AdPool from Data Server End.");

	TaskManager::instance().execute(new InitTask());
	//init_loader_.start().detach();

	return ret;
}

int IndexManager::loadIndex()
{
	int ret = 0;	
	for(std::vector<IndexPtr>::iterator it = indexes_.begin(); it != indexes_.end(); ++it)
	{
		ret = (*it)->load();
		if(ret < 0)
			return -1;
	}

	return ret;	
}


void IndexManager::addIndex(IndexPtr index)
{
	indexes_.push_back(index);
}

IndexPtr IndexManager::getIndexByName(const std::string &name)
{
	for(std::vector<IndexPtr>::iterator it = indexes_.begin(); it != indexes_.end(); ++it)
	{
		if(name == (*it)->getName())
			return *it;
	}

	return NULL;
}

bool cmp(const GroupInfo &a, const GroupInfo &b)
{
	return a.ctr > b.ctr ? true : false;
}

Ice::Int IndexManager::trigger(const TriggerInputInfo& input, GroupDict& groups, const Ice::Current& cur){

	TriggerParameter   para;
  para.age_ = input.age;
  para.gender_ = input.gender;
  para.stage_ = input.stage;
  para.grade_ = input.grade;
  para.uid_ = input.uid;
  para.school_ = input.school;
  para.ipArea_ = input.ipArea;
  para.currentArea_ = input.currentArea;
  
  if(!input.company.empty()){
 	vector<string>  companys;
	boost::algorithm::split_regex(companys, input.company, boost::regex(","));
	ostringstream  usercompany;
	usercompany << input.company << "  user's companys:" << hex;
	for (vector<string>::iterator it = companys.begin(); it != companys.end(); ++it){   
		char* chTemp = NULL;
		para.companys_.push_back(strtoul(it->c_str(), &chTemp, 16));
		usercompany << strtoul(it->c_str(), &chTemp, 16) << " ";
	}    
   }
  para.zones_ = input.zones;
  para.useNew_ = true;
  return trigger(para, groups, cur);
}

Ice::Int IndexManager::trigger(const TriggerParameter& input, GroupDict& groups, const Ice::Current& cur)
{
  if(!init_ok_){
    MCE_INFO("IndexService is initializing!");
    return 0;
  }
  
  uint64_t begin_t = getMsec();

  trigger_log_.reset();
  trigger_log_.setInput(input);

  std::vector<Ice::Long>::const_iterator zit = input.zones_.begin();

  for(; zit != input.zones_.end(); ++zit)
  {
    long zone_id = *zit;
    std::vector<GroupInfo> gVec;

    IndexPtr tgIndex = getIndexByName("TargetingIndex");
    if(!tgIndex)
    {
      MCE_WARN("getTargetingIndex return NULL");
      trigger_log_.setAdNum(0);
      continue;
    }

    std::set<AdGroupPtr> result;
    int ret = tgIndex->trigger(input, zone_id, result);
    AdCloseManager::instance().FilterClosedAds(input.uid_, result);
    trigger_log_.setAdNum(result.size());

    uint64_t tg_t = getMsec();
    trigger_log_.setUseTime(tg_t - begin_t);

    if(ret < 0)
    {
      //MCE_WARN("TargetingIndex trigger failed");
      continue;
    }

    int small_widget_num = 0;

    for(std::set<AdGroupPtr>::iterator it = result.begin(); it != result.end(); ++it)
    {
      GroupInfo gInfo;
      AdGroupPtr group = *it;
      if(group->creative_id_ <= 0)
      {
        continue;
      }
      if (DegradeManager::instance().IsEnable("ad_send_speed_lower")){
          if (AdSendSpeedLower::instance().IsOffline(group)){ continue; }
      }

      gInfo.groupID = group->group_id_;
      gInfo.memberID = group->member_id_;
      gInfo.campaignID = group->campaign_id_;
      gInfo.creatives.push_back(group->creative_id_);
      gInfo.widgetID = group->widget_id_;
      gInfo.transType = group->trans_type_;
      gInfo.memberCategory = group->member_category_;
      gInfo.memberIndustry = group->member_industry_;
      gInfo.bidUnitID = group->bid_unit_id_;
      gInfo.triggerType = tgIndex->getType();
      gInfo.price = group->zone_price_[zone_id];
      gInfo.ctr = 0.0;

      gVec.push_back(gInfo);
      checkSmallWidget(gInfo, small_widget_num);
    }
    trigger_log_.setSmallWidgetNum(small_widget_num);

    select(zone_id, input, gVec, groups);	
    trigger_log_.setAdNumAfterSelect(groups[zone_id].size());
  }

  uint64_t end_t = getMsec();
  trigger_log_.setUseTime(end_t - begin_t);
  trigger_log_.output();

  return 0;
}

//void IndexManager::select(long zone_id, const TriggerParameter &input, std::vector<GroupInfo> &gVec, GroupDict& groups)
//=======
void IndexManager::Close(Ice::Int uid, Ice::Long creative_id, const Ice::Current& cur){
  
  MCE_DEBUG("IndexManager::close, UID: " << uid << ", creative_id: " << creative_id);
  AdCloseManager::instance().CloseAd(uid, creative_id);
  return;
}

void IndexManager::checkSmallWidget(const GroupInfo &info, int &small_widget_num)
{
  if(info.widgetID == 31 || info.widgetID == 33 || info.widgetID == 112){
    ++small_widget_num;
  }
}

void IndexManager::select(long zone_id, const TriggerParameter &input, std::vector<GroupInfo> &gVec, GroupDict& groups)
{
    unsigned config_ad_num, config_fresh_num;
		IndexConfig::instance().getAdNumByAdzone(zone_id, config_ad_num, config_fresh_num);
    int small_widget_num = 0;

    long NUOMI_MEMBER= 100000009136;
    long NUOMI_ADZONE = 100000000060;

    if(gVec.size() <= config_ad_num)
		{
			groups[zone_id] = gVec;
      trigger_log_.setAdNumAfterSelect(small_widget_num);
      return;
		}

		std::vector<GroupInfo> sortedGroups;

		std::map<long, double> freshIDs;
    std::vector<GroupInfo> nuomiGroups;
		std::multimap<double, GroupInfo, std::greater<double> > freshGroups;

    if(config_fresh_num > 0){
		  FreshAdGroupsCache::instance().getFreshGroupsByAdzone(zone_id, 1, freshIDs);
    }

		for(std::vector<GroupInfo>::iterator git = gVec.begin(); git != gVec.end(); ++git)
		{
      std::map<long, double>::iterator fid_it = freshIDs.find(git->groupID);
      if(fid_it == freshIDs.end())
			{
        if(zone_id == NUOMI_ADZONE && git->memberID == NUOMI_MEMBER){
          nuomiGroups.push_back(*git);
          continue;
        }

				calcCtr(zone_id, input, *git);	
				sortedGroups.push_back(*git);
			}
			else
			{
				freshGroups.insert(make_pair(fid_it->second, *git));
			}
		}
		
		sort(sortedGroups.begin(), sortedGroups.end(), cmp);

		size_t freshGroupNum = 0, oldGroupNum = 0;

    //choose new ads
      if(freshGroups.size() <= config_fresh_num){
        for(std::multimap<double, GroupInfo>::iterator fit = freshGroups.begin(); fit != freshGroups.end(); ++fit){
          groups[zone_id].push_back(fit->second);
          checkSmallWidget(fit->second, small_widget_num);
        }

        freshGroupNum = freshGroups.size();
      }
      else
      {
        std::map<long, int> fresh_member_adv_num;
        std::multimap<double, GroupInfo>::iterator fit = freshGroups.begin();
        size_t i = 0;

        for(; fit != freshGroups.end() &&  i < config_fresh_num; ++fit){
          if(fresh_member_adv_num[fit->second.memberID] == 0){
            groups[zone_id].push_back(fit->second);
            checkSmallWidget(fit->second, small_widget_num);
            fresh_member_adv_num[fit->second.memberID] += 1;
            ++i;
          }
        }

        freshGroupNum = i;
      }


    //zone 100000000060,NUOMI ad has high priority
    if(zone_id == NUOMI_ADZONE && nuomiGroups.size()){
      int idx = rand() % (nuomiGroups.size()); 
      groups[zone_id].push_back(nuomiGroups[idx]);
      checkSmallWidget(nuomiGroups[idx], small_widget_num);
      config_ad_num -= 1;
      MCE_DEBUG("NUOMI:" << nuomiGroups[idx].groupID << ", NUOMI_NUM:" << nuomiGroups.size());
    }else if(zone_id == NUOMI_ADZONE){
      MCE_DEBUG("NUOMI_NUM:" << nuomiGroups.size());
    }

		oldGroupNum = config_ad_num - freshGroupNum;
		
    std::map<long, int> game_member_adv_num;
    int game_total_num = 0;
    int config_game_total_num = IndexConfig::instance().getGameTotalAdNum(zone_id);
    int config_game_member_num = IndexConfig::instance().getGameMemberAdNum(zone_id);

    //MCE_DEBUG("config_game_total_num: " << config_game_total_num << ", config_game_member_num: " << config_game_member_num 
        //<< ", adzone: " << zone_id);
        
		for(size_t i = 0; i < oldGroupNum && i < sortedGroups.size(); ++i)
		{
      GroupInfo &gInfo = sortedGroups[i];
      if(gInfo.memberCategory == 9){
        int adv_num = game_member_adv_num[gInfo.memberID];
        if(adv_num >= config_game_member_num || game_total_num >= config_game_total_num){
          continue;
        }else{
          game_member_adv_num[gInfo.memberID] = adv_num + 1;
          groups[zone_id].push_back(gInfo);
          checkSmallWidget(gInfo, small_widget_num);
          ++game_total_num;
        }
      }else{
        groups[zone_id].push_back(gInfo);
        checkSmallWidget(gInfo, small_widget_num);
      }
		}

    trigger_log_.setAdNumAfterSelect(small_widget_num);
}

void IndexManager::calcCtr(long zone_id, const TriggerParameter &input, GroupInfo &gInfo)
{
  const int TYPE_CPC = 10;

	stringstream zone_name;
	zone_name << zone_id;

	std::pair<double, int > res = UserDemographicCache::instance().GetDemoCtr(zone_name.str(), 
					gInfo.memberID, gInfo.groupID, 1,  input.gender_, input.stage_, input.uid_);    

  /*此处的CTR为权重，相当于ECPM*/
  if(gInfo.transType == TYPE_CPC){
    gInfo.ctr = res.first * gInfo.price;
    //MCE_DEBUG("CPC_CTR_ECPM: " << res.first << ": " <<  gInfo.ctr << ":" << gInfo.price << ": " << gInfo.groupID);
  }else{
    gInfo.ctr = 0.001 * gInfo.price;
    //MCE_DEBUG("CPM_CTR_ECPM: " << res.first << ": " <<  gInfo.ctr << " :" << gInfo.groupID);
  }

}


//void IndexManager::InitLoader::run(){
void InitTask::handle(){

  int ret = 0;
	bool bUseDB = true;

	if(bUseDB){
		MCE_INFO("--------------------------[INIT] BEGIN AdPool::instance().load()--------------------------------");
		ret = AdPool::instance().load();
		if(ret < 0)
		{
			MCE_WARN("AdPool::instance().load() failed");
			return ;
		}
		MCE_INFO("--------------------------[INIT] END AdPool::instance().load()--------------------------------");
	}

  MCE_INFO("--------------------------[INIT] BEGIN IndexManager::instance().loadIndex()----------------------");
  ret = IndexManager::instance().loadIndex();
  if(ret < 0)
  {
    MCE_WARN("loadIndex failed");
    return ;
  }
  MCE_INFO("--------------------------[INIT] END IndexManager::instance().loadIndex()-------------------------");
  IndexManager::instance().initOK();

  MCE_INFO("--------------------------[INIT] BEGIN AdCloseManager::instance().init()-------------------------");
  AdCloseManager::instance().setAdValidTime(IndexConfig::instance().getAdCloseExpired());
  ret = AdCloseManager::instance().init();
  if(ret < 0)
  {
    MCE_WARN("AdCloseManager::instance().init() FAILED");
    return ;
  }
  MCE_INFO("--------------------------[INIT] END AdCloseManager::instance().init()-------------------------");
  MCE_INFO("--------------------------[INIT] BEGIN FreshAdGroupsCache::instance().Load()----------------------");
  FreshAdGroupsCache::instance().Load();
  MCE_INFO("--------------------------[INIT] END FreshAdGroupsCache::instance().Load()----------------------");
  MCE_INFO("--------------------------[INIT] BEGIN UserDemographicCache::instance().Load()----------------------");
  UserDemographicCache::instance().Load();
  MCE_INFO("--------------------------[INIT] END UserDemographicCache::instance().Load----------------------");
  IndexTimerPtr userDemoCacheTimer = new UserDemographicCacheTimer(&(UserDemographicCache::instance()));
  IndexTimerPtr configTimer = new ConfigTimer(&(IndexConfig::instance()));
  IndexTimerPtr lowerTimer = new AdSendSpeedLowerTimer(&(AdSendSpeedLower::instance()));
  IndexTimerManager::instance().add(userDemoCacheTimer);
  IndexTimerManager::instance().add(configTimer);
  IndexTimerManager::instance().add(lowerTimer);
  IndexTimerManager::instance().start().detach();
  MCE_INFO("--------------------------[INIT] ALL INIT TASK FINNISHED----------------------");
}
