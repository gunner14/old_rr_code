/*
 * UserFeedsI.cpp
 *
 *  Created on: Aug 31, 2011
 *      Author: happy, guanghe.ge@renren-inc.com
 */

#include <stdio.h>
#include <unistd.h>
#include "UserFeedsI.h"
#include "ServiceI.h"
#include "IceLogger.h"
#include "util/cpp/TimeCost.h"
#include "PartialQuickSort.h"
#include "util/cpp/InvokeMonitor.h"
#include <Monitor/client/service/ServiceMonitor.h>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace xce::feed;
using namespace xce::serverstate;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&UserFeedsI::instance(), service.createIdentity("M", ""));

	//加入向Controller注册
	int mod = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemCache.Mod", 0);
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedItemCache.Interval", 5);
	string ctrEndpoint = service.getCommunicator()->getProperties()->getPropertyWithDefault("FeedItemCache.ControllerEndpoint", "ControllerFeedItemCacheByUserid");
	ServerStateSubscriber::instance().initialize(ctrEndpoint, &UserFeedsI::instance(), mod, interval, new XceFeedControllerChannel());

	//统计调用
	//ServiceMonitorManager::instance().start();

	//进行加载/重整/设置有效
	UserFeedsI::instance().initialize();
}

UserFeedsI::UserFeedsI() {
	ServiceI& service = ServiceI::instance();
	Ice::PropertiesPtr props = service.getCommunicator()->getProperties();

	checker_limit_ = ITEM_COUNT_LIMIT;
	checker_interval_ = props->getPropertyAsIntWithDefault("Service." + service.getName() + ".CheckerInterval", 10);

	build_history_ = props->getPropertyAsIntWithDefault("Service." + service.getName() + ".BuildHistory", 1);//单位为天 15->1
	build_interval_ = props->getPropertyAsIntWithDefault("Service." + service.getName() + ".BuildInterval", 20);//单位为分钟

	checker_ = new CheckerTimerTask(fs_, checker_limit_);
	timer_.scheduleRepeated(checker_, IceUtil::Time::seconds(checker_interval_));
}

UserFeedsI::~UserFeedsI() {
}

void UserFeedsI::initialize() {
	//直接放进task进行加载，让服务迅速启动
	MCE_DEBUG("UserFeedsI::intialize(), build_history_: " << build_history_ << " build_interval_: " << build_interval_);

	TaskManager::instance().execute(new BuildUserMapTask(build_history_, build_interval_));
}

void UserFeedsI::put(const FeedMeta& fm, const ::Ice::Current& current) {
	if (0 >= fm.userid) {
		return;
	}
	ostringstream oss;
	oss << " id: " << fm.userid << " feedid: " << fm.feedid << " type: " << fm.type << " time: " << fm.time;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	try {
		fs_.put(fm.userid, fm);
	} catch (...) {
		MCE_WARN("UserFeedsI::put Exception, id: " << fm.userid);
	}
}

FeedMetaSeq UserFeedsI::get(const IntSeq& userids, ::Ice::Int begin, ::Ice::Int limit, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userids.size: " << userids.size() << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	try {
		return getSegment(fs_.gets(userids, begin + limit), userids, begin, limit);
	} catch (std::exception& e) {
		MCE_WARN("UserFeedsI::get Exception! " << e.what());
	} catch (...) {
		MCE_WARN("UserFeedsI::get Exception, userids.size: " << userids.size());
	}

	return FeedMetaSeq();
}

FeedMetaSeq UserFeedsI::getByTypes(const IntSeq& userids, ::Ice::Int begin, ::Ice::Int limit, const IntSeq& types, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userids.size: " << userids.size() << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	try {
		if(types.empty())
			return getSegment(fs_.gets(userids, begin + limit), userids, begin, limit);
		return getSegment(fs_.gets_with_filter(userids, TypesFilter(types), IdsFilter(std::vector<string>()), begin + limit), userids, begin, limit);
	} catch (std::exception& e) {
		MCE_WARN("UserFeedsI::getByTypes Exception! " << e.what());
	} catch (...) {
		MCE_WARN("UserFeedsI::getByTypes Exception!");
	}
	return FeedMetaSeq();
}

FeedMetaSeq UserFeedsI::getWithFilter(const IntSeq& userids, ::Ice::Int begin, ::Ice::Int limit, const StrSeq& filter, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userids.size: " << userids.size() << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	try {
		if(filter.empty())
			return getSegment(fs_.gets(userids, begin + limit), userids, begin, limit);
		return getSegment(fs_.gets_with_filter(userids, TypesFilter(std::vector<int>()), IdsFilter(filter), begin + limit), userids, begin, limit);
	} catch (std::exception& e) {
		MCE_WARN("UserFeedsI::getWithFilter Exception! " << e.what());
	} catch (...) {
		MCE_WARN("UserFeedsI::getWithFilter Exception!");
	}
	return FeedMetaSeq();
}

FeedMetaSeq UserFeedsI::getByTypesWithFilter(const IntSeq& userids, ::Ice::Int begin, ::Ice::Int limit, 
		const IntSeq& types, const StrSeq& filter, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userids.size: " << userids.size() << " begin: " << begin << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
	try {
		if(filter.empty() && types.empty())
			return getSegment(fs_.gets(userids, begin + limit), userids, begin, limit);
		return getSegment(fs_.gets_with_filter(userids, TypesFilter(types), IdsFilter(filter), begin + limit), userids, begin, limit);
	} catch (std::exception& e) {
		MCE_WARN("UserFeedsI::getByTypesWithFilter Exception! " << e.what());
	} catch (...) {
		MCE_WARN("UserFeedsI::getByTypesWithFilter Exception!");
	}
	return FeedMetaSeq();
}

FeedMetaSeq UserFeedsI::getLessThanFeedidByTypesWithFilter(const IntSeq& userids, ::Ice::Long feedid, ::Ice::Int limit, const IntSeq& types, const StrSeq& filter, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userids.size: " << userids.size() << " limit: " << limit << " feedid: " << feedid;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
	try {
		return getSegment(fs_.gets_less_feedid_with_filter(userids, TypesFilter(types), IdsFilter(filter), feedid, limit), userids, 0, limit);
	} catch (std::exception& e) {
		MCE_WARN("UserFeedsI::getLessThanFeedidByTypesWithFilter Exception! " << e.what());
	} catch (...) {
		MCE_WARN("UserFeedsI::getLessThanFeedidByTypesWithFilter Exception!");
	}

	return FeedMetaSeq();
}

FeedMetaSeq UserFeedsI::getBiggerThanFeedidByTypesWithFilter(const IntSeq& userids, ::Ice::Long feedid, ::Ice::Int limit, const IntSeq& types, const StrSeq& filter, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userids.size: " << userids.size() << " limit: " << limit << " feedid: " << feedid;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	try {
		return getSegment(fs_.gets_bigger_feedid_with_filter(userids, TypesFilter(types), IdsFilter(filter), feedid, limit), userids, 0, limit);
	} catch (std::exception& e) {
		MCE_WARN("UserFeedsI::getBiggerThanFeedidByTypesWithFilter Exception! " << e.what());
	} catch (...) {
		MCE_WARN("UserFeedsI::getBiggerThanFeedidByTypesWithFilter Exception!");
	}

	return FeedMetaSeq();
}

void UserFeedsI::setLimit(::Ice::Long limit, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " limit: " << limit;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	checker_->setLimit(limit);
}

::Ice::Long UserFeedsI::getTotalCount(const ::Ice::Current& current) {
	ostringstream oss;
	oss << " ";
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	return fs_.total_count();
}

::Ice::Int UserFeedsI::getItemCount(const IntSeq& userIds, const ::Ice::Current& current) {
	ostringstream oss;
	oss << " userId.size: " << userIds.size();
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	int count = 0;
	for(IntSeq::const_iterator it = userIds.begin();
			it != userIds.end(); ++it) {
		count += fs_.item_count(*it);
	}

	return count;
}

void UserFeedsI::check_order() {
	ostringstream oss;
	oss << "UserFeedsI::check_order() ";
	TimeCost tm = TimeCost::create(oss.str());

	MCE_INFO("sizeof(FeedMeta): " << sizeof(FeedMeta) << " sizeof(FeedMetaData): " << sizeof(FeedMetaData) 
			<< " sizeof(MutexData): " << sizeof(MutexData<FeedMetaData>) << " sizeof(feedstream): " << sizeof(FeedStream<FeedMetaData>));

	fs_.check_order();
}

bool UserFeedsI::put_without_add_count(const FeedMeta& meta) {
	
	return fs_.put(meta.userid, meta, false);
}

bool UserFeedsI::puts_without_add_count(int mod, const std::vector<FeedMetaData>& feeds) {
	return fs_.puts(mod, feeds, false);
}

void UserFeedsI::add_item_count(int count) {
	ostringstream oss;
	oss << "UserFeedsI::add_item_count, count: " << count;
	TimeCost tm = TimeCost::create(oss.str());

	fs_.add_count(count);
}

void UserFeedsI::deleteFeed(int userid, long feedid, const ::Ice::Current & current) {
	if (0 >= userid) {
		return;
	}
	ostringstream oss;
	oss << " id: " << userid << " feedid: " << feedid;
	InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);

	try {
		fs_.del(userid, feedid);
	} catch (...) {
		MCE_WARN("UserFeedsI::put Exception, id: " << userid);
	}
}

FeedMetaSeq UserFeedsI::getSegment(const std::vector<std::vector<FeedMetaData> > & data, const IntSeq& userids, int begin, int limit) {
	
	int sum_size = 0;
	for(size_t i = 0; i < data.size(); ++i) {
		sum_size+=data[i].size();
	}

	FeedMetaSeq seq;
	seq.reserve(sum_size);

	for(std::vector<std::vector<FeedMetaData> >::const_iterator it = data.begin();
			it != data.end(); ++it) {
		for(std::vector<FeedMetaData>::const_iterator id = it->begin();
				id != it->end(); ++id) {
			seq.push_back(id->createFrom(id->userid));
		}
	}

	if(begin >= (int)seq.size())
		return FeedMetaSeq();
	if(begin + limit > (int)seq.size())
		limit = seq.size() - begin;

	// option 1
	//std::partial_sort(seq.begin(), seq.begin() + (begin + limit), seq.end(), FeedMetaComp());
	//return ::Feed::SeqFeedMeta(seq.begin() + begin, seq.begin() + (begin + limit));

	// option 2
	//partial_quick_sort(seq, begin, limit, FeedMetaComp());
	//return ::Feed::SeqFeedMeta(seq.begin() + begin, seq.begin() + (begin + limit));

	// option 3
	FeedMetaSeq::iterator it = seq.begin();
	std::nth_element(it, it + (begin + limit) - 1, seq.end(), FeedMetaCompare());
	if(limit > 1)
		std::nth_element(it, it + begin, it + (begin + limit) - 1, FeedMetaCompare());
	std::sort(it + begin, it + begin + limit, FeedMetaCompare());

	return FeedMetaSeq(it +begin, it + (begin+limit));
}

int UserFeedsI::getItemCountByTypes(const MyUtil::IntSeq& userids,const MyUtil::IntSeq& types,const ::Ice::Current& current)
{
  set<int>mysettypes;
  for(vector<int>::const_iterator i=types.begin();i!=types.end();i++)
  {
    mysettypes.insert(*i);
  }
  ostringstream oss;
  oss << " userids.size: " << userids.size();
  InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
  //MCE_FATAL("getItemCountByTypes function:start");
  try {
      return fs_.getItemCountByTypes(userids,mysettypes);
  } catch (std::exception& e) {
     MCE_WARN("UserFeedsI::getItemCountByTypes Exception!" << e.what());
  } catch (...) {
     MCE_WARN("UserFeedsI::getItemCountByTypes Exception");
  } 
  return -1;
}

MyUtil::Int2IntMap UserFeedsI::getItemCountMapByTypes(const MyUtil::IntSeq& userids, const MyUtil::IntSeq& types,const ::Ice::Current& current)
{
    set<int>mysettypes;
    for(vector<int>::const_iterator i=types.begin();i!=types.end();i++)
     { 
        mysettypes.insert(*i);
    }
    ostringstream oss;
    oss << " userId.size: " << userids.size() ;
    InvokeClient tm = InvokeClient::create(current, oss.str(), InvokeClient::INVOKE_LEVEL_INFO);
    //MCE_FATAL("begin getItemCountMapByTypes function");
    MyUtil::Int2IntMap result;
                
    fs_.getItemCountMapByTypes(userids,mysettypes,result);
                    
    return result;
}




void CheckerTimerTask :: runTimerTask() {
	MCE_DEBUG("CheckerTimerTask::runTimerTask user: " << fs_.user_count()
			<< " total_count: " << fs_.total_count() << " limit: " << limit());

	while(fs_.total_count() > limit()) {
		FeedMetaData data;
		if(fs_.get_min(data)) {
			data.time += 24 * 60 * 60; //一次释放一天的数据，而不是一个一个释放，提高逐出效率
			long pre_total = fs_.total_count();
			fs_.free(data);
			MCE_INFO("CheckerTimeTask[evict the Item] timeline: " << data.time << " pre_total: " << pre_total <<
					" after free total: " << fs_.total_count());
		} else {
			MCE_WARN("can not get min");
		}
	}
}

void BuildUserMapTask::handle() {

	int history_time = time_now_ - history_*24*60*60;
	int interval = interval_*60;

	MCE_DEBUG("BuildUserMapTask::handle() is starting ..., time_now_: " << time_now_
			<< " history_: " << history_ << " history_time: " << history_time
			<< " interval_: " << interval_ << " interval: " << interval);

	std::vector<BuildUserMapThreadPtr> threads;
	threads.reserve(DB_TABLE_COUNT);

	int cpu_size = (int)(sysconf(_SC_NPROCESSORS_CONF) * 0.1); //1.5->0.1
	MCE_INFO("machine cpu size: " << cpu_size);

	for (int index = 0; index < DB_TABLE_COUNT; index++) {
		BuildUserMapThreadPtr thread = new BuildUserMapThread(history_time, interval, index, time_now_, cpu_size);
		threads.push_back(thread);
		thread->start();
	}

	for (int index = 0; index < DB_TABLE_COUNT; index++) {
		threads[index]->getThreadControl().join();
	}

	//重整
	UserFeedsI::instance().check_order();

	//加载完成，设置可用状态
	ServerStateSubscriber::instance().setStatus(1);
}

void BuildUserMapThread::run() {
	MCE_DEBUG("BuildUserMapThread " << index_ << " is starting ..., history: " << history_
			<< " interval_: " << interval_);

	for (int begin = time_now_, end = begin - interval_; begin >= history_; begin = end, end = begin - interval_) {
		ostringstream oss;
		oss << "BuildUserMapThread " << index_ << " begin: " << begin << " end: " << end;
		TimeCost tm = TimeCost::create(oss.str());

		int sleep_second = 0;
		while (run_count() > cpu_count_) {
			sleep_second ++;
			sleep(1);
		}

		if (sleep_second > 0) {
			MCE_DEBUG("BuildUserMapThread sleep count: " << sleep_second);
		}

		run_count_inc();

		std::map<int, std::vector<FeedMeta> > data = PersistenceHelper::read(index_, begin, end) -> get_metas();

		int count = 0;
		for (std::map<int, std::vector<FeedMeta> >::const_iterator it = data.begin();
				it != data.end(); ++it) {
			
			for(std::vector<FeedMeta>::const_iterator ir = it->second.begin();
					ir != it->second.end(); ++ir) {
				if (UserFeedsI::instance().put_without_add_count(*ir) ) {
					count ++;
				}
			}
		}

		UserFeedsI::instance().add_item_count(count);

		run_count_dec();
	}
}
