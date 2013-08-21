package com.renren.sns.wiki.delegator.xoa;

import org.springframework.stereotype.Service;

import com.renren.app.share.model.Share;
import com.renren.app.share.model.ShareComment;
import com.renren.app.share.xoa.api.ShareCommentService;
import com.renren.app.share.xoa.api.ShareService;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;

/**
 * 
 * 分享xoa的封装
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-2
 * 
 */
@Service
public class ShareXoaDelegator {

    /**
     * 根据ownerId和shareId获取分享
     * 
     * @param ownerId
     * @param shareId
     * @return
     */
    public Share getShare(int ownerId, long shareId) {
        ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        ShareService shareService = fact.getService(ShareService.class);//通过ServiceFactory来获取具体了Service实例
        ServiceFuture<Share> serviceFuture = shareService.getShare(ownerId, shareId);
        Share share = ServiceFutureHelper.execute(serviceFuture, 500);
        return share;
    }

    /**
     * 获取分享的评论
     * 
     * @param ownerId
     * @param shareId
     * @return
     */
    public ShareComment[] getComments(int ownerId, long shareId, int offset, int limit,
            int visitorId) {
        ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        ShareCommentService shareCommentService = fact.getService(ShareCommentService.class);//通过ServiceFactory来获取具体了Service实例
        ServiceFuture<ShareComment[]> serviceFuture = shareCommentService.findComments(ownerId,
                shareId, offset, limit, visitorId);
        ShareComment[] shareComments = ServiceFutureHelper.execute(serviceFuture, 500);
        return shareComments;
    }

    /**
     * 获取评论的条数
     * 
     * @param ownerId
     * @param shareId
     * @param visitorId
     * @return
     */
    public Integer getCommentCount(int ownerId, long shareId, int visitorId) {
        ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        ShareCommentService shareCommentService = fact.getService(ShareCommentService.class);//通过ServiceFactory来获取具体了Service实例
        ServiceFuture<Integer> serviceFuture = shareCommentService.getCommentCount(ownerId,
                shareId, visitorId);
        Integer count = ServiceFutureHelper.execute(serviceFuture, 500);
        return count;
    }
}
