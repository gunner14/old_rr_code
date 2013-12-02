/*
 * SeletcHandler.h
 *
 *  Created on: 2012-2-26
 *      Author: ping.zhang
 */

#ifndef SELECTHANDLER_H_
#define SELECTHANDLER_H_

#include "MatchHandler.h"
#include "UserDemographicCache.h"
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
  protected:
    virtual void Filter(const std::set<AdGroupPtr>& bid_groups, std::set<AdGroupPtr>& filtered_groups);
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
class LeftSMEAdSelector : public Selector {
  public:
    LeftSMEAdSelector() : Selector(Left_ad, kWeekly){
    }
    virtual bool handle(MatchHandlerParamter& param);
    bool SelectByEcpm(const std::set<AdGroupPtr>& left_groups, std::set<AdGroupPtr>& selected_groups, std::map<Ice::Long, double>& ecpm_map);
  protected:

    bool GetAdByADCtr(std::set<AdGroupPtr>& groups, std::set<AdGroupPtr>& left_groups, const UserProfile& user, std::map<Ice::Long, double>& ecpm_map);
    bool GetAdByEdmCtr(std::set<AdGroupPtr>& groups, std::set<AdGroupPtr>& left_groups, const UserProfile& user, std::map<Ice::Long, double>& ecpm_map);  

    bool GetLeftAD(std::set<AdGroupPtr>& groups, std::set<AdGroupPtr>& left_groups, const UserProfile& user, std::map<Ice::Long, double>& ecpm_map);
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
