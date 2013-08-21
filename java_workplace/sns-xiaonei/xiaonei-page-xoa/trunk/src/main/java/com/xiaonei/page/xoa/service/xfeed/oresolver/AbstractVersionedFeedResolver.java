package com.xiaonei.page.xoa.service.xfeed.oresolver;

import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageIconDict;
import com.xiaonei.page.domain.identify.Identifier;
import com.xiaonei.page.service.PageIconService;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.XFeedVersion;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.service.identify.IdentifyService;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.service.xfeed.AbstractOldFeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 分享业务开始向新的新鲜事平台迁移后，我们需要做一个新老数据的兼容
 * 
 * @author yi.li@renren-inc.com since 2011-12-7
 * 
 */
public abstract class AbstractVersionedFeedResolver extends AbstractOldFeedResolver {
    
    @Autowired
    private IdentifyService identifyService;

    @Override
    public IContext resolveParam(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        String _version = XFeedUtil.getFeedString(feed.get(FeedDefUtil.XFEED_VERSION));
        if (XFeedVersion.NEW_FEED.getVersion().equals(_version)) {
            return resolveNewVersion(feed, page, curHost, reply);
        }
        return resolveOldVersion(feed, page, curHost, reply);
    }

    /**
     * 老版本的解析方法
     */
    public abstract IContext resolveOldVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply);

    /**
     * 新版本的解析方法
     */
    public abstract IContext resolveNewVersion(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply);

    protected IContext getNewShareDefaultContext(Map<String, Object> feed, PageFeedReply reply,
            Page page, User curHost, long sourceId) {
        IContext context = new ContextImpl();
        setNewShareDefaultFeedInfo(context, feed, page);
        setBiddingInfo(page, curHost, context, getSupportedFeedType(), sourceId);
        setReplyInfo(context, reply);
        return context;
    }

    private void setNewShareDefaultFeedInfo(IContext context, Map<String, Object> feed, Page page) {
        context.put(
                "time",
                XFeedUtil.getFeedString(feed.get("time")).equals("") ? System.currentTimeMillis()
                        : XFeedUtil.getFeedString(feed.get("time")));
        context.put("from.id", page.getId());
        context.put("from.name", page.getName());
        context.put("from.url", String.format("%s/%s?", OpiConstants.urlPage, page.getId()));
        context.put("from.tinyimg", page.getTinyUrl());
        try {
            Identifier identifier = identifyService.getAndCreateIfNotExist(page);
            int identifyType = 0;
            if (null != identifier) {
                identifyType = identifier.getIconType();
            }
            context.put("from.certifyType", XFeedUtil.getFeedString(identifyType));
            PageIconDict icon = PageIconService.getInstance().getFristIcon(page);
            context.put("from.certifyIcon", icon == null ? "" : icon.getPic());
            context.put("from.certifyTitle", icon == null ? "" : icon.getDesc());
        } catch (Exception e) {
            logger.error("Query page certify icon error, excpeiton: " + e);
        }
        //        context.put("origin.type", XFeedUtil.getFeedString(feed.get("origin.type")));
        context.put("share.id", XFeedUtil.getFeedString(feed.get("share.id")));
        context.put("share.comment", XFeedUtil.getFeedString(feed.get("share.comment")));
        context.put("origin.url", XFeedUtil.getFeedString(feed.get("origin.url")));
        context.put("origin.title", XFeedUtil.getFeedString(feed.get("origin.title")));
        context.put("origin.image", XFeedUtil.getFeedString(feed.get("origin.image")));
        //        context.put("stat.id", XFeedUtil.getFeedString(feed.get("statID")));
        //        context.put("stat.level", XFeedUtil.getFeedString(feed.get("level")));
        //        context.put("minigroup.id", XFeedUtil.getFeedString(feed.get("gName")));
        //        context.put("minigroup.url", XFeedUtil.getFeedString(feed.get("gUrl")));
        //        context.put("minigroup.name", XFeedUtil.getFeedString(feed.get("gID")));
    }

}
