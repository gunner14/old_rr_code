
#include "QuerySuggestion.h"
//#include "IceExt/src/ServiceI.h"

#define MCE_INFO cout


const string QuerySuggestion::DICT_PATH = "DICT_PATH";
const string QuerySuggestion::EXPIRE_TIME = "EXPIRE_TIME";
const string QuerySuggestion::PARTITION_COUNT = "PARTITION_COUNT";
const string QuerySuggestion::CACHE_SIZE_PER_PART = "CACHE_SIZE_PER_PART";


vector<int> QuerySuggestion::search(int id, string& query, int limit, bool isPre) {
	MCE_INFO("in querysuggestion search");
	UserPostingsPtr upostings = instance->Find(id);

	if(upostings) {
		if(upostings->loaded && upostings->isExpired()){
			newBuildIndex(id,upostings);
		}
	} else {
		newBuildIndex(id,upostings);
	}
	vector<int> ids = upostings->searchPostings(query,isPre);
	return sort(ids,limit);
}


vector<int> QuerySuggestion::sort(vector<int>& ids,int limit) {
	vector<int> resIds;
	int count = 0;
	for(vector<int>::iterator it = ids.begin();it != ids.end()&&count < limit;it++) {
		resIds.push_back(*it);
		count++;
	}
	return resIds;
}

void QuerySuggestion::newBuildIndex(int id, UserPostingsPtr& up) {
	map<int,string> int2StrData_;
	int expireTime = atoi(EXPIRE_TIME.c_str());
	up = new UserPostings(id,expireTime);
	int2StrData_ = getInputData(id);
	up->buildIndex(int2StrData_,dict_);
	instance->Insert(id,up);
}

bool QuerySuggestion::init(string path) {
	const char* cf = path.c_str();
	char* sec = new char[strlen(cf) + 1];
	strcpy(sec,cf);

	ifstream infile;
	infile.open(sec);
	if(!infile) {
		MCE_INFO("config source is not exist");
		return false;
	} else {
		infile(sec);
		string line;
		while(getline(infile,line)) {
			line = Util::trim(line);
			vector<string> split = Util::tokenize(line, "=");
			if(split.size() == 2) {
				if(split[0] == DICT_PATH) {
					DICT_PATH = split[1];
				}  else {
					config_[split[0]] = split[1];
				}
			} else {
				MCE_INFO("config souce is error!");
			}
		}

		if(DICT_PATH != "") {
			cf = DICT_PATH.c_str();
			sec = new char[strlen(cf) + 1];
			strcpy(sec,cf);
			string line;
			while(getline(infile,line)) {
				line = Util::trim(line);
				vector<string> split = Util::tokenize(line, " ");
				if(split.size() == 2) {
					dict_[split[0]].push_back(split[1]);
				} else {
					MCE_INFO("dict source is error!");
				}
			}
		} else {
			MCE_INFO("dict path is not exist!");
			return false;
		}

		if(config_.size() != 3) {
			map<string,string>::iterator it = config_.find(EXPIRE_TIME);
			if(it == config_.end()) {
				MCE_INFO("EXPIRE_TIME default!");
				config_[EXPIRE_TIME] = "1200";
			}
			it = config_.find(PARTITION_COUNT);
			if(it == config_.end()) {
				MCE_INFO("PARTITION_COUNT default!");
				config_[PARTITION_COUNT] = "20";
			}
			it = config_.find(CACHE_SIZE_PER_PART);
			if(it == config_.end()) {
				MCE_INFO("CACHE_SIZE_PER_PART default!");
				config_[CACHE_SIZE_PER_PART] = "500";
			}
		}
	}
	EXPIRE_TIME = config_[EXPIRE_TIME];
	instance = new UserPostingsPool(config_);
	return true;
}


