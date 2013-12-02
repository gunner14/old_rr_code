#ifndef _FEATUREGENERATOR_H
#define _FEATUREGENERATOR_H
#include "Feature.h"
#include "Access.h"
#include "Feature.h"

#include <fstream>
using namespace std;


class FeatureGenerator:public FeatureThread{
	public:
				FeatureGenerator(Access *acc, FeatureMap* featMap, const char* save_prefix);
		void		run();
	private:

		const char*	_save_prefix;
		Access		*_acc;
		FeatureMap	*_featMap;
};


#endif
