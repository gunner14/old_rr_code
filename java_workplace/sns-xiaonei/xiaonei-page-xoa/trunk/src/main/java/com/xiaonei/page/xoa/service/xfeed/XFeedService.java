package com.xiaonei.page.xoa.service.xfeed;

import java.util.Map;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.xiaonei.org.PageException;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.PageService;
import com.xiaonei.page.service.feed.FeedManager;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.XFeedVersion;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.util.XFeedUtil;
//import com.xiaonei.page.xoa.service.xfeed.comment.ISourceResolver;
//import com.xiaonei.page.xoa.service.xfeed.comment.SourceResolverFactory;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 新版新鲜事发送服务
 * 
 * @author yi.li@renren-inc.com since 2011-10-7
 * 
 */
@Service
public class XFeedService {

    private final static Log logger = LogFactory.getLog("PAGEXOA");

    @Autowired
    private FeedResolverFactory resolverFactory;

//    @Autowired
//    private SourceResolverFactory sourceResolverFactory;

    @Autowired
    private FeedManager feedManager;

    /**
     * 发送新鲜事
     * 
     * @param <T> feed数据model
     * @param pageId pageId
     * @param userId 真实用户id
     * @param feedType 新鲜事类型
     * @param clazz feed model class
     * @param feed feed数据
     * @param reply 回复数据
     * @throws Exception
     */
    public <T extends PageFeed> void publishFeed(int pageId, int userId, long feedType,
            Class<T> clazz, T feed, PageFeedReply reply) throws Exception {
        User user = WUserAdapter.getInstance().get(userId);
        Page page = PageService.getInstance().get(pageId);
        if (null == page || null == user) {
            throw new PageException("User or Page info not available, user=" + user + "|userid="
                    + userId + ", page=" + page + "|pageId=" + pageId);
        }
        FeedResolver<T> resolver = resolverFactory.getFeedResolver(clazz);
        if (null == resolver) {
            throw new PageException("Feed resolver not found exception.");
        }
        IContext context = resolver.resolveParam(feed, page, user, reply);
        feedManager.publishForXoa(page, user, feedType, true, context);
    }

    /**
     * @param pageId
     * @param userId
     * @param feedType
     * @param feed
     * @param reply
     */
    public void publishFeedForGeneric(int pageId, int userId, long feedType,
            Map<String, Object> feed, PageFeedReply reply) {
        if (logger.isDebugEnabled()) {
            logger.debug("publish feed: " + feed + "; feedType - " + feedType);
        }
        User user = WUserAdapter.getInstance().get(userId);
        Page page = PageService.getInstance().get(pageId);
        if (null == page || null == user) {
            throw new PageException("User or Page info not available, user=" + user + "|userid="
                    + userId + ", page=" + page + "|pageId=" + pageId);
        }
        Long pageFeedType = FeedTypeMap.getMappedPageFeed(feedType);
        if (null == pageFeedType) {
            throw new PageException("Can not find page feed type for (" + feedType + ")");
        }
        
        // add log for statistic
        logger.info(String.format("xfeed-statistic|%s|%s|%s|%s", pageId, userId, pageFeedType,
                feedType));

        OldFeedResolver resolver = resolverFactory.getOldFeedResolver(pageFeedType);
        if (null == resolver) {
            throw new PageException("Feed resolver not found exception.");
        }
        IContext context = resolver.resolveParam(feed, page, user, reply);
        
		{
			String statID = context.get("statID");
			String level = context.get("level");
			if (statID != null && !("".equals(statID))) {
				context.put("statID", statID);
			} else {
				StringBuilder sb = new StringBuilder("page_");
				sb.append(page.getId());
				sb.append("_2");
				context.put("stat.id", sb.toString());
			}
			if (level != null && !("".equals(level))) {
				context.put("stat.level", Integer.parseInt(level) + "");
			} else {
				context.put("stat.level", 1 + "");
			}
		}

        //判断是否是老版的新鲜事，因为page Web端使用的是UGC的XOA接口，所以最后还是会回到这
        //但是在过渡期我们要避免发出来新老两种新鲜事
        String feedVersion = XFeedUtil.getFeedString(feed.get(FeedDefUtil.XFEED_VERSION));
        if (null != feedVersion && XFeedVersion.OLD_FEED.getVersion().equals(feedVersion)) {
            return;
        }
        
        feedManager.publishForXoa(page, user, pageFeedType, true, context);
    }

    /**
     * 发布评论新鲜事
     * 
     * @param replys
     * @param source UGC id
     * @param feedType 新鲜事类型
     * @param actor 触发评论的用户id
     * @param sourceOwner 资源OwnerID
     * @throws Exception
     */
//    public void publishCommentFeed(PageFeedReply reply, long source, int feedType, int actor,
//            int sourceOwner) throws Exception {
//        User user = WUserAdapter.getInstance().get(actor);
//        Page page = PageService.getInstance().get(sourceOwner);
//        if (null == user || null == page) {
//            logger.error("publish comment feed: null user(" + actor + ")-" + user
//                    + " or null page(" + sourceOwner + ")-" + page);
//            return;
//        }
//        ISourceResolver resolver = sourceResolverFactory.getSourceResolver(feedType);
//        if (null == resolver) {
//            logger.error("publish comment feed: null source resolver(" + feedType + ")");
//            return;
//        }
//        IContext context = resolver.resolveSource(source, page, user, reply);
//        feedManager.publishForXoa(page, user, feedType, this.isAdmin(actor, sourceOwner), context);
//    }

    /**
     * 切换身份后只需要判断actor是否为pageId就能知道他是不是管理员
     * 
     * @param actor
     * @param pageId
     * @return
     */
//    private boolean isAdmin(int actor, int pageId) {
//        if (!PageService.getInstance().isPageDomainId(pageId)) {
//            return false;
//        }
//        if (actor == 0 || actor != pageId) {
//            return false;
//        }
//        if (actor == pageId) {
//            return true;
//        }
//        return false;
//    }

    /**
     * 删除新鲜事接口
     * 
     * @param userId
     * @param realUid
     * @param feedType
     * @param sourceId
     */
    public void removeFeedForGeneric(int userId, int realUid, long feedType, long sourceId) {
        try {
            logger.info(String.format(
                    "Remove Feed - [userId:%s, realUid:%s, feeType:%s, sourceId:%s]", userId,
                    realUid, feedType, sourceId));
            feedManager.removeFeed(userId, realUid, sourceId, (int) feedType);
        } catch (Exception e) {
            logger.error("Remove Feed Error - source:" + sourceId, e);
        }
    }
}
