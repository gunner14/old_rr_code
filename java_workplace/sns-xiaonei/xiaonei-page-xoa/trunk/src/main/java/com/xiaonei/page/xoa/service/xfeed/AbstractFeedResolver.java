package com.xiaonei.page.xoa.service.xfeed;


import org.apache.log4j.Logger;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageIconDict;
import com.xiaonei.page.service.PageIconService;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * @author yi.li@renren-inc.com since 2011-9-30
 * 
 * @param <T>
 */
public abstract class AbstractFeedResolver<T extends PageFeed> extends AbstractCommonResolver implements FeedResolver<T> {

    protected final static Logger logger = Logger.getLogger("PAGEXOA");

    protected IContext getDefaultContext(PageFeed feed, PageFeedReply reply, Page page,
            User curHost, long feedType, long sourceId) {
        IContext context = new ContextImpl();
        setDefaultFeedInfo(context, page, feed);
        setBiddingInfo(page, curHost, context, feedType, sourceId);
        setReplyInfo(context, reply);
        return context;
    }

    private void setDefaultFeedInfo(IContext context, Page page, PageFeed feed) {
        context.put("time", System.currentTimeMillis());
        context.put("from.id", page.getId());
        context.put("from.name", page.getName());
        context.put("from.url", String.format("%s/%s?", OpiConstants.urlPage, page.getId()));
        context.put("from.tinyimg", page.getTinyUrl());
        try {
            PageIconDict icon = PageIconService.getInstance().getFristIcon(page);
            context.put("from.certifyIcon", icon == null ? "" : icon.getPic());
            context.put("from.certifyTitle", icon == null ? "" : icon.getDesc());
        } catch (Exception e) {
            logger.error("Query page certify icon error, excpeiton: " + e);
        }
        context.put("origin.type", XFeedUtil.getFeedString(feed.getOriginType()));
        context.put("origin.url", XFeedUtil.getFeedString(feed.getOriginUrl()));
        context.put("origin.title", XFeedUtil.getFeedString(feed.getOriginTitle()));
        context.put("origin.image", XFeedUtil.getFeedString(feed.getOriginImage()));
        context.put("stat.id", XFeedUtil.getFeedString(feed.getStatId()));
        context.put("stat.level", XFeedUtil.getFeedString(feed.getStatLevel()));
        context.put("minigroup.id", XFeedUtil.getFeedString(feed.getMinigroupId()));
        context.put("minigroup.url", XFeedUtil.getFeedString(feed.getMinigroupUrl()));
        context.put("minigroup.name", XFeedUtil.getFeedString(feed.getMinigroupUrl()));
    }
    
}
