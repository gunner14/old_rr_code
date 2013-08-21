package com.xiaonei.page.xoa.controllers;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.admin.biz.model.auditPage.AuditPagePostView.AuditPageType;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.exceptions.BizValidateException;
import com.xiaonei.page.exceptions.SpamException;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.DoingService;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.SettingsService;
import com.xiaonei.page.service.SpamService;
import com.xiaonei.page.util.MetionEncrytedUtil;
import com.xiaonei.platform.component.doing.builder.ReplyDoingBuilder;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.component.doing.model.FastOrSameException;
import com.xiaonei.platform.component.doing.model.ForbiddenException;
import com.xiaonei.platform.component.doing.model.ReplyDoing;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;

@Path("pdr")
public class PostDoingReplyController {

	@Autowired
	private DoingService doingService;
	
	@Autowired
	private PageService pageService;
	
	@Autowired
	private SettingsService settingsService;
	
	@Autowired
	private AuthService authService;
	
	@Autowired
	private SpamService spamService;
	
	private final static Logger logger=Logger.getLogger("PAGEXOA");

	@Get
	@Post
	public String index(@Param("pageId") int pageId,
			@Param("doingId") int doingId, @Param("opUserId") int opUserId,
			@Param("replyContent") String replyContent) {
		Page page = null;
		try {
			page = pageService.get(pageId);
		} catch (PageException e) {
			logger.error("get page fail", e);
			return "@GET_PAGE_FAILED";
		}
		//page's user view
		User puser = SnsAdapterFactory.getUserAdapter().get(pageId);

		int type = 0;
		String link = null;
		//当前用户
		User host = WUserAdapter.getInstance().get(opUserId);

        boolean canComment = settingsService.fansCanCommentState(page);
        boolean isAdmin = authService.isPageAdministrator(opUserId, pageId);
        boolean isFans = authService.isPageActiveFans(opUserId, pageId);
        
        CheckResult cr = null;
        try {
            doingService.validateComment(host, page, isAdmin, isFans, replyContent, canComment);
            cr = SpamService.getInstance().commentValidate(isAdmin, host, page, replyContent,
                    spamService.getCheckType(CheckType.PAGE_REPLY_TYPE, page));
        } catch (BizValidateException e) {
            logger.error(e.getMessage(),e);
            return "@BizValidateException";
        } catch (SpamException e) {
            logger.error(e.getMessage(),e);
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
        Doing doing = null;
        try {
            doing = DoingHome.getInstance().getDoingById(doingId, puser.getId());
            if (doing == null) {
                logger.warn(String.format("doing not exist: doing=%d, owner=%d", doingId, puser.getId()));
                return "@DOING_NOT_EXIST";
            }
        } catch (Exception e1) {
            logger.error(String.format("get doing error: doing=%d, owner=%d", doingId, puser.getId()), e1);
            return "@DOING_NOT_EXIST";
        }

        User user = null;
        User doingUser = null;
        try {
            doingUser = WUserAdapter.getInstance().get(pageId);
            
            // 是管理员，显示page的信息
            if (isAdmin) {
                user = doingUser;
            } else {
                user = host;
            }
            // 更新状态回复
            if (user == null || doingUser == null) {
                return "@ERROR_WHEN_READ_DB";
            }
        } catch (Exception e) {
            return "@ERROR_WHEN_READ_DB";
        }
        
        doing.setOwnerName(user.getName());// 发新鲜事要用
        
        ReplyDoing rd = null;
        String inputContent = null;
        try {
            ReplyDoingBuilder replyDoingBuilder = new ReplyDoingBuilder();
            replyDoingBuilder.setNeedDispatch(false);

            rd = doingService.replyDoing(user, replyContent, doing, 0, replyDoingBuilder, null);

            // 加入@功能和短链接功能
            inputContent = rd.getReplyContent();
            String feedContent = MetionEncrytedUtil.getMetionEncrytedContentNotPublishDoing(rd
                    .getReplyContent(),host.getId());
            rd.setReplyContent(feedContent);

            logger.info(String.format("pageID=%s, doingID=%s, replyID=%s, replyOwnerID=%s.",
                    page.getId(), doing.getId(), rd.getId(), rd.getOwnerId()));

            doingService.publishfeed(page, doingUser, host, doing, rd, isAdmin);
            // 审核策略
            spamService.auditComment(page, cr, replyContent, host, rd.getId(), "/" + page.getId(),
                    AuditPageType.PAGE_STATUSCOMMENT.getType());
        } catch (ForbiddenException fbe) {
            return "@FORBIDDEN_CONTENT";
        } catch (FastOrSameException fse) {
            return "@TOO_FAST_OR_SAME";
        } catch (Exception e) {
            logger.error(e.getMessage(),e);
            rd = null;
        }
        if (rd == null) {
            return "@ERROR_WHEN_WRITE_DB";
        }

        return String.format("@%s", rd.getId());
	}
}
