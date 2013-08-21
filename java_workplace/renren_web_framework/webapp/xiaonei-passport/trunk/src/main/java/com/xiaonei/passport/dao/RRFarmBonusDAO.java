package com.xiaonei.passport.dao;

import java.sql.SQLException;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.platform.core.opt.DataAccessMgr;

/**
 * RRFarmBonusDAO <br>
 * 
 * @author tai.wang@opi-corp.com Mar 11, 2010 - 7:10:27 PM
 */
@DAO(catalog = DataAccessMgr.BIZ_KX_STAT)
public interface RRFarmBonusDAO {
	String getAppsListSql="select bonus_id from rrfarm_bonus where user_id=?";
	String setAppsListSql="insert into rrfarm_bonus (user_id,bonus_id,opr_time) values (?,?,now()) on duplicate key update bonus_id=concat(bonus_id,':',?) ,opr_time=now()";
	String delAppsListSql="delete from rrfarm_bonus";
	
	@SQL("select bonus_id from rrfarm_bonus where user_id=:1")
	public String getAppsList(final int userId) throws SQLException;

	@SQL("insert into rrfarm_bonus (user_id,bonus_id,opr_time) values (:1,:2,now()) on duplicate key update bonus_id=concat(bonus_id,':',:2) ,opr_time=now()")
	public void setAppsList(final int userId, final int bonusId)
			throws SQLException;

	@SQL("delete from rrfarm_bonus")
	public void delAppsList() throws SQLException;
}
	