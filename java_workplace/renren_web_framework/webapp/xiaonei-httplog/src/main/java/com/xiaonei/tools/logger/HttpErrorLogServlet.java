package com.xiaonei.tools.logger;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class HttpErrorLogServlet extends javax.servlet.http.HttpServlet {

    private static final long serialVersionUID = -8165598080177352422L;

    @Override
    protected void service(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger("rec404log");
        String referer = request.getHeader("Referer");//出错页面的url
        String f = request.getParameter("f"); //错误类型,404还是500
        String m = request.getParameter("m"); //出错所在的机器名
        String ref = request.getParameter("ref"); //出错的页面前的referer
        if (referer == null) referer = "-";
        if (m == null) m = "-";
        if (ref == null) ref = "-";
//2009-07-29-00:01:00 500 98.222.212.62 - - http://www.kaixin.com/Error.do
//2009-07-29-00:01:21 500 221.8.180.240 SJSWT44-14.opi.com http://home.xiaonei.com/Home.do?id=223978271 http://msg.xiaonei.com/notify/notifications.do

        log.info(f + " " + request.getRemoteAddr() + " " + m + " " + ref + " " + referer);
    }

}
