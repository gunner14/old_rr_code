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
 * 发布状态
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-2
 * 
 */
public class Feed2Map502Impl implements IFeed2Map {

    private static final int FEED_TYPE = 502;

    private static final String ITEM_TYPE = ItemType.STATUS.getValue();

    private static List<JsonFeedKeyPair> keyList = new ArrayList<JsonFeedKeyPair>();

    static {
        keyList.add(new JsonFeedKeyPair("title", "f_status_title"));
        keyList.add(new JsonFeedKeyPair("updateDate", "f_status_time"));
        keyList.add(new JsonFeedKeyPair("ownerHeadUrl", "f_from_tinyimg"));
        keyList.add(new JsonFeedKeyPair("feedId", "id"));
        keyList.add(new JsonFeedKeyPair("ownerName", "f_from_name"));
        keyList.add(new JsonFeedKeyPair("ownerId", "f_from_id"));
        keyList.add(new JsonFeedKeyPair("sourceId", "f_status_id"));
        keyList.add(new JsonFeedKeyPair("replyNum", "reply_count"));

        keyList.add(new JsonFeedKeyPair("summary", ""));
        keyList.add(new JsonFeedKeyPair("sourceUrl", ""));//个人主页地址
        keyList.add(new JsonFeedKeyPair("likedNum", ""));
        keyList.add(new JsonFeedKeyPair("isLiked", ""));
        keyList.add(new JsonFeedKeyPair("shareNum", ""));
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

        return map;
    }
}
