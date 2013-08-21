package com.renren.sns.wiki.dao.dict;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.dict.WikiWord;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * Wiki切词词条字典DB访问
 * 
 * @author yi.li@renren-inc.com since 2012-6-26
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface WikiDictionaryDAO {

    public static final String TABLE_NAME = "wiki_dictionary";

    public static final String SELECT_BEAN = " id, wiki_id, name, state, create_time ";

    /**
     * <p>
     * 获取切词字典中的记录。
     * <p>
     * 从位置start开始最大获取capacity条切词记录
     * 
     * @param start 获取记录的起始位置
     * @param capacity 获取的记录条数
     * @return List<WikiWord>
     */
    @SQL("select " + SELECT_BEAN + " from " + TABLE_NAME + " limit :start, :capacity")
    public List<WikiWord> getWikiWords(@SQLParam("start") int start,
            @SQLParam("capacity") int capacity);

    /**
     * 根据查询条件查询数据
     * 
     * @param condition
     * @return
     */
    //    @SQL("select " + SELECT_BEAN + " from " + TABLE + " where id>5 limit 20")
    @SQL("select " + SELECT_BEAN + " from " + TABLE_NAME + " ##(:condition)")
    public List<WikiWord> getWikiWordListByCondition(@SQLParam("condition") String condition);

    /**
     * @param name
     * @return
     */
    @SQL("select " + SELECT_BEAN + " from " + TABLE_NAME + " where name = :name")
    public List<WikiWord> getWikiWordListByName(@SQLParam("name") String name);

    @SQL("select " + SELECT_BEAN + " from " + TABLE_NAME + " where wiki_id = :wikiId")
    public List<WikiWord> getWikiWordListByWikiId(@SQLParam("wikiId") int wikiId);

    @SQL("insert into " + TABLE_NAME + "(wiki_id, name, create_time) values(:wikiId, :name, now())")
    public int addWikiWord(@SQLParam("wikiId") int wikiId, @SQLParam("name") String name);

    @SQL("delete from " + TABLE_NAME + " where id=:wikiWordId")
    public int deleteWikiWord(@SQLParam("wikiWordId") int wikiWordId);

}
