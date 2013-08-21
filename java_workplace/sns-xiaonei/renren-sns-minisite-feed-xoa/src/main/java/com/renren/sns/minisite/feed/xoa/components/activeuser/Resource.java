package com.renren.sns.minisite.feed.xoa.components.activeuser;

import com.renren.xcache.XCache;

public class Resource {

    final public static XCache<Long> CACHE = XCache.getCache("minisite.activeuser", Long.class);

    final public static String KEY = "active_id_";
}
