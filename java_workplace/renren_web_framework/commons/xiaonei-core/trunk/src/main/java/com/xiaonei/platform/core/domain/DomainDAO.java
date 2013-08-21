package com.xiaonei.platform.core.domain;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;


public class DomainDAO {
	private static DomainDAO instance = new DomainDAO();

	public static DomainDAO getInstance() {
		if (instance == null) {
			synchronized (DomainDAO.class) {
				if (instance == null)
					instance = new DomainDAO();
			}
		}
		return instance;
	}
	private DomainDAO(){
		super();
	}
	private static final String fields = " id , domain_name , domain_url ";
	private static final String tableName = "domain_config";
	
	private static final String sql="select "+fields+" from "+tableName;
	/**
	 * 得到列表
	 * @return
	 * @throws SQLException
	 */
	public List<DomainName> getDomainName()  throws SQLException {
		OpList op= new OpList(sql,DataAccessMgr.BIZ_STAT){
			public DomainName parse(ResultSet rs) throws SQLException {
				DomainName dn= new DomainName();
				dn.setDomainName(rs.getString("domain_name"));
				dn.setDomainUrl(rs.getString("domain_url"));
				return dn;
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
	
	public void saveDomainName(final String name,final String url) throws SQLException {
		String sql="insert into domain_config(domain_name,domain_url) values(?,?)";
		OpUpdate op = new OpUpdate(sql,DataAccessMgr.BIZ_STAT){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, name);
				ps.setString(2, url);
			}
			
		};
		DataAccessMgr.getInstance().update(op);
	}
	
	/**
	 * 得到列表 此方法已经过时
	 * @return
	 * @throws SQLException
	 */
	@Deprecated
	public List<DomainName> gets()  throws SQLException {
		OpList op= new OpList(sql,DataAccessMgr.BIZ_STAT){
			public DomainName parse(ResultSet rs) throws SQLException {
				DomainName dn= new DomainName();
				dn.setDomainName(rs.getString("domain_name"));
				dn.setDomainUrl(rs.getString("domain_url"));
				return dn;
			}
		};
		return DataAccessMgr.getInstance().queryList(op);
	}
}
