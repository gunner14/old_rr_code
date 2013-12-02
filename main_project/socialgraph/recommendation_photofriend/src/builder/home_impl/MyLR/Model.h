#ifndef MODEL_H
#define MODEL_H

#include <math.h>
#include "Feature.h"

class Logistic{
	public:
		Logistic() {}
		virtual ~Logistic() {}
		static double predict(Feature* feat);       //预测点击概率
		static DataDict<int, double> weightMap;     //模型
	private:
		static double _sigmod(double z);
};

#endif

