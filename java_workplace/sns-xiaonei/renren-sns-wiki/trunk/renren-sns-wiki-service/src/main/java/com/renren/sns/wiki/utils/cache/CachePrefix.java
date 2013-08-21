package com.renren.sns.wiki.utils.cache;

import static java.util.concurrent.TimeUnit.MINUTES;
import static java.util.concurrent.TimeUnit.HOURS;
import static java.util.concurrent.TimeUnit.DAYS;

/**
 * Cache key 的前缀枚举类
 * <p>
 * 就是key的类型，真正的Key的组成规则是{namespace}.{prefix}:{id}
 * 
 * <p>
 * <Strong>注意：</Strong>所有Key的类型都要在这里定义，以便集中管理
 * 
 * @author yi.li@renren-inc.com since 2012-3-22
 * 
 */
public enum CachePrefix {
    WIKI("wiki", (int) HOURS.toSeconds(6)), 
    WIKI_COVER("wiki_cover", (int) HOURS.toSeconds(6)),
    
    // 该prefix 为保留字，会存储wiki切词的信息，为了避免xoa-api中依赖
    // 在这里约定不要再使用以该prefix开始的prefix
    WIKI_WORDS("wds_", (int) HOURS.toSeconds(6)),
    WIKI_DUPLICATE_CONTENT("wdc_", (int) HOURS.toSeconds(1)),
    
    WIKI_HIGHLIGHT_PHOTO("whp_", (int) HOURS.toSeconds(6)),  
    WIKI_HIGHLIGHT_VEDIO("whv_", (int) HOURS.toSeconds(6)),
    WIKI_RECOMMENDATION("wrcmd_", (int) HOURS.toSeconds(6)),
    
    // below are just example not for wiki business
    TEST_MIN("mins", (int) MINUTES.toSeconds(1)), 
    TEST_HOUR("hours", (int) HOURS.toSeconds(1)), 
    TEST_DAY("days", (int) DAYS.toSeconds(1)),
    
    WIKI_FEED_PHOTO("wfp_", (int) DAYS.toSeconds(10)),
    
    WIKI_ADMIN_TICKET("wiki_admin_t_", (int)HOURS.toSeconds(6)),
    //用于缓存主页显示模块的列表,缓存15天
    WIKI_PROFILE_USER_LIST("wpul_", (int)DAYS.toSeconds(15));

    private CachePrefix(String prefix, int expireSeconds) {
        this.prefix = prefix;
        this.expireSeconds = expireSeconds;
    }

    /**
     * key的前缀类型
     */
    private String prefix;

    /**
     * 缓存过期时间（秒）
     */
    private int expireSeconds;

    public String getPrefix() {
        return prefix;
    }

    public int getExpireSeconds() {
        return expireSeconds;
    }

}
