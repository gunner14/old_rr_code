package com.renren.sns.wiki.utils.cache;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;

/**
 * @author yi.li@renren-inc.com since 2012-3-22
 * 
 */
public class CacheTestCase {

    public static void main(String args[]) {
        CacheDelegator.set(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Integer.class, "123", 1);
        Integer i = CacheDelegator.get(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Integer.class,
                "123");
        System.out.println("@result: " + i);

        CacheDelegator.delete(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Integer.class, "123");
        i = CacheDelegator.get(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Integer.class, "123");
        System.out.println("@result: " + i);

        CacheDelegator.set(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Integer.class, "124", 1);
        CacheDelegator.set(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Integer.class, "125", 3);
        List<String> ids = new ArrayList<String>();
        ids.add("124");
        ids.add("125");
        Map<String, Integer> result = CacheDelegator.getMulti(CacheNameSpace.TEST,
                CachePrefix.TEST_MIN, Integer.class, ids);
        System.out.println("Multi:" + result);

        Integer[] result1 = CacheDelegator.getMultiAsArray(CacheNameSpace.TEST,
                CachePrefix.TEST_MIN, Integer.class, ids);
        System.out.println("Multi as Array:" + result1.length);

        List<Integer> result2 = CacheDelegator.getMultiAsList(CacheNameSpace.TEST,
                CachePrefix.TEST_MIN, Integer.class, ids);
        System.out.println("Multi as List:" + result2);

        Long query = CacheDelegator.get(new QueryAction<Long>(CacheNameSpace.TEST,
                CachePrefix.TEST_MIN, Long.class, "333") {

            @Override
            public Long getFromOther() {
                return 30L;
            }
        }, null);
        System.out.println("query action:" + query);
        query = CacheDelegator.get(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Long.class, "333");
        System.out.println("query result: " + query);

        try {
            Thread.sleep(90000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        result = CacheDelegator.getMulti(CacheNameSpace.TEST, CachePrefix.TEST_MIN, Integer.class,
                ids);
        System.out.println(result);

    }
}
