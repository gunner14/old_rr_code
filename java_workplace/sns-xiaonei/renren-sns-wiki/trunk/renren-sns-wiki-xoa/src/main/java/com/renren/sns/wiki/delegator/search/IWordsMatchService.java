package com.renren.sns.wiki.delegator.search;

import java.util.Map;

/**
 * @author yi.li@renren-inc.com since Jul 30, 2012
 * 
 */
public interface IWordsMatchService {

    /**
     * 切词服务。将输入的单个或多个文本用key-value的形式组织起来切词，<br>
     * 切词结果也会已key-value的形式返回
     * 
     * map里的 key value分别代表 key 唯一标识 value 需要匹配的字符串
     * 
     * @param queryMap
     * @return Map<String, MatchResult[]> key 代表唯一标识，value 代表匹配到的结果
     */
    public Map<String, MatchResult[]> matchQuery(Map<String, String> queryMap);

    /**
     * 分词结果转换器，将不同服务的分词结果转换成统一的形式
     */
    public static interface MatchResultDecoder {

        public MatchResult[] decode();
    }
}
