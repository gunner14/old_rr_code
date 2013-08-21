package com.renren.sns.wiki.biz.feed;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.biz.feed.impl.IFeed2Map;
import com.xiaonei.platform.component.xfeed.display.model.Feed;
import com.xiaonei.xce.globalid.GlobalId;

/**
 * 将feed转化为对应的jsonObject的工厂类
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-27
 * 
 */
public class Feed2MapUtils {

    private static final Log logger = LogFactory.getLog(Feed2MapUtils.class);

    private static Map<String, IFeed2Map> obejctMap = new HashMap<String, IFeed2Map>();

    /**
     * 把新鲜事对应的字段赋值给Map,用于转化为json
     * 
     * @param feed
     * @return
     */
    @SuppressWarnings("unchecked")
    public static Map<String, Object> covert(Feed feed) {

        int feedType = getFeedType(feed);

        String className = "com.renren.sns.wiki.biz.feed.impl.Feed2Map" + feedType + "Impl";
        //根据className从map获取class

        if (!obejctMap.containsKey(className)) {
            Class<? extends IFeed2Map> c = null;
            try {
                c = (Class<? extends IFeed2Map>) Class.forName(className);
            } catch (ClassNotFoundException e) {
                logger.info("Class " + className + " NotFound");
                //把null值保存到Map
                obejctMap.put(className, null);
                return null;
            }
            try {
                IFeed2Map o = c.newInstance();
                obejctMap.put(className, o);
            } catch (Exception e) {
                logger.warn(className + " newInstance exception");
                return null;
            }
        }

        IFeed2Map feed2Map = obejctMap.get(className);
        if (feed2Map == null) {
            logger.warn("className 没有对应的实现类");
            return null;
        }
        Map<String, Object> map = feed2Map.convert(feed);

        if (map == null) {
            return null;
        }
        completeFeedMap(map);
        return map;
    }

    /**
     * 获取map后,修复完善的Map基本信息,包括sourceUrl, gloabalId,以及一些属性的初始值
     * 
     * @param map
     */
    private static void completeFeedMap(Map<String, Object> map) {
        int feedType = (Integer) map.get("feedType");

        String strOwnerId = (String) map.get("ownerId");
        String strSourceId = (String) map.get("sourceId");

        //因为ownerId是前端渲染头像必须,sourceId获取globalId必须的,所以如果这两个值不存在,将会在后面舍弃掉这两条feed,无需完善map信息,直接返回
        if (strOwnerId.length() == 0 || strSourceId.length() == 0) {
            return;
        }
        //判断是不是纯数字组成
        if (!strOwnerId.matches("\\d+") || !strSourceId.matches("\\d+")) {
            return;
        }
        int ownerId = Integer.parseInt(strOwnerId);
        long sourceId = Long.parseLong(strSourceId);

        String globalId = "";
        String sourceUrl = (String) map.get("sourceUrl");
        switch (feedType) {
            case 110://分享
                sourceUrl = "http://share.renren.com/share/" + ownerId + "/" + sourceId;
                globalId = GlobalId.fromShare(sourceId).toString();
                break;
            case 502://状态
                sourceUrl = "http://www.renren.com/" + ownerId;
                globalId = GlobalId.fromStatus(sourceId).toString();
                break;
            case 601://日志 http://blog.renren.com/blog/404478938/862714794
                sourceUrl = " http://blog.renren.com/blog/" + ownerId + "/" + sourceId;
                globalId = GlobalId.fromBlog(sourceId).toString();
                break;
            case 701://单图
                sourceUrl = "http://photo.renren.com/photo/" + ownerId + "/photo-" + sourceId;
                globalId = GlobalId.fromPhoto(sourceId).toString();
                break;
            case 709://多图
                sourceUrl = "http://photo.renren.com/photo/" + ownerId + "/album-" + sourceId;
                globalId = GlobalId.fromAlbum(sourceId).toString();
                break;
            default:
                break;
        }

        map.put("sourceUrl", sourceUrl);
        map.put("globalId", globalId);

        completeInitValue(map, "albumCount", "0");
        completeInitValue(map, "likedNum", "0");
        completeInitValue(map, "shareNum", "0");
        completeInitValue(map, "replyNum", "0");
    }

    /**
     * 根据feed获取feedType
     * 
     * @param feed
     * @return
     */
    public static int getFeedType(Feed feed) {
        int feedType = 0;
        if (feed == null) {
            return feedType;
        }
        feedType = feed.getFeedType();
        //有部分老的新鲜事取不到feedType,所以从feedMap中获取feedType
        if (feedType == 0) {
            Map<String, List<String>> feedMap = feed.getData();
            if (feedMap != null) {
                List<String> feedTypeList = feedMap.get("f_type");
                if (feedTypeList != null && feedTypeList.size() > 0) {
                    String strFeedType = feedTypeList.get(0);
                    if (strFeedType.matches("\\d+")) {
                        feedType = Integer.parseInt(strFeedType);
                    }
                }
            }
        }
        return feedType;
    }

    /**
     * 根据feed获取feedId
     * 
     * @param feed
     * @return
     */
    public static long getFeedId(Feed feed) {
        long feedId = 0;
        if (feed == null) {
            return feedId;
        }
        Map<String, List<String>> feedMap = feed.getData();
        if (feedMap != null) {
            //feed.getData().get("id").get(0);
            List<String> feedIdList = feedMap.get("id");
            if (feedIdList != null && feedIdList.size() > 0) {
                String strFeedId = feedIdList.get(0);
                if (strFeedId.matches("\\d+")) {
                    feedId = Long.parseLong(strFeedId);
                }
            }
        }
        return feedId;
    }

    /**
     * 完善需要初始化的信息
     * 
     * @param jsonObject
     */
    private static void completeInitValue(Map<String, Object> map, String key, String defaultValue) {
        if (map == null) {
            return;
        }
        if (!map.containsKey(key)) {
            map.put(key, defaultValue);
        } else {
            String value = (String) map.get(key);
            if (StringUtils.isBlank(value)) {
                map.put(key, defaultValue);
            }
        }
    }
}
