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
 * 多张照片上传新鲜事
 * 
 * @author weiwei.wang@renren-inc.com since 2012-5-4
 * 
 */
public class Feed2Map709Impl implements IFeed2Map {

    private static final int FEED_TYPE = 709;

    private static final String ITEM_TYPE = ItemType.PHOTO.getValue();

    private static List<JsonFeedKeyPair> keyList = new ArrayList<JsonFeedKeyPair>();

    static {

        keyList.add(new JsonFeedKeyPair("feedId", "id"));
        keyList.add(new JsonFeedKeyPair("sourceUrl", "f_album_url"));
        keyList.add(new JsonFeedKeyPair("sourceId", "f_album_id"));
        keyList.add(new JsonFeedKeyPair("title", "f_album_title"));
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
        //如果没有big尺寸的url,就使用f_album_photo_image,兼容老的新鲜事
        if (coverImageUrlList == null) {
            coverImageUrlList = feedMap.get("f_album_photo_image");
        }

        List<String> coverImageDescList = feedMap.get("f_album_photo_descfull");
        List<String> coverImageWidthList = feedMap.get("f_album_photo_bigWidth");
        List<String> coverImageHeightList = feedMap.get("f_album_photo_bigHeight");
        //f_album_photo_url
        List<String> albumPhotoUrlList = feedMap.get("f_album_photo_url");

        //存放封面的List
        List<Map<String, Object>> coverImagesList = new ArrayList<Map<String, Object>>();

        if (albumPhotoUrlList != null && albumPhotoUrlList.size() > 0) {
            for (int i = 0; i < albumPhotoUrlList.size(); i++) {
                Map<String, Object> coverImageMap = new HashMap<String, Object>();
                if (coverImageUrlList != null && coverImageUrlList.size() > i) {
                    String coverImageUrl = coverImageUrlList.get(i);
                    coverImageMap.put("url", coverImageUrl);
                }
                if (coverImageDescList != null && coverImageDescList.size() > i) {
                    String coverImageDesc = coverImageDescList.get(i);
                    coverImageMap.put("desc", coverImageDesc);
                }
                if (coverImageWidthList != null && coverImageWidthList.size() > i) {
                    String coverImageWidth = coverImageWidthList.get(i);
                    coverImageMap.put("width", coverImageWidth);
                }
                if (coverImageHeightList != null && coverImageHeightList.size() > i) {
                    String coverImageHeight = coverImageHeightList.get(i);
                    coverImageMap.put("height", coverImageHeight);
                }
                if (albumPhotoUrlList != null && albumPhotoUrlList.size() > i) {
                    String albumPhotoUrl = albumPhotoUrlList.get(i);
                    coverImageMap.put("photoUrl", albumPhotoUrl);
                }
                coverImagesList.add(coverImageMap);
            }
        }
        map.put("coverImages", coverImagesList);

        return map;
    }
}
