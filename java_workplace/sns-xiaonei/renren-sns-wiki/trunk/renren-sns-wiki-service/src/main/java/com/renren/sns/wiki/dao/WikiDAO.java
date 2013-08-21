package com.renren.sns.wiki.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.Wiki;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 和Wiki主表有关的数据
 * 
 * @author yi.li@renren-inc.com since 2012-3-26
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface WikiDAO {

    public static final String TABLE_NAME = "wiki";

    public static final String DB_FIELDS = "name, alias, photo_id, tiny_url, head_url, "
            + "main_url, large_url, like_count, want_count, enjoying_count, enjoyed_count, "
            + "valuation_count,valuation_score, type, sub_type, state, creator, create_time ";

    public static final String INSERT_DB_FIELDS = "id, " + DB_FIELDS;

    public static final String BEAN_FIELDS = ":wiki.id, :wiki.name, :wiki.alias, :wiki.photoId, "
            + ":wiki.tinyUrl, :wiki.headUrl, :wiki.mainUrl, :wiki.largeUrl, :wiki.likeCount, "
            + ":wiki.wantCount, :wiki.enjoyingCount, :wiki.enjoyedCount, :wiki.valuationCount, "
            + ":wiki.valuationScore, :wiki.type, :wiki.subType, :wiki.state, :wiki.creator ";

    public static final String SELECT_DB_FIELDS = "id, " + DB_FIELDS;

    /**
     * 添加一条wiki记录
     */
    @SQL("insert into " + TABLE_NAME + "(" + INSERT_DB_FIELDS + ") values (" + BEAN_FIELDS
            + ", now())")
    public void addWiki(@SQLParam("wiki") Wiki wiki);

    /**
     * 更新wiki的头像
     */
    @SQL("update " + TABLE_NAME
            + " set tiny_url = :wiki.tinyUrl, head_url = :wiki.headUrl, main_url = :wiki.mainUrl, "
            + "large_url = :wiki.largeUrl, photo_id = :wiki.photoId where id = :wiki.id")
    public void updateAvator(@SQLParam("wiki") Wiki wiki);

    /**
     * 更新wiki的名字
     */
    @SQL("update " + TABLE_NAME + " set name = :wikiName where id = :wikiId")
    public void updateWikiName(@SQLParam("wikiId") int wikiId, @SQLParam("wikiName") String name);

    /**
     * 更新wiki的状态
     */
    @SQL("update " + TABLE_NAME + " set state = :state where id = :wikiId")
    public int updateWikiState(@SQLParam("wikiId") int wikiId, @SQLParam("state") int state);

    /**
     * 根据wikiId 获取Wiki信息
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " where id = :wikiId")
    public Wiki getWikiById(@SQLParam("wikiId") int wikiId);

    /**
     * 修改wiki头像
     * 
     * @param wikiId
     * @param tinyUrl
     * @param headUrl
     * @param mainUrl
     * @param largeUrl
     */
    @SQL("update "
            + TABLE_NAME
            + " set tiny_url = :tinyUrl, head_url = :headUrl, main_url = :mainUrl, large_url=:largeUrl where id = :wikiId")
    public int updateWikiHead(@SQLParam("wikiId") int wikiId, @SQLParam("tinyUrl") String tinyUrl,
            @SQLParam("headUrl") String headUrl, @SQLParam("mainUrl") String mainUrl,
            @SQLParam("largeUrl") String largeUrl);

    /**
     * 根据wikiName得到wikiId的列表
     * 
     * @param wikiName
     * @return
     */
    @SQL("select id from " + TABLE_NAME + " where name=:wikiName")
    public List<Integer> getWikiIdListByName(@SQLParam("wikiName") String wikiName);

    /**
     * 根据wikiName和type获取wikiId列表
     * 
     * @param wikiName
     * @param type
     * @return
     */
    @SQL("select id from " + TABLE_NAME + " where name=:wikiName and type=:wikiType")
    public List<Integer> getWikiIdList(@SQLParam("wikiName") String wikiName,
            @SQLParam("wikiType") int wikiType);

    /**
     * 修改wiki的评分总分和评分人数
     * 
     * @param wikiId
     * @param valuationScore
     * @param valuationCount
     * @return
     */
    @SQL("update "
            + TABLE_NAME
            + " set valuation_score=:valuationScore, valuation_count=:valuationCount where id=:wikiId")
    public int updateValuation(@SQLParam("wikiId") int wikiId,
            @SQLParam("valuationScore") int valuationScore,
            @SQLParam("valuationCount") int valuationCount);

    /**
     * 根据评分的差值 评分人数的差值修改评分
     * 
     * @param wikiId
     * @param strChaneRating
     * @param strChangeUserCount
     * @return
     */
    @SQL("update " + TABLE_NAME + " set valuation_score=valuation_score##(:strChaneRating), "
            + "valuation_count=valuation_count##(:strChangeUserCount) where id=:wikiId")
    public int updateValuationByChange(@SQLParam("wikiId") int wikiId,
            @SQLParam("strChaneRating") String strChaneRating,
            @SQLParam("strChangeUserCount") String strChangeUserCount);

    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " where name = :wikiName")
    public List<Wiki> getWikiListByName(@SQLParam("wikiName") String wikiName);

    @SQL("select distinct w.id, w.name, w.alias, w.photo_id, w.tiny_url, w.head_url, w.main_url, w.large_url, w.like_count, w.want_count, w.enjoying_count, w.enjoyed_count, w.valuation_count,w.valuation_score, w.type, w.sub_type, w.state, w.creator, w.create_time  from wiki w left join wiki_dictionary d on w.id=d.wiki_id "
            + " ##(:condition)")
    public List<Wiki> getWikiListByCondition(@SQLParam("condition") String condition);

    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME + " limit :begin, :limit")
    public List<Wiki> getWikiList(@SQLParam("begin") int begin, @SQLParam("limit") int limit);

    @SQL("select id from wiki where type=:wikiType order by id desc limit :limit")
    public List<Integer> getWikiIdLatestList(@SQLParam("wikiType")int wikiType, @SQLParam("limit")int limit);
    
    @SQL("select id from wiki where type=:wikiType order by id asc limit :limit")
    public List<Integer> getWikiIdHotList(@SQLParam("wikiType")int wikiType, @SQLParam("limit")int limit);

}
