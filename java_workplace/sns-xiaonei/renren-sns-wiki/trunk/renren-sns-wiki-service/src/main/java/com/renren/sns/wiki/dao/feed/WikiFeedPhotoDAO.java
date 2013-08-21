package com.renren.sns.wiki.dao.feed;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.feed.WikiFeedPhoto;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 和wikiCover表相关的数据
 * 
 * @author weiwei.wang@renren-inc.com since 2012-3-31
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface WikiFeedPhotoDAO {

    public static final String TABLE_NAME = "wiki_feed_photo";

    public static final String DB_FIELDS = "photo_id, owner_id, large_url, large_width, large_height ";

    public static final String BEAN_FIELDS = ":wikiFeedPhoto.photoId, :wikiFeedPhoto.ownerId, :wikiFeedPhoto.largeUrl, :wikiFeedPhoto.largeWidth, :wikiFeedPhoto.largeHeight ";

    public static final String INSERT_DB_FIELDS = DB_FIELDS;

    public static final String INSERT_BEAN_FIELDS = BEAN_FIELDS;

    public static final String SELECT_DB_FIELDS = DB_FIELDS;

    /**
     * 根据wikiId 获取WikiCover信息
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME
            + " where large_url is null order by photo_id desc limit :limit")
    public List<WikiFeedPhoto> getWikiFeedPhotoListNoUrl(@SQLParam("limit") int limit);

    /**
     * 根据wikiId 获取WikiCover信息
     */
    @SQL("update " + TABLE_NAME + " set large_url=:largeUrl where photo_id=:photoId")
    public int updateLargeUrl(@SQLParam("photoId") long photoId,
            @SQLParam("largeUrl") String largeUrl);

    /**
     * 
     * @param photoId
     * @return
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " where photo_id=:photoId")
    public WikiFeedPhoto getWikiFeedPhotoById(@SQLParam("photoId") Long photoId);

    /**
     * 设置photo的宽高
     * 
     * @param photoId
     * @param largeWidth
     * @param largeHeight
     * @return
     */
    @SQL("update " + TABLE_NAME
            + " set large_width=:largeWidth, large_height=:largeHeight where photo_id=:photoId")
    public int updateImageWidthHeight(@SQLParam("photoId") long photoId,
            @SQLParam("largeWidth") int largeWidth, @SQLParam("largeHeight") int largeHeight);

    /**
     * 添加一张图片
     * 
     * @param wikiFeedPhoto
     * @return
     */
    @SQL("insert into " + TABLE_NAME + "(" + INSERT_DB_FIELDS + ") values(" + INSERT_BEAN_FIELDS
            + ")")
    public int addWikiFeedPhoto(@SQLParam("wikiFeedPhoto") WikiFeedPhoto wikiFeedPhoto);

    /**
     * 根据photoId获取photoId
     * 
     * @param photoId
     * @return
     */
    @SQL("select photo_id from " + TABLE_NAME + " where photo_id=:photoId")
    public Long getPhotoIdByPhotoId(@SQLParam("photoId") long photoId);

}
