#ifndef _FEATURE_EVALUATOR_H
#define _FEATURE_EVALUATOR_H
#include "Feature.h"
#include "model.h"
#include "Access.h"

class FeatureEvaluator:public FeatureThread{
	public:
				FeatureEvaluator(Access* acc, FeatureMap* featMap, Logistic* pLogistic);
		void		run();
	private:
		FeatureMap*	_featMap;
		Access*		_acc;
		Logistic*	_pLogistic;
};
#endif
