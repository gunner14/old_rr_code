/*
 * GeneralQuerySuggestion.h
 *
 *  Created on: 2012-8-27
 *      Author: cm
 */

#include <IceUtil/IceUtil.h>
#include "GeneralQS.h"
#include "QuerySuggestion.h"
#include "Util.h"
#include "IceExt/src/ServiceI.h"
#include <boost/algorithm/string.hpp>

using namespace MyUtil;

namespace com {
namespace renren {
namespace search {
namespace generalquerysuggestion {
	class ZJQuerySuggestion : public QuerySuggestion {
		public:
			map<int,string> getInputData(int id);
			vector<int> sort(vector<int>& ids, int limit);
			ZJQuerySuggestion(string& dicPath,string& configPath);
	};

	ZJQuerySuggestion::ZJQuerySuggestion() {
	}

	vector<int> ZJQuerySuggestion::sort(vector<int>& ids, int limit) {
		MCE_INFO("in sort");
		vector<int> resIds;
		int count = 0;
		for(vector<int>::iterator it = ids.begin();it != ids.end()&&count < limit;it++) {
			resIds.push_back(*it);
			count++;
		}
		return resIds;
	}

	map<int,string> ZJQuerySuggestion::getInputData(int id) {
		map<int,string> smap;
		string path = "/data/xce/XiaoNeiSearch/etc/sites.txt";
		MCE_INFO("GeneralQSI::init. path:"  << path );
		MCE_INFO("initial dict_");
		ifstream infile;
		string file;
		infile.open(path.c_str(), ifstream::in);
		if (!infile) {
			MCE_INFO("ScoreCache::LoadClickAndReply. openfile fail:"  );
			return smap;
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
			int id = atoi(strs[0].c_str());
			smap[id] = strs[1];
		}
		MCE_INFO("ZJQuerySuggestion::getInputData. id:" << id);

		//map[1] = "中国";
		//map[2] = "zhongguo";
		//map[3] = "zai北京中国";
		//map[4] = "bei 京";
		//map[5] = "人人网";
		return smap;
	}


	class GeneralQSI : public GeneralQS, public MyUtil::Singleton<GeneralQSI> {
		public:
		void init();
		virtual MyUtil::IntSeq search(int uid, const string& query, int len, int limit, bool isPre, const ::Ice::Current& = ::Ice::Current());
		private:
			ZJQuerySuggestion *qs;
	};
};
};
};
};
