package com.xiaonei.login.logic;

import com.xiaonei.platform.core.opt.DataAccessMgr;

/**
 * 三段式广告的需求
 * 
 * @author wen.he1
 *
 */
public interface LoginConfigureDao {
	
	public  final static String  selectLoginConfigureSQL = "select config_key,config_value from login_config where id > 0";
	public  final static String   DATA_SOURCE            = DataAccessMgr.BIZ_COMMON;
	
}
