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
 * 单张照片上传新鲜事
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-4
 * 
 */
public class Feed2Map701Impl implements IFeed2Map {

    private static final int FEED_TYPE = 701;

    private static final String ITEM_TYPE = ItemType.PHOTO.getValue();

    private static List<JsonFeedKeyPair> keyList = new ArrayList<JsonFeedKeyPair>();

    static {

        keyList.add(new JsonFeedKeyPair("sourceUrl", "f_album_photo_url"));
        keyList.add(new JsonFeedKeyPair("feedId", "id"));
        keyList.add(new JsonFeedKeyPair("title", "f_album_title"));
        keyList.add(new JsonFeedKeyPair("sourceId", "f_album_photo_id"));
        keyList.add(new JsonFeedKeyPair("replyNum", "reply_count"));
        keyList.add(new JsonFeedKeyPair("albumCount", "f_album_count"));
        keyList.add(new JsonFeedKeyPair("ownerName", "f_from_name"));
        keyList.add(new JsonFeedKeyPair("ownerId", "f_from_id"));
        keyList.add(new JsonFeedKeyPair("updateDate", "f_time"));

        //需要,但是没有的数据
        keyList.add(new JsonFeedKeyPair("likedNum", ""));
        keyList.add(new JsonFeedKeyPair("isLiked", ""));

        keyList.add(new JsonFeedKeyPair("coverImages", "")); //f_blog_photo_orgImage
        keyList.add(new JsonFeedKeyPair("ownerHeadUrl", ""));
        keyList.add(new JsonFeedKeyPair("summary", ""));
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

        //封面图片处理  封面图片为f_album_photo_mainImage 描述为: f_album_photo_digest
        List<String> coverImageUrlList = feedMap.get("f_album_photo_bigImage");
        if(coverImageUrlList==null){
            coverImageUrlList = feedMap.get("f_album_photo_image");
        }
        List<String> coverImageDescList = feedMap.get("f_album_photo_digest");
        List<String> coverImageWidthList = feedMap.get("f_album_photo_bigWidth");
        List<String> coverImageHeightList = feedMap.get("f_album_photo_bigHeight");

        //封面列表
        List<Map<String, Object>> coverImagesList = new ArrayList<Map<String, Object>>();

        Map<String, Object> coverImageMap = new HashMap<String, Object>();

        coverImageMap.put("photoUrl", "http://photo.renren.com/photo/" + map.get("ownerId")
                + "/photo-" + map.get("sourceId"));

        if (coverImageUrlList != null && coverImageUrlList.size() > 0) {
            String coverImageUrl = coverImageUrlList.get(0);
            coverImageMap.put("url", coverImageUrl);
        }
        if (coverImageDescList != null && coverImageDescList.size() > 0) {
            String coverImageDesc = coverImageDescList.get(0);
            coverImageMap.put("desc", coverImageDesc);
        }
        if (coverImageWidthList != null && coverImageWidthList.size() > 0) {
            String coverImageWidth = coverImageWidthList.get(0);
            coverImageMap.put("width", coverImageWidth);
        }
        if (coverImageHeightList != null && coverImageHeightList.size() > 0) {
            String coverImageHeight = coverImageHeightList.get(0);
            coverImageMap.put("height", coverImageHeight);
        }
        coverImagesList.add(coverImageMap);

        map.put("coverImages", coverImagesList);

        return map;
    }

}
