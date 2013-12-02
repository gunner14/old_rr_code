/*
 * QuerySuggestion.h
 *
 *  Created on: 2012-8-22
 *      Author: cm
 */


#ifndef QUERYSUGGESTION_H_
#define QUERYSUGGESTION_H_

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "UserPostingsPool.h"

using namespace std;

class QuerySuggestion {
public:
	QuerySuggestion() {	};
	bool init(string path);
	vector<int> search(int id, string& query,int limit, bool isPre);
	virtual ~QuerySuggestion(){ delete instance ;};

	const static string DICT_PATH;
	const static string EXPIRE_TIME;
	const static string PARTITION_COUNT;
	const static string CACHE_SIZE_PER_PART;

protected:
	virtual map<int,string> getInputData(int id) = 0;
	virtual vector<int> sort(vector<int>& ids,int limit);
	struct SplitItem {
		string item;
		bool isHZ;
	};
	map<string, vector<string> > dict_;
	map<string,string> config_;
private:
	UserPostingsPool* instance;
	void newBuildIndex(int id, UserPostingsPtr& up);
};

#endif /* QUERYSUGGESTION_H_ */
