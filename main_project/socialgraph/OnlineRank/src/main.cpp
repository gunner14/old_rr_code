#include "Feature.h"
#include "FeatureGenerator.h"
#include "param.h"
#include "model.h"
#include "TaskManager.h"

#include <iostream>
using namespace std;

static const Option opts[]={
	{"mode", 'm', "eval", "char*", "mode to run the program eval|gen"},
	{"nthread", 't', "1", "int", "num of thread to run feature generator."},
	{"access", 'a', "", "char*", "filepath of user access"},
	{"model", 'l', "model.txt", "char*", "filepath of model"},
	{"featmap", 'f', "feat.map", "char*", "filepath of featmap"},
	{"output", 'o', "./xx__", "char*", "filepath to save outputs(featmap, feature)"},
	{"help", 'h', 0, "bool", "show help"},
	{0, 0, 0, 0, 0}
};

Access* acc = NULL;
int nthread=0;
string input="";
string model="";
string mapfile="";
string prefix="";


void eval();
void gen();
void fof();

int main(int argc, char** argv){
	Param param;
	if(!param.open(argc, argv, opts) || param.get<bool>("help")){
		cout<<param.help()<<endl;
		exit(1);
	}
	param.dump_config(&cout);
	string mode = param.get<string>("mode");
	nthread = param.get<int>("nthread");
	input = param.get<string>("access");
	model = param.get<string>("model");
	mapfile = param.get<string>("featmap");
	prefix = param.get<string>("output");

	acc = new Access(input.c_str());
	if(mode=="eval"){
		eval();
	}else if(mode == "gen"){
		gen();
	}else if(mode=="fof"){
		fof();
	}else{
		exit(1);
	}
	delete acc;
	return 1;
}
void eval(){
	Logistic* plr = new Logistic(model.c_str());
	FeatureMap* featMap = new FeatureMap();
	featMap -> load(mapfile.c_str());
	Tasks fgtasks(nthread);
	fgtasks.eval(acc, featMap, plr);
	delete plr;
}
void gen(){
	FeatureMap* featMap = new FeatureMap();
	Tasks fgtasks(nthread);
	fgtasks.genfeat(acc, featMap, prefix.c_str());
	featMap->dump(mapfile.c_str());
}

void fof(){
	Tasks fgtasks(nthread);
	fgtasks.fofeval(acc);
}
