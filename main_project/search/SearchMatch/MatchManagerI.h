#include <IceUtil/IceUtil.h>
#include "SearchMatch.h"
#include "DbUtil.h"
#include "AcTypes.h"
#include "AcTrie.h"
#include "IceExt/src/ServiceI.h"
#include "IceExt/src/Channel.h"
#include "Searcher.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "string.h"
#include <boost/lexical_cast.hpp>
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "util/cpp/TimeCost.h"

namespace com {
	namespace renren {
		namespace search {
			class MatchManagerI : public MatchManager, public MyUtil::Singleton<MatchManagerI> {
				public:
					MatchManagerI();
					virtual ~MatchManagerI(){MCE_INFO("~~SearchMatch");}
					virtual com::renren::search::MatchResultListMap searchMatch(const ::com::renren::search::QueryMap& query, const Ice::Current& crt=Ice::Current());	
				private:
					AcTrie trie_;
			};
		};
	};
};
