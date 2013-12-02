module com {
	module renren {
		module search {
			struct MatchResult {
				int wikiId;
				string matchedString;
				int begin;
				int end;
			};

			sequence<MatchResult> MatchResultList;
			dictionary<string, MatchResultList> MatchResultListMap;

			dictionary<string, string> QueryMap;

			interface MatchManager
			{
				MatchResultListMap searchMatch(QueryMap query);
			};
		};
	};
};
