package com.xiaonei.page.xoa.util;

import java.text.SimpleDateFormat;
import java.util.Locale;

import org.apache.commons.lang.StringEscapeUtils;

import com.xiaonei.page.xoa.model.feed.PageFeedReply;
import com.xiaonei.platform.component.xfeed.reply.FeedReplyContentBuilder;


/**
 * @author yi.li@renren-inc.com since 2011-10-16
 *
 */
public class XFeedUtil {
    
    public static String getFeedString(String value){
        if(null == value || "".equals(value)){
            return "";
        }
        return unescapXml(value);
    }
    
    public static String getFeedString(int value){
        if(value > 0){
            return "" + value;
        }
        return "";
    }
    
    public static String getFeedString(long value){
        if(value > 0){
            return "" + value;
        }
        return "";
    }
    
    public static String getFeedString(Object value){
        if(null == value){
            return "";
        }
        return unescapXml(value.toString());
    }
    
    private static String unescapXml(String value){
        return StringEscapeUtils.unescapeXml(value);
    }
    
    public static String getFeedReplyXml(PageFeedReply reply) {
        if (null == reply) {
            return null;
        }
        FeedReplyContentBuilder builder = new FeedReplyContentBuilder();
        builder.tinyImg(reply.getTinyImg());
        builder.body(reply.getContent());
        builder.imBody(reply.getContent());
        builder.replyId(reply.getReplyId());
        builder.time(new SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.US).format(reply.getTime()));
        builder.actorName(reply.getFromName());
        builder.actor(reply.getFromId());
        return builder.getXml();
    }
}
