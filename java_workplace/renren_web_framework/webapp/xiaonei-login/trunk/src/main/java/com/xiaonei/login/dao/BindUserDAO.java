package com.xiaonei.login.dao;

import java.sql.SQLException;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.xiaonei.platform.core.opt.DataAccessMgr;

/**
 * 绑定用户相关的操作
 */
@DAO(catalog = DataAccessMgr.BIZ_USER_COMMON)
public interface BindUserDAO {

	/**
	 * 根据用户名获取绑定用户的用户 id
	 */
	@SQL("SELECT user_id FROM multi_accounts_map WHERE account = :1 AND account_type=3")
	public int getBindUseridByName(final String name) throws SQLException;
}
