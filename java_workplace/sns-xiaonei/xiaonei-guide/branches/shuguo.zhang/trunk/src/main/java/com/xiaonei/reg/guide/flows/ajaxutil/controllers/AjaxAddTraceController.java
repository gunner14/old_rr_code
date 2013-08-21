package com.xiaonei.reg.guide.flows.ajaxutil.controllers;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author inshion(xin.yin@opi-corp.com)
 * @since 2010-11-1
 */
@LoginRequired
@Path("axat")
@IgnoreUserStatusImportedValidation
public class AjaxAddTraceController {
	private static final String INIT = "/views/guide/ajax/ajax_add_trace.jsp";

	@Get
	@Post
	public String get(Invocation inv, CacheManager cm) {
		String ret = INIT;
		User host = null;
		UserProvider up = UserProvider.getInstance();
		try {
			host = up.get(cm);
			inv.getRequest().setAttribute("host", host);
			
		} catch (Exception e) {
			System.err.println(" [ host:"+(host == null ? 0 : host.getId())+" up:"+(up == null)+" ]" +" e:"+e);
			//e.printStackTrace();
		}
		
		return ret;
	}
	
	public static void main(String[] args) {
		System.err.println(" host:"+(null != null ? 0 : 81120)+" [ up:"+(null != null)+" ]" +" e:"+"");
	}
}
