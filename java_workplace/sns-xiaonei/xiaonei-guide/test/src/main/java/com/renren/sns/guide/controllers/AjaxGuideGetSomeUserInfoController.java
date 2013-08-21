package com.renren.sns.guide.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.portal.Portal;
import net.paoding.rose.web.portal.Window;

import com.renren.sns.guide.utils.GuideUtils;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.LogTimeUtils;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.ObjectProvider;

@LoginRequired
@Path( { "aggsui" })
public class AjaxGuideGetSomeUserInfoController {

	@Get
	public String index(Invocation inv, Portal portal, CacheManager cm)
			throws InterruptedException {
		portal.setTimeout(700);//超时700ms就切除
		try {
			long begin = LogTimeUtils.logBegin();
			String strUserid = inv.getRequest().getParameter("uid");
			int userId = Integer.parseInt(strUserid);
			User user = SnsAdapterFactory.getUserAdapter().get(userId);
			inv.addModel("user", user);
			ObjectProvider.getInstance().set(cm, GuideUtils.AJAX_GUIDE_USER, user);			
			addWindow(portal, "window_guidegettargetuseralbumcount", "guideGetTargetUserAlbumCount");
			addWindow(portal, "window_guidegettargetuserblogcount", "guideGetTargetUserBlogCount");
			addWindow(portal, "window_guidegettargetuserdoing", "guideGetTargetUserDoing");
			addWindow(portal, "window_guidegettargetuserfriendcount", "guideGetTargetUserFriendCount");
			addWindow(portal, "window_guidegettargetusermiddleschool", "guideGetTargetUserMiddleSchool");
			LogTimeUtils.logEnd(begin, "AjaxGuideGetSomeUserInfoController");			
		} catch (Exception e) {
			e.printStackTrace();
		}
		return "/views/guide/ajax/ajax_guide_get_some_user_info.jsp";
	}

	private void addWindow(Portal portal, String name, String path) {
		try {
			Window window = portal.addWindow(name, "/window/guideWindows/" + path);
			window.set(Window.FUTURE_CANCEL_ENABLE_ATTR, false);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
