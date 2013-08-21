package com.renren.entries.core.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.ShardBy;

/**
 * body表数据访问
 * 
 * @author yanghe.liang@opi-corp.com
 */
@DAO(catalog = "entry_body")
public interface BodyDAO {

    @SQL("INSERT INTO entry_body (entry_id, value) VALUES (:1, :2) ON DUPLICATE KEY UPDATE value=VALUES(value)")
    public void save(@ShardBy long entryId, String value);

    @SQL("delete from entry_body where entry_id = :1")
    public void delete(@ShardBy long entryId);

    @SQL("select value from entry_body where entry_id = :1")
    public String get(@ShardBy long entryId);

}
