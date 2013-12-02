#include "MatchManagerI.h"

using namespace com::renren::search;
using namespace xce::serverstate;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	MCE_INFO("initialize -----------------------");
	service.getAdapter()->add(&MatchManagerI::instance(), service.createIdentity("MatchManager",""));
	ServerStateSubscriber::instance().initialize("ControllerMatchManager",&MatchManagerI::instance(), 2, 5, new MyUtil::GadgetsChannel);
}


MatchManagerI::MatchManagerI() {
	DbUtil dbutil;
	int host = 1;
	int wiki = 1;
	while (true) {
		std::vector<Wiki> wiki_list = dbutil.getWikiListById(host, wiki);
		if (wiki_list.empty()) {
			break;
		}
		wiki = wiki_list.back().id;
		vector<Wiki>::iterator iter;
		for(iter = wiki_list.begin(); iter != wiki_list.end(); iter++) {
			if (iter->name == "") {
				continue;
			}
			BYTE *wikichr = (BYTE*)iter->name.c_str();
			trie_.addNode(wikichr, iter->name, iter->id, iter->state);
			MCE_INFO("vector wiki id: " << iter->id << ", name: " << iter->name << ", state: " << iter->state);
		}
		MCE_INFO("vector wiki id: " << wiki);
	}
	trie_.prepare();
}

MatchResultListMap MatchManagerI::searchMatch(const QueryMap& query, const Ice::Current& crt) {
	MatchResultListMap map;
	MyUtil::TimeCost tc = MyUtil::TimeCost::create("search match", 1);

	for (QueryMap::const_iterator mIterator = query.begin(); mIterator != query.end(); ++mIterator)
	{
		BYTE *searchChr = (BYTE*)mIterator->second.c_str();
		Searcher searcher(trie_, searchChr);
		MatchResultList list;
		while (searcher.hasNext())
		{
			SearchResult *searchResult = searcher.next();
			State *state = searchResult->getLastMatchedState();
			OutputMap *output = state->getOutput();

			for (OutputMap::iterator aIterator = output->begin(); aIterator	!= output->end(); ++aIterator)
			{
				//Send it to deletion
				//std::cout << "size: " << aIterator->second.size() << " minus->" << searchResult->getMinusCount() << " str->" << aIterator->second << " id->" << aIterator->first << " last->" << searchResult->getLastIndex() - searchResult->getMinusCount() << std::endl;
				MatchResult matchResult;
				matchResult.wikiId = aIterator->first;
				matchResult.matchedString = aIterator->second.name;
				// begin in matchResult is used as state flag
				matchResult.begin = aIterator->second.st;
				matchResult.end = searchResult->getLastIndex() - searchResult->getMinusCount();
				list.push_back(matchResult);
			}
			if (searchResult != NULL) {
				delete searchResult;
			}
		}
		map.insert(MatchResultListMap::value_type(mIterator->first, list));
	}
	return map;
}

