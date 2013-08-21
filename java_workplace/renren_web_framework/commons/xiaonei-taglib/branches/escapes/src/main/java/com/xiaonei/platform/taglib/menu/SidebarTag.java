package com.xiaonei.platform.taglib.menu;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.PageContext;
import javax.servlet.jsp.tagext.TagSupport;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.taglib.helper.SidebarHelper;
import com.xiaonei.platform.taglib.helper.TempletData;
import com.xiaonei.platform.taglib.utils.TagUtils;

/**
 * 
 * 
 * @author lifs
 * 
 */
public class SidebarTag extends TagSupport {
	
	public static String TYPE_MINI = "mini";//
	public static String TYPE_ONLINE = "online";//在线
	public static String TYPE_OFFLINE = "offline";//离线
	private String type;

	
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	@Override
	public int doStartTag() throws JspException {
		User host = (User) this.pageContext.getAttribute("host",PageContext.REQUEST_SCOPE);//
		HttpServletRequest request = (HttpServletRequest) pageContext.getRequest();
		String result = "";
		if(type!=null && TYPE_MINI.equals(type)){
			result = SidebarHelper.help(request, type);
		}else{
			if (host == null || (type!=null && TYPE_OFFLINE.equals(type))) {
				// 未登录时
				result = SidebarHelper.help(request, TYPE_OFFLINE);
			} else if (host != null || (type!=null && TYPE_ONLINE.equals(type))) {
				result = SidebarHelper.help(request, TYPE_ONLINE);
			}
		}
		TagUtils.out(pageContext, result);
		return super.doStartTag();
	}
	
	
}