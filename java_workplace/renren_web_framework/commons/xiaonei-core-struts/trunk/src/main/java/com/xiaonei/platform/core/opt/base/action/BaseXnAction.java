package com.xiaonei.platform.core.opt.base.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.action.ActionMessages;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.sysmsg.SysMsgManager;



/**
 * 此action是业务无关性的，不依赖其他包，只包含常量或工具方法，被提到core包中
 * 提供了基本的 域名常量 以及动作常量 提供了错误/信息提示的接口。
 * @author modified by lifs 2007-12-14
 */
public abstract class BaseXnAction extends RootAction {
	
	protected Logger log = Logger.getLogger(this.getClass());
	protected Logger logImpt = Logger.getLogger("impt");
	protected Logger logError = Logger.getLogger("err");
	protected Logger logIM = Logger.getLogger("im");
	protected Logger log404 = Logger.getLogger("404");
	protected Logger log500 = Logger.getLogger("500");
	
	/**
	 * 新的logger对象，建议用这个
	 */
	public static Log logger = LogFactory.getLog(BaseXnAction.class);
	
	
	public static final String ACTION_READY = "ready" ;
	public static final String ACTION_ADD = "add" ;
	public static final String ACTION_SHARE_LINK = "sharelink" ;
	public static final String ACTION_TO_FRIEND = "sharetofriend" ;
	public static final String ACTION_EDIT = "edit" ;
	public static final String ACTION_DEL = "del" ;
	public static final String ACTION_SAVE = "save" ;
	public static final Object urlWww = OpiConstants.urlWww;
	public static final Object urlMain = OpiConstants.urlMain;
	public static final Object urlClass = OpiConstants.urlClass;//add by lifs
	public static final Object urlTeam = OpiConstants.urlTeam;//add by lifs 
	public static final Object urlGroup = OpiConstants.urlGroup;
	public static final Object urlEvent = OpiConstants.urlEvent;
	public static final Object urlCourse = OpiConstants.urlCourse;
	public static final Object urlBlog = OpiConstants.urlBlog;
	public static final Object urlPhoto = OpiConstants.urlPhoto;
	public static final Object urlMsg = OpiConstants.urlMsg;
	public static final Object urlUpload = OpiConstants.urlUpload;
	public static final Object urlSchool = OpiConstants.urlSchool;
	public static final Object urlImg = OpiConstants.urlImg;
	public static final Object urlBrowse = OpiConstants.urlBrowse;
	public static final Object urlAdmin = OpiConstants.urlAdmin;
	public static final Object urlSupport=OpiConstants.urlSupport;
	public static final Object urlApp = OpiConstants.urlApp;
	public static final String errUnknown = "内部错误";
	public static final String errUrl = "网址格式错误";
	public static final String errUserNotFound = "该用户不存在";
	public static final String errUnivNotFound = "该学校不存在";
	public static final String toError = OpiConstants.forwardError;
	public static final String noPermission=OpiConstants.forwardNoPermission ;

	public static final int defaultLimit = 20;
	public static final int viewUserGossipLimit = 10;

	public abstract ActionForward executeXn(ActionMapping mapping,ActionForm form,HttpServletRequest request,HttpServletResponse response) throws Exception;

	public final ActionForward executeR(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response) throws Exception {
		return executeXn(mapping, form,request, response);
	}    
	public void logErr(HttpServletRequest request) {
		User host = BaseThreadUtil.currentHost(request);
		if (host != null) {
			int usr = host.getId();
			logError.error("ERROR - USER: "+usr+"; URL: "+request.getRequestURL()+"; REQUEST: "+ request.getRequestURI()+"?"+request.getQueryString());
		}
	}

	public void logNotFound(HttpServletRequest request) {
		User host = BaseThreadUtil.currentHost(request);
		if (host != null) {
			int usr = host.getId();
			logError.error("Not Found - USER: "+usr+"; URL: "+request.getRequestURL()+"; REQUEST: "+ request.getRequestURI()+"?"+request.getQueryString());
		}
	}

	public void logUrlError(HttpServletRequest request) {
		logError.error("URL: "+request.getRequestURL()+"; REQUEST: "+ request.getRequestURI()+"?"+request.getQueryString()+"; Referer:"+request.getHeader("Referer"));
	}
	
	/**
	 * @deprecated 由于关掉了resin的session，所以此方法不再可用了
	 */
	public static void addMsg(HttpSession session, final String msg) {
		ActionMessages ams = (ActionMessages) session.getAttribute(org.apache.struts.Globals.MESSAGE_KEY);
		if (ams == null || ams.isEmpty()) {
			ams = new ActionMessages();
		}
		ActionMessage am = new ActionMessage("errors.detail", msg);
		ams.clear();
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		session.setAttribute(org.apache.struts.Globals.MESSAGE_KEY, ams);
		
	}
	public static void addMsg(HttpServletRequest request, final String msg) {
		BizFilterUtil.addMsg(request, msg);
	}

	/**
	 * @deprecated 由于关掉了resin的session，所以此方法不再可用了
	 */
	public static void addErr(HttpSession session, final String err) {
		ActionMessages ams = (ActionMessages) session.getAttribute(org.apache.struts.Globals.ERROR_KEY);
		if (ams == null || ams.isEmpty()) {
			ams = new ActionMessages();
		}

		ActionMessage am = new ActionMessage("errors.detail", err);
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		session.setAttribute(org.apache.struts.Globals.ERROR_KEY, ams);
		
	}
	public static void addErr(HttpServletRequest request, final String err) {
		BizFilterUtil.addErr(request, err);
	}
	public static String getFromIp(HttpServletRequest request){
		 return request.getRemoteAddr() ;
	}
	public boolean isPost(HttpServletRequest request){
		 if(request.getMethod().toLowerCase().equals("post"))
			 return true ;
		 return false ;
	 }

	/**
	 * 下面增加一组处理错误消息的函数，目的是为了把原来的HttpSession参数去掉
	 */

	/**
	 * 添加session error消息；session使用memcache来实现
	 * @param msg
	 */
	public static void addErr(HttpServletRequest request, HttpServletResponse response, String msg){
		BizFilterUtil.addErr(request, response, msg);
	}
	
	/**
	 *  新的信息处理机制
	 * @param request
	 * @param response
	 * @param msg
	 */
	public static void addMsg(HttpServletRequest request,HttpServletResponse response,String msg){
		BizFilterUtil.addMsg(request, response, msg);
	}
	
	
}
