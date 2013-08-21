package com.renren.sns.wiki.stat.service.fingerprint;

import java.util.List;
import java.util.Map;

/**
 * 抽取feed中的关键文本工具类
 * 
 * @author yi.li@renren-inc.com since Jul 26, 2012
 * 
 */
public class FingerPrintUtil {

    public static long getFeedIdFromWikiFeed(Map<String, Object> feed) {
        return Long.parseLong(feed.get("feedId").toString());
    }

    @SuppressWarnings({ "unchecked", "rawtypes" })
    public static String getSignificantString(Map<Integer, String> textKeyMap,
            Map<String, Object> feed) {
        int feedType = (Integer) feed.get("feedType");
        String sKey = textKeyMap.get(feedType);
        if (null == sKey) {
            return null;
        }

        String[] keys = sKey.split("\\.");
        if (null == keys || keys.length < 1) {
            return null;
        }

        Map<String, Object> tempMap = feed;
        Object tempObj = null;
        for (String titleKey : keys) {
            tempObj = tempMap.get(titleKey);
            if (null == tempObj) {
                return null;
            }

            if (tempObj instanceof List) {
                tempMap = (Map<String, Object>) ((List) tempObj).get(0);
            }
        }

        String str = (String) tempObj;

        return str;
    }

}
