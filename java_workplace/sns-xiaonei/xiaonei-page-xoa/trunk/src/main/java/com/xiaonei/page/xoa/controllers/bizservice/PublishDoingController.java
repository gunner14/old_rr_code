package com.xiaonei.page.xoa.controllers.bizservice;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.app.status.xoa.api.StatusService;
import com.renren.page.feed.domain.BiddingHistory;
import com.renren.page.feed.service.BiddingService;
import com.renren.ugc.model.status.DoingVO;
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
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.SpamService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.util.AuditUtil;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.util.FeedUtil;
import com.xiaonei.page.util.Keys;
import com.xiaonei.page.util.MemcachedUtil;
import com.xiaonei.page.xoa.domain.PageXoaErrors;
import com.xiaonei.page.xoa.domain.TrackInfo;
import com.xiaonei.page.xoa.util.PageXoaUtil;
import com.xiaonei.page.xoa.util.XoaAdapter;
import com.xiaonei.platform.component.doing.builder.UpdateDoingBuilder;
import com.xiaonei.platform.component.doing.home.DoingHome;
import com.xiaonei.platform.component.doing.model.Doing;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserBasicAdapter;

/**
 * 
 * 
 * @author happyshelocks@gmail.com
 * 
 */
@Path("pd")
public class PublishDoingController {
    @Autowired
    private AuthService authService;

    @Autowired
    private SpamService spamService;

    @Autowired
    public FeedManager feedManager;

    @Autowired
    private BiddingService biddingService;

    private StatusService statusService = ServiceFactories.getFactory().getService(
            StatusService.class);

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    /**
     * 
     * 
     * @param pageId
     * @param opUserId
     * @param content
     * @param asMobile
     * @return
     */
    @Post
    public Object index(Invocation inv, @Param("pageId") int pageId,
            @Param("opUserId") int opUserId, @Param("content") String content,
            @Param("asMobile") boolean asMobile, @JsonParam TrackInfo trackInfo) {
        if (StringUtils.isEmpty(content)) {
            doErrorLog(pageId, opUserId, content, asMobile, PageXoaErrors.CONTENT_NUL.getDesc());
            return new XoaBizErrorBean(PageXoaErrors.CONTENT_NUL.getType(),
                    PageXoaErrors.CONTENT_NUL.getDesc());
        }

        if (content != null) {
            // 有可能出现用户只输入&nbsp;恶作剧的情况
            content = StringUtils.replace(content, "&nbsp;", " ");
            content = StringUtils.replace(content, "&nbsp", " ");
        }

        //取page
        Page page=PageXoaUtil.getInstance().getPage(pageId);
        if(page==null){
            return new XoaBizErrorBean(PageXoaErrors.PAGE_NOT_EXIST.getType(),
                    PageXoaErrors.PAGE_NOT_EXIST.getDesc());
        }
        
        //取user
        User host=PageXoaUtil.getInstance().getUser(opUserId);
        if(host==null){
            return new XoaBizErrorBean(PageXoaErrors.USER_NOT_EXIST.getType(),
                    PageXoaErrors.USER_NOT_EXIST.getDesc());
        }
        
        //判断权限失败
        if(!PageXoaUtil.getInstance().authUserAndPage(opUserId, pageId)){
            return new XoaBizErrorBean(PageXoaErrors.AUTH_FAILED.getType(),
                    PageXoaErrors.AUTH_FAILED.getDesc());
        }

        int type = 0;
        String link = null;
        //这里发布feed还是使用这个对象,我去什么时候能改。
        Doing doing = new Doing();
        doing.setUserId(page.getId());
        doing.setType(type);
        doing.setLink(link);
        try {
            UserBasic ub = WUserBasicAdapter.getInstance().get(pageId);
            User pusr = WUserAdapter.getInstance().get(pageId);
            @SuppressWarnings("deprecation")
            CheckResult cr = AntiSpamAdapter.getInstance().checkAndFilter(pageId,
                    spamService.getCheckType(CheckType.PAGE_STATE_TYPE, page), content);
            if (cr.getFlag() == CheckResult.PROHIBITED) {
                return new XoaBizErrorBean(PageXoaErrors.FORBIDDEN_CONTENT.getType(),
                        PageXoaErrors.FORBIDDEN_CONTENT.getDesc());
            }
            // --start---审核机制
            int auditType = AuditUtil.getAuditTypeFromCach(pageId,
                    AuditPageType.PAGE_STATUS.getType());
            if (auditType == Constants.AUDIT_AUDIT_FIRST
                    || auditType == Constants.AUDIT_ALL_AUDIT_FIRST) {
                String auditString = this.auditBefore(auditType, page, content, cr);
                if (!"".equals(auditString)) {
                    return new XoaBizErrorBean(PageXoaErrors.ERROR_NO_TICKET.getType(), auditString);
                }
            }
            // --end---审核机制
            // 内容的转义；非常重要
            content = cr.getSafePureContent();
            doing.setContent(content);

            //DoingVO和Doing对象一样;DoingVO是XOA中的方式，Doing是Jar中的方式
            DoingVO doingVO = new DoingVO();
            int userId = page.getId();
            doingVO.setUserId(page.getId());
            doingVO.setType(type);
            doingVO.setLink(link);
            doingVO.setContent(content);
            
            com.renren.ugc.model.status.UpdateDoingBuilder udb = new com.renren.ugc.model.status.UpdateDoingBuilder();
            udb.setNeedDispatch(false);
            udb.setNeedAuditing(false);
            // 去掉Doing的安全检查,Page这里已经做过了
            udb.setNeedAntispam(false);
            //添加新鲜事版本信息，兼容新老新鲜事，这样PAGE XOA可以判断是否发新鲜事
            Map<String, String> xoafeed = new HashMap<String, String>();
            xoafeed.put("PAGE_XFEED_VERSION", "0"); //用来表示发的老版本新鲜事
            udb.setFeedParams(xoafeed);


            // XOA方式更新状态
            ServiceFuture<DoingVO> sf = statusService.create(userId, doingVO, udb);
            Object returnedDoingVO = XoaAdapter.doSubmit(sf);
            if (!returnedDoingVO.getClass().isAssignableFrom(DoingVO.class)) {
                return returnedDoingVO;
            }
            DoingVO castedDoingVO = (DoingVO) returnedDoingVO;
            doing.setContent(castedDoingVO.getContent());
            doing.setDoTime(castedDoingVO.getDoTime());
            doing.setId(castedDoingVO.getId());
            BeanUtils.copyProperties(doing, castedDoingVO);

            // 清空状态数量的缓存
            MemcachedUtil.delete(Keys.DOING_COUNT_INTER, pageId);

            // --start---审核机制
            if (auditType == Constants.AUDIT_ALL_POST_FIRST
                    || auditType == Constants.AUDIT_POST_FIRST) {
                this.auditAfter(auditType, page, content, cr, doingVO);
            }
            // --end---审核机制
            //清空缓存
            MemcachedUtil.delete(Keys.PAGE_STATUS, pageId);

            Map<String, String> feedParamMap = new HashMap<String, String>();
            FeedUtil.checkIdef(feedParamMap, page);
            // 手机发送状态功能判断------------------------------->>
            boolean wap = false;
            if (authService.isSuperAdmin(opUserId)) {
                wap = asMobile == true ? true : false;
            }
            if (wap) {
                doing.setType(Doing.TYPE_PHONE);
            }
            
            logger.info(String.format(
                    "%s(%s) update status [%s] pid=[%s] dingtype=[%s] using wap:%s", host.getId(),
                    host.getName(), content, page.getId(), doing.getType(), wap));
            this.publishFeed(ub, doing, wap, feedParamMap, page, host);
        } catch (Exception e) {
            doErrorLog(pageId, opUserId, content, asMobile, e.getMessage(), e);
            doing = null;
        }
        if (doing == null) {
            doErrorLog(pageId, opUserId, content, asMobile, "at last doing is still null!");
            return new XoaBizErrorBean(PageXoaErrors.ERROR.getType(), PageXoaErrors.ERROR.getDesc());
        }
        return "@" + String.valueOf(doing.getId());
    }

    private void doErrorLog(int pageId, int opUserId, String content, boolean asMobile, String msg) {
        logger.error(String
                .format("Publish Doing failed! pageId is %s,opUserId is %s,content is %s,asMobile is %s.Msg is %s",
                        pageId, opUserId, content, asMobile, msg));
    }

    private void doErrorLog(int pageId, int opUserId, String content, boolean asMobile, String msg,
            Throwable t) {
        logger.error(
                String.format(
                        "Publish Doing failed! pageId is %s,opUserId is %s,content is %s,asMobile is %s.Msg is %s",
                        pageId, opUserId, content, asMobile, msg), t);
    }

    private String auditBefore(int auditType, Page page, String content, CheckResult cr) {
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
            logger.error(e.getMessage(), e);
        }

        return "";
    }

    private String auditAfter(int auditType, Page page, String content, CheckResult cr,
            DoingVO doing) {
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
            logger.error(e.getMessage(), e);
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
    private void publishFeed(UserBasic ub, Doing doing, boolean wap,
            Map<String, String> feedParamMap, Page page, User curHost) {
        IContext context = new ContextImpl();
        context.setWap(wap);
        context.put("ub", ub);
        context.put("doing", doing);
        context.put("feedParamMap", feedParamMap);
        //竞价相关的,biddinginfo设置为null
        BiddingHistory bidding = biddingService.buildBidding(page, curHost,
                FeedDefUtil.getSTATUS_UPDATE_Def(page), doing.getId(), null);
        context.setBidding(bidding);
        feedManager.publish(page, curHost, FeedDefUtil.getSTATUS_UPDATE_Def(page), true, context);

    }
}
