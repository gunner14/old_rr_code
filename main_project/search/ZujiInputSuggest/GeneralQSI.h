/*
 * GeneralQuerySuggestion.h
 *
 *  Created on: 2012-8-27
 *      Author: cm
 */

#include <IceUtil/IceUtil.h>
#include "GeneralQS.h"
#include "search/GeneralInputSuggest/src/QuerySuggestion.h"
#include "search/GeneralInputSuggest/src/Util.h"
#include "IceExt/src/ServiceI.h"
#include <boost/algorithm/string.hpp>


using namespace MyUtil;


namespace com {
namespace renren {
namespace search {
namespace generalquerysuggestion {
	class ZJQuerySuggestion : public QuerySuggestion {
		public:
			void  setInputData(int id,map<long,string>& data);
			vector<long> sort(vector<long>& ids, int limit);
			ZJQuerySuggestion(){};
	};

	vector<long> ZJQuerySuggestion::sort(vector<long>& ids, int limit) {
		MCE_INFO("in sort");
		vector<long> resIds;
		int count = 0;
		for(vector<long>::iterator it = ids.begin();it != ids.end()&&count < limit;it++) {
			resIds.push_back(*it);
			count++;
		}
		return resIds;
	}

	void ZJQuerySuggestion::setInputData(int id,map<long,string>& inputData) {
		string path = "/data/xce/XiaoNeiSearch/etc/sites.txt";
		MCE_INFO("GeneralQSI::init. path:"  << path );
		MCE_INFO("initial dict_");
		ifstream infile;
		string file;
		infile.open(path.c_str(), ifstream::in);
		if (!infile) {
			MCE_INFO("ScoreCache::LoadClickAndReply. openfile fail:"  );
			return;
		} else {
			MCE_INFO("ScoreCache::LoadClickAndReply. openfile OK:" );
		}

		string line;
		int cnt = 0;
		while (getline(infile, line)) {
			int to, featureid;//, click, reply, receive;
			vector<string> strs;
			boost::split(strs, line, boost::is_any_of("\t "));
			MCE_INFO(strs[0] << " " << strs[1] << " orig:" << line);
      long id = atol(strs[0].c_str());
			inputData[id] = strs[1];
		}
		MCE_INFO("ZJQuerySuggestion::getInputData. id:" << id);
	}


	class GeneralQSI : public GeneralQS, public MyUtil::Singleton<GeneralQSI> {
		public:
      void init();
			virtual MyUtil::LongArray search(int uid, const string& query,int limit, bool isPre, const ::Ice::Current& = ::Ice::Current());
		private:
			QuerySuggestion *qs;
	};
};
};
};
};
