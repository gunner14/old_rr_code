package com.xiaonei.page.xoa.service.xfeed.oresolver;

import java.util.Map;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.service.xfeed.AbstractOldFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.OldFeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;


/**
 * @author yi.li@renren-inc.com since 2011-10-11
 *
 */
@Component
public class OldDoingFeedResolver extends AbstractOldFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_STATUS_UPDATE;
    }

    @Override
    public IContext resolveParam(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long doingId = getSourceId("dID", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, doingId);
        context.put("status.id", XFeedUtil.getFeedString(feed.get("dID")));
        context.put("status.title", XFeedUtil.getFeedString(feed.get("title")));
        context.put("status.web.title", XFeedUtil.getFeedString(feed.get("titleNew")));
        context.put("status.raw", XFeedUtil.getFeedString(feed.get("raw")));
        context.put("status.time", XFeedUtil.getFeedString(feed.get("dtime")));
        context.put("status.src", XFeedUtil.getFeedString(feed.get("src")));
        context.put("status.streamID", XFeedUtil.getFeedString(feed.get("streamId")));
        context.put("status.from.id", XFeedUtil.getFeedString(feed.get("dRUserID")));
        context.put("status.from.type", XFeedUtil.getFeedString(feed.get("fromType")));
        context.put("status.from.name", XFeedUtil.getFeedString(feed.get("dRUserName")));
        context.put("status.re.id", XFeedUtil.getFeedString(feed.get("dRID")));
        context.put("status.re.title", XFeedUtil.getFeedString(feed.get("titleRoot")));
        context.put("status.re.raw", XFeedUtil.getFeedString(feed.get("rawRoot")));
        context.put("status.re.src", XFeedUtil.getFeedString(feed.get("srcRoot")));
        context.put("lbs.id", XFeedUtil.getFeedString(feed.get("lbsId")));
        context.put("lbs.pid", XFeedUtil.getFeedString(feed.get("pid")));
        context.put("lbs.content", XFeedUtil.getFeedString(feed.get("lbsContent")));
        context.put("lbs.longitude", XFeedUtil.getFeedString(feed.get("longitude")));
        context.put("lbs.latitude", XFeedUtil.getFeedString(feed.get("latitude")));
        return context;
    }

}
