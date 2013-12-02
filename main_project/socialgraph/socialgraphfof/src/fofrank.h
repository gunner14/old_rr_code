/******************************************************************************
Function:  Calculating weight for friends recommendation in facebook model 
Developer: xiongjunwu
Email:     junwu.xiong@renren-inc.com
Date:      2012-02-29
*******************************************************************************/

#ifndef __FOFRANK_H_
#define __FOFRANK_H_

//#include <iterator>
#include <cmath>
#include <set>
#include <boost/unordered_map.hpp>

#include "BuddyByAddTimeReplicaAdapter.h"
#include "socialgraph/socialgraphutil/cctypedef.h"
#include "socialgraph/socialgraphutil/timecost.h"
#include "socialgraph/socialgraphutil/clogging.h"
#include "SocialGraphFoF.h"
#include "socialgraph/socialgraphfof/src/recommendfriendblock.h"

namespace xce {
namespace socialgraph {
/******************************************************************************
The computation of rank values for friends recommendation according to Facebook's 
data model
*****************************************************************************/
class FoFRankCompute {
 public:
  static const int kAvgFoFNum = 100000;
  static const int kAvgFrdNum = 500;
  //static const unsigned int MAX_FRD_NUM = 2000;
  static const int kAvgMutFrdNum = 100;
  static const int kDispMutFrdNum = 10;

  static double getDayNum(const time_t seconds1, const time_t seconds2);
  static bool CompareRank(const FoFRank& ffr1, const FoFRank& ffr2);
  //get the desending order of the rank values for each host according to the facebook fof model
  //static void getFoFRankSeq(const int hostId, const int begin, const int limit, const FoFRankSeq& fofRankSeq);
  static FoFRankSeq getFoFRankSeq(const int hostId, const int begin, const int limit);
 
 private:
  FoFRankCompute() {}
};

inline bool FoFRankCompute::CompareRank(const FoFRank& ffr1, const FoFRank& ffr2) {
  return (ffr1.rank > ffr2.rank);
}

//calculate the time of adding friends between u and fi, fi and fof
inline double FoFRankCompute::getDayNum(const time_t seconds1, const time_t seconds2) {
  if ((seconds1 > 0) && (seconds2 > 0)) {
    const time_t now = time(NULL);
    const float d1 = (now - seconds1) / 3600 + 1; //plus 1 to promise hours > 0 
    const float d2 = (now - seconds2) / 3600 + 1; //plus 1 to promise hours > 0 
    //if d1,d2 does not devide 3600*24 firstly, their product will be overflow 
    const double hours = d1 * d2;  
    return hours;
  } else {
    //CLOG(ERROR) << " seconds1 " << seconds1 << " seconds2 " << seconds2 << endl;
    return -1;
  }
} 

};
};

#endif
