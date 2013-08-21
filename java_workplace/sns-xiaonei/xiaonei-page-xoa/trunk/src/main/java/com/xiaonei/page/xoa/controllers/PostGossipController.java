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
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.framework.exception.AntispamException;
import com.xiaonei.platform.framework.exception.FormValidationException;

@Path("pg")
public class PostGossipController {
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
			@Param("opUserId") int opUserId,
			@Param("gossipContent") String gossipContent) {
		
		User host = WUserAdapter.getInstance().get(opUserId);
		if (host == null) {
			return "@GET_USER_FAILED";
		}
		Page page = pageService.get(pageId);
		boolean isAdmin = authService.isPageAdministrator(opUserId, pageId);
		//logger.info("isAdmin:"+isAdmin);
		boolean canGossip = settingsService.fansCanSendGossip(page);
		//logger.info("canGossip:"+canGossip);
		boolean isFans = authService.isPageActiveFans(opUserId, pageId);
		//logger.info("isFans:"+isFans);
		/* 合法性验证和Spam验证 */
		CheckResult cr = null;
		try {
			gossipService.validateComment(host, isAdmin, isFans, gossipContent,
					canGossip);
			cr = SpamService.getInstance().commentValidate(isAdmin, host, page,
					gossipContent,
					spamService.getCheckType(CheckType.PAGE_GOSSIP_TYPE, page));
		} catch (BizValidateException e) {
			logger.info(e.toString());
			return "@BizValidateException";
		} catch (SpamException e) {
			logger.info(e.toString());
			return "@SpamException";
		} catch (Exception e) {
			logger.error("", e);
			return "@ERROR_WHEN_READ_DB";
		}
		if (cr == null) {
			logger.error("cr is null!");
			return "@ERROR_WHEN_WRITE_DB";
		}
		/* 合法性验证和Spam验证 --- 结束 */
		gossipContent = cr.getSafePureContent();

		// 加入@功能和短链接功能
		String inputContent = gossipContent;
		try {
			String feedContent = MetionEncrytedUtil
					.getMetionEncrytedContentNotPublishDoing(gossipContent,host.getId());
			gossipContent = feedContent;
		} catch (Exception e) {
			logger.error("Mention error:" + e);
		}

		Gossip gp = null;
		User guest = null;
		try {
			
			guest = WUserAdapter.getInstance().get(page.getId());
            // 如果管理员直接留言
            if (isAdmin) {
                return "@admin can not gossip, only reply!";
            }
            //默认非悄悄话，ip为null
            gp = GossipHome.getInstance().postGossip(host, guest, 0, gossipContent,
                    null, null, null, true, false, false);

            // 发送给guest,TODO 后面的参数目前都设置为null
            gossipService.publishFansFeed(page, gp, gossipContent, host, null, null, null);

            if (logger.isInfoEnabled()) {
                logger.info(String.format(
                        "gossip isadmin=%s from  %d to %d ,gossipReplyToId=%d", isAdmin,
                        host.getId(), page.getId(), 0));
            }
			// 审核检查
			spamService.auditComment(page, cr, gossipContent, host, gp.getId(),
					"" + page.getId(), AuditPageType.PAGE_GOSSIP.getType());
		} catch (AntispamException e) {
			logger.error(e.getMessage(), e);
			return "@TOO_FAST_OR_SAME";
		} catch (FormValidationException e) {
			logger.error(e.getMessage(), e);
			return "@ERROR_NOT_ENOUGH_PARAMETER";
		} catch (NoPermissionException e) {
			logger.error(e.getMessage(), e);
			return "@FORBIDDEN_POST";
		} catch (PostGossipException e) {
			logger.error(e.getMessage(), e);
			return "@FORBIDDEN_POST";
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
