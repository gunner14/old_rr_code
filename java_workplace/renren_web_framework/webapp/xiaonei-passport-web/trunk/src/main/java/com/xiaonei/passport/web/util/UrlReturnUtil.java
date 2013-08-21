package com.xiaonei.passport.web.util;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class UrlReturnUtil {
	protected static Log logger = LogFactory.getLog(UrlReturnUtil.class);
	
	public static String getUrlReturn(HttpServletRequest request, HttpServletResponse response, String url){
		String desc="get url ";
		if(StringUtils.isEmpty(url)){
			return url;
		}
		try{
			if(url.startsWith("f:")){
				url=url.replace("f:", "");
				desc="forward url ";
				request.getRequestDispatcher(url).forward(request, response);
				return null;
			}else if(url.startsWith("r:")){
				desc="redirect url ";
				url=url.replace("r:", "");
				response.sendRedirect(url);
				return null;
			}else{
				response.sendRedirect(url);
				return null;
			}
		}catch(Exception e){
			logger.error(desc+url+" error", e);
			throw new RuntimeException(desc+url+" error", e);
		}
		
	}
	
	public static String redirectUrl(HttpServletResponse response, String url){
		try {
			response.sendRedirect(url);
			return null;
		} catch (IOException e) {
			logger.error("redirect url "+url+" error", e);
			throw new RuntimeException("redirect url "+url+" error", e);
		}
	}
}
