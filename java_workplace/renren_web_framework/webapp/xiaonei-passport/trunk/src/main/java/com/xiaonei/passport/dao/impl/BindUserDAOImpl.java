package com.xiaonei.passport.dao.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.dao.BindUserDAO;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
@Service
public class BindUserDAOImpl implements BindUserDAO {
	private final static Log logger = LogFactory.getLog(BindUserDAOImpl.class);
	@Override
	public int getBindUseridByName(final String name) throws SQLException {
		OpUniq op =new OpUniq(BindUserDAO.getBindUseridByNameSql, DataAccessMgr.BIZ_USER_COMMON){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setString(1, name);
			}
			
		};
		try {
			return DataAccessMgr.getInstance().queryInt(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute add error", e);
			return 0;
		}
	}

}
