/*
 *  SearchLogicAdapter.h
 *  
 *   Created on: 2010-9-27
 *       Author: lijie
 **/

#ifndef PEOPLEDISTANCE_H
#define PEOPLEDISTANCE_H

#include "Singleton.h"
#include "AdapterI.h"

#include <SearchPeopleDistance.h>

using namespace com::xiaonei::search::distance;

class PeopleDistanceReplicaAdapter : public MyUtil::Singleton<PeopleDistanceReplicaAdapter>, public MyUtil::ReplicatedClusterAdapterI<PeopleDistancePrx>
{
  public:
    PeopleDistanceReplicaAdapter();
    virtual ~PeopleDistanceReplicaAdapter();

    Int2FloatMap Get(int a, const vector<int> & others) ;
};
#endif
