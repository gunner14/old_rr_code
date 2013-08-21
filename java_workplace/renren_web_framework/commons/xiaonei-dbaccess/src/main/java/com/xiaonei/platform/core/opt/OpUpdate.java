package com.xiaonei.platform.core.opt;

import java.sql.PreparedStatement;
import java.sql.SQLException;


/**
 * 
 * OpUpdate.java
 * @author drizzt drizzt.wang@gmail.com 2006-6-27 12:28:28
 */
public class OpUpdate  extends Op{
	

	/**
	 * 此构造器用于业务不散表的情况<br>
	 * @param sql sql语句
	 * @param bizName 业务名
	 */
	public OpUpdate(String sql,String bizName) {
		this.sql = sql;
		this.bizName = bizName;
	}
	
	/**
	 * 此构造器用于业务散表的情况<br>
	 * @param sql sql语句
	 * @param bizName 业务名
	 * @param tableSuffix 散表表名的后缀，如gossip_2,应该传入 2
	 */
	public OpUpdate(String sql,String bizName,int tableSuffix) {
		this.sql = sql;
		this.bizName = bizName;
		this.tableSuffix = tableSuffix;
	}
	
	public void setParam(PreparedStatement ps) throws SQLException {
	}

}