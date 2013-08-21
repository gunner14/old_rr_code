package com.xiaonei.login.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import org.springframework.dao.DataAccessException;

import com.xiaonei.platform.core.opt.DataAccessMgr;

/**
 * 违禁 IP 列表操作
 */
@DAO(catalog=DataAccessMgr.BIZ_STAT)
public interface BanIPDAO {

	/**
	 * 获取 Baned IP 列表
	 */
	@SQL("select ip from ban_ip where type=0")
	public List<String> getBanedIpList() throws DataAccessException ;
}
