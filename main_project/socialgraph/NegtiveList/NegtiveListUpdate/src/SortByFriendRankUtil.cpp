#include "SortByFriendRankUtil.h"
#include "socialgraph/socialgraphutil/clogging.h" // for clog

using namespace xce::socialgraph;
using namespace sortbyrank;
using namespace MyUtil;
using namespace std;

void SortByFriendRankUtil::SortTargetByFriendRank(std::map<int, std::vector<int> >& target) {
	time_t phrase_start = 0, phrase_end = 0;
	phrase_start = time(NULL);

	TaskManager::instance().config(TASK_LEVEL_SORTBY_FRIENDRANK, ThreadPoolConfig(0, 100));
	int task_size = 0;
	long wait = 0;
	long count = 0;

	for (map<int, vector<int> >::iterator iter = target.begin();
			iter != target.end(); ++iter) {
		if (++count % 1000 == 0) {
			CLOG(INFO) << "SortByFriendRankUtil " << count << " / " << target.size();
		}

		TaskManager::instance().execute(new SortByFriendRankTask(iter->first, iter->second));

		while ((task_size = TaskManager::instance().size(TASK_LEVEL_SORTBY_FRIENDRANK)) > 10000) {
			wait += 2;
			sleep(2);
			CLOG(INFO) << "TASK_LEVEL_SORTBY_FRIENDRANK: sleep " << wait << " second. left " << task_size << " task.";
		}
	}

	CLOG(INFO) << "\nSortTargetByFriendRank completed.";

	while (TaskManager::instance().size(TASK_LEVEL_SORTBY_FRIENDRANK) > 0) {
		CLOG(INFO) << "final left TASK_LEVEL_SORTBY_FRIENDRANK size = " <<  task_size << ".";
		sleep(2);
	}
	sleep(5);
	phrase_end = time(NULL);
	CLOG(INFO) << "[SortByFriendRankUtil] size of target = " << target.size() << ", using " << (phrase_end - phrase_start)/60.0 << "m";
	//cout << "target : " << endl;
	//cout << target.size() << endl;
	//for (vector<int>::iterator iter = target[238111132].begin();
	//		iter != target[238111132].end(); ++iter) {
	//	cout << *iter << " ";
	//}
	//cout << endl;
}

//-----------------------------------------------------------------------------------

void SortByFriendRankTask::handle() {
	if (_idList.empty()) {
		return;
	}

	map<int, int> friendRank;
	for (vector<int>::iterator iter = _idList.begin();
			iter != _idList.end(); ++iter) {
		friendRank.insert(make_pair<int, int>(*iter, 1));
	}

	//for (map<int, int>::iterator iter = friendRank.begin();
	//		iter != friendRank.end(); ++iter) {
	//	cout << iter->first << " : " << iter->second << endl;
	//}
	//cout << endl;

	RankSeq vecRank;
	try {
		vecRank = FriendRankCacheAdapter::instance().GetRank(_userId);
	} catch (Ice::Exception& e) {
		CLOG(ERROR) << "[SortByFriendRankTask] FriendRankCacheAdapter Ice Exception " << e.what();
	} catch (...) {
		CLOG(ERROR) << "[SortByFriendRankTask0] FriendRankCacheAdapter Unknown Exception";
	}


	for (RankSeq::const_iterator iter = vecRank.begin();
			iter != vecRank.end(); ++iter) {
		if (friendRank.find(iter->userid) != friendRank.end() && iter->weight > 0) {
			friendRank[iter->userid] = iter->weight;
		}
	}

	//for (map<int, int>::iterator iter = friendRank.begin();
	//		iter != friendRank.end(); ++iter) {
	//	cout << iter->first << " : " << iter->second << endl;
	//}
	//cout << endl;

	vector< pair<int, int> > sortVec;
	for (map<int, int>::iterator iter = friendRank.begin();
			iter != friendRank.end(); ++iter) {
		sortVec.push_back(make_pair<int, int>(iter->first, iter->second));
	}

	sort(sortVec.begin(), sortVec.end(), RankComp());

	_idList.clear();
	for (vector< pair<int, int> >::iterator iter = sortVec.begin();
			iter != sortVec.end(); ++iter) {
		_idList.push_back(iter->first);
	}
	
	//for (vector<int>::iterator iter = _idList.begin();
	//		iter != _idList.end(); ++iter) {
	//	cout << *iter << " ";
	//}
	//cout << endl;
}

