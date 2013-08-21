package com.xiaonei.page.xoa.controllers.bizservice;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.builder.ReflectionToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;
import org.apache.log4j.Logger;
import org.json.JSONArray;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.app.share.IShareConstants;
import com.renren.app.share.model.Share;
import com.renren.app.share.model.Url;
import com.renren.app.share.xoa.api.UrlService;
import com.renren.page.feed.domain.BiddingHistory;
import com.renren.page.feed.service.BiddingService;
import com.renren.xoa.commons.annotation.JsonParam;
import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFuture;
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
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.AuditUtil;
import com.xiaonei.page.util.ILogger;
import com.xiaonei.page.xoa.domain.PageShare;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.page.xoa.domain.TrackInfo;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.page.xoa.util.XoaAdapter;
import com.xiaonei.platform.component.xfeed.helper.FeedDefinition;
import com.xiaonei.platform.core.model.User;

/**
 * 站外分享link
 * 
 * @author erxi.qian
 * 
 */
@Path("ps")
public class PublishShareController {

    @Autowired
    private FeedManager feedManager;

    @Autowired
    private BiddingService biddingService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    /**
     * 分享的XOA接口
     */
    private com.renren.app.share.xoa.api.ShareService shareApi = ServiceFactories.getFactory()
            .getService(com.renren.app.share.xoa.api.ShareService.class);

    /**
     * 分享的URL资源相关的接口
     */
    private UrlService urlService = ServiceFactories.getFactory().getService(UrlService.class);

    private static final Map<Integer, Integer> shareFeedTypeMapping = new HashMap<Integer, Integer>();

    static {
        // share.audio.10=19
        // share.link.10=21
        // share.video.10=23

        // share.audio.20=26
        // share.link.20=28
        // share.video.20=30
        shareFeedTypeMapping.put(19, FeedDefinition.PAGE_SHARE_MUSIC);
        shareFeedTypeMapping.put(21, FeedDefinition.PAGE_SHARE_LINK);
        shareFeedTypeMapping.put(23, FeedDefinition.PAGE_SHARE_VIDEO);

        shareFeedTypeMapping.put(26, FeedDefinition.CLUB_SHARE_MUSIC);
        shareFeedTypeMapping.put(28, FeedDefinition.CLUB_SHARE_LINK);
        shareFeedTypeMapping.put(30, FeedDefinition.CLUB_SHARE_VIDEO);
    }

    /**
     * 保存分享
     * 
     * @param request
     * @return
     */
    @Post("save")
    public Object save(@Param("pageId") int pageId, @Param("opUserId") int opUserId,
            @JsonParam PageShare pageShare, @JsonParam TrackInfo trackInfo) {
        try {
            //取page
            Page page = PageXoaUtil.getInstance().getPage(pageId);
            if (page == null) {
                return new XoaBizErrorBean(PageXoaErrors.PAGE_NOT_EXIST.getType(),
                        PageXoaErrors.PAGE_NOT_EXIST.getDesc());
            }

            //取user
            User host = PageXoaUtil.getInstance().getUser(opUserId);
            if (host == null) {
                return new XoaBizErrorBean(PageXoaErrors.USER_NOT_EXIST.getType(),
                        PageXoaErrors.USER_NOT_EXIST.getDesc());
            }

            //判断权限失败
            if (!PageXoaUtil.getInstance().authUserAndPage(opUserId, pageId)) {
                return new XoaBizErrorBean(PageXoaErrors.AUTH_FAILED.getType(),
                        PageXoaErrors.AUTH_FAILED.getDesc());
            }

            Map<String, Object> map = new HashMap<String, Object>();
            // check request
            String content = com.xiaonei.websecurity.adapter.SafeFilter.getInstance()
                    .getPureContent(pageShare.getContent());
            //内容过滤
            CheckResult cr = AntiSpamAdapter.getInstance().checkAndFilter(pageId,
                    CheckType.SHARE_TYPE, content, pageShare.getClientIp());
            if (cr.getFlag() == CheckResult.PROHIBITED) {
                return new XoaBizErrorBean(PageXoaErrors.FORBIDDEN_CONTENT.getType(),
                        PageXoaErrors.FORBIDDEN_CONTENT.getDesc());
            }
            //title过滤
            String title = pageShare.getTitle();
            if (StringUtils.isEmpty(title) || StringUtils.isBlank(title)) {
                return new XoaBizErrorBean(PageXoaErrors.TITLE_NUL.getType(),
                        PageXoaErrors.TITLE_NUL.getDesc());
            }
            CheckResult cr1 = AntiSpamAdapter.getInstance().checkAndFilter(pageId,
                    CheckType.SHARE_TYPE, title, pageShare.getClientIp());
            if (cr1.getFlag() == CheckResult.PROHIBITED) {
                return new XoaBizErrorBean(PageXoaErrors.FORBIDDEN_CONTENT.getType(),
                        PageXoaErrors.FORBIDDEN_CONTENT.getDesc());
            }

            Share share = null;
            try {
                //--start---审核机制
                int auditType = AuditUtil.getAuditTypeFromCach(pageId,
                        AuditPageType.PAGE_SHARE.getType());
                if (auditType == Constants.AUDIT_AUDIT_FIRST
                        || auditType == Constants.AUDIT_ALL_AUDIT_FIRST) {
                    String auditString = this.auditBefore(auditType, page, content, cr, cr1, map);
                    if (!"".equals(auditString)) {
                        return new XoaBizErrorBean(PageXoaErrors.AUDIT_ERROR.getType(), auditString);
                    }
                }
                //--end---审核机制

                ServiceFuture<Url> future = urlService.getUrl(pageShare.getLink());
                Url url = XoaAdapter.doSubmit(future, 2000);
                //xoa调用失败
                if (url == null) {
                    return new XoaBizErrorBean(PageXoaErrors.ERROR_WHEN_INVOKE_XOA.getType(),
                            PageXoaErrors.ERROR_WHEN_INVOKE_XOA.getDesc());
                }

                share = Share.buildFromUrl(url);
                share.setUserId(pageId);
                share.setUserName(page.getName());
                share.setType(pageShare.getType());//
                share.setTitle(cr1.getSafePureContent());
                share.setComment(cr.getSafePureContent());

                //TODO 这个地方是否要修改？
                if (pageShare.getType() != 0 && pageShare.getType() == 19) {
                    String cnt = pageShare.getFromName();
                    if (!StringUtils.isBlank(cnt)) {
                        CheckResult ct = AntiSpamAdapter.getInstance().checkAndFilter(pageId,
                                CheckType.SHARE_TYPE, cnt, pageShare.getClientIp());
                        if (ct.getFlag() == CheckResult.PROHIBITED) {
                            return new XoaBizErrorBean(PageXoaErrors.FORBIDDEN_CONTENT.getType(),
                                    PageXoaErrors.FORBIDDEN_CONTENT.getDesc());
                        }
                        cnt = ct.getSafePureContent();
                    }
                    share.insertMeta(IShareConstants.META_KEY_TYPE11_ARTIST, cnt);

                    cnt = pageShare.getFromUniv();
                    if (!StringUtils.isBlank(cnt)) {
                        CheckResult ct = AntiSpamAdapter.getInstance().checkAndFilter(pageId,
                                CheckType.SHARE_TYPE, cnt, pageShare.getClientIp());
                        if (ct.getFlag() == CheckResult.PROHIBITED) {
                            return new XoaBizErrorBean(PageXoaErrors.FORBIDDEN_CONTENT.getType(),
                                    PageXoaErrors.FORBIDDEN_CONTENT.getDesc());
                        }
                        cnt = ct.getSafePureContent();
                    }
                    share.insertMeta(IShareConstants.META_KEY_TYPE11_ALBUM, cnt);
                }

                ServiceFuture<Share> shareFuture = shareApi.createShare(share, pageId,
                        pageShare.getClientIp());
                shareFuture.setParam("client", IShareConstants.CLIENT_TYPE_WEB + "");
                //设置需要获取返回值
                shareFuture.setParam("result", "1");
                share = XoaAdapter.doSubmit(shareFuture, 2000);
                //xoa调用失败
                if (share == null) {
                    return new XoaBizErrorBean(PageXoaErrors.ERROR_WHEN_INVOKE_XOA.getType(),
                            PageXoaErrors.ERROR_WHEN_INVOKE_XOA.getDesc());
                }

                //--start---审核机制
                if (auditType == Constants.AUDIT_ALL_POST_FIRST
                        || auditType == Constants.AUDIT_POST_FIRST) {
                    this.auditAfter(auditType, page, content, cr, cr1, map, share.getId());
                }
                //--end---审核机制
            } catch (Exception e) {
                ILogger.logger.error("save-share fail", e);
            }
            if (share != null) {
                // 发送新鲜事
                this.publishFeed(page, host, share, trackInfo);
            } else {
                logger.error(String.format("sharelink-save fail pid=%d,uid=%d,retshare=%s,json=%s",
                        pageId, opUserId, share, map));

            }
            return share;
        } catch (Exception e) {
            logger.error("sharelink-save fail", e);
        }
        return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
    }

    private String auditBefore(int auditType, Page page, String content, CheckResult contentcr,
            CheckResult titlecr, Map<String, Object> map) {
        try {
            int pid = page.getId();
            Random random = new Random();
            JSONArray ja = new JSONArray();
            ja.put(0, map);
            AuditInfo audit = AntiSpamHome.getInstance().getDiaryAduitType(pid, content);
            AuditPagePostView view = new AuditPagePostView();
            view.setAuditLevel(audit.getLevel());
            view.setAuditType(audit.getType());
            view.setCreatorId(pid);
            view.setCreatorName(page.getName());
            view.setContent(ja.toString());
            view.setTitle(titlecr.getHighLightContent());
            view.setContentUrl(PageDomainType.getLocalDomain().getDomainUrl() + "/" + pid);
            view.setContentType(AuditPageType.PAGE_SHARE.getType());
            view.setOwnerId(pid);
            view.setOwnerName(page.getName());

            if (auditType == Constants.AUDIT_AUDIT_FIRST) {// 先审后发
                if (contentcr.getFlag() == CheckResult.CONTENT_NEED_AUDIT
                        || titlecr.getFlag() == CheckResult.CONTENT_NEED_AUDIT) {// 含嫌疑词
                    // 送入审核后台
                    view.setTitleId(random.nextInt(99999999));// 还未发布，所以没有id，用随机数代替
                    view.setContentId(random.nextInt(99999999));
                    AuditLogic.getInstance().saveAuditPagePost(view);
                    return "您的分享已经提交到人人网审核后台，审核通过后即可发布。我们会及时给您回复，请耐心等待。";
                }
            } else if (auditType == Constants.AUDIT_ALL_AUDIT_FIRST) {// 全审(先审后发)
                view.setTitleId(random.nextInt(99999999));// 还未发布，所以没有id，用随机数代替
                view.setContentId(random.nextInt(99999999));
                AuditLogic.getInstance().saveAuditPagePost(view);
                return "您的分享已经提交到人人网审核后台，审核通过后即可发布。我们会及时给您回复，请耐心等待。";
            }
        } catch (Exception e) {
            ILogger.logger.error(e.getMessage(), e);
        }

        return "";
    }

    private String auditAfter(int auditType, Page page, String content, CheckResult contentcr,
            CheckResult titlecr, Map<String, Object> map, long id) {

        try {
            int pid = page.getId();
            JSONArray ja = new JSONArray();
            ja.put(0, map);
            AuditInfo audit = AntiSpamHome.getInstance().getDiaryAduitType(pid, content);
            AuditPagePostView view = new AuditPagePostView();
            view.setAuditLevel(audit.getLevel());
            view.setAuditType(audit.getType());
            view.setCreatorId(pid);
            view.setCreatorName(page.getName());
            view.setContent(ja.toString());
            view.setTitle(titlecr.getHighLightContent());
            view.setContentUrl(PageDomainType.getLocalDomain().getDomainUrl() + "/" + pid);
            view.setContentType(AuditPageType.PAGE_SHARE.getType());
            view.setOwnerId(pid);
            view.setOwnerName(page.getName());

            if (auditType == Constants.AUDIT_ALL_POST_FIRST) {// 全审(先发后审)
                view.setTitleId(id);
                view.setContentId(id);
                AuditLogic.getInstance().saveAuditPagePost(view);
            } else if (auditType == Constants.AUDIT_POST_FIRST) {// 先发后审
                // 含嫌疑词的状态送进审核后台
                if (contentcr.getFlag() == CheckResult.CONTENT_NEED_AUDIT
                        || titlecr.getFlag() == CheckResult.CONTENT_NEED_AUDIT) {
                    view.setTitleId(id);
                    view.setContentId(id);
                    AuditLogic.getInstance().saveAuditPagePost(view);
                }
            }
        } catch (Exception e) {
            ILogger.logger.error(e.getMessage(), e);
        }

        return "";
    }

    /**
     * 发送对应的新鲜事
     * 
     * @param page
     * @param user
     * @param share
     */
    private void publishFeed(Page page, User user, Share share, TrackInfo trackInfo) {
        IContext context = new ContextImpl();
        context.put("share", share);
        try {
            Integer feedType = shareFeedTypeMapping.get(share.getType());

            if (feedType == null) {
                logger.error("no feedType found for share:"
                        + ReflectionToStringBuilder.toString(share, ToStringStyle.MULTI_LINE_STYLE));
                return;
            }

            BiddingHistory bidding = biddingService.buildBidding(page, user, feedType,
                    share.getId(), null);
            context.setBidding(bidding);
            //update by liyi 获取statID和level这两个参数添加到context中
            if (trackInfo != null) {
                String statID = trackInfo.getStateID();
                String level = trackInfo.getLevel();
                if (statID != null && !("".equals(statID))) {
                    context.put("statID", statID);
                }
                if (level != null && !("".equals(level))) {
                    context.put("level", level);
                }
            }

            feedManager.publish(page, user, feedType, true, context);
        } catch (Exception e) {
            logger.error(String.format(
                    "publish share feed Fail: page=%s,user=%s(%s),share=%s,shareTypeMapping=%s ",
                    page.getId(), user.getId(), user.getName(),
                    ReflectionToStringBuilder.toString(share), shareFeedTypeMapping), e);
        }
    }
}
