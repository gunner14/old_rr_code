package com.xiaonei.reg.register.controllers.msn;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date May 11, 2010 3:53:26 PM
 * MSN合作。注册分支判断页面
 */

@Path("msnlogin")
public class MsnLoginController {

	private RegLogger mslog = RegLogger.getLoggerWithType(this.getClass());
	
	private static final String enc = "UTF-8";
	
	@Get
	@Post
	public String index(Invocation inv){
    	mslog.debug("begin into msn login page .....");
        
        HttpServletRequest request = inv.getRequest();
        String email = StringUtils.trimToEmpty(request.getParameter("email"));
        request.setAttribute("regEmail", isValidEmailAddr(email)? email: "");
        
        String defaultURL = Globals.urlWww + "/Home.do";
        String encDest, orig, origURL = defaultURL;
		try {
			encDest = URLEncoder.encode(defaultURL, enc);
			orig    = Globals.urlInvite + "/crtmt?tUrl=" + encDest;
			origURL = URLEncoder.encode(orig, enc);
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
        request.setAttribute("origURL", origURL);
        return "/pages/register/reg/msn_login.jsp";
    }
	
	private static boolean isValidEmailAddr(String email) {
		String pattern3 = "\\w*@{1}[\\w&&[^_]][\\w\\.]*\\w";
		String pattern4 = ".*\\.\\..*";
		return Pattern.matches(pattern3, email)
				&& !Pattern.matches(pattern4, email);
	}
}
