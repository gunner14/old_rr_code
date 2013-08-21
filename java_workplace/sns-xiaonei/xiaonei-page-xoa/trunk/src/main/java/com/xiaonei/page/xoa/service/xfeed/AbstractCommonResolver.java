package com.xiaonei.page.xoa.service.xfeed;

import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.page.feed.domain.BiddingHistory;
import com.renren.page.feed.service.BiddingService;
import com.xiaonei.page.domain.Page;
import com.xiaonei.page.domain.feed.FeedReply;
import com.xiaonei.page.service.feed.IContext;
import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.page.xoa.util.XFeedUtil;
import com.xiaonei.platform.core.model.User;

/**
 * @author yi.li@renren-inc.com since 2011-10-11
 * 
 */
public abstract class AbstractCommonResolver {

    @Autowired
    private BiddingService biddingService;

    //FIXME 异常怎么处理
    protected void setBiddingInfo(Page page, User curHost, IContext context, long feedType,
            long sourceId) {
        BiddingHistory bidding = biddingService.buildBidding(page, curHost, feedType, sourceId,
                null);
        context.setBidding(bidding);
        context.setSendMini(true);
        context.setSendNews(true);
    }

    protected void setReplyInfo(IContext context, PageFeedReply reply) {
        if (null == reply) {
            return;
        }
        FeedReply feedReply = new FeedReply();
        feedReply.setFromId(reply.getFromId());
        feedReply.setReplyId(reply.getReplyId());
        feedReply.setFromName(reply.getFromName());
        feedReply.setContent(reply.getContent());
        feedReply.setImContent(reply.getImContent());
        feedReply.setTime(reply.getTime());
        feedReply.setTinyImag(reply.getTinyImg());
        context.setFeedReply(feedReply);
    }

    protected void wrapListData(String key, List<Integer> datas, IContext context) {
        if (null == datas) {
            return;
        }
        int i = 0;
        for (Integer item : datas) {
            context.put(key + "." + i, XFeedUtil.getFeedString(item));
            i++;
        }
    }

    protected void wrapListData(String oldKey, String preKey, String postKey,
            Map<String, Object> feed, IContext context) {
        if (null == feed) {
            return;
        }
        Object data = feed.get(oldKey);
        if (null == data) {
            return;
        }
        if (data instanceof List) {
            List<?> dataList = (List<?>) data;
            int i = 0;
            for (Object item : dataList) {
                String innerKey = getInnerKey(preKey, postKey, i);
                context.put(innerKey, item.toString());
                i++;
            }
        } else {
            String innerKey = getInnerKey(preKey, postKey, 0);
            context.put(innerKey, data.toString());
        }
    }
    
    /**
     * @param inPreKey
     * @param inPostKey
     * @param preKey
     * @param postKey
     * @param feed
     * @param context
     * @param count
     */
    protected void wrapListForNewFeed(String inPreKey, String inPostKey, String preKey,
            String postKey, Map<String, Object> feed, IContext context, int count) {
        if (null == feed) {
            return;
        }

        //下面的处理基于完全信任数据来源
        for (int i = 0; i < count; i++) {
            String inKey = getInnerKey(inPreKey, inPostKey, i);
            String outKey = getInnerKey(preKey, postKey, i);
            context.put(outKey, XFeedUtil.getFeedString(feed.get(inKey)));
        }
    }
    
    /**
     * 获取新鲜事所需数据的Key
     * 
     * @param preKey    前缀
     * @param postKey   后缀
     * @param i         index
     * @return
     */
    private String getInnerKey(String preKey, String postKey, int i){
        StringBuilder innerKey = new StringBuilder();
        innerKey.append(preKey).append(".").append(i);
        if (null != postKey && !("".equals(postKey))) {
            innerKey.append(".").append(postKey);
        }
        
        return innerKey.toString();
    }
}
