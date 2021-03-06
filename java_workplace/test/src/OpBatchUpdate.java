package com.xiaonei.platform.core.opt;

import java.sql.PreparedStatement;
import java.sql.SQLException;


/**
 * 批量sql更新，前提是从一个connection
 * @author fusong.li@opi-corp.com
 * 2008-3-27 下午07:01:31
 */
public class OpBatchUpdate  extends Op{
	
	/**
	 * 此构造器用于业务不散表的情况<br>
	 * @param sql
	 */
	public OpBatchUpdate(String sql) {
		this.sql = sql;
	}
	/**
	 * 此构造器用于业务不散表的情况<br>
	 * @param sql sql语句
	 * @param bizName 业务名
	 */
	public OpBatchUpdate(String sql,String bizName) {
		this.sql = sql;
		this.bizName = bizName;
	}

	/**
	 * 此构造器用于业务散表的情况<br>
	 * @param sql sql语句
	 * @param tableSuffix 散表表名的后缀，如gossip_2,应该传入 2
	 */
	public OpBatchUpdate(String sql,int tableSuffix) {
		this.sql = sql;
		this.tableSuffix = tableSuffix;
	}
	/**
	 * 此构造器用于业务散表的情况<br>
	 * @param sql sql语句
	 * @param bizName 业务名
	 * @param tableSuffix 散表表名的后缀，如gossip_2,应该传入 2
	 */
	public OpBatchUpdate(String sql,String bizName,int tableSuffix) {
		this.sql = sql;
		this.bizName = bizName;
		this.tableSuffix = tableSuffix;
	}
	
	public void setParam(PreparedStatement ps) throws SQLException {
	}

}