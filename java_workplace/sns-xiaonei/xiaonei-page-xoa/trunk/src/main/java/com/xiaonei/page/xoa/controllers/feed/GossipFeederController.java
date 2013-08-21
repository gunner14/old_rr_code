package com.xiaonei.page.xoa.controllers.feed;

import java.util.Map;

import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.renren.page.feed.domain.def.FeedDefBean;
import com.xiaonei.page.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.feed.FeedConfig;
import com.xiaonei.page.service.AuthService;
import com.xiaonei.page.service.GossipService;
import com.xiaonei.page.service.feed.FeedConfigService;
import com.xiaonei.page.service.feed.FeedPublisher;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.service.feed.impl.XoaPageAdminGossipPublisher;
import com.xiaonei.page.service.feed.impl.XoaPageFansGossipPublisher;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.controllers.CheckerUtil;
import com.xiaonei.page.xoa.domain.PageHostPair;
import com.xiaonei.page.xoa.util.PageIdChecker;
import com.xiaonei.platform.component.gossip.view.GossipView;
import com.xiaonei.platform.core.model.User;

/**
 * 
 * 留言新鲜事的统一接口。
 * 
 * //TODO 这里的代码肯定要改，用new gossip对象，填充内容的方式。 
 * 
 * @author happyshelocks@gmail.com
 * 
 */
@Path("gf")
public class GossipFeederController {

    @Autowired
    private GossipService gossipService;

    @Autowired
    private FeedDefBean feedDefBean;

    @Autowired
    private FeedConfigService configService;

    @Autowired
    private AuthService authService;

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    @Post
    @Get
    public String index(@Param("pageId") int pageId, @Param("curHost") int curHost,@Param("gossipId") long gossipId) {
//        不需要这段逻辑        
//        if(!PageIdChecker.isPageDomainIn(curHost)){
//            return "@done";
//        }
        PageHostPair pageHostPair = CheckerUtil.getInstance().checkPageAndHost(pageId, curHost);
        Page page = pageHostPair.getPage();
        User host = pageHostPair.getHost();
        if (page == null || host == null) {
            return pageHostPair.getMsg();
        }
        GossipView gv = gossipService.getGossip(pageId, gossipId);
        if (gv == null) {
            return "@gossip-not-exist";
        }
        IContext context = new ContextImpl();
        context.put("body", gv.getBody());
        context.put("gossipId", gossipId);

        //管理员的粉丝新鲜事,不会发mini feed
        if (authService.isPageAdministrator(curHost, pageId)) {
            logger.info("admin gossip!");
            publishByAdmin(XoaPageAdminGossipPublisher.getInstance(), page, host,
                    FeedDefUtil.getGOSSIP_ADMIN_Def(page), context);
        } else {
            logger.info("fans gossip");
            publishByFans(XoaPageFansGossipPublisher.getInstance(), page, host,
                    FeedDefUtil.getGOSSIP_Def(page), context);
        }
        return "@done";
    }

    /**
     * 管理员分发
     * 
     * @param publisher
     * @param page
     * @param curHost
     * @param feedType
     * @param context
     * @throws PageException
     */
    private boolean publishByAdmin(FeedPublisher publisher, Page page, User curHost, long feedType,
            IContext context) throws PageException {
        try {
            // 特殊控制了的，不走其他的逻辑了......
            if (null != feedDefBean && feedDefBean.controlled(feedType, page)) {
                boolean news = feedDefBean.isSendNewsFeed(feedType, page);
                context.setSendNews(news);
                logger.info(String
                        .format("publish-feed byAdmin special feed controlled:page=%s(%s),user=%s(%s),feedType=%s(%s)",
                                page.getId(), page.getName(), curHost.getId(), curHost.getName(),
                                feedType, publisher.getFeedName()));
                // 都不发
                if (!news) {
                    return false;
                }
                // 如果page是关闭的，并且不是用于测试的。就不发送NewsFeed
                // 也不计入log
                if (page.isClosed() && !feedDefBean.getTestPages().contains(page.getId())) {
                    context.setSendNews(false);
                }
            } else {
                // 读取配置
                // 记录的时候没有设置的是不会记录在库表里面的
                // 为了判断是不是被配置过，所以要读取所有的，然后再获取，
                // 如果是在插库的时候做，那以后新增新鲜事的时候要统一设置一遍
                Map<Long, FeedConfig> configs = configService.findByPage(page.getId());
                if (logger.isDebugEnabled()) {
                    logger.debug(String
                            .format("publish-feed byAdmin config:domain=%s,page=%s(%s),user=%s(%s),feedType=%s(%s),configs=%s,context=%s",
                                    publisher.getSupportedDomain().getId(), page.getId(),
                                    page.getName(), curHost.getId(), curHost.getName(), feedType,
                                    publisher.getFeedName(), configs, context));
                }
                FeedConfig config = configs.get(feedType);
                // 没有设置过,使用默认的策略
                if (configs.isEmpty()) {
                    // 是否发送新鲜事的控制
                    context.setSendNews(true);
                } else {
                    // 其他的配置了，这一项没有配置,不发了
                    if (config == null) {
                        if (logger.isDebugEnabled()) {
                            logger.debug(String
                                    .format("publish-feed byAdmin no config of type:domain=%s,page=%s(%s),user=%s(%s),feedType=%s(%s),context=%s",
                                            publisher.getSupportedDomain().getId(), page.getId(),
                                            page.getName(), curHost.getId(), curHost.getName(),
                                            feedType, publisher.getFeedName(), context));
                        }
                        return false;
                    } else {
                        // 是否发送新鲜事的控制
                        context.setSendNews(config.getSendNews());
                    }
                }

                // 如果page是关闭的，并且不是用于测试的。就不发送NewsFeed
                // 也不计入log
                if (page.isClosed() && !feedDefBean.getTestPages().contains(page.getId())) {
                    context.setSendNews(false);
                }

                // 如果前面的策略允许发，然后过新鲜事发送几条的策略
                /*
                 * 即： 1、关闭了的page，是永远不发newsFeed的 2、后台设置过了的page，按照新鲜事类型的配置来处理，
                 */
                if (Boolean.TRUE.equals(context.getSendNews())) {
                    // 检查是否是设置过了的不发送newsfeed
                    context.setSendNews(true);
                }
            }
            
            //一击必杀，管理员不发mini feed
            context.setSendMini(false);
            publisher.publish(page, curHost, true, context, context.getImgType(),
                    context.getImgSrc());
        } catch (Exception e) {
            logger.error(
                    String.format(
                            "publish-feed byAdmin fail:domain=%s,page=%s(%s),user=%s(%s),feedType=%s(%s),context=%s",
                            publisher.getSupportedDomain().getId(), page.getId(), page.getName(),
                            curHost.getId(), curHost.getName(), feedType, publisher.getFeedName(),
                            context), e);
            // FIXME: 回滚积分
            throw new PageException(e);
        }

        if (logger.isDebugEnabled()) {
            logger.debug(String
                    .format("publish-feed byAdmin over:domain=%s,page=%s(%s),user=%s(%s),feedType=%s(%s),context=%s",
                            publisher.getSupportedDomain().getId(), page.getId(), page.getName(),
                            curHost.getId(), curHost.getName(), feedType, publisher.getFeedName(),
                            context));
        }

        return true;
    }

    private boolean publishByFans(FeedPublisher publisher, Page page, User curHost, long feedType,
            IContext context) throws PageException {
        try {
            if (feedDefBean.controlled(feedType, page)) {
                boolean mini = feedDefBean.isSendMiniFeed(feedType, page);
                context.setSendMini(mini);
                boolean news = feedDefBean.isSendNewsFeed(feedType, page);
                context.setSendNews(news);
                logger.info(String
                        .format("publish-feed byFans special feed controlled:page=%s(%s),user=%s(%s),feedType=%s(%s)",
                                page.getId(), page.getName(), curHost.getId(), curHost.getName(),
                                feedType, publisher.getFeedName()));
                // 都不发
                if (!news) {
                    return false;
                }
            } else {// 如果没特殊配置过，默认发新鲜事
                context.setSendMini(true);
                context.setSendNews(true);
            }
            publisher.publish(page, curHost, false, context, context.getImgType(),
                    context.getImgSrc());
        } catch (Exception e) {
            logger.error(String.format(
                    "publish-feed byFans fail:page=%s(%s),user=%s(%s),feedType=%s(%s),context=%s",
                    page.getId(), page.getName(), curHost.getId(), curHost.getName(), feedType,
                    publisher.getFeedName(), context), e);
            throw new PageException(e);
        }
        if (logger.isDebugEnabled()) {
            logger.debug(String
                    .format("publish-feed byFans over:domain=%s,page=%s(%s),user=%s(%s),feedType=%s(%s),context=%s",
                            publisher.getSupportedDomain().getId(), page.getId(), page.getName(),
                            curHost.getId(), curHost.getName(), feedType, publisher.getFeedName(),
                            context));
        }

        return true;
    }

}
