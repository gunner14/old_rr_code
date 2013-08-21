package com.renren.sns.wiki.delegator.search;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.delegator.search.impl.SearchMatchService;
import com.renren.sns.wiki.delegator.search.impl.WordSegMatchService;

/**
 * @author yi.li@renren-inc.com since Jul 30, 2012
 * 
 */
public class WordsMatchServiceFactory {

    private static volatile IWordsMatchService matchService;

    private static Object lock = new Object();

    private static Log logger = LogFactory.getLog(WordsMatchServiceFactory.class);

    public static IWordsMatchService getWordsMatchService() {
        if (null == matchService) {
            synchronized (lock) {
                if (null == matchService) {
                    String useSeg = System.getProperty("renren.wiki.useSeg");
                    if (logger.isDebugEnabled()) {
                        logger.debug("Wiki xoa server use segMatch? " + (null != useSeg));
                    }
                    if (null != useSeg) {
                        matchService = new WordSegMatchService();
                    } else {
                        matchService = new SearchMatchService();
                    }
                }
            }
        }

        return matchService;
    }
}
