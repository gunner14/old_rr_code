package com.renren.entries.core.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.RowHandler;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.ShardBy;
import net.paoding.rose.jade.annotation.UseMaster;

import com.renren.entries.model.Entry;

/**
 * info表数据访问
 * 
 * @author yanghe.liang@opi-corp.com
 */
//TODO: 要增加updated字段
@DAO(catalog = "entry_info")
public interface InfoDAO {

    @SQL("INSERT INTO entry_info (entry_id, value) VALUES (:1, :2) ON DUPLICATE KEY UPDATE value=VALUES(value)")
    public void save(@ShardBy long entryId, String value);

    @SQL("DELETE FROM entry_info WHERE entry_id = :1")
    public void delete(@ShardBy long entryId);

    @SQL("SELECT entry_id, updated, value FROM entry_info WHERE entry_id = :1")
    @RowHandler(rowMapper = EntryRowMapper.class)
    public Entry get(@ShardBy long entryId);

    @UseMaster
    @SQL("SELECT entry_id, updated, value FROM entry_info WHERE entry_id = :1")
    @RowHandler(rowMapper = EntryRowMapper.class)
    public Entry getFromMaster(@ShardBy long entryId);
    
    @SQL("UPDATE entry_info SET value=:3 WHERE entry_id = :1 AND value=:2")
    public boolean cas(@ShardBy long entryId,  String originValue, String newValue);
    
}
