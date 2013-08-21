package com.renren.sns.wiki.biz.feed.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringEscapeUtils;
import org.apache.commons.lang.StringUtils;

import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.model.feed.JsonFeedKeyPair;
import com.xiaonei.platform.component.xfeed.display.model.Feed;

/**
 * 发表日志新鲜事
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-4
 * 
 */
public class Feed2Map601Impl implements IFeed2Map {

    private static final int FEED_TYPE = 601;

    private static final String ITEM_TYPE = ItemType.BLOG.getValue();

    private static List<JsonFeedKeyPair> keyList = new ArrayList<JsonFeedKeyPair>();

    static {

        keyList.add(new JsonFeedKeyPair("sourceUrl", "f_blog_url"));
        keyList.add(new JsonFeedKeyPair("title", "f_blog_title"));
        keyList.add(new JsonFeedKeyPair("ownerHeadUrl", "f_from_tinyimg"));
        keyList.add(new JsonFeedKeyPair("feedId", "id"));
        keyList.add(new JsonFeedKeyPair("ownerName", "f_from_name"));
        keyList.add(new JsonFeedKeyPair("ownerId", "f_from_id"));
        keyList.add(new JsonFeedKeyPair("sourceId", "f_blog_id"));
        keyList.add(new JsonFeedKeyPair("updateDate", "f_time"));
        keyList.add(new JsonFeedKeyPair("summary", "f_blog_digest")); //日志内容

        //需要,但是没有的数据
        keyList.add(new JsonFeedKeyPair("likedNum", ""));
        keyList.add(new JsonFeedKeyPair("isLiked", ""));
        keyList.add(new JsonFeedKeyPair("albumCount", ""));

        keyList.add(new JsonFeedKeyPair("albumCount", ""));
        keyList.add(new JsonFeedKeyPair("coverImages", "")); //f_blog_photo_orgImage
        keyList.add(new JsonFeedKeyPair("ownerName", ""));
        keyList.add(new JsonFeedKeyPair("ownerHeadUrl", ""));
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
                String value = feedKeyList.get(0);
                if (jsonKey.equals("summary") && value.indexOf(";&#") != -1) {
                    value = StringEscapeUtils.unescapeHtml(value);
                }
                map.put(jsonKey, value);
            } else {
                map.put(jsonKey, "");
            }
        }

        //封面图片处理 f_share_video_image
        List<String> coverImageUrlList = null;
        String coverType = "";
        if (feedMap.containsKey("f_blog_photo_orgImage")) {
            coverImageUrlList = feedMap.get("f_blog_photo_orgImage");
            coverType = "image";
        } else if (feedMap.containsKey("f_blog_video_image")) {
            coverImageUrlList = feedMap.get("f_blog_video_image");
            coverType = "video";
        }
        map.put("coverType", coverType);

        //封面list,用户转化为jsonArray
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
