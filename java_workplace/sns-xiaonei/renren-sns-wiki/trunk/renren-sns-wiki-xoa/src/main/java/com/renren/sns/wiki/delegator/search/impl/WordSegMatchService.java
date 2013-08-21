package com.renren.sns.wiki.delegator.search.impl;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import com.renren.sns.wiki.delegator.search.IWordsMatchService;
import com.renren.sns.wiki.delegator.search.MatchResult;
import com.renren.wiki.word.WikiWordService;

/**
 * 利用高锋开发的分词技术提供的词匹配服务
 * 
 * @author yi.li@renren-inc.com since Jul 30, 2012
 * 
 */
public class WordSegMatchService implements IWordsMatchService {

    @Override
    public Map<String, MatchResult[]> matchQuery(Map<String, String> queryMap) {
        Map<String, com.renren.wiki.word.MatchResult[]> results = WikiWordService
                .matchQuery(queryMap);
        if (null == results) {
            return Collections.emptyMap();
        }

        Map<String, MatchResult[]> finalResults = new HashMap<String, MatchResult[]>();
        for (Entry<String, com.renren.wiki.word.MatchResult[]> entry : results.entrySet()) {
            MatchResult[] matches = new WordSegMatchResultDecoder(entry.getValue()).decode();
            finalResults.put(entry.getKey(), matches);
        }

        return finalResults;
    }

    public static class WordSegMatchResultDecoder implements IWordsMatchService.MatchResultDecoder {

        private com.renren.wiki.word.MatchResult[] matchResults;

        public WordSegMatchResultDecoder(com.renren.wiki.word.MatchResult[] matchResults) {
            this.matchResults = matchResults;
        }

        @Override
        public MatchResult[] decode() {
            if (null == matchResults) {
                return null;
            }

            MatchResult[] decodedResults = new MatchResult[matchResults.length];
            int i = 0;
            for (com.renren.wiki.word.MatchResult item : matchResults) {
                MatchResult decodeResult = new MatchResult();
                decodeResult.setMatchString(item.matchedString);
                decodeResult.setWikiId(item.wikiId);
                decodeResult.setBegin(item.begin);
                decodeResult.setEnd(item.end);
                //                decodeResult.setState()

                decodedResults[i++] = decodeResult;
            }

            return decodedResults;
        }

    }
}
