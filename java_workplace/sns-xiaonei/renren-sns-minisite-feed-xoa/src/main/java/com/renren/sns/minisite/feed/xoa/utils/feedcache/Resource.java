package com.renren.sns.minisite.feed.xoa.utils.feedcache;

import com.renren.xcache.XCache;


public class Resource {

    final public static XCache<Long> CACHE = XCache.getCache("minisite.feeds", Long.class);

    final public static XCache<Long> INDEX_CACHE = XCache.getCache("minisite.feeds_index",
            Long.class, true);

}
