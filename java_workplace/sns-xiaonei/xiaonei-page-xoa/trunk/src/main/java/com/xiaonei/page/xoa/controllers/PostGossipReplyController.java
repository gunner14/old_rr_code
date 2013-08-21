package com.xiaonei.page.xoa.controllers;

import java.util.regex.Pattern;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.admin.biz.model.auditPage.AuditPagePostView.AuditPageType;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.exceptions.BizValidateException;
import com.xiaonei.page.exceptions.SpamException;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.GossipService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.SettingsService;
import com.xiaonei.page.service.SpamService;
import com.xiaonei.page.util.MetionEncrytedUtil;
import com.xiaonei.platform.component.gossip.exception.NoPermissionException;
import com.xiaonei.platform.component.gossip.exception.PostGossipException;
import com.xiaonei.platform.component.gossip.home.GossipHome;
import com.xiaonei.platform.component.gossip.model.Gossip;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.framework.exception.AntispamException;
import com.xiaonei.platform.framework.exception.FormValidationException;

@Path("pgr")
public class PostGossipReplyController {

	@Autowired
	private PageService pageService;

	@Autowired
	private AuthService authService;

	@Autowired
	private SettingsService settingsService;

	@Autowired
	private GossipService gossipService;

	@Autowired
	private SpamService spamService;

	private final static Logger logger = Logger.getLogger("PAGEXOA");

	public static final Pattern PATTERN_GOSSIP = Pattern.compile(
			"<p>(.*?)</p>$", Pattern.DOTALL);// 找出上一级的内容

	@Get
	public String index(@Param("pageId") int pageId,
			@Param("gossipId") long gossipId, @Param("opUserId") int opUserId,
			@Param("replyTo") int replyTo,
			@Param("replyContent") String replyContent) {
		if (replyTo == 0) {
			return "@INVALID_REPLYID！";
		}

		User host = WUserAdapter.getInstance().get(opUserId);
		if (host == null) {
			return "@GET_USER_FAILED";
		}
		Page page = pageService.get(pageId);
		boolean isAdmin = authService.isPageAdministrator(opUserId, pageId);
		logger.info("isAdmin:" + isAdmin);
		boolean canGossip = settingsService.fansCanSendGossip(page);
		logger.info("canGossip:" + canGossip);
		boolean isFans = authService.isPageActiveFans(opUserId, pageId);
		logger.info("isFans:" + isFans);
		/* 合法性验证和Spam验证 */
		CheckResult cr = null;
		try {
			gossipService.validateComment(host, isAdmin, isFans, replyContent,
					canGossip);
			cr = SpamService.getInstance().commentValidate(isAdmin, host, page,
					replyContent,
					spamService.getCheckType(CheckType.PAGE_GOSSIP_TYPE, page));
		} catch (BizValidateException e) {
			logger.error(e.getMessage(), e);
			return "@BizValidateException";
		} catch (SpamException e) {
			logger.error(e.getMessage(), e);
			return "@SpamException";
		} catch (Exception e) {
			logger.error(e.getMessage(), e);
			return "@ERROR_WHEN_READ_DB";
		}
		if (cr == null) {
			logger.error("cr is null!");
			return "@ERROR_WHEN_WRITE_DB";
		}
		/* 合法性验证和Spam验证 --- 结束 */
		replyContent = cr.getSafePureContent();

		// 加入@功能和短链接功能
		String inputContent = replyContent;
		try {
			String feedContent = MetionEncrytedUtil
					.getMetionEncrytedContentNotPublishDoing(replyContent,host.getId());
			replyContent = feedContent;
		} catch (Exception e) {
			logger.error(e.getMessage(), e);
		}

		Gossip gp = null;
		User guest = null;
		try {

			guest = WUserAdapter.getInstance().get(page.getId());
			// 回复
			// 回复留言
			// 封装回复内容，把被回复的内容加上
			GossipView gv = GossipHome.getInstance().getGossip(page.getId(),
					gossipId);
			if (gv != null) {
				replyContent = gossipService.composeBody(replyContent, gv,
						PATTERN_GOSSIP);
			} else {
				logger.warn(String
						.format("gossip isadmin=%s from %d to %d ,gossipReplyToId=%d,gv=%s not found",
								isAdmin, host.getId(), page.getId(), replyTo,
								gossipId));
			}
			// 管理员回复
			if (isAdmin) {
				logger.info("admin gossip reply!");
				gp = GossipHome.getInstance().postGossip(guest, guest, replyTo,
						replyContent, null, null, null, true, false, false);
				gossipService.createAdminReplyFangle(page, gp, host, replyTo);
				// 发新鲜事
				gossipService.publishAdminFeed(page, gp, replyContent, host,
						null);
				if (logger.isInfoEnabled()) {
					logger.info(String
							.format("gossip isadmin=%s from %d to %d ,gossipReplyToId=%d",
									isAdmin, host.getId(), page.getId(),
									replyTo));
				}
			} else {
				logger.info("user gossip reply!");
				// 粉丝回复
				gp = GossipHome.getInstance().postGossip(host, guest, replyTo,
						replyContent, null, null, null, true, false, false);
				// 暂时不记录远程log
				gossipService.createFansReplyFangle(page, gp, host, replyTo,
						null);
				if (logger.isInfoEnabled()) {
					logger.info(String
							.format("gossip isadmin=%s from %d to %d ,gossipReplyToId=%d",
									isAdmin, host.getId(), page.getId(),
									replyTo));
				}
			}
			// 审核检查
			spamService.auditComment(page, cr, replyContent, host, gp.getId(),
					"" + page.getId(), AuditPageType.PAGE_GOSSIP.getType());
		} catch (AntispamException e) {
			logger.error(e.getMessage(), e);
			return "@TOO_FAST_OR_SAME";
		} catch (FormValidationException e) {
			logger.error(e.getMessage(), e);
			return "@ERROR_NOT_ENOUGH_PARAMETER";
		} catch (NoPermissionException e) {
			logger.error(e.getMessage(), e);
			return "@Errors.FORBIDDEN_POST";
		} catch (PostGossipException e) {
			logger.error(e.getMessage(), e);
			return "@Errors.FORBIDDEN_POST";
		} catch (Exception e) {
			logger.error(e.getMessage(), e);
		} finally {
		}
		if (gp == null) {
			return "@ERROR_WHEN_WRITE_DB";
		}
		return "@"+gp.getId();
	}
}
