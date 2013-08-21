package com.renren.sns.wiki.biz.feed.impl;

import java.util.Map;

import com.xiaonei.platform.component.xfeed.display.model.Feed;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-2
 * 
 */
public interface IFeed2Map {

    /**
     * 实现类中需要加入jsonObject.put("globalId", globalId.toString());用户后面集中获取喜欢信息
     * 
     * @param feed
     * @return
     */
    public abstract Map<String, Object> convert(Feed feed);

}
