#include "FoFEvaluator.h"
#include "Feature.h"
#include "SocialGraphFoFAdapter.h"
#include "socialgraph/socialgraphfof/src/fofrank.h"
#include "fofdbutil.h"
using namespace com::xiaonei::xce;
using namespace xce::socialgraph;

FoFEvaluator::FoFEvaluator(Access* acc){
	_acc=acc;
}

void FoFEvaluator::run(){
  	ConnectionPoolManager::instance().initialize();
	int end=_part_num+_scope;
	int host=0;
	FoFRankSeq fofRankSeq;
	for(int i=_part_num; i<end; i++){
		if((i-_part_num)%100==0){
			printf("thread %d running..., %.2f%\n", _threadnum, ((float)(i-_part_num))/_scope*100.0);
		}
		host=_acc->at(i).uid;
		try{
			fofRankSeq =
			xce::socialgraph::SocialGraphFoFAdapter::instance().getFoFRankSeq(host, 100);
		}catch(...){
			printf("exception: %d\n", host);
			continue;
		}
		if(fofRankSeq.size()==0)continue;
    		FoFRankDbUtil::setFoFRankSeq(host, fofRankSeq);
	}
}
