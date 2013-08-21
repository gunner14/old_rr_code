package com.renren.sns.wiki.xoa.util;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import xce.tripod.client.EntryType;
import xce.tripod.client.TripodCacheClient;
import xce.tripod.client.TripodCacheClientFactory;

import com.renren.sns.wiki.xoa.UGCType;

/**
 * 用来从缓存中读取内容相关的切词结果数据
 * 
 * @author yi.li@renren-inc.com since 2012-4-19
 * 
 */
public class WikiWordsCacheUtil {

    //这里跟wiki service中的namespace是统一个copy为了减少依赖，在这里复制一份
    private static final String COMMON_PREFIX = "wds_";

    private static final String DOING_PREFIX = COMMON_PREFIX + "d";

    private static final String DOING_COMMENT_PREFIX = COMMON_PREFIX + "dc";

    private static final String BLOG_PREFIX = COMMON_PREFIX + "b";

    private static final String BLOG_COMMENT_PREFIX = COMMON_PREFIX + "bc";

    private static final String SHARE_PREFIX = COMMON_PREFIX + "s";

    private static final String SHARE_COMMENT_PREFIX = COMMON_PREFIX + "sc";

    private static final String PHOTO_PREFIX = COMMON_PREFIX + "p";

    private static final String PHOTO_COMMENT_PREFIX = COMMON_PREFIX + "pc";

    private static final String ALBUM_PREFIX = COMMON_PREFIX + "a";

    private static final String ALBUM_COMMENT_PREFIX = COMMON_PREFIX + "ac";

    private static final int CACHE_EXPIRE_SECONDS = 60 * 60 * 6;

    /**
     * 根据UGC类型及其资源id在缓存中获取对应的wiki切词结果
     * 
     * @param ugcType UGC类型
     * @param sourceId ugcId
     * @return 获取wikiWords的数据，如果miss则返回null
     */
    public static String getWikiWords(UGCType ugcType, long sourceId) {
        TripodCacheClient tCache = TripodCacheClientFactory
                .getClient(EntryType.BIZ_SOCIAL_WIKI_FEED);
        String keyPrefix = getCacheKeyPrefix(ugcType);
        String result = (String) tCache.get(keyPrefix + sourceId);
        return result;
    }

    /**
     * 删除UGC类型及其资源id在缓存中对应的wiki切词结果
     * 
     * @param ugcType UGC类型
     * @param sourceId ugcId
     */
    public static void removeWikiWords(UGCType ugcType, long sourceId) {
        TripodCacheClient tCache = TripodCacheClientFactory
                .getClient(EntryType.BIZ_SOCIAL_WIKI_FEED);
        String keyPrefix = getCacheKeyPrefix(ugcType);
        tCache.remove(keyPrefix + sourceId);
    }

    /**
     * 批量获取UGC内容对应的wiki切词结果
     * 
     * @param ugcType UGC类型
     * @param sourceIds ugcId
     * @return 返回一个Map类型的数据，如果某个id对应的数据不在cache中，在map中获取该id的数据会返回null
     */
    public static Map<String, String> batchGetWikiWords(UGCType ugcType, List<String> sourceIds) {
        TripodCacheClient tCache = TripodCacheClientFactory
                .getClient(EntryType.BIZ_SOCIAL_WIKI_FEED);
        String keyPrefix = getCacheKeyPrefix(ugcType);
        List<String> queryKeys = new ArrayList<String>();
        for (String sourceId : sourceIds) {
            queryKeys.add(keyPrefix + sourceId);
        }

        Map<String, Object> temp_result = new HashMap<String, Object>();
        temp_result = tCache.multiGet(queryKeys);
        if (null == temp_result || temp_result.size() == 0) {
            return Collections.emptyMap();
        }

        Map<String, String> result = new HashMap<String, String>();
        Set<String> keys = temp_result.keySet();
        for (String key : keys) {
            result.put(key.substring(keyPrefix.length()), (String) temp_result.get(key));
        }
        return result;
    }

    /**
     * 向缓存中设置wikiWords
     * 
     * @param ugcType UGC类型
     * @param sourceId ugcId
     * @param wikiWords wikiWords数据
     */
    public static void setWikiWords(UGCType ugcType, long sourceId, String wikiWords) {
        TripodCacheClient tCache = TripodCacheClientFactory
                .getClient(EntryType.BIZ_SOCIAL_WIKI_FEED);
        String keyPrefix = getCacheKeyPrefix(ugcType);
        tCache.set(keyPrefix + sourceId, wikiWords, CACHE_EXPIRE_SECONDS);
    }

    /**
     * 粗暴的方式获取CachePrefix，尽量保持util轻量，今后类型多的时候这里就是重构点
     */
    private static String getCacheKeyPrefix(UGCType ugcType) {
        if (ugcType == UGCType.DOING) {
            return DOING_PREFIX;
        }
        if (ugcType == UGCType.DOING_COMMENT) {
            return DOING_COMMENT_PREFIX;
        }
        if (ugcType == UGCType.BLOG) {
            return BLOG_PREFIX;
        }
        if (ugcType == UGCType.BlOG_COMMENT) {
            return BLOG_COMMENT_PREFIX;
        }
        if (ugcType == UGCType.SHARE) {
            return SHARE_PREFIX;
        }
        if (ugcType == UGCType.SHARE_COMMENT) {
            return SHARE_COMMENT_PREFIX;
        }
        if (ugcType == UGCType.PHOTO) {
            return PHOTO_PREFIX;
        }
        if (ugcType == UGCType.PHOTO_COMMENT) {
            return PHOTO_COMMENT_PREFIX;
        }
        if (ugcType == UGCType.ALBUM) {
            return ALBUM_PREFIX;
        }
        if (ugcType == UGCType.ALBUM_COMMENT) {
            return ALBUM_COMMENT_PREFIX;
        }
        return "";
    }

}
