package com.renren.sns.wiki.dao.rank;

import com.renren.sns.wiki.model.rank.WikiRankRecord;
import com.renren.sns.wiki.utils.WikiDBConstants;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;
import net.paoding.rose.jade.annotation.ShardBy;

/**
 * 用户积分排名的DAO
 * <p>
 * 用来存储某一个积分在wiki上的排名
 * 
 * @author yi.li@renren-inc.com since 2012-5-15
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface WikiRankDAO {

    public static final String TABLE_NAME = "wiki_user_rank";

    public static final String FIELDS = "wiki_id, score, capacity, rank, last_update_time ";

    public static final String BEAN_FIELDS = ":wikiId, :score, :capacity, :rank ";

    /**
     * 根据积分查询该积分<tt>score</tt>在所在wiki上的排名
     * 
     * @param wikiId wiki ID
     * @param score 积分
     * @return 该积分在对饮wiki上的排名
     */
    @SQL("select id, " + FIELDS + " from " + TABLE_NAME
            + " where wiki_id = :wikiId and score = :score")
    public WikiRankRecord getRankByScore(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("score") int score);

    /**
     * 获取排在积分<tt>score</tt>前一位的积分纪录
     * 
     * @param wikiId wikiID
     * @param score 积分
     * @return 返回排在前一位的积分纪录{@link WikiRankRecord}，如果没有纪录排在前面，则返回null
     */
    @SQL("select id, " + FIELDS + " from " + TABLE_NAME
            + " where wiki_id = :wikiId and score > :score order by score asc limit 1")
    public WikiRankRecord getPreRank(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("score") int score);

    /**
     * 更新指定积分在某个wiki上的排名和容量
     * 
     * @param wikiId wiki ID
     * @param score 需要被更新的积分
     * @param rankOffset 排名变化的偏移。0表示不变化，-n表示上升n位，n表示下降n位（其中n为正数）
     * @param capacityOffset 容量变化的偏移。0表示不变化，-n表示上升n位，n表示下降n位（其中n为正数）
     * @return 被更新的记录条数
     */
    @SQL("update "
            + TABLE_NAME
            + " set rank = rank + :rankOffset, capacity = capacity + :capacityOffset, last_update_time = now() "
            + "where wiki_id = :wikiId and score = :score")
    public int updateRankAndCapacity(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("score") int score, @SQLParam("rankOffset") int rankOffset,
            @SQLParam("capacityOffset") int capacityOffset);

    /**
     * 更新某个wiki下从积分<tt>startScore</tt>到<tt>endScore</tt>之间的排名数据。
     * <p>
     * 该更新在原来排名的基础上加上<tt>rankOffset</tt>为最终排名
     * 
     * @param wikiId wiki ID
     * @param startScore 起始积分值
     * @param endScore 结束积分值
     * @param rankOffset 排名变化的偏移。0表示不变化，-n表示上升n位，n表示下降n位（其中n为正数）
     * @return
     */
    @SQL("update " + TABLE_NAME + " set rank = rank + :rankOffset, last_update_time = now() "
            + "where wiki_id = :wikiId and score > :startScore and score < :endScore")
    public int updateRankByScoreRange(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("startScore") int startScore, @SQLParam("endScore") int endScore,
            @SQLParam("rankOffset") int rankOffset);

    /**
     * 插入新的排名记录
     * 
     * @param wikiId wikiID
     * @param score 积分
     * @param rank 排名
     * @param capacity 容量
     */
    @SQL("insert into " + TABLE_NAME + "(" + FIELDS + ")values(" + BEAN_FIELDS + ", now())")
    public void insertRank(@ShardBy @SQLParam("wikiId") int wikiId, @SQLParam("score") int score,
            @SQLParam("rank") int rank, @SQLParam("capacity") int capacity);

    /**
     * 删除某个积分的排名
     * 
     * @param wikiId wiki ID
     * @param score 积分
     */
    @SQL("delete from " + TABLE_NAME + " where wiki_id = :wikiId and score = :score")
    public void removeRank(@ShardBy @SQLParam("wikiId") int wikiId, @SQLParam("score") int score);

}
