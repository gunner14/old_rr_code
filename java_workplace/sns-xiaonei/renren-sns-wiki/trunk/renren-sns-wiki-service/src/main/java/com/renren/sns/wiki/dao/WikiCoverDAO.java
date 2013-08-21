package com.renren.sns.wiki.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.WikiCover;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 和wikiCover表相关的数据
 * 
 * @author weiwei.wang@renren-inc.com since 2012-3-31
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface WikiCoverDAO {

    public static final String TABLE_NAME = "wiki_cover";

    public static final String DB_FIELDS = "photo_id, xlarge_url, large_url, main_url, head_url, "
            + "tiny_url, x_scale, y_scale, height_scale, width_scale, xlarge_url_width, "
            + "xlarge_url_height, extra_option ";

    public static final String BEAN_FIELDS = ":wikiCover.photoId, :wikiCover.xlargeUrl, :wikiCover.largeUrl, "
            + ":wikiCover.mainUrl, :wikiCover.headUrl, :wikiCover.tinyUrl, :wikiCover.xScale, "
            + ":wikiCover.yScale, :wikiCover.heightScale, :wikiCover.widthScale, :wikiCover.xlargeUrlWidth, "
            + ":wikiCover.xlargeUrlHeight, :wikiCover.extraOption ";

    public static final String INSERT_DB_FIELDS = "wiki_id, " + DB_FIELDS;

    public static final String INSERT_BEAN_FIELDS = ":wikiCover.wikiId, " + BEAN_FIELDS;

    public static final String SELECT_DB_FIELDS = "wiki_id, " + DB_FIELDS;

    /**
     * 添加一条wiki记录
     */
    @SQL("insert into " + TABLE_NAME + "(" + INSERT_DB_FIELDS + ") values (" + INSERT_BEAN_FIELDS
            + ")")
    public int addWikiCover(@SQLParam("wikiCover") WikiCover wikiCover);

    /**
     * 根据wikiId 获取WikiCover信息
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " where wiki_id = :wikiId")
    public WikiCover getWikiCoverById(@SQLParam("wikiId") int wikiId);

    /**
     * 根据wikiId获取wikiId,用户获取某个wiki是否有自定义头图
     */
    @SQL("select wiki_id from wiki_cover where wiki_id=:wikiId limit 1")
    public Integer getWikiIdByWikiId(@SQLParam("wikiId") int wikiId);

    /**
     * 修改cover图
     * 
     * @param wikiId
     * @param xlargeUrl
     * @param largeUrl
     * @param mainUrl
     * @param headUrl
     * @param tinyUrl
     * @param xlargeUrlWidth
     * @param xlargeUrlHeight
     * @return
     */
    @SQL("update "
            + TABLE_NAME
            + " set xlarge_url=:xlargeUrl, large_url=:largeUrl, main_url=:mainUrl, head_url=:headUrl, "
            + "tiny_url=:tinyUrl, xlarge_url_width=:xlargeUrlWidth, "
            + "xlarge_url_height=:xlargeUrlHeight where wiki_id=:wikiId")
    public int updateWikiCover(@SQLParam("wikiId") int wikiId,
            @SQLParam("xlargeUrl") String xlargeUrl, @SQLParam("largeUrl") String largeUrl,
            @SQLParam("mainUrl") String mainUrl, @SQLParam("headUrl") String headUrl,
            @SQLParam("tinyUrl") String tinyUrl, @SQLParam("xlargeUrlWidth") int xlargeUrlWidth,
            @SQLParam("xlargeUrlHeight") int xlargeUrlHeight);

    /**
     * 修改cover图
     * 
     * @param wikiCover
     * @return
     */
    @SQL("update "
            + TABLE_NAME
            + " set xlarge_url=:wikiCover.xlargeUrl, large_url=:wikiCover.largeUrl, main_url=:wikiCover.mainUrl, head_url=:wikiCover.headUrl, "
            + "tiny_url=:wikiCover.tinyUrl, xlarge_url_width=:wikiCover.xlargeUrlWidth, "
            + "xlarge_url_height=:wikiCover.xlargeUrlHeight where wiki_id=:wikiCover.wikiId")
    public int updateWikiCover(@SQLParam("wikiCover") WikiCover wikiCover);

    @SQL("delete from " + TABLE_NAME + " where wiki_id = :wikiId")
    public int deleteWikiCoverByWikiId(@SQLParam("wikiId") int wikiId);

}
