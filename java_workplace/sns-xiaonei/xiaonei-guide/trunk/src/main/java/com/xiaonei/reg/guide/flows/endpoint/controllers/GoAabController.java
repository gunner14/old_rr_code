package com.xiaonei.reg.guide.flows.endpoint.controllers;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.commons.interceptors.access.annotation.IgnoreUserStatusImportedValidation;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.util.GuideLogger;


/**
 * GoAabController.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-26 新建文件 
 */
@LoginRequired
@Path("goaab")
@IgnoreUserStatusImportedValidation
public class GoAabController {

	private final static String INIT = "f:lead";
	private final static String SUCC = "/views/guide/endpoint/goaab.jsp";
	
	@Post
	@Get
	public String post(Invocation inv){
		
		String ret = INIT;
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		final User host = GuideXFiveRequestUtil.getCurrentHost(request);
		
		if(request == null){
			GuideLogger.printLog(" request null ",GuideLogger.ERROR);
		}
		else if(host == null){
			GuideLogger.printLog(" host null ",GuideLogger.ERROR);
		}
		else{
		    String toUrl = GuideXFiveRequestUtil.getParamString(request, "to", host.getId());
		    try {
		        toUrl = URLDecoder.decode(toUrl, "UTF-8");
            } catch (UnsupportedEncodingException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            request.setAttribute("toUrl", toUrl);
			ret = SUCC;
			
		}
		return ret;
	}
	public static void main(String[] args) throws UnsupportedEncodingException {
        String a = "http://af.sdf.sdfl?a=sdf";
        System.out.println(URLEncoder.encode(a, "UTF-8"));
        System.out.println(URLDecoder.decode(a, "UTF-8"));
    }
}
