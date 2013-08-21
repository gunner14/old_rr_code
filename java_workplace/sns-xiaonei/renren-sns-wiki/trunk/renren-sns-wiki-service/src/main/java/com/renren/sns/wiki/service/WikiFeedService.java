package com.renren.sns.wiki.service;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import xce.ilikeit.LikeInfo;

import com.renren.sns.wiki.biz.FeedLikeBiz;
import com.renren.sns.wiki.biz.FriendBiz;
import com.renren.sns.wiki.biz.WikiFeedBiz;
import com.renren.sns.wiki.biz.feed.Feed2MapUtils;
import com.renren.sns.wiki.biz.feed.RewardFeedBiz;
import com.renren.sns.wiki.biz.feed.WikiFeedPhotoBiz;
import com.renren.sns.wiki.biz.feed.WikiFeedPhotoUtils;
import com.renren.sns.wiki.constant.WikiConstant;
import com.renren.sns.wiki.delegator.xce.WUserCacheDelegator;
import com.renren.sns.wiki.delegator.xoa.PhotoXoaDelegator;
import com.renren.sns.wiki.enums.ItemType;
import com.renren.sns.wiki.enums.WikiFeedType;
import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.model.feed.WikiFeedPhoto;
import com.renren.sns.wiki.service.feed.WikiFeedPhotoService;
import com.renren.sns.wiki.utils.HtmlUtils;
import com.renren.sns.wiki.utils.concurrent.AsynTaskExecutorUtil;
import com.xiaonei.platform.component.xfeed.display.model.Feed;
import com.xiaonei.platform.component.xfeed.display.model.FeedSet;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.WUserCache;

/**
 * wiki新鲜事相关
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-20
 * 
 */
@Service
public class WikiFeedService {

    private static final Log logger = LogFactory.getLog(WikiFeedService.class);

    @Autowired
    private WikiFeedBiz wikiFeedBiz;

    @Autowired
    private FeedLikeBiz feedLikeBiz;

    @Autowired
    private FriendBiz friendBiz;

    @Autowired
    private WikiFeedPhotoService wikiFeedPhotoService;

    @Autowired
    private WikiFeedPhotoBiz wikiFeedPhotoBiz;

    @Autowired
    private WUserCacheDelegator wUserCacheDelegator;

    @Autowired
    private RewardFeedBiz rewardFeedBiz;

    @Autowired
    private PhotoXoaDelegator photoXoaDelegator;

    /**
     * 根据feedIds获取feed对象数组
     * 
     * @param feedIds
     * @return
     */
    public Feed[] getFeeds(String currentName, int currentID, long[] feedIds) throws WikiException {
        if (feedIds == null || feedIds.length == 0) {
            return null;
        }
        if (logger.isDebugEnabled()) {
            StringBuilder sb = new StringBuilder("wikiFeedService.getFeeds feedIds:");
            for (long feedId : feedIds) {
                sb.append(feedId).append(", ");
            }
            logger.debug(sb.toString());
        }
        FeedSet feedSet = null;
        try {
            feedSet = wikiFeedBiz.getFeedSetByIds(currentName, currentID, feedIds);
        } catch (Exception e) {
            logger.warn("get feed by feedId failture", e);
            throw new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, "根据feedId获取新鲜事失败");
        }
        if (feedSet == null || feedSet.getData().length < 1) {
            return null;
        }
        return feedSet.getData();
    }

    /**
     * 根据feedIds获取feed对象数组
     * 
     * @param user, user可以为null,自动补充默认值
     * @param feedIds
     * @return
     */
    public Feed[] getFeeds(int wikiId, long[] feedIds) {
        String currentName = "wiki";
        int currentID = wikiId;
        Feed[] feeds = getFeeds(currentName, currentID, feedIds);
        if (feedIds != null && feedIds.length > 0) {//如果有不存在的feed,将不存在的feed记录下来
            if (feeds == null) {
                for (long feedId : feedIds) {
                    rewardFeedBiz.rewardFeed(wikiId, feedId, "feed is null");
                }
            } else if (feeds.length != feedIds.length) {
                Set<Long> feedIdSet = new HashSet<Long>();
                for (long feedId : feedIds) {
                    feedIdSet.add(feedId);
                }
                for (Feed feed : feeds) {
                    long feedId = Feed2MapUtils.getFeedId(feed);
                    if (feedId != 0) {
                        feedIdSet.remove(feedId);
                    }
                }
                for (long feedId : feedIdSet) {
                    rewardFeedBiz.rewardFeed(wikiId, feedId, "feed is null");
                }
            }
        }
        return feeds;
    }

    /**
     * 将新鲜事数组转化为List
     * 
     * @param feeds
     * @param wikiId 用于记录出错feedId所在wiki
     * @return
     */
    private List<Map<String, Object>> feeds2List(Feed[] feeds, int wikiId) {
        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
        if (feeds == null || feeds.length < 1) {
            return list;
        }
        for (int i = 0; i < feeds.length; i++) {
            Feed feed = feeds[i];
            //用于记录有问题的wiki
            long feedId = 0;
            try {
                feedId = Feed2MapUtils.getFeedId(feed);
            } catch (Exception e) {
                logger.error("获取feedId失败,feedMap是:" + feed.getData(), e);
            }
            Map<String, Object> map = null;
            try {
                map = wikiFeedBiz.covert2Map(feed);
            } catch (Exception e) {
                //记录出错信息
                rewardFeedBiz.rewardFeed(wikiId, feedId, "wikiFeedBiz.covert2Map(feed) exception");
                logger.error("covert2JsonObject error, feed:" + feed.getData().toString(), e);
                continue;
            }

            if (map == null) {
                logger.warn("feedType=" + feed.getFeedType() + "map(feed convert) is null");
                rewardFeedBiz.rewardFeed(wikiId, feedId, "map(feed convert) is null");
                continue;
            }
            //每个内容都有发布者,是必须条件
            String strOwnerId = (String) map.get("ownerId");
            String strSourceId = (String) map.get("sourceId");
            String strUpdateDate = (String) map.get("updateDate");
            if (strOwnerId.length() == 0) {
                rewardFeedBiz.rewardFeed(wikiId, feedId, "don't contain ownerId");
            } else if (strSourceId.length() == 0) {
                rewardFeedBiz.rewardFeed(wikiId, feedId, "don't contain sourceId");
            } else if (strUpdateDate.length() == 0) {
                rewardFeedBiz.rewardFeed(wikiId, feedId, "don't contain updateDate");
            } else {
                list.add(map);
            }
        }

        return list;
    }

    /**
     * 过滤title,sumary,desc的html标签
     * 
     * @param jsonArray
     */
    @SuppressWarnings("unchecked")
    private void filterHtml(List<Map<String, Object>> list) {
        for (Map<String, Object> map : list) {
            int feedType = (Integer) map.get("feedType");

            String title = (String) map.get("title");
            if (title != null && feedType != 502) {
                title = HtmlUtils.html2text(title);
                map.put("title", title);
            }
            String summary = (String) map.get("summary");
            if (summary != null) {
                summary = HtmlUtils.html2text(summary);
                map.put("summary", summary);
            }

            Object o = map.get("coverImages");

            if (o instanceof List<?>) {
                List<Map<String, Object>> coverImageList = null;
                coverImageList = (List<Map<String, Object>>) o;
                for (Map<String, Object> coverImageMap : coverImageList) {
                    String desc = (String) coverImageMap.get("desc");
                    if (desc != null) {
                        desc = HtmlUtils.html2text(desc);
                        coverImageMap.put("desc", desc);
                    }
                }
            }

        }
    }

    /**
     * 完善图片尺寸
     * 
     * @param list
     * @param wikiId 用于记录有问题的feed出现所在的wiki
     * 
     */
    @SuppressWarnings("unchecked")
    private void completeCoverImagesInfo(List<Map<String, Object>> list, int wikiId) {
        //找出所有的没有图片尺寸的photoId,ownerId
        Map<String, String> photoOwnerMap = new HashMap<String, String>();
        for (Map<String, Object> map : list) {
            int feedType = (Integer) map.get("feedType");
            if (feedType == 701 || feedType == 709) {
                Object o = map.get("coverImages");

                if (o != null && o instanceof List<?>) {
                    List<Map<String, Object>> coverImageList = (List<Map<String, Object>>) o;
                    for (Map<String, Object> coverImageMap : coverImageList) {
                        String strWidth = (String) coverImageMap.get("width");
                        String strHeight = (String) coverImageMap.get("height");
                        if (StringUtils.isBlank(strWidth) || StringUtils.isBlank(strHeight)
                                || "0".equals(strWidth) || "0".equals(strHeight)) {} else {
                            continue;
                        }
                        String photoUrl = (String) coverImageMap.get("photoUrl");
                        String strOwnerId = null;
                        String strPhotoId = null;
                        if (photoUrl.contains("?")) {
                            strOwnerId = WikiFeedPhotoUtils.getUrlValue(photoUrl, "owner");
                            strPhotoId = WikiFeedPhotoUtils.getUrlValue(photoUrl, "id");
                        } else {
                            Pattern p = Pattern
                                    .compile("http://photo.renren.com/photo/(\\d+)/photo-(\\d+)");
                            Matcher m = p.matcher(photoUrl);
                            if (m.find()) {
                                strOwnerId = m.group(1);
                                strPhotoId = m.group(2);
                            }
                        }
                        if (strOwnerId != null && strPhotoId != null) {
                            photoOwnerMap.put(strPhotoId, strOwnerId);
                        }
                    }
                }
            }
        }

        //获取photoId对应的photo
        Map<String, WikiFeedPhoto> wikiFeedPhotoMap = wikiFeedPhotoService
                .getWikiFeedPhotoMap(new ArrayList<String>(photoOwnerMap.keySet()));
        for (String strPhotoId : photoOwnerMap.keySet()) {
            WikiFeedPhoto wikiFeedPhoto = wikiFeedPhotoMap.get(strPhotoId);
            if (wikiFeedPhoto == null) {
                long photoId = Long.parseLong(strPhotoId);
                int ownerId = Integer.parseInt(photoOwnerMap.get(strPhotoId));
                wikiFeedPhoto = new WikiFeedPhoto();
                wikiFeedPhoto.setPhotoId(photoId);
                wikiFeedPhoto.setOwnerId(ownerId);
                String largeUrl = wikiFeedPhotoService.getLargeUrl(ownerId, photoId);
                if (largeUrl == null) {
                    largeUrl = "0";
                }
                wikiFeedPhoto.setLargeUrl(largeUrl);
                if (largeUrl.startsWith("http")) {
                    //异步处理图片尺寸
                    handleWikiFeedPhoto(wikiFeedPhoto);
                }
                wikiFeedPhotoMap.put(strPhotoId, wikiFeedPhoto);
            }
        }

        //将图片信息填充进来
        Iterator<Map<String, Object>> it = list.iterator();
        while (it.hasNext()) {
            Map<String, Object> map = it.next();
            int feedType = (Integer) map.get("feedType");
            if (feedType == 701 || feedType == 709) {
                String strFeedId = (String) map.get("feedId");
                Object o = map.get("coverImages");
                if (o != null && o instanceof List) {
                    List<Map<String, Object>> coverImageList = (List<Map<String, Object>>) o;
                    Iterator<Map<String, Object>> coverImageIt = coverImageList.iterator();
                    while (coverImageIt.hasNext()) {
                        Map<String, Object> coverImageMap = coverImageIt.next();
                        if (!coverImageMap.containsKey("photoUrl")) {
                            if (logger.isDebugEnabled()) {
                                logger.debug("不包含photoUrl,移除此照片,strFeedId=" + strFeedId);
                            }
                            coverImageIt.remove();
                            continue;
                        }
                        String strWidth = (String) coverImageMap.get("width");
                        String strHeight = (String) coverImageMap.get("height");
                        if (StringUtils.isBlank(strWidth) || StringUtils.isBlank(strHeight)
                                || "0".equals(strWidth) || "0".equals(strHeight)) {
                            String photoUrl = (String) coverImageMap.get("photoUrl");
                            String strPhotoId = null;
                            if (photoUrl.contains("?")) {
                                strPhotoId = WikiFeedPhotoUtils.getUrlValue(photoUrl, "id");
                            } else {
                                Pattern p = Pattern
                                        .compile("http://photo.renren.com/photo/(\\d+)/photo-(\\d+)");
                                Matcher m = p.matcher(photoUrl);
                                if (m.find()) {
                                    strPhotoId = m.group(2);
                                }
                            }
                            if (strPhotoId == null) {
                                coverImageIt.remove();
                            } else if (!wikiFeedPhotoMap.containsKey(strPhotoId)) {
                                coverImageIt.remove();
                            } else {
                                WikiFeedPhoto wikiFeedPhoto = wikiFeedPhotoMap.get(strPhotoId);
                                //largeUrl为"0"时说明xoa获取不到本条新鲜事,可以异步删除

                                if (wikiFeedPhoto == null) {
                                    if (logger.isDebugEnabled()) {
                                        logger.debug("wikiFeedPhoto is null, and strPhotoId="
                                                + strPhotoId);
                                    }
                                    coverImageIt.remove();
                                } else if (wikiFeedPhoto.getLargeWidth() == 0
                                        || wikiFeedPhoto.getLargeHeight() == 0) {
                                    coverImageIt.remove();
                                } else {
                                    String largeUrl = wikiFeedPhoto.getLargeUrl();
                                    if (largeUrl != null && largeUrl.startsWith("http")) {
                                        coverImageMap.put("url", largeUrl);
                                        coverImageMap.put("width", wikiFeedPhoto.getLargeWidth());
                                        coverImageMap.put("height", wikiFeedPhoto.getLargeHeight());
                                    } else {
                                        //获取照片是否已经删除
                                        try {
                                            boolean isDeleted = photoXoaDelegator.isPhotoDeleted(
                                                    wikiFeedPhoto.getOwnerId(), wikiFeedPhoto
                                                            .getPhotoId());

                                            if (isDeleted) {
                                                rewardFeedBiz.rewardFeed(wikiId, strFeedId,
                                                        "photo deleted");
                                            }
                                        } catch (Exception e) {
                                            logger.warn("获取照片是否删除异常", e);
                                        }
                                        coverImageIt.remove();
                                    }
                                }
                            }
                        }
                    }//while循环结束,处理完所有图片
                    if (coverImageList.size() == 0) {
                        rewardFeedBiz.rewardFeed(wikiId, strFeedId, "all photo is invalid");
                        it.remove();
                    }
                } else {
                    rewardFeedBiz.rewardFeed(wikiId, strFeedId, "don't have coverImages property");
                }

            }
        }//完善图片信息end
    }

    private void handleWikiFeedPhoto(final WikiFeedPhoto wikiFeedPhoto) {
        AsynTaskExecutorUtil.getAsynExecutor().addTask("completePhotoWidthHeight",
                wikiFeedPhoto.getPhotoId() + "", new Runnable() {

                    @Override
                    public void run() {
                        //处理图片时为避免默认值影响后面处理,先做神拷贝
                        WikiFeedPhoto copyWikiFeedPhoto = new WikiFeedPhoto();
                        copyWikiFeedPhoto.setPhotoId(wikiFeedPhoto.getPhotoId());
                        copyWikiFeedPhoto.setOwnerId(wikiFeedPhoto.getOwnerId());
                        copyWikiFeedPhoto.setLargeUrl(wikiFeedPhoto.getLargeUrl());

                        //处理图片尺寸
                        WikiFeedPhotoUtils.completePhotoWidthHeight(copyWikiFeedPhoto);
                        //保存到数据库中
                        wikiFeedPhotoBiz.addWikiFeedPhoto(copyWikiFeedPhoto);
                    }
                }, 5000);
    }

    /**
     * 完善wiki新鲜事的用户信息
     * 
     * @param jsonArray
     */
    private void completeOwnerInfo(List<Map<String, Object>> list) {
        Set<Integer> userIdSet = new HashSet<Integer>();
        for (Map<String, Object> map : list) {
            String strOwnerId = (String) map.get("ownerId");
            if (strOwnerId != null && strOwnerId.matches("\\d+")) {
                int userId = Integer.parseInt(strOwnerId);
                userIdSet.add(userId);
            }
        }
        Map<Integer, WUserCache> userCacheMap = wUserCacheDelegator
                .getUserCacheMap(new ArrayList<Integer>(userIdSet));
        for (Map<String, Object> map : list) {
            String strOwnerId = (String) map.get("ownerId");
            if (strOwnerId != null && strOwnerId.matches("\\d+")) {
                int ownerId = Integer.parseInt(strOwnerId);
                WUserCache userCache = userCacheMap.get(ownerId);
                map.put("ownerName", userCache.getName());
                map.put("ownerHeadUrl", userCache.getTiny_Url());
            }
        }
    }

    /**
     * 完善wiki新鲜事的喜欢信息
     * 
     * @param jsonArray
     */
    private void completeLikeInfo(List<Map<String, Object>> list, User user) {
        if (list == null || list.size() == 0 || user == null) {
            return;
        }
        List<String> gidStrList = new ArrayList<String>();
        for (Map<String, Object> map : list) {
            //globalId
            String strGlobalId = (String) map.get("globalId");
            if (!StringUtils.isBlank(strGlobalId)) {
                gidStrList.add(strGlobalId);
            }
        }

        if (logger.isDebugEnabled()) {
            logger.debug("gidStrList:" + gidStrList);
        }

        int count = gidStrList.size();
        Map<String, LikeInfo> likeInfoMap = feedLikeBiz.getLikeDetailMapBatch(gidStrList
                .toArray(new String[count]), user.getId());
        for (Map<String, Object> map : list) {
            String globalId = (String) map.get("globalId");
            if (globalId != null) {
                LikeInfo likeInfo = likeInfoMap.get(globalId);
                int selfCount = 0;
                int totalCount = 0;
                if (likeInfo != null) {
                    selfCount = likeInfo.selfCount;
                    totalCount = likeInfo.totalCount;
                }

                if (selfCount > 0) {
                    map.put("isLiked", true);
                } else {
                    map.put("isLiked", false);
                }
                map.put("likedNum", totalCount + "");
            }

            //删除掉globalId属性,喜欢接口需要这个字段,所以保留这个字段
            //jsonObject.remove("globalId");
        }
    }

    /**
     * 获取最新的新鲜事Id的接口
     * 
     * @param wikiId
     * @param host
     * @param itemType 不能为空
     * @param page
     * @param pageSize
     * @param extra 额外返回的条数,用户判断时候还有下一页
     * @return
     */
    public long[] getLatestFeedIds(int wikiId, ItemType itemType, int begin, int limit) {
        if (itemType == null) {
            itemType = ItemType.ALL;
        }
        int[] types = itemType.getFeedTypes();

        long[] feedIds = wikiFeedBiz.getFeedIdByTypesFromSocialWikiLatestByWikiId(wikiId, begin,
                limit, types);
        return feedIds;
    }

    /**
     * 获取最热的新鲜事
     * 
     * @param wikiId
     * @param itemType
     * @param begin
     * @param limit
     * @return
     */
    public long[] getHotFeedIds(int wikiId, ItemType itemType, int begin, int limit) {
        if (itemType == null) {
            itemType = ItemType.ALL;
        }
        int[] types = itemType.getFeedTypes();
        long[] feedIds = wikiFeedBiz.getFeedIdByTypesFromSocialWikiHotByWikiId(wikiId, begin,
                limit, types);
        return feedIds;
    }

    /**
     * 获取自己好友的新鲜事
     * 
     * @param wikiId
     * @param user
     * @param itemType
     * @param page
     * @param pageSize
     * @param extra
     * @return
     */
    public long[] getFriendFeedIds(int wikiId, int userId, ItemType itemType, int begin, int limit) {
        //获取共同好友
        List<Integer> userIdList = friendBiz.getFeedSetByIds(userId, 1000);
        if (logger.isDebugEnabled()) {
            logger.debug(userId + "的好友列表" + userIdList);
        }
        if (userIdList == null) {
            return null;
        }
        int[] userIds = new int[userIdList.size()];
        for (int i = 0; i < userIdList.size(); i++) {
            userIds[i] = userIdList.get(i).intValue();
        }
        if (itemType == null) {
            itemType = ItemType.ALL;
        }
        int[] types = itemType.getFeedTypes();

        long[] feedIds = wikiFeedBiz.getFeedidByTypesFromSocialWikiByUserid(userIds, wikiId, begin,
                limit, types);
        return feedIds;
    }

    /**
     * 根据条件,获取对应的feedId数组
     * 
     * @param wikiId
     * @param user
     * @param itemType
     * @param begin
     * @param limit
     * @param wikiFeedType
     * @return
     */
    public long[] getFeedIdList(int wikiId, User user, ItemType itemType, int begin, int limit,
            WikiFeedType wikiFeedType) {
        long[] feedIds = null;

        switch (wikiFeedType) {
            case LATEST:
                feedIds = this.getLatestFeedIds(wikiId, itemType, begin, limit);
                break;
            case HOT:
                feedIds = this.getHotFeedIds(wikiId, itemType, begin, limit);
                break;
            case FRIEND:
                feedIds = this.getFriendFeedIds(wikiId, user.getId(), itemType, begin, limit);
                break;
            default:
                feedIds = this.getLatestFeedIds(wikiId, itemType, begin, limit);
                break;
        }

        if (logger.isDebugEnabled()) {
            StringBuilder sb = new StringBuilder();
            sb.append("get feed condition wikiId=").append(wikiId).append(", begin=").append(begin)
                    .append(", limit=").append(limit)
                    .append(" and return result tmpFeedIds[] is:[");
            for (long feedId : feedIds) {
                sb.append(feedId).append(",");
            }
            sb.append("]");

            logger.debug(sb.toString());
        }
        return feedIds;
    }

    public List<Map<String, Object>> getRawFeed(int wikiId, Feed[] feeds) {
        List<Map<String, Object>> list = null;
        list = this.feeds2List(feeds, wikiId);
        if (list == null) {
            list = new ArrayList<Map<String, Object>>();
        }
        return list;
    }

    /**
     * 获取新鲜事的裸数据，不包括统计数据
     * 
     * @param wikiId wiki id
     * @param user 用户对象
     * @param itemType UGC分类：状态、日志、相册、视频
     * @param begin 开始位置
     * @param limit 最大长度
     * @param wikiFeedType wiki feed类型：最热，最新
     * @return Feed数据
     */
    public List<Map<String, Object>> getRawFeed(int wikiId, User user, ItemType itemType,
            int begin, int limit, WikiFeedType wikiFeedType) {
        long[] feedIds = getFeedIdList(wikiId, user, itemType, begin, limit, wikiFeedType);
        if (null == feedIds || feedIds.length == 0) {
            return Collections.emptyList();
        }

        Feed[] feeds = getFeeds(wikiId, feedIds);
        return getRawFeed(wikiId, feeds);
    }

    /**
     * 根据条件生成瀑布流新鲜事的格式
     * 
     * @param wikiId
     * @param host
     * @param itemType
     * @param page
     * @param pageSize
     * @param wikiFeedType
     * @return
     */
    public Map<String, Object> getFeedMap(int wikiId, User user, ItemType itemType, int page,
            int pageSize, WikiFeedType wikiFeedType) {
        if (page <= 0) {
            page = 0;
        }
        if (pageSize <= 0 || pageSize > 200) {
            pageSize = WikiConstant.DEFAULT_FEED_PAGE_SIZE;
        }
        int begin = page * pageSize;
        int limit = pageSize + 5;

        long[] tmpFeedIds = getFeedIdList(wikiId, user, itemType, begin, limit, wikiFeedType);

        boolean hasMore = false;
        int feedsCount = 0;
        if (tmpFeedIds != null) {
            feedsCount = tmpFeedIds.length;
        }
        if (feedsCount > pageSize) {
            hasMore = true;
            feedsCount = pageSize;
        }

        long[] feedIds = new long[feedsCount];
        for (int i = 0; i < feedsCount; i++) {
            feedIds[i] = tmpFeedIds[i];
        }
        Feed[] feeds = this.getFeeds(wikiId, feedIds);

        Map<String, Object> map = getFeedMap(wikiId, user, feeds);
        map.put("hasMore", hasMore);

        //最热的请求会加isLatest
        if (wikiFeedType == WikiFeedType.HOT) {
            boolean isLatest = false;
            if (page == 0 && pageSize >= 6 && map.size() < 6) {
                isLatest = true;
            }
            map.put("isLatest", isLatest);
        }

        return map;

    }

    /**
     * 
     * @param wikiId, 用来统计有问题的feed所属id,无实际作用,可以传0
     * @param user
     * @param feeds
     * @return
     */
    public Map<String, Object> getFeedMap(int wikiId, User user, Feed[] feeds) {
        List<Map<String, Object>> list = getRawFeed(wikiId, feeds);

        // fill extra data 
        // 过滤title,sumary,desc的html标签
        filterHtml(list);

        // 完善图片尺寸
        completeCoverImagesInfo(list, wikiId);

        // 完善owner的名字和头像
        completeOwnerInfo(list);

        // 完善喜欢信息
        completeLikeInfo(list, user);
        // end fill extra data

        Map<String, Object> map = new HashMap<String, Object>();

        map.put("feeds", list);

        return map;
    }
}
