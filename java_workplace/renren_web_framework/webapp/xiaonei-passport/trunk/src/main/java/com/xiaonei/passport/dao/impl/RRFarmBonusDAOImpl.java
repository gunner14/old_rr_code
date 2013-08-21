package com.xiaonei.passport.dao.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.dao.RRFarmBonusDAO;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
@Service
public class RRFarmBonusDAOImpl implements RRFarmBonusDAO {
	private final static Log logger = LogFactory.getLog(RRFarmBonusDAOImpl.class);
	@Override
	public void delAppsList() throws SQLException {
		OpUpdate op =new OpUpdate(RRFarmBonusDAO.delAppsListSql, DataAccessMgr.BIZ_KX_STAT);
		try {
			DataAccessMgr.getInstance().insert(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute del error", e);
		}
	}

	@Override
	public String getAppsList(final int userId) throws SQLException {
		OpUniq op =new OpUniq(RRFarmBonusDAO.getAppsListSql, DataAccessMgr.BIZ_KX_STAT){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
			}
			
		};
		try {
			Object object=DataAccessMgr.getInstance().queryUnique(op);
			return  object==null? null :(String) object;
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute add error", e);
			return null;
		}
	}

	@Override
	public void setAppsList(final int userId, final int bonusId) throws SQLException {
		OpUpdate op =new OpUpdate(RRFarmBonusDAO.setAppsListSql, DataAccessMgr.BIZ_KX_STAT){

			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setInt(2, bonusId);
				ps.setInt(3, bonusId);
			}
			
		};
		try {
			DataAccessMgr.getInstance().insert(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute add error", e);
		}
	}

}
