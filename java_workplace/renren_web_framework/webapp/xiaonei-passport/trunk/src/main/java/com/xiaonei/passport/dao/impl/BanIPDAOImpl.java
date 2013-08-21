package com.xiaonei.passport.dao.impl;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.dao.BanIPDAO;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
@Service
public class BanIPDAOImpl implements BanIPDAO{
	private final static Log logger = LogFactory.getLog(BanIPDAOImpl.class);
	@SuppressWarnings("unchecked")
	@Override
	public List<String> getBanedIpList() throws DataAccessException {
		OpList op =new OpList(BanIPDAO.getBanedIpListSql, DataAccessMgr.BIZ_STAT){

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				String ip= rs.getString("ip");
				return ip;
			}
			
		};
		try {
			return (List<String>) DataAccessMgr.getInstance().queryList(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute add error", e);
			return null;
		}
	}

}
