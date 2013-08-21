package com.renren.sns.wiki.delegator.xoa;

import org.springframework.stereotype.Service;

import com.renren.app.share.model.ShareComment;
import com.renren.app.share.xoa.api.ShareCommentService;
import com.renren.photo.xoa.api.service.PhotoService;
import com.renren.ugc.model.album.Photo;
import com.renren.xoa.lite.ServiceFactories;
import com.renren.xoa.lite.ServiceFactory;
import com.renren.xoa.lite.ServiceFuture;
import com.renren.xoa.lite.ServiceFutureHelper;

/**
 * 
 * 分享xoa的封装
 * 
 * @author weiwei.wang@renren-inc.com since 2012-7-2
 * 
 */
@Service
public class PhotoXoaDelegator {

    /**
     * 根据ownerId和shareId获取分享
     * 
     * @param ownerId
     * @param shareId
     * @return
     */
    public Photo getPhoto(int ownerId, long photoId) {
        Photo photo = null;
        ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        PhotoService photoService = fact.getService(PhotoService.class);//通过ServiceFactory来获取具体了Service实例
        try {
            ServiceFuture<Photo> serviceFuture = photoService.getPhoto(ownerId, ownerId, photoId,
                    "", true);
            photo = ServiceFutureHelper.execute(serviceFuture, 500);
        } catch (Exception e) {
            //图片不存在的时候会抛出异常,此处忽略
        }
        return photo;
    }

    /**
     * 获取删除的照片,如果照片删除就返回photo对象,否则返回null
     * 
     * @param ownerId
     * @param photoId
     * @return
     */
    public Photo getPhotoDeleted(int ownerId, long photoId) {
        Photo photo = null;
        ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        PhotoService photoService = fact.getService(PhotoService.class);//通过ServiceFactory来获取具体了Service实例
        try {
            ServiceFuture<Photo> serviceFuture = photoService.getPhotoDeleted(ownerId, ownerId,
                    photoId);
            photo = ServiceFutureHelper.execute(serviceFuture, 500);
        } catch (Exception e) {
            //图片不存在的时候会抛出异常,此处忽略
        }
        return photo;
    }

    public boolean isPhotoDeleted(int ownerId, long photoId) {
        Photo photo = getPhotoDeleted(ownerId, photoId);
        return photo != null;
    }

    /**
     * 获取分享的评论
     * 
     * @param ownerId
     * @param shareId
     * @return
     */
    public ShareComment[] getComments(int ownerId, long shareId, int offset, int limit,
            int visitorId) {
        ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        ShareCommentService shareCommentService = fact.getService(ShareCommentService.class);//通过ServiceFactory来获取具体了Service实例
        ServiceFuture<ShareComment[]> serviceFuture = shareCommentService.findComments(ownerId,
                shareId, offset, limit, visitorId);
        ShareComment[] shareComments = ServiceFutureHelper.execute(serviceFuture, 500);
        return shareComments;
    }

    /**
     * 获取评论的条数
     * 
     * @param ownerId
     * @param shareId
     * @param visitorId
     * @return
     */
    public Integer getCommentCount(int ownerId, long shareId, int visitorId) {
        ServiceFactory fact = ServiceFactories.getFactory();//获取ServiceFactory实例
        ShareCommentService shareCommentService = fact.getService(ShareCommentService.class);//通过ServiceFactory来获取具体了Service实例
        ServiceFuture<Integer> serviceFuture = shareCommentService.getCommentCount(ownerId,
                shareId, visitorId);
        Integer count = ServiceFutureHelper.execute(serviceFuture, 500);
        return count;
    }
}
