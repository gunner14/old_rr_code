package com.renren.sns.wiki.utils;

import org.jsoup.Jsoup;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-18
 * 
 */
public class HtmlUtils {

    /**
     * 过滤掉html标签
     * 
     * @param html
     * @return
     */
    public static String html2text(String html) {
        if (html == null) {
            return null;
        }
        return Jsoup.parse(html).text();
    }

}
