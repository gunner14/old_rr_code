/*
 * SeletcHandler.h
 *
 *  Created on: 2012-2-26
 *      Author: ping.zhang
 */

#ifndef SELECTHANDLER_H_
#define SELECTHANDLER_H_

#include "MatchHandler.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
namespace xce {
namespace ad {


class Selector : public IceUtil::Shared {
  public:
    Selector(PubType pub_type, SendType send_type):pub_type_(pub_type), send_type_(send_type){

    }
    virtual bool handle(MatchHandlerParamter& para);//为后续实现复杂策略做扩展接口
    virtual bool select(const set<AdGroupPtr>& bid_gids, set<AdGroupPtr>&result);//
    virtual void build(MatchHandlerParamter& para, const set<AdGroupPtr>&result, SendType send_type);
  private:
    PubType pub_type_;
    SendType send_type_;
};
typedef IceUtil::Handle<Selector> SelectorPtr;

class BrandAdSelector : public Selector {
  public:
    BrandAdSelector() : Selector(Brand_ad, kWeekly){

    }

};
class SMEAdSelector : public Selector {
  public:
    SMEAdSelector() : Selector(Self_ad, kWeekly){

    }
    virtual bool select(const set<AdGroupPtr>& bid_gids,set<AdGroupPtr>&result);
};
class ProductAdSelector : public Selector {
  public:
    ProductAdSelector() : Selector(Product_ad, kDaily) {
    }
};

class SelectHandler : public EdmMatchHandler {
  public:
    virtual bool handle(MatchHandlerParamter& para);
};

}
}
#endif /* SELECTHANDLER_H_ */
