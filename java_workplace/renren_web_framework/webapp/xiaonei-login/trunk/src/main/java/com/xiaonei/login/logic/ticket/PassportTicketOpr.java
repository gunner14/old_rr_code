package com.xiaonei.login.logic.ticket;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public interface PassportTicketOpr {

    /**
     * 获取跳转地址
     * 
     * @param request
     * @param ticket
     * @param domain
     * @param needNotify
     * @return
     */
    public String forwardSetTicket(HttpServletRequest request, String ticket, String domain,
            String origUrl, String p3p, boolean autoLogin, boolean needNotify);

    public String getPassportFromCookie(HttpServletRequest request);

    public int getUserIdByPassport(HttpServletRequest request, HttpServletResponse response);

    public String id2Ticket(int id);

    public void savePassport2Cookie(int userid, HttpServletRequest request,
            HttpServletResponse response, String passport, boolean isPersistent);

    public int setTicket(String ticket, boolean autoLogin, HttpServletRequest request,
            HttpServletResponse response);

    public int ticket2Id(String passport);
    
    /**
     * <p> 通过userId从cache找是否他当天第一次登陆
     * <p> 如果返回的结果不是null，表明ta当天已经登陆过，
     * <p> 否则，需要以userId作为key向cache写入数据
     * 
     * @param userId
     * @return 返回true表示已经登陆过，否则当天未登陆
     */
    public boolean isAutoLoginByDay(int userId);
}
