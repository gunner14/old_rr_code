#include "TripodProducerFactory.h"
#include "SearchTripodProducer.h"
#include "UserTimeTripodProducer.h"
#include "BuddyByIdCacheProducer.h"
#include "BuddyRelationCacheProducer.h"
#include "SingleTableProducerNew.h"
#include "LikeTripodProducer.h"
#include "MenuTripodProducer.h"
#include "HomeFootprintProducer.h"
#include "UserViewCountProducer.h"
#include "MiniGroupTripodProducer.h"
#include "User2MiniGroupTripodProducer.h"
//Proto
#include "UserConfig.pb.h"
#include "UserDesc.pb.h"
#include "UserBase.pb.h"
#include "user_network.pb.h"

namespace com {
namespace renren {
namespace tripod {

using com::renren::tripod::SingleTableProducer;

TripodProducerFactory::TripodProducerFactory() {
    map_.insert(make_pair("SearchCache", new com::renren::searchtripod::SearchTripodProducer));
    map_.insert(std::make_pair("buddybyidcache", new xce::buddy::BuddyByIdCacheProducer));
    map_.insert(std::make_pair("buddyrelationcache", new xce::buddy::BuddyRelationCacheProducer));

    //User服务
    //----------------------------------------------------------
    map_.insert(std::make_pair("UserConfig", new SingleTableProducer<xce::tripod::UserConfigData>));
    map_.insert(std::make_pair("UserContact", new SingleTableProducer<xce::tripoddata::userdesc::PbUserContact>));
    map_.insert(std::make_pair("UserTime", new SingleTableProducer<xce::tripoddata::userdesc::PbUserTime>));
    map_.insert(std::make_pair("UserPassport", new SingleTableProducer<xce::tripoddata::userbase::PbUserPassport>));
    map_.insert(std::make_pair("UserName", new SingleTableProducer<xce::tripoddata::userbase::PbUserName>));
    map_.insert(std::make_pair("UserRight", new SingleTableProducer<xce::tripoddata::userbase::PbUserRight>));
    map_.insert(std::make_pair("UserState", new SingleTableProducer<xce::tripoddata::userbase::PbUserState>));
    map_.insert(std::make_pair("UserLoginCount", new SingleTableProducer<xce::tripoddata::userbase::PbUserLoginCount>));
    map_.insert(std::make_pair("UserBorn", new SingleTableProducer<xce::tripoddata::userbase::PbUserBorn>));
    map_.insert(std::make_pair("UserStage", new SingleTableProducer<xce::tripoddata::userbase::PbUserStage>));
    map_.insert(std::make_pair("UserDoing", new SingleTableProducer<xce::tripoddata::userbase::PbUserDoing>));
    map_.insert(std::make_pair("UserUrl", new SingleTableProducer<xce::tripoddata::userbase::PbUserUrl>));

    //App服务
    // ----------------------------------------------------------
    map_.insert(std::make_pair("ILikeIt", new xce::liketripod::LikeTripodProducer));
    map_.insert(std::make_pair("RecentMenu", new xce::menutripod::MenuTripodProducer(xce::menu::RECENTLY)));
    map_.insert(std::make_pair("FavoriteMenu", new xce::menutripod::MenuTripodProducer(xce::menu::FAVORITE)));
    map_.insert(std::make_pair("FootprintHead", new xce::footprint::HomeFootprintProducer(xce::footprint::CACHE_SIZE)));
    map_.insert(std::make_pair("FootprintBody", new xce::footprint::HomeFootprintProducer(xce::footprint::CACHE_BUF_SIZE)));
    map_.insert(std::make_pair("UserViewCount", new xce::footprint::UserViewCountProducer));
    map_.insert(std::make_pair("MiniGroup", new xce::minigrouptripod::MiniGroupTripodProducer));
    map_.insert(std::make_pair("User2MiniGroup", new xce::minigrouptripod::User2MiniGroupTripodProducer));

    //UserNetwork
    map_.insert(std::make_pair("NetworkHistory", new SingleTableProducer<xce::tripoddata::usernetwork::PbNetworkHistory>));
    map_.insert(std::make_pair("HighSchoolInfo", new SingleTableProducer<xce::tripoddata::usernetwork::PbHighSchoolInfo>));
    map_.insert(std::make_pair("UniversityInfo", new SingleTableProducer<xce::tripoddata::usernetwork::PbUniversityInfo>));
    map_.insert(std::make_pair("WorkplaceInfo", new SingleTableProducer<xce::tripoddata::usernetwork::PbWorkplaceInfo>));
    map_.insert(std::make_pair("CollegeInfo", new SingleTableProducer<xce::tripoddata::usernetwork::PbCollegeInfo>));
    map_.insert(std::make_pair("ElementarySchoolInfo", new SingleTableProducer<xce::tripoddata::usernetwork::PbElementarySchoolInfo>));
    map_.insert(std::make_pair("JuniorHighSchoolInfo", new SingleTableProducer<xce::tripoddata::usernetwork::PbJuniorHighSchoolInfo>));
    map_.insert(std::make_pair("RegionInfo", new SingleTableProducer<xce::tripoddata::usernetwork::PbRegionInfo>));
    
}

TripodProducerPtr TripodProducerFactory::get(const std::string& businessId) {
    std::map<std::string, TripodProducerPtr>::const_iterator findIt = map_.find(businessId);
    if(findIt != map_.end()) {
        return findIt->second;
    }
    return TripodProducerPtr();
}

}}} //com::renren::tripod

