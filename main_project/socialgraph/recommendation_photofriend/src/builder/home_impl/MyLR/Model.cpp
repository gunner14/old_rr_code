#include "Model.h"
#include <iostream>
#include "../friend_recommend_builder.h"
using namespace std;
using namespace xce::socialgraph;

DataDict<int, double> Logistic::weightMap;

double Logistic::predict(Feature* feat){
//	MCE_INFO("1.predict:");
	int accesstype = feat->getLabel();            
	double factor = 1.0;                          
	double weit = 0.0;
	map<int, double>::iterator it = weightMap.GetInstance()->find(0);
	if(it != weightMap.GetInstance()->end())
		weit = it->second;                     //获取偏置项
	for(int i = 0; i < feat->size(); i++){
		it = weightMap.GetInstance()->find(feat->at(i));
		if(it != weightMap.GetInstance()->end())
			weit += it->second;
	}
	double prob = factor * _sigmod(weit);
	return prob;
}

double Logistic::_sigmod(double z){
        return 1.0/(1+exp(-z));
}


