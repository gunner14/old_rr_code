package com.renren.sns.wiki.dao.feed;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;
import net.paoding.rose.jade.annotation.ShardBy;

import com.renren.sns.wiki.model.hotfeed.HotFeedRecord;
import com.renren.sns.wiki.utils.WikiDBConstants;

/**
 * 热门feed数据DAO
 * 
 * @author yi.li@renren-inc.com since 2012-4-24
 * 
 */
@DAO(catalog = WikiDBConstants.DB_WIKI)
public interface HotFeedDAO {

    public static final String SELECT_BEAN = "wiki_id, feed_id, feed_type, position, update_time ";

    public static final String TABLE = "wiki_hot_feed";

    /**
     * 根据{@code wikiId}获取热门新鲜事数据
     * 
     * @param wikiId
     * @return List<HotFeedRecord>
     */
    @SQL("select " + SELECT_BEAN + " from " + TABLE + " where wiki_id = :wikiId")
    public List<HotFeedRecord> getWikiHotFeedRecord(@ShardBy @SQLParam("wikiId") int wikiId);

    /**
     * 批量的插入热门新鲜事数据
     * 
     * @param records 热门新鲜事记录
     * @param wikiId wikiId
     */
    @SQL("insert into "
            + TABLE
            + "("
            + SELECT_BEAN
            + ") values(:records.wikiId, :records.feedId, :records.feedType, :records.position, now())")
    public void insertWikiHotFeedRecord(@SQLParam("records") List<HotFeedRecord> records,
            @ShardBy int wikiId);

    @SQL("delete from " + TABLE + " where wiki_id = :wikiId")
    public void clearWikiHotFeedRecord(@ShardBy @SQLParam("wikiId") int wikiId);

    /**
     * 删除Wiki中的某条热门feed
     * 
     * @param wikiId wikiId
     * @param feedId feedId
     */
    @SQL("delete from " + TABLE + " where wiki_id = :wikiId and feed_id = :feedId")
    public void deleteWikiHotFeedRecord(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("feedId") long feedId);

    /**
     * 删除wiki中的多条热门feed
     * 
     * @param wikiId wiki ID
     * @param feedIds feedid列表
     */
    @SQL("delete from " + TABLE + " where wiki_id = :wikiId and feed_id in (:feedIds)")
    public void deleteWikiHotFeedRecords(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("feedIds") List<Long> feedIds);

    /**
     * 按权重对最热新鲜事进行排序
     * 
     * @param wikiId wiki id
     * @param start 起始记录
     * @param limit 获取的记录最大条数
     * @return List<HotFeedRecord>
     */
    @SQL("select " + SELECT_BEAN + " from " + TABLE
            + " where wiki_id = :wikiId order by position desc limit :start, :limit")
    public List<HotFeedRecord> getWikiHotFeedByWeight(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("start") int start, @SQLParam("limit") int limit);

    /**
     * 更新WikiHotFeed的权重信息
     * 
     * @param wikiId
     * @param record
     * @author yi.li@renren.com since 2012-7-5
     */
    @SQL("replace into " + TABLE + "(" + SELECT_BEAN + ")"
            + " values(:record.wikiId, :record.feedId, :record.feedType, :record.position, now())")
    public void updateWikiHotFeed(@ShardBy @SQLParam("wikiId") int wikiId,
            @SQLParam("record") HotFeedRecord record);
}
