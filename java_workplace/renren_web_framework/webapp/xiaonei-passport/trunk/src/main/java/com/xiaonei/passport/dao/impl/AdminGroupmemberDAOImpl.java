package com.xiaonei.passport.dao.impl;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.dao.AdminGroupmemberDAO;
import com.xiaonei.passport.model.AdminGroupmember;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;

@Service
public class AdminGroupmemberDAOImpl implements AdminGroupmemberDAO {
	private final static Log logger = LogFactory.getLog(AdminGroupmemberDAOImpl.class);
	@Override
	public void add(final int userid, final String ip) throws DataAccessException {
		OpUpdate op =new OpUpdate(AdminGroupmemberDAO.addSql, DataAccessMgr.BIZ_COMMON){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userid);
				ps.setString(2, ip);
			}
			
		};
		try {
			DataAccessMgr.getInstance().insert(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute add error", e);
		}

	}

	@Override
	public void del(final int userid) throws DataAccessException {
		OpUpdate op =new OpUpdate(AdminGroupmemberDAO.delSql, DataAccessMgr.BIZ_COMMON){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userid);
			}
			
		};
		try {
			DataAccessMgr.getInstance().insert(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute del error", e);
		}
	}

	@SuppressWarnings("unchecked")
	@Override
	public List<AdminGroupmember> getAdminGroupList()
			throws DataAccessException {
		OpList op =new OpList(AdminGroupmemberDAO.getAdminGroupListSql, DataAccessMgr.BIZ_COMMON){

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				AdminGroupmember groupmember=new AdminGroupmember();
				groupmember.setAdminid(rs.getInt("adminid"));
				groupmember.setRestrictIp(rs.getString("restrict_ip"));
				return groupmember;
			}
			
		};
		try {
			return (List<AdminGroupmember>)DataAccessMgr.getInstance().queryList(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute add error", e);
			return null;
		}
	}

}
