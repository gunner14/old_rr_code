package com.renren.sns.wiki.service.feed;

import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.sns.wiki.biz.feed.WikiFeedPhotoBiz;
import com.renren.sns.wiki.delegator.xoa.PhotoXoaDelegator;
import com.renren.sns.wiki.model.feed.WikiFeedPhoto;
import com.renren.ugc.model.album.Photo;

/**
 * 处理没有尺寸的照片
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-17
 * 
 */
@Service
public class WikiFeedPhotoService {

    @Autowired
    private WikiFeedPhotoBiz wikiFeedPhotoBiz;

    @Autowired
    private PhotoXoaDelegator photoXoaDelegator;

    //    private Log logger = LogFactory.getLog(this.getClass());

    public Map<String, WikiFeedPhoto> getWikiFeedPhotoMap(List<String> photoIdList) {
        Map<String, WikiFeedPhoto> wikiFeedPhotoMap = wikiFeedPhotoBiz
                .getWikiFeedPhotoMap(photoIdList);
        for (String photoId : photoIdList) {
            WikiFeedPhoto value = wikiFeedPhotoMap.get(photoId);
            if (value == null) {
                value = wikiFeedPhotoBiz.getWikiFeedPhoto(Long.parseLong(photoId));
                wikiFeedPhotoMap.put(photoId, value);
            }
        }
        return wikiFeedPhotoMap;
    }

    public WikiFeedPhoto getWikiFeedPhoto(final Long photoId) {
        WikiFeedPhoto wikiFeedPhoto = wikiFeedPhotoBiz.getWikiFeedPhoto(photoId);
        if (wikiFeedPhoto == null || !wikiFeedPhoto.getLargeUrl().startsWith("http")) {
            return null;
        }
        return wikiFeedPhoto;
    }

    /**
     * 根据ownerId和photoId返回大图的url
     * 
     * @param ownerId
     * @param photoId
     * @return
     */
    public String getLargeUrl(int ownerId, long photoId) {
        Photo photo = photoXoaDelegator.getPhoto(ownerId, photoId);
        String largeUrl = null;
        if (photo != null) {
            largeUrl = photo.getLargeUrl();
        }
        return largeUrl;
    }

    /**
     * 是否已经存在数据库里
     * 
     * @param ownerId
     * @param photoId
     * @return
     */
    public boolean contains(long photoId) {
        Long longPhotoId = wikiFeedPhotoBiz.getPhotoIdByPhotoId(photoId);
        if (longPhotoId == null) {
            return false;
        } else {
            return true;
        }
    }

}
