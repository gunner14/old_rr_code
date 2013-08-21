package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.FeedService;
import com.xiaonei.page.service.GossipService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.util.Util;
import com.xiaonei.platform.component.feed.helper.FeedDefinition;
import com.xiaonei.platform.component.gossip.home.GossipHome;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.adminPermission.AdminManager;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

@Path("dg")
public class DelGossipController {

	private final static Logger logger = Logger.getLogger("PAGEXOA");

	@Autowired
	private GossipService gossipService;

	@Autowired
	private FeedService feedService;

	@Autowired
	private AuthService authService;

	@Autowired
	private PageService pageService;

	@Get
	@Post
	public String index(@Param("pageId") int pageId,
			@Param("gossipId") long gossipId, @Param("opUserId") int executorId) {

		User host = WUserAdapter.getInstance().get(executorId);
		if (host == null) {
			return "@该用户不存在！";
		}
		boolean isadmin = false;
		Page page = pageService.get(pageId);
		if (page != null) {
			isadmin = authService.isPageAdministrator(host, pageId);
		} else {
			return "该@page不存在！";
		}
		logger.info("isAdmin:"+isadmin);
		try {
			GossipView gossip = GossipHome.getInstance().getGossip(pageId,
					gossipId);
			//page管理员和有权限的网站管理员可以强制删除留言，用户只能删除自己发出的留言
			if (pageId > 0
					&& gossip != null
					&& (executorId == gossip.getGuestId()
							|| isadmin
							|| ((!Util.isLoverPages(page) && host.getAuth() >= 10)) || (Util
							.isLoverPages(page) && AdminManager.getInstance()
							.hasRight(host.getId(), 189, 64)))) {

				gossipService.deleteGossipWithLog(gossipId, pageId, executorId,
						null);	
				// 删除新鲜事
				if (isadmin) {
					feedService.removeFeed(gossipId,
							(int) FeedDefinition.PAGE_GOSSIP_ADMIN, host);
				} else {
					feedService.removeFeed(gossipId,
							(int) FeedDefinition.PAGE_GOSSIP, host);
				}
			}else{
				return "@你无权删除该留言！";
			}
		} catch (Exception e) {
			logger.error("删除留言失败", e);
			return "@删除留言失败";
		}
		return "@" + String.valueOf(gossipId);
	}
}
