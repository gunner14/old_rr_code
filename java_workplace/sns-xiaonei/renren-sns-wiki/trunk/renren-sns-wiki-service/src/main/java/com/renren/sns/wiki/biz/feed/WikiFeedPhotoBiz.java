package com.renren.sns.wiki.biz.feed;

import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.dao.feed.WikiFeedPhotoDAO;
import com.renren.sns.wiki.model.feed.WikiFeedPhoto;
import com.renren.sns.wiki.utils.cache.CacheDelegator;
import com.renren.sns.wiki.utils.cache.CacheNameSpace;
import com.renren.sns.wiki.utils.cache.CachePrefix;
import com.renren.sns.wiki.utils.cache.CacheDelegator.QueryAction;

@Service
public class WikiFeedPhotoBiz {

    @Autowired
    private WikiFeedPhotoDAO wikiFeedPhotoDAO;

    /**
     * 添加photo到数据库
     * 
     * @param wikiFeedPhoto
     */
    public int addWikiFeedPhoto(WikiFeedPhoto wikiFeedPhoto) {
        return wikiFeedPhotoDAO.addWikiFeedPhoto(wikiFeedPhoto);
    }

    public void clearCache(long photoId) {
        CacheDelegator.delete(CacheNameSpace.WIKI, CachePrefix.WIKI_FEED_PHOTO,
                WikiFeedPhoto.class, photoId);
    }

    /**
     * 
     * @param photoIdList
     * @return
     */
    public Map<String, WikiFeedPhoto> getWikiFeedPhotoMap(List<String> photoIdList) {
        Map<String, WikiFeedPhoto> wikiFeedPhotoMap = CacheDelegator.getMulti(CacheNameSpace.WIKI,
                CachePrefix.WIKI, WikiFeedPhoto.class, photoIdList);
        return wikiFeedPhotoMap;
    }

    /**
     * 根据photoId获取wikiFeedPhoto
     * 
     * @param photoId
     * @return
     */
    public WikiFeedPhoto getWikiFeedPhoto(final Long photoId) {
        final WikiFeedPhoto wikiFeedPhoto = CacheDelegator.get(new QueryAction<WikiFeedPhoto>(
                CacheNameSpace.WIKI, CachePrefix.WIKI_FEED_PHOTO, WikiFeedPhoto.class, photoId) {

            @Override
            public WikiFeedPhoto getFromOther() {
                WikiFeedPhoto wikiFeedPhoto = wikiFeedPhotoDAO.getWikiFeedPhotoById(photoId);
                if (wikiFeedPhoto != null) {
                    String largeUrl = wikiFeedPhoto.getLargeUrl();
                    int width = wikiFeedPhoto.getLargeWidth();
                    int height = wikiFeedPhoto.getLargeHeight();
                    if (largeUrl != null && largeUrl.startsWith("http")) {
                        if (width == 0 || height == 0) {
                            WikiFeedPhotoUtils.completePhotoWidthHeight(wikiFeedPhoto);
                            //将宽高插入到数据库
                            wikiFeedPhotoDAO.updateImageWidthHeight(photoId, wikiFeedPhoto
                                    .getLargeWidth(), wikiFeedPhoto.getLargeHeight());
                        }
                    }
                }
                return wikiFeedPhoto;
            }
        }, null);
        return wikiFeedPhoto;
    }

    /**
     * 根据photoId获取photoId
     * 
     * @param photoId
     * @return
     */
    public Long getPhotoIdByPhotoId(long photoId) {
        return wikiFeedPhotoDAO.getPhotoIdByPhotoId(photoId);
    }

}
