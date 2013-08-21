package com.xiaonei.passport.dao.impl;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.dao.IpAttributionDAO;
import com.xiaonei.passport.model.BabordIPs;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
@Service
public class IpAttributionDAOImpl implements IpAttributionDAO {
	private final static Log logger = LogFactory.getLog(IpAttributionDAOImpl.class);
	@SuppressWarnings("unchecked")
	@Override
	public List<BabordIPs> getBanedIpList() throws DataAccessException {
		OpList op =new OpList(IpAttributionDAO.getBanedIpListSql, DataAccessMgr.BIZ_COMMON){

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				BabordIPs babordIPs=new BabordIPs();
				babordIPs.setStart(rs.getString("start"));
				babordIPs.setEnd(rs.getString("end"));
				return babordIPs;
			}
			
		};
		try {
			return (List<BabordIPs>)DataAccessMgr.getInstance().queryList(op);
		} catch (SQLException e) {
			logger.error("IpAttributionDAOImpl execute getBanedIpList error", e);
			return null;
		}
	}

}
