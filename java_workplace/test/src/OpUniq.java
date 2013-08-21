package com.xiaonei.platform.core.opt;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;


/**
 * 
 * OpList.java
 * @author drizzt drizzt.wang@gmail.com 2006-6-27 5:14:07
 */
public class OpUniq extends Op{
	public Object result;
	
	/**
	 * 此构造器用于业务不散表的情况<br>
	 * @param sql sql语句
	 * @param bizName 业务名
	 */
	public OpUniq(String sql,String bizName) {
		this.sql = sql;
		result = null;
		this.bizName = bizName;
	}

	/**
	 * 此构造器用于业务散表的情况<br>
	 * @param sql sql语句
	 * @param bizName 业务名
	 * @param tableSuffix 散表表名的后缀，如gossip_2,应该传入 2
	 */
	public OpUniq(String sql,String bizName,int tableSuffix) {
		this.sql = sql;
		result = null;
		this.bizName = bizName;
		this.tableSuffix = tableSuffix;
	}
	
	public void setParam(PreparedStatement ps) throws SQLException {
	}

	public Object parse(ResultSet rs) throws SQLException {
		return null;
	}

	public final Object getResult() {
		return result;
	}
	
	public final void add(Object ob) {
		result = ob;
	}
	
}