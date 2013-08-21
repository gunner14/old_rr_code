package com.kaixin.register.logic;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;

import com.kaixin.Globals;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.reg.usertrace.model.RegVisitCookieLogView;
import com.xiaonei.reg.usertrace.model.RegVisitLogView;
import com.xiaonei.reg.usertrace.model.RegVisitOperate;
import com.xiaonei.reg.usertrace.model.RegVisitOperateSub;

/**
 * @author weige (wei.cheng@opi-corp.com)
 *
 * Mar 18, 2009 3:27:32 PM
 * RegUserActionLogic.java
 * 用户行为action记录前的转换操作，转换为合理的数据结构
 * 提供基本数据模型,如果有例外，自己单独set
 */

public class RegUserActionLogic {

	private static final String S1_KEY = "ss";
	private static final String S2_KEY = "appId";
	private static final String S3_KEY = "g";
	private static final String S4_KEY = "b";
	private static final int KX_STAGE_DEFAULT = 30;
	private static final String CHANNEL_REG_COOKIE_NAME = "rgss";
	/**
	 * 单例
	 */
	private RegUserActionLogic() {
		super();	
	}

	private static RegUserActionLogic _instance = new RegUserActionLogic();

	public static RegUserActionLogic getInstance() {
		return _instance;
	}
	
	/**
	 * 获取referer
	 * @param request
	 * @return
	 */
	private String getReferer(HttpServletRequest request){
		String referer = request.getHeader("Referer");
		String tmp = (referer != null && referer.length()>200) ? referer.substring(0, 200) : referer;
		return tmp;
	}
	
	/**
	 * 获取domain域名
	 * @param request
	 * @return
	 */
	private String getRegDomain(HttpServletRequest request){
		String regUrl = request.getRequestURL() != null ? request.getRequestURL().toString() : null;
		if(regUrl != null && regUrl.indexOf("http://")>-1){
			String temp = regUrl.substring(7);
			if(temp != null && temp.indexOf("/")>-1){
				regUrl = temp.substring(0, temp.indexOf("/"));
			}
		}
		return regUrl;
	}
	
	/**
	 * 获取用户的ip信息
	 * @param request
	 * @return
	 */
	private String getUserIp(HttpServletRequest request){
		return BizFilterUtil.getClientIP(request);
	}
	
	/**
	 * 获取渠道的cookie信息，如果没有则返回null.同时在客户端种入cookie信息
	 * @param request
	 * @param response
	 * @return
	 */
	private String getRegCookie(HttpServletRequest request, HttpServletResponse response){
		String svalue=null;
		String sname=null;
		try{
			Cookie cookies[]=request.getCookies();
			Cookie sCookie=null;
			if(cookies != null && cookies.length >0){
				for(int i=0;i < cookies.length; i++){
					sCookie=cookies[i];
					sname=sCookie.getName();
					if(CHANNEL_REG_COOKIE_NAME.equals(sname)){
						//cookie exist
						svalue=sCookie.getValue();
					}
				}
			}
		}catch(Exception ex){
			ex.printStackTrace();
		}
		if(svalue == null){
			setChannelRegCookie(request, response);
		}
		return svalue;
	}
	
	/**
	 * 种入cookie信息,有效期24小时
	 * @param request
	 * @param response
	 */
	private void setChannelRegCookie(HttpServletRequest request, HttpServletResponse response){
		Cookie ck0 = new Cookie(CHANNEL_REG_COOKIE_NAME , "" + request.getParameter(S1_KEY));
        ck0.setDomain("." + Globals.domain.toString());
        ck0.setMaxAge(24*60*60);
        response.addCookie(ck0);
	}
	
	/**
	 * 获取sesson 信息
	 * @param request
	 * @return
	 */
	private String getXnSessId(HttpServletRequest request){
		return CookieManager.getInstance().getCookie(request, "XNESSESSIONID");
	}
	
	/**
	 * 获取RegVisitLogView 基本对象，如果哪个分支不一样，自己单独设置
	 * @param request
	 * @return
	 */
	public RegVisitLogView getRegVisitHisDao(HttpServletRequest request){
		RegVisitLogView logView = new RegVisitLogView();
		try{
			String xnSessId =  getXnSessId(request);
			String remoteIp =  getUserIp(request);
			logView.setIp(remoteIp == null ? "" : remoteIp);
			logView.setXnSessId(xnSessId == null ? "" : xnSessId);
			String actionIdStr =  request.getParameter("action_id");
			int actionid = 0;
			if(actionIdStr!= null && !"".equals(actionIdStr)){
				try{
					actionid = Integer.parseInt(actionIdStr);
				}catch(Exception ex){
				}
			}
			logView.setActionid(actionid);
			String referer = getReferer(request);
			logView.setReferer(referer != null ? referer : "");
			String s1 = request.getParameter(S1_KEY);
			String s2 = request.getParameter(S2_KEY);
			String s3 = request.getParameter(S3_KEY);
			String s4 = request.getParameter(S4_KEY);
			if(StringUtils.isEmpty(s4)) s4 = s3;
			logView.setS1(s1!=null ? s1: "");
			logView.setS2(s2!=null ? s2: "");
			logView.setS3(s3!=null ? s3: "");
			logView.setS4(s4!=null ? s4: "");
			
			String regUrl = getRegDomain(request);
			logView.setDomainName(regUrl == null ? "" : regUrl);
			String inviterIdStr =  request.getParameter("inviterId");
			int inviter_id = 0;
			if(inviterIdStr!= null && !"".equals(inviterIdStr)){
				try{
					inviter_id = Integer.parseInt(inviterIdStr);
				}catch(Exception ex){
				}
			}
			logView.setInviterId(inviter_id);
		}catch(Exception ex){
			ex.printStackTrace();
		}
		return logView;
	}
	
	/**
	 * 获取RegVisitOperate 基本对象，如果哪个分支不一样，自己单独设置
	 * @param request
	 * @return RegVisitOperate
	 */
	public RegVisitOperate getRegVisitOperate(HttpServletRequest request, User host){
		RegVisitOperate logVisit = new RegVisitOperate();
		try{
			String xnSessId =  getXnSessId(request);
			String remoteIp =  getUserIp(request);
			logVisit.setIp(remoteIp == null ? "" : remoteIp);
			logVisit.setXnSessId(xnSessId == null ? "" : xnSessId);
			String actionIdStr =  request.getParameter("action_id");
			int actionid = 0;
			if(actionIdStr!= null && !"".equals(actionIdStr)){
				try{
					actionid = Integer.parseInt(actionIdStr);
				}catch(Exception ex){
				}
			}
			logVisit.setActionid(actionid);
			String email = request.getParameter("regEmail");
			logVisit.setEmail(email !=null ? email : "");
			logVisit.setUserid(host !=null ? host.getId() : 0);
			logVisit.setStage(KX_STAGE_DEFAULT);
			String s1 = request.getParameter(S1_KEY);
			String s2 = request.getParameter(S2_KEY);
			String s3 = request.getParameter(S3_KEY);
			String s4 = request.getParameter(S4_KEY);
			if(StringUtils.isEmpty(s4)) s4 = s3;
			logVisit.setS1(s1!=null ? s1: "");
			logVisit.setS2(s2!=null ? s2: "");
			logVisit.setS3(s3!=null ? s3: "");
			logVisit.setS4(s4!=null ? s4: "");
			
			String regUrl = getRegDomain(request);
			logVisit.setDomainName(regUrl == null ? "" : regUrl);
			String inviterIdStr =  request.getParameter("inviterId");
			int inviter_id = 0;
			if(inviterIdStr!= null && !"".equals(inviterIdStr)){
				try{
					inviter_id = Integer.parseInt(inviterIdStr);
				}catch(Exception ex){
				}
			}
			logVisit.setInviterId(inviter_id);
		}catch(Exception ex){
			ex.printStackTrace();
		}
		return logVisit;
	}
	
	/**
	 * 获取RegVisitOperateSub 基本对象，如果哪个分支不一样，自己单独设置
	 * @param request
	 * @return RegVisitOperateSub
	 */
	public RegVisitOperateSub getRegVisitOperateSub(HttpServletRequest request, User host){
		RegVisitOperateSub logSub = new RegVisitOperateSub();
		try{
			String actionIdStr =  request.getParameter("action_id");
			int actionid = 0;
			if(actionIdStr!= null && !"".equals(actionIdStr)){
				try{
					actionid = Integer.parseInt(actionIdStr);
				}catch(Exception ex){
				}
			}
			logSub.setActionid(actionid);
			String email = request.getParameter("regEmail");
			logSub.setEmail(email !=null ? email : "");
			logSub.setUserid(host !=null ? host.getId() : 0);
			String s1 = request.getParameter(S1_KEY);
			String s2 = request.getParameter(S2_KEY);
			String s3 = request.getParameter(S3_KEY);
			String s4 = request.getParameter(S4_KEY);
			if(StringUtils.isEmpty(s4)) s4 = s3;
			logSub.setS1(s1!=null ? s1: "");
			logSub.setS2(s2!=null ? s2: "");
			logSub.setS3(s3!=null ? s3: "");
			logSub.setS4(s4!=null ? s4: "");
		}catch(Exception ex){
			ex.printStackTrace();
		}
		return logSub;
	}
	
	/**
	 * 获取RegVisitCookieLogView 基本对象，如果哪个分支不一样，自己单独设置
	 * @param request
	 * @return RegVisitCookieLogView
	 */
	public RegVisitCookieLogView getRegVisitCookieDao(HttpServletRequest request, 
			HttpServletResponse response){
		RegVisitCookieLogView logView = new RegVisitCookieLogView();
		try{
			String xnSessId =  getXnSessId(request);
			String remoteIp =  getUserIp(request);
			logView.setIp(remoteIp == null ? "" : remoteIp);
			logView.setXnSessId(xnSessId == null ? "" : xnSessId);
			String actionIdStr =  request.getParameter("action_id");
			int actionid = 0;
			if(actionIdStr!= null && !"".equals(actionIdStr)){
				try{
					actionid = Integer.parseInt(actionIdStr);
				}catch(Exception ex){
				}
			}
			logView.setActionid(actionid);
			String referer = getReferer(request);
			logView.setReferer(referer != null ? referer : "");
			String s1 = request.getParameter(S1_KEY);
			String s2 = request.getParameter(S2_KEY);
			String s3 = request.getParameter(S3_KEY);
			String s4 = request.getParameter(S4_KEY);
			if(StringUtils.isEmpty(s4)) s4 = s3;
			logView.setS1(s1!=null ? s1: "");
			logView.setS2(s2!=null ? s2: "");
			logView.setS3(s3!=null ? s3: "");
			logView.setS4(s4!=null ? s4: "");
			
			String regUrl = getRegDomain(request);
			logView.setDomainName(regUrl == null ? "" : regUrl);
			String inviterIdStr =  request.getParameter("inviterId");
			int inviter_id = 0;
			if(inviterIdStr!= null && !"".equals(inviterIdStr)){
				try{
					inviter_id = Integer.parseInt(inviterIdStr);
				}catch(Exception ex){
				}
			}
			logView.setInviterId(inviter_id);
			String reg_cookie = getRegCookie(request, response);
			logView.setReg_cookie(reg_cookie == null ? "" : reg_cookie);
			logView.setReg_flag(reg_cookie == null ? 0 : 1);
		}catch(Exception ex){
			ex.printStackTrace();
		}
		return logView;
	}
}

