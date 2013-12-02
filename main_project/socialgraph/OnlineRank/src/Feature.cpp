#include "assert.h"
#include "Feature.h"
#include "Access.h"
#include "UserBaseAdapter.h"

#include <fstream>

void GetFeature(int hostid, int guestid, int accesstype, FeatureMap* featMap, Feature* feat){
	assert(feat!=NULL);
	xce::adapter::userbase::UserBaseFullViewPtr userbase1=0, userbase2=0;
	try{
		userbase1 = xce::adapter::userbase::UserBaseAdapter::
			    instance().getUserBaseFullView(hostid);
		userbase2 = xce::adapter::userbase::UserBaseAdapter::
			    instance().getUserBaseFullView(guestid);
	}catch(...){
		return;
	}

	int findex=-1;
	findex=featMap->setHLevel( userbase1->level() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setHStar( userbase1->star() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setHLogin( userbase1->logincount() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setHGender( userbase1->gender() );
	if(findex!=-1)feat->add(findex);

	findex=featMap->setGLevel( userbase2->level() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setGStar( userbase2->star() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setGLogin( userbase2->logincount() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setGGender( userbase2->gender() );
	if(findex!=-1)feat->add(findex);

	findex=featMap->setBYear( userbase1->birthyear(), userbase2->birthyear() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setBCity( userbase1->citycode(), userbase2->citycode() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setBUniv( userbase1->univ(), userbase2->univ() );
	if(findex!=-1)feat->add(findex);
	findex=featMap->setBStage( userbase1->stage(), userbase2->stage() );
	if(findex!=-1)feat->add(findex);

	if(accesstype==SHOW)feat->setLabel(0);
	else	feat->setLabel(1);
}

int FeatureMap::_setFeat(string key){
	int f=0;
	pthread_mutex_lock(&mu);
	FeatureMap::iterator itrFM=this->find(key);
	if(itrFM!=this->end())
		f=itrFM->second;
	else{
		f=index;
		this->insert(pair<string, int>(key, index++));
	}
	pthread_mutex_unlock(&mu);
	return f;
}

int FeatureMap::setHLevel(int hlevel){
	string desc="hlevel_";
	if(hlevel<=0||hlevel>10)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", hlevel);
		desc+=buf;
	}
	return _setFeat(desc);
}
int FeatureMap::setHStar(int hstar){
	string desc="hstar_";
	if(hstar<=0||hstar>10)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", hstar);
		desc+=buf;
	}
	return _setFeat(desc);
}
int FeatureMap::setHLogin(int hlog){
	hlog=hlog/50;
	string desc="hlog_";
	if(hlog<=0||hlog>200)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", hlog);
		desc+=buf;
	}
	return _setFeat(desc);
}
int FeatureMap::setHGender(string hgender){
	string desc="hgender_";
	if(hgender.compare("男生")==0)
		desc+="1";
	else if(hgender.compare("女生"))
		desc+="0";
	else
		desc+="x";
	return _setFeat(desc);
}

int FeatureMap::setGLevel(int glevel){
	string desc="glevel_";
	if(glevel<=0||glevel>10)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", glevel);
		desc+=buf;
	}
	return _setFeat(desc);
}

int FeatureMap::setGStar(int gstar){
	string desc="gstar_";
	if(gstar<=0||gstar>10)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", gstar);
		desc+=buf;
	}
	return _setFeat(desc);
}

int FeatureMap::setGLogin(int glog){
	glog=glog/50;
	string desc="glog_";
	if(glog<=0||glog>200)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", glog);
		desc+=buf;
	}
	return _setFeat(desc);
}

int FeatureMap::setGGender(string ggender){
	string desc="ggender_";
	if(ggender.compare("男生")==0)
		desc+="1";
	else if(ggender.compare("女生")==0)
		desc+="0";
	else
		desc+="x";
	return _setFeat(desc);
}

int FeatureMap::setBYear(int hyear, int gyear){
	string desc="byear_";
	int absyear=abs(hyear-gyear);
	if(absyear<=0||absyear>80)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", absyear);
		desc+=buf;
	}
	return _setFeat(desc);
}
int FeatureMap::setBCity(string hcity, string gcity){
	string desc="bcity_";
	if(hcity==gcity)
		desc+="1";
	else
		desc+="0";
	return _setFeat(desc);
}
int FeatureMap::setBUniv(int huniv, int guniv){
	string desc="buniv_";
	if(huniv==guniv)
		desc+="1";
	else
		desc+="0";
	return _setFeat(desc);
		
}
int FeatureMap::setBStage(int hstage, int gstage){
	string desc="bstage_";
	int absstage=abs(hstage-gstage);
	if(absstage<0||absstage>50)
		desc+="x";
	else{
		char buf[5];
		sprintf(buf, "%d", absstage);
		desc+=buf;
	}
	return _setFeat(desc);
		
}

void FeatureMap::dump(const char* file){
	ofstream ofs(file);
	FeatureMap::iterator itrMap=this->begin();
	for(; itrMap!=this->end(); itrMap++){
		ofs<<itrMap->first<<"\t"<<itrMap->second<<endl;
	}
	ofs.close();
}

void FeatureMap::load(const char* file){
	this->clear();
	ifstream ifs(file);
	string line="";
	int maxid=0;
	while(getline(ifs, line)){
		size_t pos = line.find_first_of("\t");
		if(pos==string::npos)continue;
		string key = line.substr(0, pos);
		int fid = atoi(line.substr(pos+1).c_str());
		if(fid>maxid)maxid=fid;
		this->insert(pair<string, int>(key, fid));
	}
	index = maxid;
	ifs.close();
}
