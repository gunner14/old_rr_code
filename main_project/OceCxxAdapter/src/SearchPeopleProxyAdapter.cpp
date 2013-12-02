#include "SearchPeopleProxyAdapter.h"
#include "IceExt/src/Channel.h"

SearchPeopleProxyAdapter::SearchPeopleProxyAdapter()
	:MyUtil::ReplicatedClusterAdapterI<SearchPeopleProxyPrx>("ControllerSearchPeopleProxy", 120, 300,
			new MyUtil::SearchControllerChannel())
{
}

SearchPeopleProxyAdapter::~SearchPeopleProxyAdapter()
{
}

bool SearchPeopleProxyAdapter::Send(const map<int,string> & infos)
{
  SearchPeopleProxyPrx proxy = getProxyOneway(0);
  if( proxy )  { 
    MCE_INFO(proxy);
    try  {
      proxy->Send(infos);
      MCE_INFO("send success.size:" + infos.size());
    } catch (Ice::Exception& e) {
      MCE_INFO("SearchPeopleProxyAdapter::send Ice::Exception " << e);
      return false;
    } catch (std::exception& e) {
      MCE_INFO("SearchPeopleProxyAdapter::send std::exception " << e.what());
      return false;
    } catch (...) {
      MCE_INFO("SearchPeopleProxyAdapter::send Unknown exception");
      return false;
    } 
  }  else  {
    MCE_WARN("proxy is not valid!!!");
  }
  return true;
}



