package com.xiaonei.passport.web.util;


import javax.servlet.http.HttpServletResponse;
/**
 * 返回ajax调用的值给前端
 * @author Administrator
 *
 */
public class AjaxWriter {
	public final static String ENCODE="UTF8";
	public static void write(String content, HttpServletResponse response) {
		try{
			response.setCharacterEncoding(ENCODE);
			response.getWriter().write(content);
			response.getWriter().flush();
		}catch(Exception e){
			throw new RuntimeException(e);
		}
		
	}
	public static void write(int content, HttpServletResponse response) {
		String value=String.valueOf(content);
		write(value, response);
	}
}
