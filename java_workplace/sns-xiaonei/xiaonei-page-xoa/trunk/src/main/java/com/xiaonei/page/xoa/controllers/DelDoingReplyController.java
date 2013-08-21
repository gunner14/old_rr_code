package com.xiaonei.page.xoa.controllers;

import java.sql.SQLException;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.platform.component.doing.home.DoingFeedUtil;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("ddr")
public class DelDoingReplyController {
	@Autowired
	private AuthService authService;

	@Autowired
	private DoingService doingService;

	@Autowired
	private PageService pageService;

	private final static Logger logger = Logger.getLogger("PAGEXOA");

	/**
	 * 
	 * @param pageId
	 * @param doingId
	 * @param doingReplyId
	 * @param opUserId
	 * @return 删除成功返回相应replyId；否则，返回0
	 */
	@Get
	public String index(@Param("pageId") int pageId,
			@Param("doingId") int doingId,
			@Param("doingReplyId") long doingReplyId,
			@Param("opUserId") int opUserId) {
		User puser = SnsAdapterFactory.getUserAdapter().get(pageId);
		User host = WUserAdapter.getInstance().get(opUserId);
		Page page = null;
		
		try {
			page = pageService.get(pageId);
		} catch (Exception e) {
			logger.error(e.getMessage(), e);
			return "@0";
		}
		if(page==null){
			return "@0";
		}
		// 以page所有者的身份执行删除
		if (authService.isPageAdministrator(opUserId, pageId)) {
			host = puser;
		}
		// 删除回复表的对应记录
		if (doingService.delReplyDoingWithLog(doingReplyId, doingId,
				puser.getId(), host.getId(), FeedDefinition.PAGE_STATUS_UPDATE,
				null) > 0) {
			try {
				this.publishFeed(page, doingReplyId, doingId, puser);
			} catch (SQLException e) {
				logger.error(e.getMessage(), e);
				// igonre this
			}
			return "@" + doingReplyId;
		}

		return "@0";
	}

	private void publishFeed(Page page, long replyId, int doingId, User puser)
			throws SQLException {
		Doing doing = DoingHome.getInstance().getDoingById(doingId,
				page.getId());
		// User doingUser = WUserAdapter.getInstance().get(page.getId());

		// 发feed，同步回复列表，只给page发吧
		logger.info(String.format(
				"publish del status reply feed:page=%s,replyId=%s,doingId=%s",
				page.getId(), replyId, doingId));
		DoingFeedUtil.deleteDoingFeedReply(puser, doing, replyId,
				(int) FeedDefUtil.getSTATUS_UPDATE_REPLY_Def(page));
	}
}
