package com.xiaonei.platform.component.tools;

import java.io.PrintWriter;

import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForward;
import org.json.JSONObject;

public class AjaxUtil {
	
	/**
	 * @param response
	 * @param object
	 * 	需要输出到页面的内容,可以是String,可以是JSONObject
	 * @return
	 */
	public static ActionForward writeObject(HttpServletResponse response,Object object){
		try{
   		    response.setContentType("text/html;charset=utf-8");
     		PrintWriter writer =response.getWriter();
     		writer.print(object) ;
     		writer.flush();
     		writer.close() ;
 	    }catch(Exception e){
 		   e.printStackTrace() ;
 	    }
 	    return null;
	}
	
	public static ActionForward returnJSON(HttpServletResponse response,int code,String msg){
		JSONObject ret  = new JSONObject();
		ret.put("code",code);
		ret.put("msg", msg);
		return writeObject(response,ret);
	}

}
