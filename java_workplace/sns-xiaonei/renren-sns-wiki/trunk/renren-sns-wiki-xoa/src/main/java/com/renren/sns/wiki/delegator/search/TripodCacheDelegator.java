package com.renren.sns.wiki.delegator.search;

import org.springframework.stereotype.Service;

import com.renren.sns.wiki.xoa.UGCType;
import com.renren.sns.wiki.xoa.util.WikiWordsCacheUtil;

/**
 * 通用Cache代理类
 * 
 * @author yi.li@renren-inc.com since 2012-6-25
 * 
 */
@Service
public class TripodCacheDelegator {

    /**
     * 将对应UGC内容的wiki切词结果存储至Cache中
     * 
     * @param ugcType UGC类型
     * @param sourceId UGC ID
     * @param wikiWords Wiki切词结果数据
     */
    public void setToCache(UGCType ugcType, long sourceId, String wikiWords) {
        WikiWordsCacheUtil.setWikiWords(ugcType, sourceId, wikiWords);
    }
}
