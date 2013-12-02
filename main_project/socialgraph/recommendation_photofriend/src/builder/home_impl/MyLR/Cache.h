/*
 * Cache.cpp
 *
 *  Created on: 2012-9-24
 *      Author: yxy
 */

#ifndef CACHE_CPP_
#define CACHE_CPP_
#include <string>
#include <iostream>
#include <list>
#include "Profile.h"
using namespace std;

class Cache {
public:
	Cache();
	~Cache();
	static Profile getProfileFromCache(const string& id); //获取用户资料
	static map<string, Profile> getProfilesFromCache(list<string> ids); //批量获取用户资料

	static void getApsvFromCache(const string& id, vector<int>& vec);//获取id的apsv
private:
	static string _getKey(const string& line);
	static string _getValue(const string& line);
	static void _setProfile(Profile& profile, const string& accessValue);

};

#endif /* CACHE_CPP_ */
