package com.xiaonei.page.xoa.controllers;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.page.feed.domain.BiddingHistory;
import com.renren.page.feed.service.BiddingService;
import com.xiaonei.admin.biz.model.auditPage.AuditPagePostView;
import com.xiaonei.admin.biz.model.auditPage.AuditPagePostView.AuditPageType;
import com.xiaonei.admin.common.logic.audit.AuditLogic;
import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.filter.AuditInfo;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.page.Constants;
import com.xiaonei.page.PageDomainType;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.SpamService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.AuditUtil;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.util.FeedUtil;
import com.xiaonei.page.util.Keys;
import com.xiaonei.page.util.MemcachedUtil;
import com.xiaonei.platform.component.doing.builder.UpdateDoingBuilder;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.component.doing.model.FastOrSameException;
import com.xiaonei.platform.component.doing.model.ForbiddenException;
import com.xiaonei.platform.component.doing.model.TooLongException;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserBasicAdapter;

@Path("pd")
public class PostDoingController {
	
	@Autowired
	private PageService pageService;
	
	@Autowired
	private AuthService authService;
	
	@Autowired
	private SpamService spamService;
	
	@Autowired
	public FeedManager feedManager;
	
	@Autowired
	private BiddingService biddingService;
	
	private final static Logger logger=Logger.getLogger("PAGEXOA");
	
	/**
	 * TODO 为什么发状态后，横栏上不显示发了状态？
	 * 
	 * @param pageId
	 * @param opUserId
	 * @param content
	 * @param asMobile
	 * @return
	 */
	@Get
	@Post
	public String index(@Param("pageId")int pageId,@Param("opUserId")int opUserId,@Param("content")String content,@Param("asMobile")int asMobile){
		if (StringUtils.isEmpty(content)) {
			return "@CONTENT_NULL";
		}
		
		if (content == null) {
			// 有可能出现用户只输入&nbsp;恶作剧的情况
			content = StringUtils.replace(content, "&nbsp;", " ");
			content = StringUtils.replace(content, "&nbsp", " ");
		}
		
		Page page = null;
		try {
			page = pageService.get(pageId);
		} catch (PageException e) {
			logger.error("get page fail", e);
			return "@GET_PAGE_FAILED";
		}
		
		int type = 0;
		String link = null;
		User host = WUserAdapter.getInstance().get(opUserId);
		// 没有权限
		try {
			if (!(authService.isPageAdministrator(host, pageId))) {
				logger.warn(String.format("%d not admin of page: %d", host.getId(), pageId));
				return "@NOT_PAGE_ADMIN";
			}
		} catch (PageException e) {
			logger.error("DB error ", e);
			return "@DB_ERROR";
		}
		
		Doing doing = new Doing();
		doing.setUserId(pageId);
		doing.setType(type);
		doing.setLink(link);
		try {
			UserBasic ub = WUserBasicAdapter.getInstance().get(pageId);
			User pusr = WUserAdapter.getInstance().get(pageId);
			@SuppressWarnings("deprecation")
			CheckResult cr = AntiSpamAdapter.getInstance().checkAndFilter(pageId, spamService.getCheckType(CheckType.PAGE_STATE_TYPE, page),
					content);
			if (cr.getFlag() == CheckResult.PROHIBITED) {
				return "@PROHIBITED";
			}
			// --start---审核机制
			int auditType = AuditUtil.getAuditTypeFromCach(pageId, AuditPageType.PAGE_STATUS.getType());
			if (auditType == Constants.AUDIT_AUDIT_FIRST || auditType == Constants.AUDIT_ALL_AUDIT_FIRST) {
				String auditString = this.auditBefore(auditType, page, content, cr, doing);
				if (!"".equals(auditString)) {
					return "@AUDIT_PROHIBITED";
				}
			}
			// --end---审核机制
			doing.setContent(content);
			
			UpdateDoingBuilder updateDoingBuilder = new UpdateDoingBuilder();
			updateDoingBuilder.setNeedDispatch(false);
			updateDoingBuilder.setNeedAuditing(false);
			updateDoingBuilder.setNeedAntispam(false);
			doing = DoingHome.getInstance().updateDoing(pusr, doing, null, updateDoingBuilder);
			// 清空状态数量的缓存
			MemcachedUtil.delete(Keys.DOING_COUNT_INTER, pageId);
			
			// --start---审核机制
			if (auditType == Constants.AUDIT_ALL_POST_FIRST || auditType == Constants.AUDIT_POST_FIRST) {
				this.auditAfter(auditType, page, content, cr, doing);
			}
			// --end---审核机制
			//清空缓存
			MemcachedUtil.delete(Keys.PAGE_STATUS,pageId);

			Map<String, String> feedParamMap = new HashMap<String, String>();
			FeedUtil.checkIdef(feedParamMap, page);
			// 手机发送状态功能判断------------------------------->>
			boolean wap = false;
			if (authService.isSuperAdmin(opUserId)) {
				wap = asMobile == 1 ? true : false;
			}
			if (wap) {
				doing.setType(Doing.TYPE_PHONE);
			}
			logger.info(String.format("%s(%s) update status [%s] pid=[%s] dingtype=[%s] using wap:%s", host.getId(), host.getName(), content,
					page.getId(), doing.getType(), wap));
			
			this.publishFeed(ub, doing, wap, feedParamMap, page, host);
		} catch (ForbiddenException fbe) {
			logger.error("doing cr content:" + doing.getContent(), fbe);
			return "@FORBIDDEN_CONTENT";
		} catch (FastOrSameException fse) {
			logger.error("doing cr content:" + doing.getContent(), fse);
			return "@TOO_FAST_OR_SAME";
		} catch (TooLongException tle) {
			logger.error("doing cr content:" + doing.getContent(), tle);
			return "@ErrorsExt.TOO_LONG";
		} catch (Exception e) {
			logger.error(e.getMessage() + " doing cr content:" + doing.getContent(), e);
			doing = null;
		}

		if (doing == null) {
			return "@DB_ERROR";
		}
		return "@"+String.valueOf(doing.getId());
	}
	
	private String auditBefore(int auditType, Page page, String content, CheckResult cr, Doing doing) {
		int pid = page.getId();
		Random random = new Random();

		AuditInfo audit = AntiSpamHome.getInstance().getDiaryAduitType(pid, content);
		AuditPagePostView view = new AuditPagePostView();
		view.setAuditLevel(audit.getLevel());
		view.setAuditType(audit.getType());
		view.setCreatorId(pid);
		view.setCreatorName(page.getName());
		view.setContent(cr.getHighLightContent());
		view.setTitle(cr.getHighLightContent());
		view.setContentUrl(PageDomainType.getLocalDomain().getDomainUrl() + "/" + pid);
		view.setContentType(AuditPageType.PAGE_STATUS.getType());
		view.setOwnerId(pid);
		view.setOwnerName(page.getName());

		try {
			if (auditType == Constants.AUDIT_AUDIT_FIRST) {// 先审后发
				if (cr.getFlag() == CheckResult.CONTENT_NEED_AUDIT) {// 含嫌疑词
					// 送入审核后台
					view.setTitleId(random.nextInt(99999999));// 还未发布，所以没有id，用随机数代替
					view.setContentId(random.nextInt(99999999));
					AuditLogic.getInstance().saveAuditPagePost(view);
					return "您的状态已经提交到人人网审核后台，审核通过后即可发布。我们会及时给您回复，请耐心等待。";
				}
			} else if (auditType == Constants.AUDIT_ALL_AUDIT_FIRST) {// 全审(先审后发)
				view.setTitleId(random.nextInt(99999999));// 还未发布，所以没有id，用随机数代替
				view.setContentId(random.nextInt(99999999));
				AuditLogic.getInstance().saveAuditPagePost(view);
				return "您的状态已经提交到人人网审核后台，审核通过后即可发布。我们会及时给您回复，请耐心等待。";
			}
		} catch (Exception e) {
			logger.error(e.getMessage(),e);
		}

		return "";
	}

	private String auditAfter(int auditType, Page page, String content, CheckResult cr, Doing doing) {
		int pid = page.getId();

		AuditInfo audit = AntiSpamHome.getInstance().getDiaryAduitType(pid, content);
		AuditPagePostView view = new AuditPagePostView();
		view.setAuditLevel(audit.getLevel());
		view.setAuditType(audit.getType());
		view.setCreatorId(pid);
		view.setCreatorName(page.getName());
		view.setContent(cr.getHighLightContent());
		view.setTitle(cr.getHighLightContent());
		view.setContentUrl(PageDomainType.getLocalDomain().getDomainUrl() + "/" + pid);
		view.setContentType(AuditPageType.PAGE_STATUS.getType());
		view.setOwnerId(pid);
		view.setOwnerName(page.getName());

		try {
			if (auditType == Constants.AUDIT_ALL_POST_FIRST) {// 全审(先发后审)
				view.setTitleId(doing.getId());
				view.setContentId(doing.getId());
				AuditLogic.getInstance().saveAuditPagePost(view);
			} else if (auditType == Constants.AUDIT_POST_FIRST) {// 先发后审
				// 含嫌疑词的状态送进审核后台
				if (cr.getFlag() == CheckResult.CONTENT_NEED_AUDIT) {
					view.setTitleId(doing.getId());
					view.setContentId(doing.getId());
					AuditLogic.getInstance().saveAuditPagePost(view);
				}
			}
		} catch (Exception e) {
			logger.error(e.getMessage(),e);
		}

		return "";
	}

	/**
	 * 发新鲜事
	 * 
	 * @param ub
	 * @param doing
	 * @param wap
	 * @param feedParamMap
	 * @param page
	 * @param curHost
	 */
	private void publishFeed(UserBasic ub, Doing doing, boolean wap, Map<String, String> feedParamMap, Page page, User curHost) {
		IContext context = new ContextImpl();
		context.setWap(wap);
		context.put("ub", ub);
		context.put("doing", doing);
		context.put("feedParamMap", feedParamMap);
		// 竞价相关的
		//JSONObject biddingInfo = com.xiaonei.page.service.feed.FeedUtil.getBiddingInfo(inv.getRequest());
		BiddingHistory bidding = biddingService.buildBidding(page, curHost, FeedDefUtil.getSTATUS_UPDATE_Def(page), doing.getId(), null);
		context.setBidding(bidding);
		feedManager.publish(page, curHost, FeedDefUtil.getSTATUS_UPDATE_Def(page), true, context);

	}
}
