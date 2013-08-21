package com.renren.sns.wiki.dao.adminwiki;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.admin.OperateIndexSlide;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 首页slide
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-21
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface OperateIndexSlideDAO {

    public static final String TABLE_NAME = "operate_index_slide";

    public static final String DB_FIELDS = "wiki_id, wiki_type, image_url, image_width, image_height, `order`";

    public static final String BEAN_FIELDS = ":operateIndexSlide.wikiId, :operateIndexSlide.wikiType, :operateIndexSlide.imageUrl, :operateIndexSlide.imageWidth, :operateIndexSlide.imageHeight, :operateIndexSlide.order";

    public static final String INSERT_DB_FIELDS = DB_FIELDS;

    public static final String INSERT_BEAN_FIELDS = BEAN_FIELDS;

    public static final String SELECT_DB_FIELDS = "id, insert_time, " + DB_FIELDS;

    /**
     * 添加运营首页slide的一条记录
     */
    @SQL("insert into " + TABLE_NAME + "(" + INSERT_DB_FIELDS + ") values (" + INSERT_BEAN_FIELDS
            + ")")
    public int addOperateIndexSlide(
            @SQLParam("operateIndexSlide") OperateIndexSlide operateIndexSlide);

    /**
     * 根据wiki_id获取wikiId,用于测试记录是否已经存在
     * 
     * @param wikiId
     * @param operateType
     * @return
     */
    @SQL("select wiki_id from " + TABLE_NAME + " where wiki_id=:wikiId")
    public Integer getWikiId(@SQLParam("wikiId") int wikiId);

    /**
     * 根据wikiType获取一个wikiId的运营列表
     * 
     * @param wikiType
     * @param operateType
     * @return
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME
            + " where wiki_type=:wikiType order by `order` asc, wiki_id desc")
    public List<OperateIndexSlide> getWikiIdList(@SQLParam("wikiType") int wikiType);

    /**
     * 从运营数据中删除某个wiki
     * 
     * @param wikiId
     * @param operateType
     * @return
     */
    @SQL("delete from " + TABLE_NAME + " where wiki_id=:wikiId")
    public int delete(@SQLParam("wikiId") int wikiId);

    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " where wiki_id=:wikiId")
    public OperateIndexSlide getOperateIndexSlideByWikiId(@SQLParam("wikiId") int wikiId);

    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " where id=:id")
    public OperateIndexSlide getOperateIndexSlideById(@SQLParam("id") int id);

    @SQL("update "
            + TABLE_NAME
            + " set wiki_id=:operateIndexSlide.wikiId, image_url=:operateIndexSlide.imageUrl, image_width=:operateIndexSlide.imageWidth, image_height=:operateIndexSlide.imageHeight, `order`=:operateIndexSlide.order where id=:operateIndexSlide.id")
    public int updateOperateIndexSlide(
            @SQLParam("operateIndexSlide") OperateIndexSlide operateIndexSlide);

    @SQL("delete from " + TABLE_NAME + " where wiki_id=:wikiId")
    public int deleteOperateIndexSlideByWikiId(@SQLParam("wikiId") int wikiId);

}
