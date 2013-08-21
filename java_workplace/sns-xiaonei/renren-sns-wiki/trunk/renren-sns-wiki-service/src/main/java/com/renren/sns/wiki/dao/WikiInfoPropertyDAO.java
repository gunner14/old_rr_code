package com.renren.sns.wiki.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.info.WikiInfoProperty;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 对应的table名字是wiki_info
 * 
 * @author weiwei.wang@renren-inc.com since 2012-03-30
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface WikiInfoPropertyDAO {

    public static final String TABLE_NAME = "wiki_info_property";

    public static final String DB_FIELDS = "name,value,field_name,priority,hide,wiki_id";

    public static final String BEAN_FIELDS = ":wikiInfoProperty.name,:wikiInfoProperty.value, :wikiInfoProperty.fieldName,:wikiInfoProperty.priority,:wikiInfoProperty.hide,:wikiInfoProperty.wikiId";

    public static final String INSERT_DB_FIELDS = DB_FIELDS;

    public static final String INSERT_BEAN_FIELDS = BEAN_FIELDS;

    public static final String SELECT_DB_FIELDS = "id," + DB_FIELDS;

    /**
     * 添加一条记录
     */
    @SQL("insert into " + TABLE_NAME + "(" + DB_FIELDS + ") values (" + BEAN_FIELDS + ")")
    public int addWikiInfoProperty(@SQLParam("wikiInfoProperty") WikiInfoProperty wikiInfoProperty);

    /**
     * 根据wikiId获取wiki的信息
     * 
     * @param wikiId
     * @return
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME
            + " where wiki_id=:wikiId order by priority,id")
    public List<WikiInfoProperty> getPropertyListByWikiId(@SQLParam("wikiId") int wikiId);

    @SQL("select id from " + TABLE_NAME + " where wiki_id=:wikiId and name=:name limit 1")
    public Integer getWikiInfoPropertyId(@SQLParam("wikiId") int wikiId,
            @SQLParam("name") String name);

    @SQL("update " + TABLE_NAME + " set name=:name, value=:value, priority=:priority where id=:id")
    public int updateWikiInfoProperty(@SQLParam("id") int id, @SQLParam("name") String name,
            @SQLParam("value") String value, @SQLParam("priority") int priority);

    @SQL("delete from " + TABLE_NAME + " where id=:wikiInfoPropertyId")
    public int deleteWikiInfoPropertyByWikiId(
            @SQLParam("wikiInfoPropertyId") int wikiInfoPropertyId);

}
