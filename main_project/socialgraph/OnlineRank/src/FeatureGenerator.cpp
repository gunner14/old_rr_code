#include "FeatureGenerator.h"
#include "UserBaseAdapter.h"

using namespace std;

FeatureGenerator::FeatureGenerator(Access* acc, FeatureMap* featMap, const char* save_prefix){
	_acc=acc;
	_featMap=featMap;
	_save_prefix=save_prefix;

}

void FeatureGenerator::run(){
	char save[255];
	sprintf(save, "%s_%d", _save_prefix, _threadnum);
	ofstream ofs(save);
	int end=_part_num+_scope;
	Feature *feat = new Feature();
	for(int i=_part_num; i<end; i++){
		if((i-_part_num)%100==0){
			printf("thread %d running..., %.2f%\n", _threadnum, ((float)(i-_part_num))/_scope*100.0);
		}
		int host=_acc->at(i).uid;
		map<int, int> uas=_acc->at(i).userAccess;
		map<int, int>::iterator itrAcc=uas.begin();
		for(; itrAcc!=uas.end(); itrAcc++){
			int subject=itrAcc->first;
			int type=itrAcc->second;
			GetFeature(host, subject, type, _featMap, feat);
			feat->dump(ofs);
			feat->reset();
		}
	}
	ofs.close();
	delete feat;
}

