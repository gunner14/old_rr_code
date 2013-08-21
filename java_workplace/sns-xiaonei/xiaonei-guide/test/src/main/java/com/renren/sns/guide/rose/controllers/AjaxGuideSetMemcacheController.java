package com.renren.sns.guide.rose.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午04:07:15
 */
public class AjaxGuideSetMemcacheController {
    
    @Post
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        String memKey=inv.getRequest().getParameter("memKey");
        MemCacheManager mem = com.xiaonei.platform.core.cache.MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide); 
        mem.set(memKey, "display", (int)com.xiaonei.platform.core.cache.MemCacheKeys.day*21);
        return null;
    }

}
 