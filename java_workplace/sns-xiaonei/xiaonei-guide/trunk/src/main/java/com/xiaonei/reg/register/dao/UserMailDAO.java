/**
 * xiaonei-reg
 * com.xiaonei.reg.register.dao
 * UserRegisterIpDAO.java
 * Sep 20, 2009 - 2:28:30 PM
 */
package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.UserMail;
import com.xiaonei.xce.XceAdapter;

/**
 * UserMailDAO
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 20, 2009 - 2:28:30 PM
 * 
 */
public class UserMailDAO extends XnDAO {
	private static UserMailDAO _instance = new UserMailDAO();

	public static UserMailDAO getInstance() {
		return _instance;
	}

	private UserMailDAO() {
	}

	public int setUserMail(final UserMail userMail) throws SQLException {
		final String sqlInsert = "insert into user_mail (id,correspond_mail) values (?, ?)";
		OpUpdate op = new OpUpdate(sqlInsert, XceAdapter.DB_USER_MAIL) {
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userMail.getId());
				ps.setString(2, userMail.getCorrespond_mail());
			}
		};
		return DataAccessMgr.getInstance().update(op);
	}
}
