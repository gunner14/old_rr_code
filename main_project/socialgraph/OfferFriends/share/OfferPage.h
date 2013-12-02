#ifndef __OFFER_PAGE_H__
#define __OFFER_PAGE_H__

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <OfferFriends.h>
#include <Singleton.h>
#include "socialgraph/OfferFriends/share/PageUtil.h"

namespace xce {
namespace offerfriends {


class OfferPage: public MyUtil::Singleton<OfferPage> {
public:
    MyUtil::IntSeq createPageOffer(int userId, int limit);
    friend class MyUtil::Singleton<OfferPage>;
private:
    

};

}
}

#endif
