package com.xiaonei.commons.interceptors.access;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;

/**
 * {@link UserAliasInterceptor} 拦截对Web控制器的调用，将当前用户保存为多个属性名
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class UserAliasInterceptor extends ControllerInterceptorAdapter {

	private String[] alias = new String[] { "visiter", "host" };

	private HostHolder hostHolder;

	public void setAlias(String[] alias) {
		this.alias = alias;
	}

	@Autowired
	public void setHostHolder(HostHolder hostHolder) {
		this.hostHolder = hostHolder;
	}

	@Override
	public Object before(Invocation invocation) throws Exception {
		if (hostHolder.getUser() != null) {
			for (int i = 0; i < alias.length; i++) {
				invocation.getModel().add(alias[i], hostHolder.getUser());
			}
		}
		return true;
	}
}
