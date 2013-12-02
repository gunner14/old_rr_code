#include "FeatureGenerator.h"
#include "FeatureEvaluator.h"
#include "FoFEvaluator.h"
#include "TaskManager.h"

Tasks::Tasks(int nthread):_nthread(nthread){
	//_vecThreads.clear();
}

Tasks::~Tasks(){
	for(int j=0; j < _nthread; j++){
		delete _vecThreads[j];
	}
	_vecThreads.clear();
}

void Tasks::_assign(int total){
	if(total==0)return;
	int scope = total/_nthread;
	int res = total%_nthread;
	int i=0;
	for(; i<_nthread-1; i++)
		_vecThreads[i]->distribute(i, i*scope, scope);
	if(res==0)
		_vecThreads[i]->distribute(i, i*scope, scope);
	else
		_vecThreads[i]->distribute(i, i*scope, scope+res);

	for(int i=0; i<_nthread; i++)
		_vecThreads[i]->start();
	for(int i=0; i<_nthread; i++)
		_vecThreads[i]->join();
}

void Tasks::genfeat(Access* acc, FeatureMap* featMap, const char* save_prefix){
	for(int i=0; i<_nthread; i++){
		_vecThreads.push_back(new FeatureGenerator(acc, featMap, save_prefix));
	}
	_assign(acc->size());
}

void Tasks::eval(Access* acc, FeatureMap* featMap, Logistic* pLR){
	for(int i=0; i<_nthread; i++){
		_vecThreads.push_back(new FeatureEvaluator(acc, featMap, pLR));
	}
	_assign(acc->size());
}

void Tasks::fofeval(Access* acc){
	for(int i=0; i<_nthread; i++){
		_vecThreads.push_back(new FoFEvaluator(acc));
	}
	_assign(acc->size());
}
