package com.renren.sns.wiki.dao.recommend;

import java.util.List;

import com.renren.sns.wiki.utils.WikiDBConstants;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

/**
 * Wiki推荐数据DAO
 * 
 * @author yi.li@renren-inc.com since 2012-5-14
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface WikiRecommendationDAO {

    public final static String TABLE = "wiki_recommendation";

    public final static String INSERT_COLUMNS = " wiki_id, recommended_wiki_id, add_time ";

    @SQL("select recommended_wiki_id from " + TABLE + " where wiki_id = :wikiId")
    public List<Integer> getRecommendation(@SQLParam("wikiId") int wikiId);

    @SQL("insert into " + TABLE + "(" + INSERT_COLUMNS + ")"
            + " values( :wikiId, :recommendedWikiId, now())")
    public void addRecommendation(@SQLParam("wikiId") int wikiId,
            @SQLParam("recommendedWikiId") int recommendedWikiId);

    @SQL("delete from " + TABLE + " where wiki_id = :wikiId")
    public void clearRecommendation(@SQLParam("wikiId") int wikiId);

    @SQL("delete from " + TABLE + " where id = :recommendationId")
    public void removeRecommendationById(@SQLParam("recommendationId") long recommendationId);
}
