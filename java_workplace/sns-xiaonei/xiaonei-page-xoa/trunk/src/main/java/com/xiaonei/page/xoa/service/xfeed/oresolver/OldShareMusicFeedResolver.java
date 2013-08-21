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
public class OldShareMusicFeedResolver extends AbstractOldFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_SHARE_MUSIC;
    }

    @Override
    public IContext resolveParam(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long shareId = getSourceId("sID", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, shareId);
        context.put("share.id", XFeedUtil.getFeedString(feed.get("sID")));
        context.put("share.comment", XFeedUtil.getFeedString(feed.get("comm")));
        context.put("share.music.title", XFeedUtil.getFeedString(feed.get("title")));
        context.put("share.music.singer", XFeedUtil.getFeedString(feed.get("singer")));
        context.put("share.music.url", XFeedUtil.getFeedString(feed.get("url")));
        return context;
    }

}
