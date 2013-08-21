package com.xiaonei.passport.web.event.fail.impl;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.web.event.fail.IFailLoginListern;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;
import com.xiaonei.passport.web.util.LoginContextUtil;
@Service
public class PageGetDefaultErrorListern implements IFailLoginListern {

    @Override
    public String process(HttpServletRequest request,
            HttpServletResponse response, PassportForm pForm,
            int code, LoginResult resultWrapper) {
        
        String errorUrl = LoginContextUtil.getErrorUrl(pForm.getErrorUrl(),pForm.getOrigURL());
        
        if(StringUtils.startsWith(errorUrl, "http://zhan.renren.com/home")) {
            return errorUrl.replaceAll("&amp;", "&");
        }
        return errorUrl;
    }
    @Override
    public boolean support(HttpServletRequest request, HttpServletResponse response, PassportForm pForm, int code, LoginResult result) {
        if(LoginStatusCode.LOGIN_OK.getCode() == code){
            return false;
        }
        return true;
    }

}
