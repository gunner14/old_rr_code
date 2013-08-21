/**
 * 
 */
package com.xiaonei.passport.dao;

import java.util.List;

import org.springframework.dao.DataAccessException;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.passport.model.BabordIPs;
import com.xiaonei.platform.core.opt.DataAccessMgr;

/**
 * IpAttributionDAO <br>
 * 
 * @author tai.wang@opi-corp.com Aug 23, 2010 - 9:31:17 PM
 */
@DAO(catalog = DataAccessMgr.BIZ_COMMON)
public interface IpAttributionDAO {
	String getBanedIpListSql="select start,end from ip_attribution where babord=1";
	
	@SQL("select start,end from ip_attribution where babord=1")
	public List<BabordIPs> getBanedIpList() throws DataAccessException;
}
