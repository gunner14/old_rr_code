/*
 *  SearchLogicAdapter.h
 *  
 *   Created on: 2010-9-27
 *       Author: lijie
 **/

#ifndef SEARCHPEOPLEPROXYADAPTER_H_
#define SEARCHPEOPLEPROXYADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"

#include <SearchPeopleProxy.h>

using namespace xce::searchcache;

class SearchPeopleProxyAdapter : public MyUtil::Singleton<SearchPeopleProxyAdapter>, public MyUtil::ReplicatedClusterAdapterI<SearchPeopleProxyPrx> {

  public:
    SearchPeopleProxyAdapter();

    virtual ~SearchPeopleProxyAdapter();

    bool Send(const map<int,string> & infos);

};
#endif
