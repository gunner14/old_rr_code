package com.xiaonei.xnmonitor.controllers;

import java.io.UnsupportedEncodingException;

import javax.mail.MessagingException;
import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.ReqMapping;
import net.paoding.rose.web.annotation.Return;

@ReqMapping(path = "")
public class IndexController {

    @ReqMapping(path = "")
    @Return("index")
    public void index(Invocation inv) throws UnsupportedEncodingException, MessagingException {
    	HttpServletRequest request = inv.getRequest();
    	System.out.println(request.getRequestURL());
        //		IMDNSReport report = new IMDNSReport();
        //		report.sendMail();
    }

}
