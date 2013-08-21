package com.xiaonei.page.xoa.service.xfeed.resovler;

import org.springframework.stereotype.Component;

import com.xiaonei.page.domain.Page;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.util.FeedDefUtil;
import com.xiaonei.page.xoa.model.feed.PageFeed;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.model.feed.PageShareMusicFeed;
import com.xiaonei.page.xoa.service.xfeed.AbstractFeedResolver;
import com.xiaonei.page.xoa.service.xfeed.FeedResolver;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-7
 *
 */
@Component
public class ShareMusicFeedResolver extends AbstractFeedResolver<PageShareMusicFeed> implements
        FeedResolver<PageShareMusicFeed> {

    @Override
    public Class<? extends PageFeed> getSupportClass() {
        return PageShareMusicFeed.class;
    }

    @Override
    public IContext resolveParam(PageShareMusicFeed feed, Page page, User curHost,
            PageFeedReply reply) {
        IContext context = getDefaultContext(feed, reply, page, curHost,
                FeedDefUtil.PAGE_SHARE_MUSIC, feed.getShareId());
        context.put("share.id", XFeedUtil.getFeedString(feed.getShareId()));
        context.put("share.comment", XFeedUtil.getFeedString(feed.getShareComment()));
        context.put("share.music.title", XFeedUtil.getFeedString(feed.getMusicTitle()));
        context.put("share.music.singer", XFeedUtil.getFeedString(feed.getMusicSinger()));
        context.put("share.music.url", XFeedUtil.getFeedString(feed.getMusicUrl()));
        return context;
    }

}
