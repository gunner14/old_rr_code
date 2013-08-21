package com.xiaonei.platform.framework.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import org.apache.log4j.Logger;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.log.XnLog;


/**
 * XnDAO抽象类，实现了DAO接口，实现了XnLog接口。继承自此XnDAO的子类可以方便使用Log
 * @author lifs
 * 
 */
public abstract class XnDAO implements DAO, XnLog{
	
	public static int inc(final String table, final String column, final int id) throws SQLException {
		OpUpdate op = new OpUpdate("update "+table+" set "+column+" = "+column + " + 1 where id = ?",DataAccessMgr.BIZ_COMMON) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}
		
		};
		return DataAccessMgr.getInstance().update(op);
	}
	public static int dec(final String table, final String column, final int id) throws SQLException {
		OpUpdate op = new OpUpdate("update "+table+" set "+column+" = "+column+" - 1 where id = ? and "+column+" > 0",DataAccessMgr.BIZ_COMMON) {
		
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, id);
			}
		
		};
		return DataAccessMgr.getInstance().update(op);
	}
	public static int set(final String table, final String column, final int id, final int value) throws SQLException {
		if(table.equals("users")){
			/*
			try{
				User u = new User();
				u.setId(id);
				u.getProperties().put(column,String.valueOf(value));
				WUserAdapter.getInstance().set(u);
				//System.out.println("------------column:"+column+"="+String.valueOf(value)) ;
			}catch(Exception e){
				this.getLog().error("dao ice::",e);
			}*/
			return 1;
		}else{
			OpUpdate op = new OpUpdate("update "+table+" set "+column+" = ? where id = ?",DataAccessMgr.BIZ_USER_CONFIG) {
			
				@Override
				public void log(Logger log) {
					log.debug("update "+table+" set "+column+" = "+value+" where id = "+id);
				}
	
				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, value);
					ps.setInt(2, id);
				}
			
			};
			return DataAccessMgr.getInstance().update(op);
		}
	}
}
