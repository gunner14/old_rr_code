package com.xiaonei.passport.web.event.fail.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.renren.passport.service.TicketInfo;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.constants.OrigUrlDefaultDef;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.ticket.PassportTicketOpr;
import com.xiaonei.passport.web.util.LoginContextUtil;
import com.xiaonei.passport.web.util.OrigUrlUtil;

@Service
public class PassportDomainErrorListern extends DefaultErrorListern{
	@Autowired
	 private PassportTicketOpr passportTicketOpr;
	@Override
	public String process(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			int code, LoginResult result) {
		 final String passport = passportTicketOpr.getPassportFromCookie(request);
		 String queryString=request.getQueryString();
	        // passport无效
	     if (passport == null) {
	            return OrigUrlUtil.getUrl(0, OrigUrlDefaultDef.DEFAULT_LOGIN_URL, pForm.getOrigURL(), code, pForm.getEmail(), pForm.getLoginRegistrationId(),queryString);
	     }  
		TicketInfo ticket=passportTicketOpr.ticket2Id(passport);
		if(ticket!=null&&ticket.getUserId()>0){
			return LoginContextUtil.getOrigUrl(pForm.getOrigURL());
		}
		return OrigUrlUtil.getUrl(0, OrigUrlDefaultDef.DEFAULT_LOGIN_URL, pForm.getOrigURL(), code, pForm.getEmail(), pForm.getLoginRegistrationId(),queryString);
	}

	@Override
	public boolean support(HttpServletRequest request,
			HttpServletResponse response, PassportForm pForm,
			int code, LoginResult result) {
		if(LoginStatusCode.FAILNOTPASSPORTDOMAIN.getCode()==code){
			return true;
		}
		return false;
	}

}
