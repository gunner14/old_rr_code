/*
 * =====================================================================================
 *
 *       Filename:  LikeTripodProducer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月10日 10时08分06秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */

#ifndef __LIKE_TRIPOD_PRODUCER_H__
#define __LIKE_TRIPOD_PRODUCER_H__

#include "TripodProducer.h"
#include "boost/lexical_cast.hpp"
#include "app/ILikeIt/share/ILikeItUtil.h"
#include "ServiceI.h"

namespace xce {
namespace liketripod {

using namespace std;
using namespace com::renren::tripod;
using namespace MyUtil;
using namespace com::xiaonei::xce;

class LikeTripodProducer : virtual public TripodProducer {
public:
  LikeTripodProducer() {
  }
  virtual ~LikeTripodProducer() {}
  virtual DataMap create(const KeySeq& keys, bool isMaster);
  virtual KeySeq createHotKeys(const string& beginKey, int limit);
  virtual KeySeq createKeys(const string& beginKey, int limit);
private:
  vector<long> parseFrom(const KeySeq& keys);
};

}
}

#endif

