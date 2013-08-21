package com.renren.sns.guide.business;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Window;

import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-5-26 上午11:21:57
 */
public interface GuideWindowHandler{
    public void execute(Invocation inv, Window window, CacheManager cm);
}
 