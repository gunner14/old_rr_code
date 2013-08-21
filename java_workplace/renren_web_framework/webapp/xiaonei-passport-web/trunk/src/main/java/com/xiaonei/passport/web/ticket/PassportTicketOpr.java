package com.xiaonei.passport.web.ticket;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.renren.passport.service.LoginType;
import com.renren.passport.service.TicketInfo;
import com.xiaonei.passport.web.model.LoginResult;
import com.xiaonei.passport.web.model.PassportForm;

public interface PassportTicketOpr {
	
    public String getPassportFromCookie(HttpServletRequest request);

    public LoginResult getUserIdByPassport(HttpServletRequest request, HttpServletResponse response, final PassportForm pForm, LoginType loginType);

    public void savePassport2Cookie(final int userid, final HttpServletRequest request,
            final HttpServletResponse response, final String passport, String passportOld,final boolean isPersistent);

    public int setTicket(final String ticket, String oldTicket,final boolean autoLogin,
            final HttpServletRequest request, final HttpServletResponse response);
    public TicketInfo ticket2Id(String passport);
    
}
