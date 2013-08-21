package com.renren.sns.wiki.stat.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.dict.WikiDictionaryDAO;
import com.renren.sns.wiki.model.dict.WikiWord;

/**
 * Wiki切词字典服务
 * 
 * @author yi.li@renren-inc.com since 2012-6-26
 * 
 */
@Service
public class WikiWordsDictService {

    private static Map<String, Integer> dict = new HashMap<String, Integer>();

    private Log logger = LogFactory.getLog(WikiWordsDictService.class);

    private volatile boolean isInitilized = false;

    @Autowired
    private WikiDictionaryDAO wikiDictionaryDAO;

    /**
     * 根据词条获取wikiId
     * 
     * @param words 词条信息
     * @return 词条对应的wikiId列表，如果没有相应的wiki则返回大小为0的List
     */
    public List<Integer> getWikiIds(List<String> words) {
        initDict();
        List<Integer> wikiIds = new ArrayList<Integer>();

        for (String word : words) {
            Integer wikiId = dict.get(word);
            if (null != wikiId) {
                wikiIds.add(wikiId);
            }
        }

        return wikiIds;
    }

    public void initDict() {
        if (!isInitilized) {
            synchronized (this) {
                if (!isInitilized) {
                    List<WikiWord> words = wikiDictionaryDAO.getWikiWords(0, 520);

                    if (logger.isDebugEnabled()) {
                        logger.debug("Find wiki words: " + (null == words ? 0 : words.size()));
                    }

                    for (WikiWord word : words) {
                        dict.put(word.getName(), word.getWikiId());
                    }
                    isInitilized = true;
                }

            }
        }
    }
}
