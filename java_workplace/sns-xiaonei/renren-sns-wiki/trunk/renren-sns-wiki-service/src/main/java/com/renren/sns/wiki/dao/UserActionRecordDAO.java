package com.renren.sns.wiki.dao;

import java.util.Collection;
import java.util.List;
import java.util.Map;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.renren.sns.wiki.model.UserActionRecord;
import com.renren.sns.wiki.model.WikiCover;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * action_reword对应表的DAO
 * 
 * @author weiwei.wang@renren-inc.com since 2012-4-5
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface UserActionRecordDAO {

    public static final String TABLE_NAME = "user_action_record";

    public static final String DB_FIELDS = "wiki_id, user_id, user_name, user_headurl, interest, interest_time, rating, rating_time, comment, comment_time ";

    public static final String BEAN_FIELDS = ":userActionRecord.wikiId, :userActionRecord.userId, :userActionRecord.userName, :userActionRecord.userHeadurl, :userActionRecord.interest, :userActionRecord.interestTime, :userActionRecord.rating, :userActionRecord.ratingTime, :userActionRecord.comment, :userActionRecord.commentTime ";

    public static final String INSERT_DB_FIELDS = DB_FIELDS;

    public static final String INSERT_BEAN_FIELDS = BEAN_FIELDS;

    public static final String SELECT_DB_FIELDS = "id, " + DB_FIELDS + ", action_time";

    /**
     * 添加一条记录
     */
    @SQL("insert into " + TABLE_NAME + "(" + INSERT_DB_FIELDS + ") values (" + INSERT_BEAN_FIELDS
            + ")")
    public void addWiki(@SQLParam("wikiCover") WikiCover wikiCover);

    /**
     * 根据wikiId和userId获取actionReword信息
     */
    @SQL("select " + SELECT_DB_FIELDS + " from " + TABLE_NAME
            + " where wiki_id = :wikiId and user_id=:userId")
    public UserActionRecord getUserActionRecord(@SQLParam("wikiId") int wikiId,
            @SQLParam("userId") int userId);

    /**
     * userId为key, 对象为value的mao
     * 
     * @param wikiId
     * @param userIds
     * @return
     */
    @SQL("select user_id," + SELECT_DB_FIELDS + " from " + TABLE_NAME
            + " where wiki_id=:wikiId and user_id in(:userIds)")
    public Map<Integer, UserActionRecord> getUserActionRecord(@SQLParam("wikiId") int wikiId,
            @SQLParam("userIds") Collection<Integer> userIds);

    @SQL("select wiki_id," + SELECT_DB_FIELDS + " from " + TABLE_NAME
            + " where  user_id = :userIds")
    public Map<Integer, UserActionRecord> getUserActionRecord(@SQLParam("userId") int userId);

    @SQL("insert into "
            + TABLE_NAME
            + "(wiki_id, wiki_type, user_id, user_name, user_headurl, interest, interest_time) values(:wikiId, :wikiType, :userId, :userName, :userHeadurl, :interest, now())")
    public void addUserActionRecord(@SQLParam("wikiId") int wikiId,
            @SQLParam("wikiType") int wikiType, @SQLParam("userId") int userId,
            @SQLParam("userName") String userName, @SQLParam("userHeadurl") String userHeadurl,
            @SQLParam("interest") String interest);

    @SQL("update "
            + TABLE_NAME
            + " set interest=:interest, interest_time=now() where wiki_id=:wikiId and user_id=:userId")
    public void updateInterest(@SQLParam("wikiId") int wikiId, @SQLParam("userId") int userId,
            @SQLParam("interest") String interest);

    @SQL("update " + TABLE_NAME
            + " set rating=:rating, rating_time=now() where wiki_id=:wikiId and user_id=:userId")
    public int updateRating(@SQLParam("wikiId") int wikiId, @SQLParam("userId") int userId,
            @SQLParam("rating") int rating);

    @SQL("update " + TABLE_NAME
            + " set comment=:comment, comment_time=now() where wiki_id=:wikiId and user_id=:userId")
    public int updateComment(@SQLParam("wikiId") int wikiId, @SQLParam("userId") int userId,
            @SQLParam("comment") String comment);

    /**
     * 获取某个wiki的评分的总和
     * 
     * @param wikiId
     * @return
     */
    @SQL("select sum(rating) from " + TABLE_NAME + " where wiki_id = :wikiId")
    public Integer getTotalRating(@SQLParam("wikiId") int wikiId);

    /**
     * 获取评分的总人数
     * 
     * @param wikiId
     * @return
     */
    @SQL("select count(rating) from " + TABLE_NAME + " where wiki_id = :wikiId and rating > 0")
    public Integer getRatingCount(@SQLParam("wikiId") int wikiId);

    /**
     * 删除某条记录
     * 
     * @param wikiId
     * @param userId
     */
    @SQL("delete from " + TABLE_NAME + " where wiki_id=:wikiId and user_id=:userId")
    public int deleteUserActionRecord(@SQLParam("wikiId") int wikiId, @SQLParam("userId") int userId);

    /**
     * 获取用户某个类型,某个兴趣的wikiId列表
     * 
     * @param userId
     * @param type
     * @param interest
     * @return
     */
    @SQL("select u.wiki_id from user_action_record u left join wiki w on u.wiki_id=w.id where u.user_id=:userId and w.type=:type and interest=:interest")
    public List<Integer> getWikiIdList(@SQLParam("userId") int userId, @SQLParam("type") int type,
            @SQLParam("interest") String interest);

    @SQL("select count(1) from user_action_record where user_id=:userId and wiki_type=:wikiType and interest=:interest")
    public int getCount(@SQLParam("userId") int userId, @SQLParam("wikiType") int wikiType,
            @SQLParam("interest") String interest);

    @SQL("select wiki_id from user_action_record where user_id=:userId and wiki_type=:wikiType and interest=:interest order by action_time desc limit :offset,:limit")
    public List<Integer> getWikiIdList(@SQLParam("userId") int userId,
            @SQLParam("wikiType") int wikiType, @SQLParam("interest") String interest,
            @SQLParam("offset") int offset, @SQLParam("limit") int limit);

}
