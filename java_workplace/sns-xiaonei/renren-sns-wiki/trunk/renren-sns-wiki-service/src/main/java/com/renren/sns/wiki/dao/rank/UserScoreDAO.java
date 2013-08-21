package com.renren.sns.wiki.dao.rank;

import com.renren.sns.wiki.model.rank.UserScore;
import com.renren.sns.wiki.utils.WikiDBConstants;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

/**
 * 用户积分数据DAO
 * <p>
 * 主要用来存储每个用户在其相关的wiki上的积分
 * 
 * @author yi.li@renren-inc.com since 2012-5-15
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface UserScoreDAO {

    public static final String TABLE = "wiki_user_score";

    public static final String FIELDS = "wiki_id, user_id, score, last_update_time ";

    public static final String BEAN_FIELDS = ":wikiId, :userId, :score";

    /**
     * 获取用户在某个wiki下的积分
     * 
     * @param userId user ID
     * @param wikiId wiki ID
     * @return 返回用户的积分，如果用户没有积分返回null
     */
    @SQL("select id, " + FIELDS + " from " + TABLE
            + " where user_id = :userId and wiki_id = :wikiId")
    public UserScore getUserScore(@SQLParam("userId") int userId, @SQLParam("wikiId") int wikiId);

    /**
     * 增量的更新用户积分
     * 
     * @param userId user ID
     * @param wikiId wiki ID
     * @param incrScore 用户增加的积分
     * @return 返回更新的记录条数
     */
    @SQL("update " + TABLE + " set score = score + :incrScore, last_update_time = now() "
            + "where user_id = :userId and wiki_id = :wikiId")
    public int updateUserScoreIncrementally(@SQLParam("userId") int userId,
            @SQLParam("wikiId") int wikiId, @SQLParam("incrScore") int incrScore);

    /**
     * 插入一条用户的积分记录
     * 
     * @param userId user ID
     * @param wikiId wiki ID
     * @param score 积分
     */
    @SQL("insert into " + TABLE + "(" + FIELDS + ")values(" + BEAN_FIELDS + ", now())")
    public void insertUserScore(@SQLParam("userId") int userId, @SQLParam("wikiId") int wikiId,
            @SQLParam("score") int score);
}
