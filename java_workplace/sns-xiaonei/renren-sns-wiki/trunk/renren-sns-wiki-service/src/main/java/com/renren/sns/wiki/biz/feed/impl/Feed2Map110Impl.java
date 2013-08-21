package com.renren.sns.wiki.biz.feed.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.model.feed.JsonFeedKeyPair;
import com.xiaonei.platform.component.xfeed.display.model.Feed;

/**
 * 
 * 视频分享110
 * 
 * 将stype是110的feed转化为wiki新鲜事的Map(json)形式
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-2
 * 
 */
public class Feed2Map110Impl implements IFeed2Map {

    private static final int FEED_TYPE = 110;

    private static final String ITEM_TYPE = ItemType.VIDEO.getValue();

    private static List<JsonFeedKeyPair> keyList = new ArrayList<JsonFeedKeyPair>();

    static {
        keyList.add(new JsonFeedKeyPair("updateDate", "f_time"));
        keyList.add(new JsonFeedKeyPair("ownerId", "f_from_id"));
        keyList.add(new JsonFeedKeyPair("ownerName", "f_from_name"));
        keyList.add(new JsonFeedKeyPair("ownerHeadUrl", "f_from_tinyimg"));
        keyList.add(new JsonFeedKeyPair("sourceId", "f_share_id"));
        keyList.add(new JsonFeedKeyPair("summary", "f_share_summary"));
        keyList.add(new  JsonFeedKeyPair("feedId", "id"));
        keyList.add(new JsonFeedKeyPair("replyNum", "reply_count"));
        keyList.add(new JsonFeedKeyPair("title", "f_share_video_title"));
        keyList.add(new JsonFeedKeyPair("playerUrl", "f_share_video_url"));
        //需要,但是没有的数据
        keyList.add(new JsonFeedKeyPair("summary", ""));
        keyList.add(new JsonFeedKeyPair("sourceUrl", ""));//终端页地址  http://share.renren.com/share/431716353/12800802834
        keyList.add(new JsonFeedKeyPair("likedNum", ""));
        keyList.add(new JsonFeedKeyPair("isLiked", ""));
        keyList.add(new JsonFeedKeyPair("albumCount", ""));

        keyList.add(new JsonFeedKeyPair("albumCount", ""));
        keyList.add(new JsonFeedKeyPair("coverImages", ""));
    }

    public Map<String, Object> convert(Feed feed) {

        if (feed == null) {
            return null;
        }

        Map<String, Object> map = new HashMap<String, Object>();
        map.put("itemType", ITEM_TYPE);
        map.put("feedType", FEED_TYPE);

        Map<String, List<String>> feedMap = feed.getData();
        for (JsonFeedKeyPair jsonFeedKeyPair : keyList) {
            String jsonKey = jsonFeedKeyPair.getJsonKey();
            String feedKey = jsonFeedKeyPair.getFeedKey();
            if (StringUtils.isBlank(jsonKey)) {
                continue;
            }
            List<String> feedKeyList = feedMap.get(feedKey);
            if (feedKeyList != null && feedKeyList.size() > 0) {
                map.put(jsonKey, feedKeyList.get(0));
            } else {
                map.put(jsonKey, "");
            }
        }
        //封面图片处理 f_share_video_image
        List<String> coverImageUrlList = feedMap.get("f_share_video_image");

        List<Map<String, Object>> coverImagesList = new ArrayList<Map<String, Object>>();
        if (coverImageUrlList != null && coverImageUrlList.size() > 0) {
            for (String coverImageUrl : coverImageUrlList) {
                Map<String, Object> coverImageMap = new HashMap<String, Object>();
                coverImageMap.put("url", coverImageUrl);
                coverImageMap.put("width", "293");
                coverImageMap.put("height", "192");
                coverImagesList.add(coverImageMap);
            }
        }
        map.put("coverImages", coverImagesList);

        return map;
    }

}
