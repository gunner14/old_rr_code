package com.renren.entries.core.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.RowHandler;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.ShardBy;

import com.renren.entries.model.Hit;

/**
 * {@link keyDAO} 规范了对索引表的操作
 * 
 * @author yanghe.liang@renren-inc.com
 * @author qieqie.wang@gmail.com
 * 
 */
//TODO: entry_index的id要默认降序排列?
@DAO(catalog = "entries")
public interface IndexDAO {

    String TABLE_NAME = "entry_index_##(:1)";

    /**
     * 增加一个索引条目
     * 
     * @param field 索引类别
     * @param id 一个全局唯一的ID
     * @param indexValue 索引值
     * @param entryId 被索引的
     */
    @SQL("INSERT INTO " + TABLE_NAME + " (id, index_value, entry_id) VALUES (:2, :3, :4)")
    public void add(String field, long id, @ShardBy Object indexValue, long entryId);

    /**
     * 删除给定的索引条目
     * 
     * @param field 索引类别
     * @param indexValue 索引值
     * @param entryId 被索引的
     */
    @SQL("DELETE FROM " + TABLE_NAME + " WHERE index_value = :2 AND entry_id = :3")
    public void delete(String field, @ShardBy Object indexValue, long entryId);

    /**
     * 查找索引到的、在指定区间上的索引条目
     * 
     * @param field 索引类别
     * @param indexValue 索引key
     * @param upperCursor 限制返回的索引条目的ID,最大不能超过或等于这个ID
     * @param limit 限制返回的索引条目个数，最大不能超过这个数目
     * @return
     */
    @SQL("SELECT id, entry_id FROM "
            + TABLE_NAME
            + " WHERE index_value = :2 #if(:3){ AND entry_id BETWEEN :3.start AND :3.end} AND id < :4  ORDER BY id DESC LIMIT :5")
    @RowHandler(rowMapper = HitRowMapper.class)
    public List<Hit> nextPage(String field, @ShardBy Object indexValue, Between entryRange,
            long upperCursor, int limit);

    /**
     * 
     * @param field
     * @param indexValue
     * @param lowerCursor
     * @param limit
     * @return
     */
    @SQL("SELECT id, entry_id FROM "
            + TABLE_NAME
            + " WHERE index_value = :2 #if(:3){ AND entry_id BETWEEN :3.start AND :3.end} AND id > :4 ORDER BY id DESC LIMIT :5")
    @RowHandler(rowMapper = HitRowMapper.class)
    public List<Hit> prePage(String field, @ShardBy Object indexValue, Between entryRange,
            long lowerCursor, int limit);

    /**
     * 查找索引到的、在指定区间上的索引条目
     * 
     * @param field 索引类别
     * @param indexValue 索引key
     * @param upperCursor 限制返回的索引条目的ID,最大不能超过或等于这个ID
     * @param entries
     * @param limit 限制返回的索引条目个数，最大不能超过这个数目
     * @return
     */
    @SQL("SELECT id, entry_id FROM " + TABLE_NAME
            + " WHERE index_value = :2  AND entry_id IN (:4) AND id < :3 ORDER BY id DESC LIMIT :5")
    @RowHandler(rowMapper = HitRowMapper.class)
    public List<Hit> nextPage(String field, @ShardBy Object indexValue, long upperCursor,
            long[] entries, int limit);

    /**
     * 
     * @param field
     * @param indexValue
     * @param lowerCursor
     * @param entries
     * @param limit
     * @return
     */
    @SQL("SELECT id, entry_id FROM " + TABLE_NAME
            + " WHERE index_value = :2  AND entry_id in (:4) AND id > :3 ORDER BY id DESC LIMIT :5")
    @RowHandler(rowMapper = HitRowMapper.class)
    public List<Hit> prePage(String field, @ShardBy Object indexValue, long lowerCursor,
            long[] entries, int limit);

    /**
     * 查找索引到的、在指定区间上的索引条目
     * 
     * @param field 索引类别
     * @param indexValue 索引key
     * @param upperCursor 限制返回的索引条目的ID,最大不能超过或等于这个ID
     * @param lowerCursor 限制返回的索引条目的ID,最小不能小于这个ID
     * @param limit 限制返回的索引条目个数，最大不能超过这个数目
     * @return
     */
    @SQL("SELECT id, entry_id FROM "
            + TABLE_NAME
            + " WHERE index_value = :2 #if(:3){ AND entry_id BETWEEN :3.start AND :3.end} AND (id BETWEEN :5 AND :4) ORDER BY id DESC LIMIT :6")
    @RowHandler(rowMapper = HitRowMapper.class)
    public List<Hit> rangePage(String field, @ShardBy Object indexValue, Between entryRange,
            long upperCursor, long lowerCursor, int limit);

    /**
     * 查找索引到的、在指定区间上的索引条目
     * 
     * @param field 索引类别
     * @param indexValue 索引key
     * @param upperCursor 限制返回的索引条目的ID,最大不能超过或等于这个ID
     * @param lowerCursor 限制返回的索引条目的ID,最小不能小于这个ID
     * @param entries
     * @param limit 限制返回的索引条目个数，最大不能超过这个数目
     * @return
     */
    @SQL("SELECT id, entry_id FROM "
            + TABLE_NAME
            + " WHERE index_value = :2 AND entry_id in (:5) AND (id BETWEEN :4 AND :3) ORDER BY id DESC LIMIT :6")
    @RowHandler(rowMapper = HitRowMapper.class)
    public List<Hit> rangePage(String field, @ShardBy Object indexValue, long upperCursor,
            long lowerCursor, long[] entries, int limit);

}
