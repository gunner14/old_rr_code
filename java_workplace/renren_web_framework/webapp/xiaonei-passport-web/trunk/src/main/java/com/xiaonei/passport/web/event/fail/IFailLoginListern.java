package com.xiaonei.passport.web.event.fail;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;

public interface IFailLoginListern {
	String defualtErrorUrl="http://www.renren.com/SysHome.do";
	
	public String process(HttpServletRequest request, HttpServletResponse response, PassportForm pForm, int code, LoginResult result);
	
	
	public boolean support(HttpServletRequest request, HttpServletResponse response, PassportForm pForm, int code, LoginResult result);
}
