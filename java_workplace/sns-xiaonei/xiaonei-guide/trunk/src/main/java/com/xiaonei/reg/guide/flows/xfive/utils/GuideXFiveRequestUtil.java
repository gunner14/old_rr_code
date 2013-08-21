package com.xiaonei.reg.guide.flows.xfive.utils;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.websecurity.adapter.SafeFilter;

/**
 * GuideXFiveRequestUtil.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-6 添加setErrorMsg()
 * 2010-9-6 添加setAttribute(HttpServletRequest request, String attname, String att)
 */
public class GuideXFiveRequestUtil {
	public static User getCurrentHost(HttpServletRequest request) {
		if(request == null){
			GuideLogger.printLog(" request null! ",GuideLogger.ERROR);
			return null;
		}
		User host = null;
		try {
			host = BaseThreadUtil.currentHost(request);
		} catch (Exception e) {
			GuideLogger.printLog(""+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return host;
	}



	public static HttpServletRequest getRequest(Invocation inv) {
		if(inv == null){
			GuideLogger.printLog(" inv null! ",GuideLogger.ERROR);
			return null;
		}
		HttpServletRequest request = null;
		try {
			request = inv.getRequest();
		} catch (Exception e) {
			GuideLogger.printLog(""+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return request;
	}
	
	public static HttpServletResponse getResponse(Invocation inv) {
		if(inv == null){
			GuideLogger.printLog(" inv null! ",GuideLogger.ERROR);
			return null;
		}
		HttpServletResponse response = null;
		try {
			response = inv.getResponse();
		} catch (Exception e) {
			GuideLogger.printLog(""+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		return response;
	}
	
	public static String getBrowserInfo(HttpServletRequest request){
		String info = null;
		info = request.getHeader("User-Agent");
		if(info == null) return " UA[no agent]";
		return " UA["+info+"]";
	}
	
	public static int getParamInteger(HttpServletRequest request,String param,int userid){
		int ret = 0;
		if(MyStringUtils.notEmpty(param)){
			String ucnt = request.getParameter(param);
			try {
				ret = Integer.parseInt(ucnt);
				GuideLogger.printLog(" host:"+userid+" type:"+param+" cnt:"+ret);
			} catch (NumberFormatException e) {
				GuideLogger.printLog(" host:"+userid+" type:"+param+" err:"+e.toString());
			}
		}
		return ret;
	}
	
	public static String getParamString(HttpServletRequest request,String param,int userid){
		
		String ret = "";
		if(request == null){
			GuideLogger.printLog(" host:"+userid+"["+param+"]"+"request null");
		}
		else if(MyStringUtils.notEmpty(param)){
			String value = request.getParameter(param);
			if(value == null){
				String ref = request.getHeader("Referer");
				GuideLogger.printLog(" host:"+userid+" "+param+" null!"+" ref:"+ref);
			}
			else{
				ret = value;
				GuideLogger.printLog(" host:"+userid+" "+param+":"+value);
			}
		}
		return ret;
	}
	
	public static void setErrorMsg(HttpServletRequest request,String msg){
	    request.setAttribute("guide_lead_erromsg", msg);
	}
	
	public static String writeAjax(final HttpServletResponse response, final String str) {
        try {
            response.setContentType("text/html; charset=UTF-8");
            response.getWriter().print(str);
            response.getWriter().flush();
            response.getWriter().close();
        } catch (IOException e) {} finally {}
        return null;
    }



    public static void setAttribute(HttpServletRequest request, String attname, String att) {
        try {
            if(MyStringUtils.isNullOrEmpty(attname)){
                return;
            }
            att = SafeFilter.getInstance().getPureContent(att);
            if(!MyStringUtils.isNullOrEmpty(att)){
                request.setAttribute(attname, att);
            }
            else{
                request.setAttribute(attname, "");
            }
        } catch (Exception e) {
            
        }
    }
	
	
}
