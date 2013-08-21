package com.xiaonei.platform.taglib.helper;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.jsp.PageContext;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.taglib.menu.SidebarTag;

/**
 * 
 */

public class SidebarHelper{
	
	public static String help(HttpServletRequest request ,String type){
		StringBuffer sb = new StringBuffer();
		
		if(SidebarTag.TYPE_MINI.equals(type)){
			return getMiniSidebar();
		}else if(SidebarTag.TYPE_OFFLINE.equals(type)){
			return getNotLoginSidebar(request);
		}else if(SidebarTag.TYPE_ONLINE.equals(type)){
			return alreadyLogin(request);
		}
		return sb.toString();
	}
	/**
	 * 得到Mini时的sidebar
	 * @return
	 */
	private static String getMiniSidebar(){
		String result="";
		try {
			String wrap1 = TempletData.getInstance().getMap(TempletData.Sidebar_Wrapper_sidebar);
			if(wrap1!=null){
				String wrap2 = TempletData.getInstance().getMap(TempletData.Sidebar_Wrapper_loginpanel);
				if(wrap2!=null){
					String t1 = TempletData.getInstance().getMap(TempletData.Sidebar_YouCan);
					if(t1!=null){
						result = wrap2.replace("${taglib_templet_content}", t1);
					}
					result = wrap1.replace("${taglib_templet_content}", result);
				}
			}	
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}
	
	/**
	 * 未登录时的sidebar
	 * @param request
	 * @return
	 */
	private static String getNotLoginSidebar(HttpServletRequest request){
		String t1 = TempletData.getInstance().getMap(TempletData.Sidebar_Login);
		String t2 = TempletData.getInstance().getMap(TempletData.Sidebar_QuickLink);
//		String t3 = TempletData.getInstance().getMap(TempletData.Sidebar_YouCan);
		String loginEmail = "";
		try {
			Cookie cookies[]=request.getCookies();
			if(cookies != null) {
				for(int i=0;i<cookies.length;i++){
					if(cookies[i].getName().equals("wh_email")) {
						loginEmail = cookies[i].getValue();
					}
				}
			}
		} catch(Exception e) {

		}
		String result = "";
		result = t1.replace("${loginEmail}", loginEmail);
		if(request.getAttribute("origURL")==null){
			result = result.replace("${requestScope.origURL}", "");
		}else{
			result = result.replace("${requestScope.origURL}", (String)request.getAttribute("origURL"));
		}
		result += t2;
//		result += t3;
		
		String wrap2 = TempletData.getInstance().getMap(TempletData.Sidebar_Wrapper_loginpanel);
		result = wrap2.replace("${taglib_templet_content}", result);
		

		String wrap1 = TempletData.getInstance().getMap(TempletData.Sidebar_Wrapper_sidebar);
		result = wrap1.replace("${taglib_templet_content}", result);
		return result;
	}
	
	/**
	 * 登录页面中sidebar
	 * @param request
	 * @return
	 */
	private static String alreadyLogin(HttpServletRequest request){
		User host = (User) request.getAttribute("host");//
		
		String t1 = TempletData.getInstance().getMap(TempletData.Sidebar_Search);
		String result = "";
		result += t1;
		//菜单排序

		if(host!=null){
			//超级管理员菜单
			if(host.getAuth()>=20){
				String t3 = TempletData.getInstance().getMap(TempletData.Sidebar_SuperAdminMenu);
				result += t3;
			}else if(host.getAuth()>=5){
				//管理员菜单
				String t2 = TempletData.getInstance().getMap(TempletData.Sidebar_AdminMenu);
				result += t2;
			}
		}

		String wrap1 = TempletData.getInstance().getMap(TempletData.Sidebar_Wrapper_sidebar);
		result = wrap1.replace("${taglib_templet_content}", result);
		return result;
	}
	
}
