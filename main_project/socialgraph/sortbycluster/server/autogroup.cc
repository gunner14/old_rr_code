#include "autogroup.h"

#include "LogWrapper.h"

namespace xce {
namespace socialgraph {

//主分组函数，需要子类实现其中的某些函数
void AutoGroup::Grouping(int userid) {
	Initialize(userid);	

	if (hook()) {
		CreateGraph();

		CreateGroup();

		CreateGroupTags();

		CreateStats();
	}
}

void AutoGroup::Grouping(const std::vector<int>& idlist) {
	if ((int)idlist.size() > 1000) { 					//limit is 1000, break;
		return;
	}
	friendlist_ = idlist;		//copy idlist

	if (hook()) {
		CreateGraph();

		CreateGroup();

		CreateGroupTags();

		CreateStats();
	}
}

//初始化创建好友关系
void AutoGroup::Initialize(int userid) {
	userid_ = userid;

	try {
		friendlist_ = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendListAsc(userid_, -1);
	} catch (Ice::Exception e) {
		MCE_WARN("[AutoGroup] Initialize userid(" << userid_ << ") Ice::Exception " << e.what());
	} catch (...) {
		MCE_WARN("[AutoGroup] Initialize userid(" << userid_ << ") unknown::Exception");
	}
}

//创建好友关系图
void AutoGroup::CreateGraph() {
	friendgraphptr_ = boost::shared_ptr<FriendsGraph>(new FriendsGraph((int)friendlist_.size()));

	std::map<int, std::vector<int> > ff_map;
	try {
		ff_map = xce::buddy::adapter::BuddyByIdCacheAdapter::instance().getFriendLists(friendlist_);
	} catch (Ice::Exception e) {
		MCE_WARN("[AutoGroup] CreateGraph BuddyByIdCacheAdapter userid(" << userid_ << ") Ice::Exception " << e.what());
	} catch (...) {
		MCE_WARN("[AutoGroup] CreateGraph BuddyByIdCacheAdapter userid(" << userid_ << ") unknown::Exception");
	}
	for (int i = 0; i < friendgraphptr_->friendsnum_; ++i) {
		std::map<int, std::vector<int> >::iterator fit;
		try {
			fit = ff_map.find(friendlist_.at(i));
		} catch (std::exception e) {
			MCE_WARN("[AutoGroup] CreateGraph map userid(" << userid_ << ") std::exception " << e.what());
		} catch (...) {
			MCE_WARN("[AutoGroup] CreateGraph map userid(" << userid_ << ") unknown::Exception");
		}

		if (fit != ff_map.end()) {
			for (int j = i + 1; j < friendgraphptr_->friendsnum_; ++j) {
				std::vector<int>::iterator fsameit = lower_bound(fit->second.begin(), fit->second.end(), friendlist_.at(j));
				if (fsameit != fit->second.end() && *fsameit == friendlist_.at(j)) {
					friendgraphptr_->graph_[i][j] = 1;
					friendgraphptr_->graph_[j][i] = 1;
				}	
			}
		}
	}
}

}
}
