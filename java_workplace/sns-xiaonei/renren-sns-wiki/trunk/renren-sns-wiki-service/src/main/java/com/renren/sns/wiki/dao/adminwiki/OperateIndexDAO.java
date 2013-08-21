package com.renren.sns.wiki.dao.adminwiki;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.admin.OperateIndex;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 
 * @author weiwei.wang@renren-inc.com since 2012-8-20
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface OperateIndexDAO {

    public static final String TABLE_NAME = "operate_index";

    public static final String DB_FIELDS = "wiki_id, wiki_type, operate_type, `order`";

    public static final String BEAN_FIELDS = ":operateIndex.wikiId, :operateIndex.wikiType, :operateIndex.operateType, :operateIndex.order";

    public static final String INSERT_DB_FIELDS = DB_FIELDS;

    public static final String INSERT_BEAN_FIELDS = BEAN_FIELDS;

    public static final String SELECT_DB_FIELDS = "id, insert_time, " + DB_FIELDS;

    /**
     * 添加运营首页的一条记录
     */
    @SQL("insert into " + TABLE_NAME + "(" + INSERT_DB_FIELDS + ") values (" + INSERT_BEAN_FIELDS
            + ")")
    public int addOperateIndex(@SQLParam("operateIndex") OperateIndex operateIndex);

    /**
     * 更具wiki_id和operateType获取wikiId,用于测试记录是否已经存在
     * 
     * @param wikiId
     * @param operateType
     * @return
     */
    @SQL("select wiki_id from " + TABLE_NAME
            + " where wiki_id=:wikiId and operate_type=:operateType")
    public Integer getWikiId(@SQLParam("wikiId") int wikiId,
            @SQLParam("operateType") String operateType);

    /**
     * 根据wikiType和operateType获取一个wikiId的运营列表
     * 
     * @param wikiType
     * @param operateType
     * @return
     */
    @SQL("select "
            + SELECT_DB_FIELDS
            + " from "
            + TABLE_NAME
            + " where wiki_type=:wikiType and operate_type=:operateType order by `order` asc, wiki_id desc")
    public List<OperateIndex> getWikiIdList(@SQLParam("wikiType") int wikiType,
            @SQLParam("operateType") String operateType);

    /**
     * 从运营数据中删除某个wiki
     * 
     * @param wikiId
     * @param operateType
     * @return
     */
    @SQL("delete from " + TABLE_NAME + " where wiki_id=:wikiId and operate_type=:operateType")
    public int delete(@SQLParam("wikiId")int wikiId, @SQLParam("operateType")String operateType);

}
