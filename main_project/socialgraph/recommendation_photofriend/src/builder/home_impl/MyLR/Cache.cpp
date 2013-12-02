#include "mysql/mysql.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "OceCxxAdapter/src/SocialGraphFoflrTripodAdapter.h"
#include "Cache.h"
using namespace xce::socialgraph;


Cache::Cache() {
}

Cache::~Cache() {
}

Profile Cache::getProfileFromCache(const string& id) {
	Profile profile(id);
	string str;
	if (!SocialGraphFoflrTripodAdapter::instance().Get(id, &str))
		return profile;
	_setProfile(profile, str);
//	cout << id << "\t" << profile.getUniv() << "\t" << profile.getGender()
//			<< "\t" << profile.getBirthYear() << "\t" << profile.getCity()
//			<< endl;
	return profile;
}

map<string, Profile> Cache::getProfilesFromCache(list<string> ids) {
	map < string, Profile > profileMap;
	map < string, string > values;
	map<string, bool> result =
			SocialGraphFoflrTripodAdapter::instance().MultiGet(ids, &values);
	list<string>::iterator ids_it = ids.begin();
	map<string, string>::iterator values_it = values.begin();
	while (ids_it != ids.end()) {
		string id = *ids_it;
		Profile profile(id);
		values_it = values.find(id);
		if (values_it != values.end()) {
			_setProfile(profile, values_it->second);
		}
		profileMap.insert(pair<string, Profile> (id, profile));
		++ids_it;
	}
	return profileMap;
}

string Cache::_getKey(const string& line) {
	size_t pos = line.find("\t");
	return line.substr(0, pos);
}

string Cache::_getValue(const string& line) {
	size_t pos = line.find("\t");
	return line.substr(pos + 1);
}

void Cache::_setProfile(Profile& profile, const string& accessValue) {
	vector < string > vec;
	boost::split(vec, accessValue, boost::is_any_of("\t"));
	if (vec.size() >= 18) {
		if (vec[0] != "")
			profile.setUniv(atoi(vec[0].c_str()));
		if (vec[4] != "")
			profile.setGender(vec[4]);
		if (vec[8] != "")
			profile.setBirthYear(atoi(vec[8].c_str()));
		if (vec[9] != "")
			profile.setCity(vec[9]);
	}
}
