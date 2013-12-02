#ifndef MODEL_H
#define MODEL_H
#include <math.h>

#include "Feature.h"


class Logistic{
	public:
		Logistic(const char* file);
		double eval(Feature* feat);
		virtual ~Logistic(){}
	private:
		inline double logistic(double z){
			return 1.0/(1+exp(-z));
		}
		void _loadModel(const char* file);
		double _weights[MAX_FEAT_INX];
};

#endif
