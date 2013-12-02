#ifndef ADDEMOQUALITY_ICE
#define ADDEMOQUALITY_ICE

module xce{
module ad{ 

dictionary<long, double> FreshAdQualitySet;
dictionary<string, FreshAdQualitySet> FreshAdQualityMap;
dictionary<string, double> AdQualityMap;


interface AdCreativeQualityManager {
  AdQualityMap GetDemoPool(); 
  FreshAdQualityMap GetFreshAdMap();
};

};
};

#endif
