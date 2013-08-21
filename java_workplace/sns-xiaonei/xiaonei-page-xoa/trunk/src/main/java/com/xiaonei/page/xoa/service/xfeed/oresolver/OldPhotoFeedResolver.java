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
public class OldPhotoFeedResolver extends AbstractOldFeedResolver implements OldFeedResolver {

    @Override
    public long getSupportedFeedType() {
        return FeedDefUtil.PAGE_PHOTO_PUBLISH;
    }

    @Override
    public IContext resolveParam(Map<String, Object> feed, Page page, User curHost,
            PageFeedReply reply) {
        long albumId = getSourceId("album.id", feed);
        IContext context = getDefaultContext(feed, reply, page, curHost, albumId);
        context.put("origin.url", XFeedUtil.getFeedString(feed.get("origin.url")));
        context.put("origin.title", XFeedUtil.getFeedString(feed.get("origin.title")));
        context.put("album.id", XFeedUtil.getFeedString(feed.get("album.id")));
        context.put("album.title", XFeedUtil.getFeedString(feed.get("album.name")));
        //        context.put("album.digest", XFeedUtil.getFeedString(feed.get("")));
        context.put("album.url", XFeedUtil.getFeedString(feed.get("album.url")));
        context.put("album.count", getuploadCount(feed));

        int updateCount = getuploadCount(feed);
        if (updateCount > 3) {
            updateCount = 3;
        }
        wrapListForNewFeed("photo", "id", "album.photo", "id", feed, context, updateCount);
        wrapListForNewFeed("photo", "hUrl", "album.photo", "image", feed, context, updateCount);
        wrapListForNewFeed("photo", "mUrl", "album.photo", "mainImage", feed, context, updateCount);
        wrapListForNewFeed("photo", "lUrl", "album.photo", "bigImage", feed, context, updateCount);
        wrapListForNewFeed("photo", "url", "album.photo", "url", feed, context, updateCount);
        wrapListForNewFeed("photo", "title", "album.photo", "desc", feed, context, updateCount);

        //        wrapListData("pId", "album.photo", "id", feed, context);
        //        wrapListData("src", "album.photo", "image", feed, context);
        //        wrapListData("bigSrc", "album.photo", "bigImage", feed, context);
        //        wrapListData("mainSrc", "album.photo", "mainImage", feed, context);
        //        wrapListData("url", "album.photo", "url", feed, context);
        //        wrapListData("status", "album.photo", "desc", feed, context);
        //        wrapListData("descfull", "album.photo", "descfull", feed, context);
        return context;
    }
    
	private int getuploadCount(Map<String, Object> feed) {
		int count = 0;
		String uploadCountKey = "upload.0.count"; 
		
        if (feed.containsKey(uploadCountKey)) {
            try {
                int upCount = Integer.parseInt(XFeedUtil.getFeedString(feed.get(uploadCountKey)));
                return upCount;
            } catch (NumberFormatException e) {}
        }
		
        
        //下面的这段代码是由于以前upload.count还是single key的时候来兼容的
		for (int i = 0; i < 4; i++) {
			String key = "photo." + i + ".id";
			if (feed.containsKey(key)) {
				count++;
			}
		}

		if (count == 0) {
			throw new RuntimeException("publish photo feed(mul): no photo data");
		}
		if (count > 3) {
			return 3;
		}
		return count;
	}

}
