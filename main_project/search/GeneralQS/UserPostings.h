/*
 * UserPosting.h
 *
 *  Created on: 2012-8-29
 *      Author: cm
 */

#ifndef USERPOSTINGS_H_
#define USERPOSTINGS_H_

#include <set>
#include <vector>
#include <map>
#include <string>
#include <boost/thread/mutex.hpp>
using namespace std;

class UserPostings {
public:
	UserPostings(int _uid,int expireTime) : uid(_uid),loaded(false),timestamp(time(0)),expired_Time(expireTime){}
  void buildIndex(map<int,string>& inData,map<string,vector<string> >& dict);
  vector<int> searchPostings(string& query, bool isPre);

  int GetUid() const { return uid; }
  	bool isExpired() {
  	  time_t now = time(0);
  	  return now - timestamp > expired_Time;
  	}
  time_t timestamp;
  boost::mutex mutex_;
  bool loaded;

private:
	struct DocItem {
		int docId;
		vector<int> positions;
	};
	struct SplitItem {
		string item;
		bool isHZ;
	};
  void splitWord(string& value, vector<SplitItem>& words);
  void searchHZ(string& query,vector<DocItem>& ids);
  void searchWD(string& query,vector<DocItem>& ids, bool isPre,bool firstWD);
  void addWord(map<string, vector<DocItem> >& posting, string& word, int docId, int pos);
  void searchWDInternal(string& query, vector<DocItem>& result, vector<string> subStr,int start,int num, bool isPre,bool firstWD);
  void union_DocItem(vector<DocItem>& A, vector<DocItem>& B);
  void intersect_DocItem(vector<DocItem>& A, vector<DocItem>& B, bool isPre);
  void getIsPreMatch(vector<DocItem> midResults,vector<DocItem>& tmpMidIds);

  time_t expired_Time;
	int uid;
	map<string, vector<DocItem> > postingsHZ;
	map<string, vector<DocItem> > postingsWD;
};

#endif /* USERPOSTING_H_ */
