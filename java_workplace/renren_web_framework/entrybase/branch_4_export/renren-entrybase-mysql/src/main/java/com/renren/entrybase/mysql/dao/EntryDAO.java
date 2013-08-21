package com.renren.entrybase.mysql.dao;

import java.util.Date;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.RowHandler;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;
import net.paoding.rose.jade.annotation.ShardBy;

import com.renren.entrybase.model.Entry;
import com.renren.entrybase.mysql.dao.supports.EntryRowMapper;

/**
 * ${ENTRY_TABLE}
 * 
 * @author qieqie.wang@gmail.com
 */
@DAO
public interface EntryDAO {

    /**
     * @param shard 散表值
     * @param seq 序列号
     * @param id 业务id
     * @param createTime 创建时间
     * @param info 除大文本内容外的信息串 
     * @param body 内容
     */
    @SQL("INSERT IGNORE INTO ${ENTRY_TABLE} (entry_seq, entry_id, shard_value, create_time, optimism_lock, info#if(:body){, body})"
            + " VALUES (:entrySeq, :entryId, :shardValue, :createTime, 0, :info"
            + "#if(:body){, :body})")
    void addEntry(//
            @ShardBy @SQLParam("shardValue") long shard, //
            @SQLParam("entrySeq") long seq,//
            @SQLParam("entryId") long id, //
            @SQLParam("createTime") Date createTime,//
            @SQLParam("info") String info,//
            @SQLParam("body") String body//
    );

    /**
     * 
     * @param shard 散表值
     * @param id 业务id
     * @param info 除大文本内容外的信息串 
     * @param optimismLock 该记录的版本号，通过这个版本号实现数据更新的乐观锁
     */
    @SQL("UPDATE ${ENTRY_TABLE} SET info=:info, optimism_lock=optimism_lock+1 " //
    		+ "WHERE entry_id=:entryId and optimism_lock=:optimismLock ")
    boolean updateInfo(//
            @ShardBy @SQLParam("shardValue") long shard, //
            @SQLParam("entryId") long id,//
            @SQLParam("info") String info,//
            @SQLParam("optimismLock") int optimismLock//
    );

    /**
     * 
     * @param shard 散表值
     * @param id 业务id
     * @param body 大文本内容 
     * @param optimismLock 该记录的版本号，通过这个版本号实现数据更新的乐观锁
     */
    @SQL("UPDATE ${ENTRY_TABLE} SET body=:body, optimism_lock=optimism_lock+1  "//
    		+ "WHERE entry_id=:entryId and optimism_lock=:optimismLock ")
    boolean updateBody(//
            @ShardBy @SQLParam("shardValue") long shard, //
            @SQLParam("entryId") long id,//
            @SQLParam("body") String body,//
            @SQLParam("optimismLock") int optimismLock//
    );

    /**
     * 结合乐观锁进行数据修改，如果版本不对则修改不成功
     * @param shard 散表值
     * @param id 业务id
     * @param info 除大文本内容外的信息串 
     * @param body 大文本内容 
     * @param optimismLock 该记录的版本号，通过这个版本号实现数据更新的乐观锁
     */
    @SQL("UPDATE ${ENTRY_TABLE} SET info=:info, body=:body, optimism_lock=optimism_lock+1 "//
    		+ "WHERE entry_id=:entryId and optimism_lock=:optimismLock")
    boolean updateEntry(//
            @ShardBy @SQLParam("shardValue") long shard, //
            @SQLParam("entryId") long id,//
            @SQLParam("info") String info,//
            @SQLParam("body") String body,//
            @SQLParam("optimismLock") int optimismLock//
    );
     
    /**
     * 
     * @param shard 散表值
     * @param id 业务id
     */
    @SQL("DELETE FROM ${ENTRY_TABLE} WHERE entry_id=:entryId")
    void delete(//
            @ShardBy @SQLParam("shardValue") long shard, //
            @SQLParam("entryId") long id//
    );

    /**
     * 
     * @param shard 散表值
     * @param id 业务id
     * @return
     */
    @SQL("SELECT entry_seq, entry_id, shard_value, create_time, info, body, optimism_lock"
            + " FROM ${ENTRY_TABLE} WHERE entry_id=:entryId")
    @RowHandler(rowMapper = EntryRowMapper.class)
    Entry get(//
            @ShardBy @SQLParam("shardValue") long shard, //
            @SQLParam("entryId") long id//
    );

}
