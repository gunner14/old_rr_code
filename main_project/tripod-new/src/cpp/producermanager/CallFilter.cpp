#include "CallFilter.h"
#include "TripodProducerManager.h"

namespace com {
namespace renren {
namespace tripod {

CountPair::CountPair(int maxCount) : curCount_(0), maxCount_(maxCount) {
}

bool CountPair::inc() {
    IceUtil::Mutex::Lock lock(*this);
    return ++curCount_ <= maxCount_;
}

void CountPair::dec() {
    IceUtil::Mutex::Lock lock(*this);
    --curCount_;
}

FilterHolder::FilterHolder() {
    //add business config here
    filterMap_.insert(make_pair("SearchCache", new CountPair(20)));
    filterMap_.insert(make_pair("buddybyidcache", new CountPair(100)));
    filterMap_.insert(make_pair("buddyrelationcache", new CountPair(100)));

    //User服务
    //----------------------------------------------------------
    filterMap_.insert(make_pair("TTicket", new CountPair(20)));
    filterMap_.insert(make_pair("PTicket", new CountPair(20)));
    filterMap_.insert(make_pair("UserConfig", new CountPair(20)));
    filterMap_.insert(make_pair("UserContact", new CountPair(20)));
    filterMap_.insert(make_pair("UserTime", new CountPair(20)));
    filterMap_.insert(make_pair("UserPassport", new CountPair(20)));
    filterMap_.insert(make_pair("UserName", new CountPair(20)));
    filterMap_.insert(make_pair("UserRight", new CountPair(20)));
    filterMap_.insert(make_pair("UserState", new CountPair(20)));
    filterMap_.insert(make_pair("UserLoginCount", new CountPair(20)));
    filterMap_.insert(make_pair("UserBorn", new CountPair(20)));
    filterMap_.insert(make_pair("UserStage", new CountPair(20)));
    filterMap_.insert(make_pair("UserDoing", new CountPair(20)));
    filterMap_.insert(make_pair("UserUrl", new CountPair(20)));

    //App服务
    //----------------------------------------------------------
    filterMap_.insert(make_pair("ILikeIt", new CountPair(20)));
    filterMap_.insert(make_pair("RecentMenu", new CountPair(20)));
    filterMap_.insert(make_pair("FavoriteMenu", new CountPair(20)));
    filterMap_.insert(make_pair("FootprintHead", new CountPair(20)));
    filterMap_.insert(make_pair("FootprintBody", new CountPair(20)));
    filterMap_.insert(make_pair("UserViewCount", new CountPair(20)));
    filterMap_.insert(make_pair("MiniGroup", new CountPair(20)));
    filterMap_.insert(make_pair("User2MiniGroup", new CountPair(20)));

    //UserNetwork
    filterMap_.insert(std::make_pair("NetworkHistory", new CountPair(20)));
    filterMap_.insert(std::make_pair("HighSchoolInfo", new CountPair(20)));
    filterMap_.insert(std::make_pair("UniversityInfo", new CountPair(20)));
    filterMap_.insert(std::make_pair("WorkplaceInfo", new CountPair(20)));
    filterMap_.insert(std::make_pair("CollegeInfo", new CountPair(20)));
    filterMap_.insert(std::make_pair("ElementarySchoolInfo", new CountPair(20)));
    filterMap_.insert(std::make_pair("JuniorHighSchoolInfo", new CountPair(20)));
    filterMap_.insert(std::make_pair("RegionInfo", new CountPair(20)));
}

CallFilter::CallFilter(const FilterHolder& filterHolder, const std::string& businessId) :
                        filterHolder_(filterHolder), businessId_(businessId) {
}

bool CallFilter::filter() {
    std::map<std::string, CountPairPtr>::iterator findIt = filterHolder_.filterMap_.find(businessId_);
    if(findIt == filterHolder_.filterMap_.end()) {
        throw TripodProducerNotSupportedException();
        return true;
    }

    CountPairPtr counter = findIt->second;
    if(counter->inc()) {
        return true;
    }else {
            std::ostringstream os;
            os << "CallFilter::filter too many calls for business " << businessId_ << " max is " << counter->maxCount_;
            MCE_WARN(os.str());
            return false;
    }
}

CallFilter::~CallFilter() {
    std::map<std::string, CountPairPtr>::iterator findIt = filterHolder_.filterMap_.find(businessId_);
    if(findIt == filterHolder_.filterMap_.end()) {
        return;
    }

    CountPairPtr counter = findIt->second;

    counter->dec();
}

}
}
}
