#include "FeedCreatorAdapter.h"

using namespace xce::feed;


FeedCreatorAdapter::FeedCreatorAdapter()
{
	_managers.resize(cluster() > 0 ? cluster() : 1);
	_managersOneway.resize(cluster() > 0 ? cluster() : 1);
}

FeedCreatorAdapter::~FeedCreatorAdapter()
{

}

FeedCreatorPrx FeedCreatorAdapter::getManager()
{
        MCE_INFO("FeedCreatorAdapter::getManager");
	return locate<FeedCreatorPrx> (_managers, "FA", 0, TWO_WAY);
}

FeedCreatorPrx FeedCreatorAdapter::getManagerOneway()
{
	return locate<FeedCreatorPrx> (_managersOneway, "FA", 0, ONE_WAY);
}

bool FeedCreatorAdapter::CreateEDM(int stype, int version, const MyUtil::Str2StrMap& props) {
  bool ret = false;
  ret = getManager()->CreateEDM(stype, version, props);
  return ret;
}

bool FeedCreatorAdapter::Create(int stype, int version, const MyUtil::Str2StrMap& props) {
  bool ret = false;
  ret = getManager()->Create(stype, version, props);
  return ret;
}
