package com.xiaonei.reg.register.controllers.init;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.reg.guide.flows.xfive.utils.GuideXFiveRequestUtil;
import com.xiaonei.reg.guide.util.GuideLogger;

@Path("searchreg")
public class Xn6234Controller {

	@Get
	@Post
	public String index(Invocation inv) {
		HttpServletRequest request = GuideXFiveRequestUtil.getRequest(inv);
		HttpServletResponse response = GuideXFiveRequestUtil.getResponse(inv);
		if(request == null || response == null){
			GuideLogger.printLog(" request or response null "+(request==null)+(response==null),GuideLogger.ERROR);
			return "@系统忙 请稍后再试";
		}
		String q = request.getParameter("q");
		if(q==null){
			q = "";
		}
		request.setAttribute("q", q);
		String y = request.getParameter("y");
		if(y==null){
		    y = "";
		}
		request.setAttribute("y", y);
		return "/pages/register/reg/reg_6234.jsp";
	}
	
	public static void main(String[] args) throws UnsupportedEncodingException {
	    String a = "%C7%E5%BB%AA%B4%F3%D1%A7";
	    String aa = URLDecoder.decode(a,"GB2312");
	    System.out.println(aa);
	    
	    Object oriq = null;
	    String querystring = "yy=sdf23&q=%C7%E5%BB%AA%B4%F3%D1%A7&from=234gs&y=sdf";
	    String myq = "";
	    try {
	        if(oriq == null){
	            try{
	                String newquerystring = java.net.URLDecoder.decode(querystring,"GB2312");
	                String [] reqs = newquerystring.split("&");
	                for (String string : reqs) {
	                    System.out.println(string);
	                    if(string.startsWith("q")){
	                        myq = string.substring(2);
	                        break;
	                    }
	                }
	                System.out.println(myq);
	            }
	            catch(Exception e){
	                
	            }
	        }
        } catch (Exception e) {
            // TODO: handle exception
        }
    }

}
