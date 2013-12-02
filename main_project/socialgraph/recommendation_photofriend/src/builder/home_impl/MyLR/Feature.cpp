#include <fstream>
#include <sstream>
#include <math.h>
#include "assert.h"
#include "Feature.h"
#include "Cache.h"

DataDict<string, int> FeatureMap::featMap;
int FeatureMap::index = FeatureMap::featMap.GetInstance()->size() + 1;

void GetFeature(string guest, string host, vector<int>& vec_hid_photo_feature,vector<int>& vec_gid_photo_feature, int type, Feature* feat) {
	//通过用户id获取feature
	assert(feat != NULL);
	Profile profile1 = Cache::getProfileFromCache(guest);
	Profile profile2 = Cache::getProfileFromCache(host);
	int findex = -1;

	findex = FeatureMap::setGGender(profile1.getGender());
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setHGender(profile2.getGender());
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setBYear(profile1.getBirthYear(),
			profile2.getBirthYear());
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setBCity(profile1.getCity(), profile2.getCity());
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setBUniv(profile1.getUniv(), profile2.getUniv());
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setGAcnt(vec_hid_photo_feature[0]);
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setGPcnt(vec_hid_photo_feature[1]);
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setGScnt(vec_hid_photo_feature[2]);
	if (findex != -1)
		feat->add(findex);

	findex = FeatureMap::setHVisitednum(vec_gid_photo_feature[0]);
	if (findex != -1)
		feat->add(findex);

	feat->setLabel(type);

	//测试feature
//	cout << "id : " << guest << "\t";
//	for(int i = 0; i < feat->size(); i++)
//		cout << feat->at(i) << "\t";
//	cout<<endl;
}

void GetFeature(string guest, string host, Profile profile1, Profile profile2, vector<int>& vec_hid_photo_feature,vector<int>& vec_gid_photo_feature, int type, Feature* feat) {
        //通过用户id获取feature
        assert(feat != NULL);
        int findex = -1;

        findex = FeatureMap::setGGender(profile1.getGender());
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setHGender(profile2.getGender());
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setBYear(profile1.getBirthYear(),
                        profile2.getBirthYear());
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setBCity(profile1.getCity(), profile2.getCity());
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setBUniv(profile1.getUniv(), profile2.getUniv());
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setGAcnt(vec_hid_photo_feature[0]);
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setGPcnt(vec_hid_photo_feature[1]);
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setGScnt(vec_hid_photo_feature[2]);
        if (findex != -1)
                feat->add(findex);

        findex = FeatureMap::setHVisitednum(vec_gid_photo_feature[0]);
        if (findex != -1)
                feat->add(findex);

        feat->setLabel(type);
}

FeatureMap::FeatureMap() {
}

int FeatureMap::_setFeat(const string& key) {
	int f = 0;
	map<string, int>::iterator itrFM = featMap.GetInstance()->find(key);
	if (itrFM != featMap.GetInstance()->end())
		f = itrFM->second;
	else {
		f = index;
		featMap.GetInstance()->insert(pair<string, int> (key, index++));
	}
	return f;
}

int FeatureMap::setHGender(const string& hgender) {
	string desc = "hgender_";
	if (hgender.compare("男生") == 0)
		desc += "1";
	else if (hgender.compare("女生") == 0)
		desc += "0";
	else
		desc += "x"; //x代表异常或小概率情况
	return _setFeat(desc);
}
int FeatureMap::setHVisitednum(int vnum) {
	string desc = "hvnum_";
	ostringstream oss;
	int f = 0;
	oss << desc;
	if (vnum < 0|| vnum>400)
		oss << "x";
	else
	{
		f = vnum/5 + 1;
		oss << f;
	}
	return _setFeat(oss.str());
}

int FeatureMap::setGAcnt(int anum) {
	string desc = "ganum_";
	ostringstream oss;
	int f = 0;
	oss << desc;
	if (anum < 0|| anum>200)
		oss << "x";
	else
	{
		f = anum/5 + 1;
		oss << f;
	}
	return _setFeat(oss.str());
}

int FeatureMap::setGPcnt(int pnum) {
	string desc = "gpnum_";
	ostringstream oss;
	int f = 0;
	oss << desc;
	if (pnum < 0 || pnum>300)
		oss << "x";
	else
	{
		f = pnum/5 + 1;
		oss << f;
	}
	return _setFeat(oss.str());
}

int FeatureMap::setGScnt(int snum) {
	string desc = "gsnum_";
	ostringstream oss;
	int f = 0;
	oss << desc;
	if (snum < 0 || snum > 50)
		oss << "x";
	else
	{
                f = snum;
		oss << f;
	}
	return _setFeat(oss.str());
}
int FeatureMap::setGGender(const string& ggender) {
	string desc = "ggender_";
	if (ggender.compare("男生") == 0)
		desc += "1";
	else if (ggender.compare("女生") == 0)
		desc += "0";
	else
		desc += "x"; //x代表异常或小概率情况
	return _setFeat(desc);
}

int FeatureMap::setBYear(int hyear, int gyear) {
	string desc = "byear_";
	ostringstream oss;
	oss << desc;
	int absyear = abs(hyear - gyear);
	if (absyear < 0 || absyear > 80)
		oss << "x"; //x代表异常或小概率情况
	else
		oss << absyear;
	return _setFeat(oss.str());
}
int FeatureMap::setBCity(const string& hcity, const string& gcity) {
	string desc = "bcity_";
	if (hcity == gcity)
		desc += "1";
	else
		desc += "0";
	return _setFeat(desc);
}
int FeatureMap::setBUniv(int huniv, int guniv) {
	string desc = "buniv_";
	if (huniv == guniv)
		desc += "1";
	else
		desc += "0";
	return _setFeat(desc);
}

void FeatureMap::dump(const string& filename) {
	ofstream out(filename.c_str());
	map<string, int>::iterator itrMap = featMap.GetInstance()->begin();
	for (; itrMap != featMap.GetInstance()->end(); ++itrMap) {
		out << itrMap->first << "\t" << itrMap->second << endl;
	}
	out.close();
}

