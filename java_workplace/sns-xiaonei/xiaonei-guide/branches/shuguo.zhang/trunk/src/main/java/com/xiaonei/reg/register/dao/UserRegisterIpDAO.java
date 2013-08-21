/**
 * xiaonei-reg
 * com.xiaonei.reg.register.dao
 * UserRegisterIpDAO.java
 * Sep 20, 2009 - 2:28:30 PM
 */
package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.UserRegisterIp;
import com.xiaonei.xce.XceAdapter;

/**
 * UserRegisterIpDAO
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 20, 2009 - 2:28:30 PM
 * 
 */
public class UserRegisterIpDAO extends XnDAO {
	private static UserRegisterIpDAO _instance = new UserRegisterIpDAO();

	public static UserRegisterIpDAO getInstance() {
		return _instance;
	}

	private UserRegisterIpDAO() {
	}

	public int setRegisterIp(final UserRegisterIp userRegisterIp)
			throws SQLException {
		final String sqlInsert = "insert into user_register_ip (id,register_ip) values (?, ?)";
		OpUpdate op = new OpUpdate(sqlInsert, XceAdapter.DB_USER_REGISTER_IP) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userRegisterIp.getId());
				ps.setString(2, userRegisterIp.getRegister_ip());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
	
	final static String sqlSelect = "select register_ip from user_register_ip where id = ? limit 1";
	public String getUserRegisterIp(final int user_id) throws SQLException{
		OpUniq op = new OpUniq(sqlSelect, XceAdapter.DB_USER_REGISTER_IP){
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, user_id);
			}
			
			@Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseIp(rs);
            }
			
		};
		 return (String) DataAccessMgr.getInstance().queryUnique(op);
	}

	protected String parseIp(ResultSet rs) throws SQLException {
		return rs.getString("register_ip");
	}
}
