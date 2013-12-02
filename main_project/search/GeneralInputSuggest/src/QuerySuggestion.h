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

using namespace std;

class QuerySuggestion {
public:
	QuerySuggestion() {	};
	bool init(string configPath,string filePath);
	void addData(long id,string data,float staticScore = 0);
	vector<long> search(string& query,int limit, bool isPre, map<long,float>& dynamicScore);
	virtual ~QuerySuggestion(){	};
	void dumpData();

	const static string DICT_PATH;
	const static string BATCH_SIZE;

protected:
	virtual vector<long> sort(vector<long>& ids,int limit,map<long,float>& dynamicScore);
	struct SplitItem {
		string item;
		bool isHZ;
	};

private:
	map<string, vector<string> > dict_;

	struct IndexData {
		string name;
		float staticScore;
	};
	map<long,IndexData> batchData_;
	map<long,IndexData> indexData_;
	string dict_path;
	int batch_size;

	struct DocItem {
		long docId;
		vector<int> positions;
	};
	map<string, vector<DocItem> > postingsHZ;
	map<string, vector<DocItem> > postingsWD;
	string filePath_;


	void buildIndex(long id,string data);
	vector<long> searchPostings(string& query, bool isPre);

  void splitWord(string& value, vector<SplitItem>& words);
  void searchHZ(string& query,vector<DocItem>& ids);
  void searchWD(string& query,vector<DocItem>& ids, bool isPre,bool firstWD);
  void addWord(map<string, vector<DocItem> >& posting, string& word, long docId, int pos);
  void searchWDInternal(string& query, vector<DocItem>& result, vector<string> subStr,int start,int num, bool isPre,bool firstWD);
  void union_DocItem(vector<DocItem>& A, vector<DocItem>& B);
  void intersect_DocItem(vector<DocItem>& A, vector<DocItem>& B, bool isPre);
  void getIsPreMatch(vector<DocItem> midResults,vector<DocItem>& tmpMidIds);
  void reSearchHZ(string& query, vector<DocItem>& ids);
  void reSearchWD(vector<string> subStr, vector<DocItem>& result);
};

#endif /* QUERYSUGGESTION_H_ */
