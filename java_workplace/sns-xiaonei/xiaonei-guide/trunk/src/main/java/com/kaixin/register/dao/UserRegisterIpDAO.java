/**
 * kaixin-reg
 * com.dodoyo.register.dao
 * UserRegisterIpDAO.java
 * Oct 20, 2009 - 2:28:30 PM
 */
package com.kaixin.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.kaixin.register.model.UserRegisterIp;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.xce.XceAdapter;

/**
 * 
 * @author wei.cheng@opi-corp.com
 * @date
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
}
