package com.renren.sns.lead.dataproviders;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

public interface ILeadInitMehtod{
	void doing(Invocation inv,Portal portal, User host, CacheManager cm);
}