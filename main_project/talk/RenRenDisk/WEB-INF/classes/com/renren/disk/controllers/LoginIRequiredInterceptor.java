package com.renren.disk.controllers;

import java.lang.annotation.Annotation;

import javax.servlet.http.Cookie;

import org.springframework.beans.factory.annotation.Autowired;

import com.renren.disk.config.DConstant;
import com.renren.disk.dao.DUserDAO;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

public class LoginIRequiredInterceptor extends ControllerInterceptorAdapter {
	

	@Override
	protected Class<? extends Annotation> getRequiredAnnotationClass() {
		return LoginIRequired.class;
	}

	@Autowired
	private DUserDAO userDao;	
	@Autowired 
	private HostHolder hostHolder;

	@Override
	public Object before(Invocation inv) throws Exception {
		User host = hostHolder.getUser();
		if(host == null){
			return "@http://www.renren.com/SysHome.do?origURL=" + DConstant.renren_base_path + "main";
		}
		
		Cookie[] cookies = inv.getRequest().getCookies();
		String token = null;
		for (Cookie cookie : cookies) {
			if (cookie.getName().equalsIgnoreCase("a_t")) {
				token = cookie.getValue();
				break;
			}
		}
		if (token == null) {
			return "@http://www.renren.com/SysHome.do?origURL=" + DConstant.renren_base_path + "main";
		}
		inv.getRequest().setAttribute("uid", host.getId());
		inv.getRequest().setAttribute("token", token);
	    return true;
	}
}
