package com.renren.sns.wiki.delegator.search.impl;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import com.renren.sns.wiki.delegator.search.IWordsMatchService;
import com.renren.sns.wiki.delegator.search.MatchResult;
import com.xiaonei.xce.search.match.SearchMatchAdapter;

/**
 * 传玉的状态机服务提供的此匹配服务
 * 
 * @author yi.li@renren-inc.com since Jul 30, 2012
 * 
 */
public class SearchMatchService implements IWordsMatchService {

    @Override
    public Map<String, MatchResult[]> matchQuery(Map<String, String> queryMap) {
        Map<String, com.renren.search.MatchResult[]> results = SearchMatchAdapter.getInstance()
                .matchQuery(queryMap);
        if (null == results) {
            return Collections.emptyMap();
        }

        Map<String, MatchResult[]> finalResults = new HashMap<String, MatchResult[]>();
        for (Entry<String, com.renren.search.MatchResult[]> entry : results.entrySet()) {
            MatchResult[] matches = new SearchMatchResultDecoder(entry.getValue()).decode();
            finalResults.put(entry.getKey(), matches);
        }

        return finalResults;
    }

    public static class SearchMatchResultDecoder implements IWordsMatchService.MatchResultDecoder {

        private com.renren.search.MatchResult[] matchResults;

        public SearchMatchResultDecoder(com.renren.search.MatchResult[] matchResults) {
            this.matchResults = matchResults;
        }

        @Override
        public MatchResult[] decode() {
            if (null == matchResults) {
                return null;
            }

            MatchResult[] decodedResults = new MatchResult[matchResults.length];
            int i = 0;
            for (com.renren.search.MatchResult item : matchResults) {
                MatchResult decodeResult = new MatchResult();
                decodeResult.setMatchString(item.matchedString);
                decodeResult.setWikiId(item.wikiId);
                decodeResult.setBegin(item.begin);
                decodeResult.setEnd(item.end);

                decodedResults[i++] = decodeResult;
            }

            return decodedResults;
        }

    }
}
