#ifndef _FOF_EVALUATOR_H
#define _FOF_EVALUATOR_H
#include "Feature.h"
#include "model.h"
#include "Access.h"

class FoFEvaluator:public FeatureThread{
	public:
				FoFEvaluator(Access* acc);
		void		run();
	private:
		Access*		_acc;
};
#endif
