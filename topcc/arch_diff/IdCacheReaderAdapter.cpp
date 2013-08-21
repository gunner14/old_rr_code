#include "IdCacheReaderAdapter.h"
#include "Date.h"

namespace idcache
{

IdCacheReaderPrx IdCacheReaderAdapter::getIdCacheReaderPrx(int id)
{
	return getProxy(abs(id));
}

IdRelationInfoPtr IdCacheReaderAdapter::getIdRelationInfo(int id)
{
	IdCacheReaderPrx prx = getIdCacheReaderPrx(id);
	if(prx)
	{
		try
		{
			return prx->getIdRelationInfo(id);
		}catch(Ice::Exception& e)
		{
		}
	}
	return NULL;
}

int IdCacheReaderAdapter::getRelatedUserId(int id)
{
	IdCacheReaderPrx prx = getIdCacheReaderPrx(id);
	if(prx)
	{
		try
		{
			return prx->getRelatedUserId(id);
		}catch(Ice::Exception& e)
		{
		}
	}
	return 0;
}

int IdCacheReaderAdapter::getRelatedTransId(int id)
{
	IdCacheReaderPrx prx = getIdCacheReaderPrx(id);
	if(prx)
	{
		try
		{
			return prx->getRelatedTransId(id);
		}catch(Ice::Exception& e)
		{
		}
	}
	return 0;
}


void IdCacheReaderAdapter::createIdRelation(int id, int transId, int virtualId) 
{
	std::vector<IdCacheReaderPrx> allProxies = getAllProxySeq(abs(id));
	for(std::vector<IdCacheReaderPrx>::const_iterator it = allProxies.begin(); it != allProxies.end(); ++it )
	{
		try
		{
			(*it)->ice_timeout(10000)->createIdRelation(id, transId, virtualId);
		}catch(Ice::Exception& e)
		{
		}
	}
}

void IdCacheReaderAdapter::deleteIdRelation(int id, int virtualId) 
{
	std::vector<IdCacheReaderPrx> allProxies = getAllProxySeq(abs(id));
	for(std::vector<IdCacheReaderPrx>::const_iterator it = allProxies.begin(); it != allProxies.end(); ++it )
	{
		try
		{
			(*it)->ice_timeout(10000)->deleteIdRelation(id, virtualId);
		}catch(Ice::Exception& e)
		{
		}
	}
}

void IdCacheReaderAdapter::setData(int id, const Ice::ObjectPtr& data) 
{
	MyUtil::ObjectResultPtr res = new MyUtil::ObjectResult;
	res->data.insert(make_pair(id, data));
	std::vector<IdCacheReaderPrx> allProxies = getAllProxySeq(abs(id));
	for(std::vector<IdCacheReaderPrx>::const_iterator it = allProxies.begin(); it != allProxies.end(); ++it )
	{
		try
		{
			(*it)->ice_timeout(10000)->setData(res);
		}catch(Ice::Exception& e)
		{
		}
	}
}

bool IdCacheReaderAdapter::isVirtualId(int id)
{
	if(!isMaybeVirtualId(id))
	{
		return false;
	}
	IdCacheReaderPrx prx = getIdCacheReaderPrx(id);
	if(prx)
	{
		try
		{
			return prx->isVirtualId(id);
		}catch(Ice::Exception& e)
		{
		}
	}
	return false;
}

bool IdCacheReaderAdapter::isMaybeVirtualId(int id)
{
	return id > MIN_FILTER_ID && id < MAX_FILTER_ID;
}

}

