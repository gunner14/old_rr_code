#include "model.h"

#include <assert.h>
#include <fstream>
using namespace std;

Logistic::Logistic(const char* file){
	memset(_weights, 0, sizeof(double)*MAX_FEAT_INX);
	_loadModel(file);
}

void Logistic::_loadModel(const char* file){
	ifstream ifs(file);
	string line="";
	while(getline(ifs, line)){
		size_t pos=line.find_first_of("\t");
		if(pos==string::npos)continue;
		int index = atoi(line.substr(0, pos).c_str());
		assert(index<MAX_FEAT_INX);
		double weight=atof(line.substr(pos+1).c_str());
		_weights[index]=weight;
	}
	ifs.close();
}

double Logistic::eval(Feature* feat){
	Feature* f=feat;
	double weit=_weights[0];
	for(int i=0; i<f->size(); i++){
		weit+=_weights[f->at(i)];
	}
	double prob = logistic(weit);
	return prob;
}
