package com.xiaonei.login.logic;

import java.util.Map;

public interface ILoginConfigure {
	
	/***
	 * 从缓存中获取数据，该缓存的数据由一个定时任务5分钟自动加载过来
	 * 
	 * @return
	 */
	Map<String, String> getLoginConfigure();
}
