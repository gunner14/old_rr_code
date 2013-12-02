/*
 * UserPosting.cpp
 *
 *  Created on: 2012-8-29
 *      Author: cm
 */

#include "UserPostings.h"
#include "Util.h"
#include <algorithm>


//=======buildIndex=====

void UserPostings::splitWord(string& value, vector<SplitItem>& words) {
  value = Util::trim(value);
  vector<string> splits = Util::tokenize(value, " ");
  for(vector<string>::const_iterator it = splits.begin(); it != splits.end();it++) {
    string val = *it;
    string subStr="";
    for(vector<string>::size_type i = 0;i < val.size();i++) {
      if((val[i]&0x80) != 0) {
        if(subStr.size() > 0) {
          SplitItem si;
          si.item = subStr;
          si.isHZ = false;
          words.push_back(si);
          subStr = "";
        }
        SplitItem si;
        si.item = val.substr(i,3);
        si.isHZ = true;
        words.push_back(si);
        i += 2;
      } else {
        if(subStr.size()==0) {
          subStr = string(1, val[i]);
        } else {
        	subStr.append(1,val[i]);
        }
      }
    }
    if(subStr.size() > 0) {//切分的词中没有中文
      SplitItem si;
      si.item = subStr;
      si.isHZ = false;
      words.push_back(si);
    }
  }
}
//========buildIndex====

//=========search===

void UserPostings::getIsPreMatch(vector<DocItem> midResults,vector<DocItem>& tmpMidIds) {
	for(vector<DocItem>::iterator it = midResults.begin();it != midResults.end();it++) {
		DocItem di = *it;
		for(vector<int>::iterator itt = di.positions.begin();itt != di.positions.end();itt++) {
			if(*itt == 0) {
				tmpMidIds.push_back(di);
				break;
			}
		}
	}
}

void UserPostings::searchWDInternal(string& query, vector<DocItem>& result, vector<string> subStr,int start,int num,
		bool isPre,bool firstWD) {
	if(start >= query.size()) {
		vector<DocItem> tmpResult;
		subStr.resize(num);
		bool first = true;
		for(vector<string>::iterator it = subStr.begin();it != subStr.end();it++) {
			vector<DocItem> midResult;
			map<string,vector<DocItem> >::iterator itt = postingsWD.lower_bound(*it);
			for(map<string,vector<DocItem> >::iterator iter = itt;iter != postingsWD.end();iter++) {
				if(Util::startsWith(iter->first,*it)) {
					vector<DocItem> tmp = postingsWD[iter->first];
					union_DocItem(midResult,tmp);
				} else {
					break;
				}
			}

			if(firstWD&&first&&isPre) {
				vector<DocItem> tmpMidIds;
				getIsPreMatch(midResult,tmpMidIds);
				if(tmpMidIds.size() != midResult.size()) {
					midResult.assign(tmpMidIds.begin(),tmpMidIds.end());
				}
			}
			intersect_DocItem(tmpResult,midResult,isPre);
			first = false;
			if(tmpResult.size() == 0){
				return;
			}
		}
		if(tmpResult.size() > 0) {
			union_DocItem(result,tmpResult);
		}
		return;
	} else {
		for(unsigned int end = start + 1;end <= query.size();++end) {
			string temp = query.substr(start,end-start);
			subStr[num] = temp;
			searchWDInternal(query,result,subStr,end,num+1,isPre,firstWD);
		}
	}
}


void  UserPostings::searchWD(string& query, vector<DocItem>& ids, bool isPre,bool firstWD) {
	vector<string> subStr(query.size());
	searchWDInternal(query, ids, subStr, 0, 0,isPre,firstWD);
}

void UserPostings::searchHZ(string& query, vector<DocItem>& ids) {
	map<string, vector<DocItem> >::iterator it = postingsHZ.find(query);
	if(it != postingsHZ.end()) {
		ids.assign(it->second.begin(),it->second.end());
	}
}

void UserPostings::union_DocItem(vector<DocItem>& A, vector<DocItem>& B) {
	vector<DocItem> C;
	vector<DocItem>::iterator itA = A.begin();
	vector<DocItem>::const_iterator itB = B.begin();

	while(itA != A.end() && itB != B.end()) {
		if((*itA).docId == (*itB).docId) {
			DocItem item;
			item.docId = (*itA).docId;
			vector<int> posA = (*itA).positions;
			Util::union_vec(posA, (*itB).positions);
			item.positions = posA;
			C.push_back(item);
			++itA;
			++itB;
		} else if ((*itA).docId < (*itB).docId) {
			C.push_back(*itA);
			++itA;
		} else {
			C.push_back(*itB);
			++itB;
		}
	}

	while(itA != A.end()){
		C.push_back(*itA);
		++itA;
	}

	while(itB != B.end()){
		C.push_back(*itB);
		++itB;
	}

	A.swap(C);
}

void UserPostings::intersect_DocItem(vector<DocItem>& A, vector<DocItem>& B, bool isPre) {
	vector<DocItem> C;
  if(A.size() == 0){
	  for(vector<DocItem>::iterator it = B.begin();it != B.end();it++) {
		  DocItem si = *it;
		  C.push_back(si);
	  }
	  A.swap(C);
	  return;
  }

	vector<DocItem>::iterator itA = A.begin();
	vector<DocItem>::const_iterator itB = B.begin();

	while(itA != A.end() && itB != B.end()) {
		if((*itA).docId == (*itB).docId) {
			DocItem item;
			item.docId = (*itA).docId;

			vector<int> positionsA = (*itA).positions;
			vector<int> positionsB = (*itB).positions;
			for (vector<int>::iterator posItB = positionsB.begin(); posItB != positionsB.end(); posItB++) {
				int posB = *posItB;
				for (vector<int>::iterator posItA = positionsA.begin(); posItA != positionsA.end(); posItA++) {
					int posA = *posItA;
					if (isPre && ((posA + 1) == posB)) {
						item.positions.push_back(posB);
					} else if (!isPre && (posA < posB)) {
						item.positions.push_back(posB);
					}
				}
			}

			if(item.positions.size() > 0)
				C.push_back(item);
			++itA;
			++itB;
		} else if((*itA).docId > (*itB).docId) {
			++itB;
		}else {
			++itA;
		}
	}
	A.swap(C);
}

vector<int> UserPostings::searchPostings(string& query, bool isPre) {

	transform(query.begin(), query.end(), query.begin(), ::tolower);

	vector<SplitItem> merQuerys;
	splitWord(query, merQuerys);

	vector<int> result;
	vector<DocItem> ids;

	bool isFirst = true;
	for(vector<SplitItem>::iterator it = merQuerys.begin();it != merQuerys.end();it++) {
		bool firstHZ = false, firstWD = false;
		SplitItem si = *it;
		vector<DocItem> res;
		if(si.isHZ) {
			if(isFirst)
				firstHZ = true;
			searchHZ(si.item, res);
		} else {
			if(isFirst)
				firstWD = true;
			searchWD(si.item, res, isPre,firstWD);
		}

		intersect_DocItem(ids, res, isPre);

		if(isFirst&&isPre&&firstHZ) {
			vector<DocItem> tmpIds;
			getIsPreMatch(ids,tmpIds);
			if(tmpIds.size() != ids.size()) {
				ids.assign(tmpIds.begin(),tmpIds.end());
			}
		}
		isFirst = false;

		if(ids.size() == 0) {
			break;
		}
	}

	for(vector<DocItem>::iterator it = ids.begin();it != ids.end();it++) {
			int docId = it->docId;
			vector<int>::iterator itt = find(result.begin(),result.end(),docId);
			if(itt == result.end()) {
				result.push_back(docId);
			}
		}
	return result;
}
//========

//==========buildIndex========

void UserPostings::addWord(map<string, vector<DocItem> >& posting, string& word, int docId, int pos) {
	map<string, vector<DocItem> >::iterator it = posting.find(word);
	if (it == posting.end()) {
		DocItem item;
		item.docId = docId;
		item.positions.push_back(pos);
		posting[word].push_back(item);
	} else {
		DocItem& lastItem = *(posting[word].rbegin());
		if (lastItem.docId == docId) {
			lastItem.positions.push_back(pos);
		} else {
			DocItem item;
			item.docId = docId;
			item.positions.push_back(pos);
			posting[word].push_back(item);
		}
	}
}


void UserPostings::buildIndex(map<int,string>& dataInput, map<string,vector<string> >& dict) {
	string name;
	unsigned int id;
	for(map<int,string>::iterator it = dataInput.begin();it != dataInput.end();++it) {
		name = it->second;
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		vector<SplitItem> words;
		splitWord(name, words);

		id = it->first;
		for(unsigned int i = 0;i < words.size();i++) {
			SplitItem si = words[i];
			if(words[i].isHZ) {
				addWord(postingsHZ, words[i].item, id, i);
				map<string, vector<string> >::iterator iter = dict.find(words[i].item);
				if(iter != dict.end()) {
					vector<string> pinyins = iter->second;
					for(vector<string>::iterator itt = pinyins.begin();itt != pinyins.end();itt++) {
						addWord(postingsWD, *itt, id, i);
					}
				} else {
					//log
				}
			} else {
				addWord(postingsWD, words[i].item, id, i);
			}
		}
	}

	timestamp = time(0);
	loaded = true;
}

