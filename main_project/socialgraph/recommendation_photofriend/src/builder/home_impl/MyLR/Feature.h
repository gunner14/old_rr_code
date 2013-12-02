#ifndef _FEATURE_H_
#define _FEATURE_H_
#include <string>
#include <iostream>
#include "Profile.h"
#include "socialgraph/recommendation_photofriend/src/common/data_dict.h"
using namespace xce::socialgraph;
using namespace std;

#define MAX_FEAT_INX 1000

class Feature {
public:
	Feature() {
		_index = new int[MAX_FEAT_INX];
		memset(_index, 0, sizeof(int) * MAX_FEAT_INX);
		_size = 0;
		_label = -1;
	}
	virtual ~Feature() {
		delete[] _index;
		_index = NULL;
		_label = -1;
	}
	inline void add(int fid) {
		_index[_size++] = fid;
	}
	inline void setLabel(int l) {
		_label = l;
	}
	inline int getLabel() {
		return _label;
	}
	inline size_t size() {
		return _size;
	}
	inline int at(size_t i) {
		return _index[i];
	}
	inline void reset() {
		_size = 0;
		_label = -1;
	}
	inline void dump(ostream &os) {
		if (_size == 0)
			return;
		for (size_t i = 0; i < _size; i++) {
			os << _index[i] << " ";
		}
		os << _label << endl;
	}
public:
	int* _index;
	size_t _size;
	int _label;
};

class FeatureMap {
public:
	FeatureMap();
	virtual ~FeatureMap() {
	}
	static int setGGender(const string& ggender);
	static int setGAcnt(int anum);
	static int setGPcnt(int pnum);
	static int setGScnt(int snum);

	static int setHGender(const string& hgender);
	static int setHVisitednum(int vnum);

	static int setBUniv(int guniv, int huniv);
	static int setBYear(int gbirth, int hbirth);
	static int setBCity(const string& gcity, const string& hcity);

	static void dump(const string& filename);

	static DataDict<string, int> featMap;
	static int index;
private:
	static int _setFeat(const string& key);
};

void GetFeature(string guest, string host, vector<int>& vec_hid_photo_feature, vector<int>& vec_gid_photo_feature, int type, Feature* feat);
void GetFeature(string guest, string host, Profile profile1, Profile profile2, vector<int>& vec_hid_photo_feature,vector<int>& vec_gid_photo_feature, int type, Feature* feat);
#endif
