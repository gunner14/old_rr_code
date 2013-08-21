package com.xiaonei.commons.interceptors.access;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.Rc;

import org.springframework.stereotype.Component;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;

/**
 * 
 */
@Component("hostHolder")
public class HostHolderImpl implements HostHolder {

	@Override
	public User getUser() {
		HttpServletRequest request = Rc.request();
		if (request != null) {
			return BaseThreadUtil.currentHost(request);
		} else {
			return null;
		}
	}

	public void setUser(User user) {
		HttpServletRequest request = Rc.request();
		if (request != null) {
			BaseThreadUtil.setHost(Rc.request(), user);
		}
	}

}
