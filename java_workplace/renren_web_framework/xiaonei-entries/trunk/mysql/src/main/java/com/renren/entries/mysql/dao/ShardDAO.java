package com.renren.entries.mysql.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

@DAO
public interface ShardDAO {

    @SQL("SELECT entry_shard_value FROM ${INDEX_SHARD_TABLE} WHERE entry_id=:entryId")
    Long getShardValue(@SQLParam("entryId") long entryId);

    @SQL("SELECT entry_seq FROM ${INDEX_SHARD_TABLE} WHERE entry_id=:entryId")
    Long getSequence(@SQLParam("entryId") long entryId);

    // TODO：这个可以搞成on duplicated的形式
    @SQL("INSERT INTO ${INDEX_SHARD_TABLE} (entry_seq, entry_id, entry_shard_value) VALUES (:entrySeq, :entryId, :shardValue)")
    void addShardValue(//
            @SQLParam("entrySeq") long entryseq,//
            @SQLParam("entryId") long entryId, //
            @SQLParam("shardValue") long shardValue);

    @SQL("DELETE FROM ${INDEX_SHARD} WHERE entry_id=:entryId")
    void deleteShardValue(@SQLParam("entryId") long entryId);
}
