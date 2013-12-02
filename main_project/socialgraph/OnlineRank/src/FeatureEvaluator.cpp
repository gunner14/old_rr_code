#include "FeatureEvaluator.h"
#include "Feature.h"
#include "SocialGraphFoFAdapter.h"
#include "socialgraph/socialgraphfof/src/fofrank.h"
#include "fofdbutil.h"
using namespace com::xiaonei::xce;
using namespace xce::socialgraph;

FeatureEvaluator::FeatureEvaluator(Access* acc, FeatureMap* featMap, Logistic* pLogistic){
	_acc=acc;
	_featMap=featMap;
	_pLogistic=pLogistic;
}

void FeatureEvaluator::run(){
	printf("begin thread %d\n", _threadnum);
  	ConnectionPoolManager::instance().initialize();
	Feature* feat = new Feature;
	int end=_part_num+_scope;
	int host=0;
	FoFRankSeq fofRankSeq;
	FoFRankSeq::iterator it;
	map<int, int>::iterator itr;
	for(int i=_part_num; i<end; i++){
		if((i-_part_num)%100==0){
			printf("thread %d running..., %.2f%\n", _threadnum, ((float)(i-_part_num))/_scope*100.0);
		}
		host=_acc->at(i).uid;
		map<int, int> &UA=_acc->at(i).userAccess;
		fofRankSeq =
		xce::socialgraph::SocialGraphFoFAdapter::instance().getFoFRankSeq(host, 1000);
		if(fofRankSeq.size()==0)continue;
		it = fofRankSeq.begin();
		for(;it!=fofRankSeq.end(); ++it){
			int accesstype=-1;
			itr=UA.find(it->fofId);
			if(itr!=UA.end())accesstype=itr->second;
			GetFeature(host, it->fofId, accesstype, _featMap, feat);
			double prob = _pLogistic->eval(feat);
			double nrank = exp(prob) * it->rank;
			it->rank = nrank;
			feat->reset();
		}
		sort(fofRankSeq.begin(), fofRankSeq.end(), FoFRankCompute::CompareRank);
		size_t len=0;
		if(fofRankSeq.size()>100)len = 100;
		else len=fofRankSeq.size();
		FoFRankSeq foftop(fofRankSeq.begin(), fofRankSeq.begin()+len);
    		FoFRankDbUtil::setFoFRankSeq(host, foftop);
	}
	delete feat;
}
