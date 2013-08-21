package com.xiaonei.page.xoa.service.xfeed;

import java.util.Map;

import com.xiaonei.page.xoa.model.feed.ReplyContainer;

/**
 * 回复一条新鲜事，好的我们发新鲜事
 * 
 * @author yi.li@renren-inc.com since 2011-10-14
 * 
 */
public class CommentFeedService {

    /**
     * 发布评论新鲜事
     * 
     * @param replys        
     * @param source
     * @param sourceOwner
     * @param feedType
     * @param actor
     */
    public void publishComment(ReplyContainer replys, long source, int sourceOwner, int feedType,
            int actor) {
        Map<String, String> feedData = getFeedData(source, feedType);
        wrapReplyAndUser(replys, actor);
    }

    private Map<String, String> getFeedData(long source, int feedType) {
        return null;
    }

    /**
     * 填充评论信息中
     * 
     * @param replys
     * @param actor
     */
    private void wrapReplyAndUser(ReplyContainer replys, int actor) {

    }
}
