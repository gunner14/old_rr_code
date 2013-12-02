#include "sort_util.h"
#include "friendinfoutil.h"

namespace xce {
namespace socialgraph {

void SortUtil::FilterUserPhoto(std::vector<int>& idlist) {
	using namespace xce::photostatefilter::adapter;

	std::map<int, bool> result = PhotoStateFilterAdapter::instance().getPhotoStateBatch(idlist);
	std::vector<int> photo_id_list;
	std::vector<int> retain_id_list;

	for (std::vector<int>::iterator iter = idlist.begin();
			iter != idlist.end(); ++iter) {
		if (result[*iter]) {			//has head 
			photo_id_list.push_back(*iter);
		} else {
			retain_id_list.push_back(*iter);
		}
	}
	idlist.clear();
	idlist.insert(idlist.end(), photo_id_list.begin(), photo_id_list.end());
	idlist.insert(idlist.end(), retain_id_list.begin(), retain_id_list.end());
}

bool valueCmp(const pair<int, float>& map1, const pair<int, float>& map2) {
  return map1.second > map2.second;
}

void SortUtil::RankByUserValue(std::vector<int>& idlist) {
  // star photo , online condition
  using namespace std;
  map<int, bool> userIdStarMap = FriendInfoUtil::getStateStarBatch(idlist);
  map<int, bool> userIdPhotoMap = FriendInfoUtil::getPhotoStateBatch(idlist);
  map<int, string> userIdLoginDaysMap = FriendInfoUtil::getLoginDayState(idlist);

  vector< pair<int, float> > userIdScoreSeq;

  const int perDays = 7;
  float para[4] ={ 0.499, 0.129, 0.199, 0.086 };
  vector<float> paraSeq(para, para + 4);
  
  for(vector<int>::const_iterator iter = idlist.begin(); iter != idlist.end(); iter++) {
    float userScore = 0.0;
    if (true == userIdStarMap[*iter]) {
      userScore += 0.5;
    }
    else if (true == userIdStarMap[*iter]) {
      userScore += 0.25;
    }
  /* login days */
    string loginDays = userIdLoginDaysMap[*iter];
    float predictDays = 0;
    int paraNum;
    for(paraNum = 0;  paraNum != paraSeq.size(); paraNum++) {
      int loginDaysCount = 0;
      for(int i = 0; i != perDays; i++) {
        if('1' == loginDays[i + perDays * paraNum]) {
          loginDaysCount++;
        }
      }
      predictDays += loginDaysCount * paraSeq[paraNum];
    }
    userScore += 0.5 * predictDays / (perDays * paraSeq.size());
    userIdScoreSeq.push_back(make_pair(*iter, userScore));
  }

  sort(userIdScoreSeq.begin(), userIdScoreSeq.end(), valueCmp);
  idlist.clear();
  for(vector< pair<int, float> >::iterator iter = userIdScoreSeq.begin();
      iter != userIdScoreSeq.end(); iter++) {
    idlist.push_back(iter->first);
  }
}

}
}
