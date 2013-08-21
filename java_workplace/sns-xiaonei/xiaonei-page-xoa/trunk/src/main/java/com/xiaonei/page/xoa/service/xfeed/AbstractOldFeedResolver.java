package com.xiaonei.page.xoa.service.xfeed;

import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.PageIconDict;
import com.xiaonei.page.domain.identify.Identifier;
import com.xiaonei.page.service.PageIconService;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.service.feed.impl.ContextImpl;
import com.xiaonei.page.service.identify.IdentifyService;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * @author yi.li@renren-inc.com since 2011-10-11
 * 
 */
public abstract class AbstractOldFeedResolver extends AbstractCommonResolver implements
        OldFeedResolver {

    protected final static Logger logger = Logger.getLogger("PAGEXOA");

    @Autowired
    private IdentifyService identifyService;

    protected IContext getDefaultContext(Map<String, Object> feed, PageFeedReply reply, Page page,
            User curHost, long sourceId) {
        IContext context = new ContextImpl();
        setDefaultFeedInfo(context, feed, page);
        setBiddingInfo(page, curHost, context, getSupportedFeedType(), sourceId);
        setReplyInfo(context, reply);
        return context;
    }

    private void setDefaultFeedInfo(IContext context, Map<String, Object> feed, Page page) {
        context.put("time", System.currentTimeMillis());
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
            logger.error("Query page certify icon error, excpeiton: ", e);
        }
        context.put("origin.type", XFeedUtil.getFeedString(feed.get("wap")));
        context.put("origin.url", XFeedUtil.getFeedString(feed.get("hLink")));
        context.put("origin.title", XFeedUtil.getFeedString(feed.get("hText")));
        context.put("origin.image", XFeedUtil.getFeedString(feed.get("icon")));
        context.put("stat.id", XFeedUtil.getFeedString(feed.get("statID")));
        context.put("stat.level", XFeedUtil.getFeedString(feed.get("level")));
        context.put("minigroup.id", XFeedUtil.getFeedString(feed.get("gName")));
        context.put("minigroup.url", XFeedUtil.getFeedString(feed.get("gUrl")));
        context.put("minigroup.name", XFeedUtil.getFeedString(feed.get("gID")));
    }

    protected long getSourceId(String key, Map<String, Object> feed) {
        String strDoingId = XFeedUtil.getFeedString(feed.get(key));
        if ("".equals(strDoingId)) {
            throw new RuntimeException("get source id error!!! --> null");
        }
        long sourceId = Long.parseLong(strDoingId);
        return sourceId;
    }
}
