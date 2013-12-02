/*
 * =====================================================================================
 *
 *       Filename:  MiniGroupTripodProducer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月10日 18时20分16秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  xiaoguang.yang (), xiaoguang.yang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef _MINIGROUP_TRIPOD_PRODUCER_H__
#define _MINIGROUP_TRIPOD_PRODUCER_H__

#include "TripodProducer.h"
#include "boost/lexical_cast.hpp"
#include "ServiceI.h"

namespace xce{
  namespace minigrouptripod{

using namespace std;
using namespace com::renren::tripod;
using namespace MyUtil;
using namespace com::xiaonei::xce;

class MiniGroupTripodProducer : virtual public TripodProducer{
public:
  MiniGroupTripodProducer(){
  }
  virtual ~MiniGroupTripodProducer(){}
  virtual DataMap create(const KeySeq& keys, bool isMaster);
  virtual KeySeq createHotKeys(const string& beginKey, int limit);
  virtual KeySeq createKeys(const string& beginKey, int limit);
private:
  vector<long> parseFrom(const KeySeq& keys);

};



}
}



#endif
