
#include "QuerySuggestion.h"
#include "Util.h"
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <algorithm>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define MCE_INFO cout<<


const string QuerySuggestion::DICT_PATH = "DICT_PATH";
const string QuerySuggestion::BATCH_SIZE = "BATCH_SIZE";


vector<long> QuerySuggestion::search(string& query, int limit, bool isPre, map<long,float>& dynamicScore) {
	MCE_INFO("in querysuggestion search")<<endl;

	vector<long> ids = searchPostings(query,isPre);
	return sort(ids,limit,dynamicScore);
}

void* func(void* ptr) {
	((QuerySuggestion*)ptr)->dumpData();
	pthread_exit(NULL);
}

void QuerySuggestion::dumpData() {
	const char* cf = filePath_.c_str();
	char* sec = new char[strlen(cf) + 1];
	strcpy(sec,cf);
	fstream ffile;
	ffile.open(sec);

	if(!ffile) {
		return;
	}
	ofstream ofile(sec,ios::app);//append
	for(map<long,IndexData>::iterator it = batchData_.begin();it != batchData_.end();it++) {
		IndexData ida = it->second;
		ofile<<it->first<<" "<<ida.name<<" "<<ida.staticScore<<endl;
	}
	ofile.close();
	ffile.close();
	batchData_.clear();
}


void QuerySuggestion::addData(long id,string data,float staticScore) {
	if(indexData_.size() != 0) {
		map<long,IndexData>::iterator it = indexData_.find(id);
		string name;
		float score;
		IndexData idat;
		if(it != indexData_.end()) {
			idat = it->second;
			name = idat.name;
			score = idat.staticScore;
			if(name == data && score == staticScore) {
				return;
			}
		}
	}

	IndexData ida;
	ida.name = data;
	ida.staticScore = staticScore;
	batchData_[id] = ida;
	indexData_[id] = ida;

	pthread_t tid;
	bool createThread = false;
	if(batchData_.size() == batch_size) {
		createThread = true;
		int ret;
		ret = pthread_create(&tid,NULL,func,this);
		if(ret != 0) {
			MCE_INFO("create thread error")<<endl;
			return;
		}
	}

	buildIndex(id,data);

	if(createThread)
		pthread_join(tid,NULL);
}


vector<long> QuerySuggestion::sort(vector<long>& ids,int limit,map<long,float>& dynamicScore) {
	vector<long> resIds;
	int count = 0;
	//从indexData_中可获取id对应的staticScore，和dynamicScore相加后排序
	for(vector<long>::iterator it = ids.begin();it != ids.end()&&count < limit;it++) {
		resIds.push_back(*it);
		count++;
	}
	return resIds;
}


bool QuerySuggestion::init(string configPath,string filePath) {
	std::cout<<"begin init~"<<std::endl;
	const char* cf = configPath.c_str();
	char* sec = new char[strlen(cf) + 1];
	strcpy(sec,cf);
	fstream ffile;
	ffile.open(sec);
	if(!ffile) {
		MCE_INFO("config file is not exist")<<endl;
		return false;
	}
	ffile.close();

	//initial batch_size
	ifstream infile(sec);
	string line;
	while(getline(infile,line)) {
		line = Util::trim(line);
		vector<string> split = Util::tokenize(line, "=");
		if(split.size() == 2) {
			if(split[0] == DICT_PATH) {
				dict_path = split[1];
			}  else if (split[0] == BATCH_SIZE){
				batch_size = atoi(split[1].c_str());
			}
		} else {
			MCE_INFO("config souce is error!");
		}
	}
	infile.close();

	//initial dict_
	if(dict_path != "") {
		const char* cfdict = dict_path.c_str();
		char* dict_sec = new char[strlen(cfdict) + 1];
		strcpy(dict_sec,cfdict);
		fstream dict_file;
		dict_file.open(dict_sec);
		if(!dict_file) {
			MCE_INFO("pinyin table is not exist")<<endl;
		} else {
			ifstream dict_infile(dict_sec);
				while(getline(dict_infile,line)) {
					line = Util::trim(line);
					vector<string> split = Util::tokenize(line, " ");
					if(split.size() == 2) {
						dict_[split[0]].push_back(split[1]);
					} else {
						MCE_INFO("dict source is error!")<<endl;
					}
				}
				dict_infile.close();
		}
		dict_file.close();
	} else {
		MCE_INFO("dict path is not exist!")<<endl;
		return false;
	}

	//initial indexData,buildIndex
	filePath_ = filePath;
	const char* cffile = filePath_.c_str();
	char* file_sec = new char[strlen(cffile)+1];
	strcpy(file_sec,cffile);
	fstream file_file;
	file_file.open(file_sec);
	if(!file_file) {
		MCE_INFO("fileData is not exist")<<endl;
	} else {
		ifstream file_infile(file_sec);
		while(getline(file_infile,line)) {
			line = Util::trim(line);
			vector<string> split = Util::tokenize(line, " ");
			string data = split[1];
			int size = split.size();
			for(int index = 2;index < size - 1;index++) {
				data.append(" ").append(split[index]);
			}
			buildIndex(atol(split[0].c_str()),data);
			IndexData ida;
			ida.name = data;
			ida.staticScore = atol(split[size-1].c_str());
			indexData_[atol(split[0].c_str())] = ida;
		}
		file_infile.close();
	}
	file_file.close();
	return true;
}


void QuerySuggestion::splitWord(string& value, vector<SplitItem>& words) {
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
    if(subStr.size() > 0) {
      SplitItem si;
      si.item = subStr;
      si.isHZ = false;
      words.push_back(si);
    }
  }
}

void QuerySuggestion::getIsPreMatch(vector<DocItem> midResults,vector<DocItem>& tmpMidIds) {
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

void QuerySuggestion::reSearchHZ(string& query, vector<DocItem>& ids) {
	vector<DocItem> res;
	res.swap(ids);
	map<long,IndexData>::iterator it;
	long id;
	IndexData ida;
	DocItem ditem;
	string name;
	string::size_type index;
	for(vector<DocItem>::iterator iter = res.begin();iter != res.end();iter++) {
		ditem = *iter;
		id = ditem.docId;
		it = indexData_.find(id);
		if(it != indexData_.end()) {
			ida = it->second;
			name = ida.name;
			index = name.find(query.c_str());
			if(index == string::npos) {//update
				std::cout<<"not find and erase"<<endl;
			} else {
				ids.push_back(ditem);
			}
		} else {
			MCE_INFO("data error!");
		}
	}
}


void QuerySuggestion::searchHZ(string& query, vector<DocItem>& ids) {
	map<string, vector<DocItem> >::iterator it = postingsHZ.find(query);
	if(it != postingsHZ.end()) {
		ids.assign(it->second.begin(),it->second.end());
	}
	if(ids.size() != 0) {
		reSearchHZ(query,ids);
	}
}

void QuerySuggestion::reSearchWD(vector<string> subStr, vector<DocItem>& tmpResult) {
	vector<DocItem> res;
	res.swap(tmpResult);
	map<long,IndexData>::iterator it;
	IndexData ida;
	long id;
	string name;
	DocItem ditem;
	vector<SplitItem> sitem;
	int len = subStr.size();
	for(vector<DocItem>::iterator iter = res.begin();iter != res.end();iter++) {
		ditem = *iter;
		id = ditem.docId;
		it = indexData_.find(id);
		if(it != indexData_.end()) {
			ida = it->second;
			name = ida.name;
			sitem.clear();
			splitWord(name,sitem);
			int inc = 0;
			SplitItem si;
			vector<string> pinyins;
			map<string,vector<string> >::const_iterator dicIter;
			for(vector<SplitItem>::iterator itt = sitem.begin();itt != sitem.end()&&inc < len;itt++) {
				si = *itt;
				if(si.isHZ) {
					dicIter = dict_.find(si.item);
					if(dicIter != dict_.end()) {
						pinyins = dicIter->second;
						for(vector<string>::iterator pinIter = pinyins.begin();pinIter != pinyins.end();pinIter++) {
							string pinyin = *pinIter;
							if(Util::startsWith(pinyin,subStr[inc])) {
								++inc;
								break;
							}
						}
					} else {
						MCE_INFO("the dict_ is not enough!");
					}
				} else {
					if(Util::startsWith(si.item,subStr[inc])) {
						++inc;
					}
				}
			}
			if(inc == len) {
				tmpResult.push_back(ditem);
			}
		} else {
			MCE_INFO("data error!");
		}
	}
}


void QuerySuggestion::searchWDInternal(string& query, vector<DocItem>& result, vector<string> subStr,int start,int num,
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
		//reSearchWD
		if(tmpResult.size() != 0) {
			reSearchWD(subStr, tmpResult);
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

void  QuerySuggestion::searchWD(string& query, vector<DocItem>& ids, bool isPre,bool firstWD) {
	vector<string> subStr(query.size());
	searchWDInternal(query, ids, subStr, 0, 0,isPre,firstWD);
}


void QuerySuggestion::union_DocItem(vector<DocItem>& A, vector<DocItem>& B) {
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


void QuerySuggestion::intersect_DocItem(vector<DocItem>& A, vector<DocItem>& B, bool isPre) {
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


vector<long> QuerySuggestion::searchPostings(string& query, bool isPre) {

	transform(query.begin(), query.end(), query.begin(), ::tolower);

	vector<SplitItem> merQuerys;
	splitWord(query, merQuerys);

	vector<long> result;
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
			vector<long>::iterator itt = find(result.begin(),result.end(),docId);
			if(itt == result.end()) {
				result.push_back(docId);
			}
		}
	return result;
}


void QuerySuggestion::addWord(map<string, vector<DocItem> >& posting, string& word, long docId, int pos) {
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


void QuerySuggestion::buildIndex(long id ,string data) {
	string name = data;
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	vector<SplitItem> words;
	splitWord(name, words);

	for(unsigned int i = 0;i < words.size();i++) {
		SplitItem si = words[i];
		if(words[i].isHZ) {
			addWord(postingsHZ, words[i].item, id, i);
			map<string, vector<string> >::iterator iter = dict_.find(words[i].item);
			if(iter != dict_.end()) {
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




