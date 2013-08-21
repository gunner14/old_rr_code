package com.xiaonei.commons.interceptors.access;

import com.xiaonei.platform.core.model.User;

/**
 * 
 * 把{@link HostHolder}声明作为控制器或其父类的field，使得控制器中可以取得到当前访问者信息
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public interface HostHolder {

	/**
	 * 返回当前访问者，如果没有登录的话返回null
	 * 
	 * @return
	 */
	public User getUser();

	/**
	 * 
	 * @param user
	 */
	public void setUser(User user);
}
