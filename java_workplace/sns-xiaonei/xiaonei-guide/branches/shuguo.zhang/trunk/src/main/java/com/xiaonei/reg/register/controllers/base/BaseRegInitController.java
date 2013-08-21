package com.xiaonei.reg.register.controllers.base;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.AsSuperController;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.login.logic.LogRegLogic;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.register.controllers.init.Xn6401Controller;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.logic.additional.RegChannelActivityLogic;
import com.xiaonei.reg.register.logic.pretreatment.InviteUrlParasLogic;
import com.xiaonei.reg.usertrace.dao.InviteUrlParamsDAO;
import com.xiaonei.reg.usertrace.model.RegOprDefine;
import com.xiaonei.reg.usertrace.q.UTUtils;

/**
 * BaseRegInitAction
 * <p/>
 * 注册初始化页面继承之
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-3 - 下午05:52:24
 */
@AsSuperController
public abstract class BaseRegInitController extends BaseRegController {

	@Override
	final public RegForward executeRt(RegMapping mapping, RegForm form,
			HttpServletRequest request, HttpServletResponse response) {
		User user = BaseThreadUtil.currentHost(request);
		/* 登录的用户自动登录 */
		if (null != user) {
			try {
				return new RegForward(Globals.urlWww + "/Home.do", true);
			} catch (Exception e) {
				e.printStackTrace();
			}
			return null;
		}

		this.getPageId(request);
		BaseRegForm regForm = (BaseRegForm) form;
		/* 保存来源 */
		if (regForm == null || regForm.getFromUrl() == null
				|| "".equals(regForm.getFromUrl())) {
			// 第一次的初始化
			regForm = getRegForm();
			// 拼装来源url
			String fu = request.getParameter("f");
			if (StringUtils.isNotEmpty(fu)) {
				regForm.setFromUrl(fu);
			}else{
				regForm.setFromUrl("none");
			}
			IAccType ie = (IAccType) regForm;
			String email = StringUtils.trimToEmpty(request.getParameter("email"));
			if(StringUtils.isNotEmpty(email)){
				ie.setRegEmail(email.toLowerCase());
				request.setAttribute("regEmail", email);
			}
			regForm.setFrom(StringUtils.trimToEmpty(regForm.getFrom()));
		}
		/* 渠道用，from 表明从哪条渠道过来的注册 */
		// 激活时读出，返回给相应渠道信息
		String from = StringUtils.trimToNull(request.getParameter("from"));
		String ayid = StringUtils.trimToNull(request.getParameter("ayid"));
		// 处理登录失败的情况
		RegForward lf = this.dealLoginFail(mapping, regForm, request, response);
		if(lf!=null){
		    return lf;
		}
		String ss = getSs(request);
		String inviterId = getInviteId(request);
		RegChannelActivityLogic.saveChannelInfoToCookie(inviterId, from, ayid,
				ss, response);
		/* 用户跟踪日志增加 */
		String actionStr = getInitActionId(request);
		String g = getG(request);
		String b = getB(request);
		userTraceLog.insertVisitHisFirst(request, actionStr, ss, null, g, b, 0);
		/* 跳转 */
		regForm.setG(g);
		regForm.setSs(ss);
		regForm.setB(b);
		regForm.setAction_id(getSbActionId(request.getRequestURI(), request));
		request.setAttribute(getRegFormName(), regForm);
		/* app connect 注册比较特殊，所以暂时单独设置下吧 */
		String appId = getAppId(request);
		request.setAttribute("appId", appId);
		
		RegForward rt = executeRIt(mapping, regForm, request, response);
		return rt;
	}

	private String getInviteId(HttpServletRequest request) {
		if (StringUtils.isNotEmpty(request.getParameter("i"))) {
			return request.getParameter("i");
		}

		if (StringUtils.isNotEmpty(request.getParameter("iid"))) {
			Map<String, String> m;
			try {
				m = InviteUrlParasLogic.getInstance().getParas(request);
			} catch (Exception e) {
				return null;
			}
			return StringUtils.trimToNull(m.get("i"));
		}
		return null;
	}

	/**
	 * 获得b值
	 * 
	 * @param request
	 * @return
	 */
	private String getB(HttpServletRequest request) {
	    String b = "";
	    b = this.doLoginFail(b, "b");
	    if(StringUtils.isNotEmpty(b)){
	        return b;
	    }
		return StringUtils.isEmpty(request.getParameter("b")) ? request
				.getParameter("g") : request.getParameter("b");
	}

	/**
	 * 获得g值
	 * 
	 * @param request
	 * @return
	 */
	private String getG(HttpServletRequest request) {
	    String g = "";
        g = this.doLoginFail(g, "g");
        if(StringUtils.isNotEmpty(g)){
            return g;
        }        
		return StringUtils.trimToEmpty(request.getParameter("g"));
	}

	/**
	 * get appId
	 * @param request
	 * @return
	 * @author wei.cheng@opi-corp.com
	 * @version 1.0
	 * @date 创建时间：Apr 20, 2010 10:06:00 AM
	 */
	private String getAppId(HttpServletRequest request){
	    return StringUtils.isEmpty(StringUtils.trimToEmpty(request.getParameter("app_id")))?
	                StringUtils.trimToEmpty((String)request.getAttribute("appId")) :
	                    StringUtils.trimToEmpty(request.getParameter("app_id"));
	}
	/**
	 * 子类实现具体流程
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 */
	public abstract RegForward executeRIt(final RegMapping mapping,
			final RegForm form, final HttpServletRequest request,
			final HttpServletResponse response);

	/**
	 * 页面上的form id
	 * 
	 * @return
	 */
	protected String getRegFormName() {
		return "regform";
	}

	/**
	 * init 对应的 action form
	 * 
	 * @return
	 */
	public abstract BaseRegForm getRegForm();

	/**
	 * 回写action id
	 * 
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-21 - 下午04:55:21
	 */
	protected String getActionId() {
		try {
			return "20"
					+ this.getClass().getName().split("Xn")[1]
							.split("Controller")[0];
		} catch (Exception e) {
			System.err.println("get action id err : "
					+ this.getClass().getName());
			return null;
		}
	}

	/**
	 * 4位pageid
	 * 
	 * @param request
	 * @return
	 */
	private String getPageId(HttpServletRequest request) {
		String pageId = "";
		String actionId = request.getParameter("action_id");
		if (StringUtils.isNotEmpty(actionId)) {
			pageId = (StringUtils.substring(actionId, 2));
		} else if (StringUtils.isNotEmpty(getActionId())) {
			pageId = (StringUtils.substring(getActionId(), 2));
		}
		return pageId;
	}

	/**
	 * 获得init actionid
	 * 
	 * @param requestURI
	 * @param actionId
	 * @param request
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-3 - 下午05:18:39
	 */
	private String getInitActionId(HttpServletRequest request) {
		if (!StringUtils.isEmpty(getPageId(request))) {
			return "10" + getPageId(request);
		}
		String actionId = request.getParameter("action_id");
		// 如果页面上有参数传递
		if (!StringUtils.isEmpty(actionId)) {
			return actionId;
		}
		String urlp = "^/xn[0-9]{4}\\.do$";
		String requestURI = request.getRequestURI();
		if (Pattern.compile(urlp).matcher(requestURI).matches() == false) {
			return String.valueOf(RegOprDefine.NORMAL_VISIT_XN_REG);
		}
		String[] pa = requestURI.split("/");
		String path = pa[pa.length - 1].split("\\.")[0];
		return path.replaceAll("xn", "10");
	}

	/**
	 * 获得actionid submit
	 * 
	 * @param requestURI
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-3 - 下午09:46:42
	 */
	private String getSbActionId(String requestURI, HttpServletRequest request) {
		if (!StringUtils.isEmpty(getPageId(request))) {
			return "20" + getPageId(request);
		}
		String urlp = "^/xn[0-9]{4}\\.do$";
		if (Pattern.compile(urlp).matcher(requestURI).matches() == false) {
			return String.valueOf(RegOprDefine.DEFAULT_XN_REG);
		}
		String[] pa = requestURI.split("/");
		String path = pa[pa.length - 1].split("\\.")[0];
		return path.replaceAll("xn", "20");
	}

	/**
	 * 获得ss
	 * 
	 * @param _ss
	 * @param iid
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-21 - 下午04:16:33
	 */
	private String getSs(HttpServletRequest request) {
		String _ss = request.getParameter("ss");
		String iid = request.getParameter("iid");
		String ss = _ss;
		String uuid = iid;
		//搜索注册页面的统计需求,xn5223
		ss = dealSs(request, ss);
		ss = doLoginFail(ss, "ss");
		if (ss != null && !"".equals(ss)) {
			return ss;
		}
		// 取不到，重新从数据库中获取
		if (uuid != null && !"".equals(uuid)) {
			try {
				ss = InviteUrlParamsDAO.getInstance().getSubSourceByUuid(uuid);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return ss != null ? ss : "";
	}

	/**
	 * login failed flow, reset ss, b and g params 
	 * @param retStr   return
	 * @param keyStr   need to find
	 * @return
	 */
    private String doLoginFail(String retStr, String keyStr) {
        if(this.loginFailkHm != null && this.loginFailkHm.size() >0){
		    String _tmpStr = (String)this.loginFailkHm.get(keyStr);
		    if(StringUtils.isNotEmpty(_tmpStr)){
		        retStr = _tmpStr;
		    }
		}
        return retStr;
    }

    /**
     * find person flow add , need to check referer message
     * @param request
     * @param ss
     * @return
     */
    private String dealSs(HttpServletRequest request, String ss) {
        String referer = request == null ? "" : request
                .getHeader("Referer");
		if(StringUtils.contains(referer, "http://browse.renren.com/searchEx.do?ss=17038")){
		    ss = "17038";
		}
        return ss;
    }

    /**
     * login failed flow, reset form init value 
     * @param mapping
     * @param form
     * @param request
     * @param response
     * @return
     */
    private RegForward dealLoginFail(final RegMapping mapping,
            final RegForm form, final HttpServletRequest request,
            final HttpServletResponse response){
        if(isFailedFlow(request)){
            RegShortCommonForm regForm = (RegShortCommonForm) form;
            //new work flow
            String ss = "", bparas = "", remail = "", loginType = "";
            String xnSessId =  getXnSessId(request);
            loginType = request.getParameter("loginType");
            String current_key =  MEM_CACHE_NEW_LOGINFAIL_KEY + (xnSessId == null ? "" : xnSessId);
            try {
                current_key = URLEncoder.encode(current_key, "UTF-8");
            } catch (UnsupportedEncodingException e1) {
                current_key = "invalid_key_failed";
            }
            MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
            Object obj = mem.get(current_key);
            log.info("----[weige]---- key=" + current_key + ",value=" + obj);
            if(obj != null){
                //reset cache data
                mem.set(current_key, obj, (int)new Date(MemCacheKeys.minute * 15).getTime());
                Map<?, ?> ap = (Map<?, ?>)obj;
                this.printMap(ap, log);
                if(ap != null){
                    loginFailkHm = new HashMap<String, Object>();
                    try {
                        remail = (String)ap.get("email");                           
                        regForm.setRegEmail(remail);
                        request.setAttribute("email", remail);
                        
                        String rpassword = (String)ap.get("password");
                        regForm.setPwd(rpassword);
                        request.setAttribute("password", rpassword);
                        
                        String rfromUrl = (String)ap.get("fromUrl");
                        if(rfromUrl != null && !"".equals(rfromUrl)){
                            regForm.setFromUrl(rfromUrl);
                            request.setAttribute("fromUrl", rfromUrl);
                        }
                        String rss = (String)ap.get("ss");
                        if(StringUtils.isEmpty(rss) || "null".equals(rss)){
                            //设置默认值的情况
                            ss = DEFAULT_SS;
                        }else{
                            ss = rss;                            
                        }
                        log.debug("ss=" + ss + ", loginType=" + loginType);
                        regForm.setSs(ss);
                        request.setAttribute("ss", ss);
                        
                        String riid = (String)ap.get("iid");
                        regForm.setUuid(riid);
                        request.setAttribute("uuid", riid);
                        
                        bparas = (String)ap.get("b");
                        request.setAttribute("b", bparas);
                        String rg = (String)ap.get("g");
                        request.setAttribute("g", rg);                    
                        
                        loginFailkHm.put("ss", ss);
                        loginFailkHm.put("b", bparas);
                        loginFailkHm.put("g", rg);
                        
                        String rfrom = (String)ap.get("from");
                        regForm.setFrom(rfrom);
                        request.setAttribute("from", rfrom);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
            log.debug("----[weige]---- regemail is " + remail + ",ip=" + getFromIp(request));
            if(remail == null || "".equals(remail) || !LogRegLogic.getInstance().checkEmailDomain(remail.toLowerCase())){
                //不在列表中的邮箱
                try {
                    return new RegForward("http://passport.renren.com/PLogin.do?domain=renren.com", true);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        return null;
    }
    
    private boolean isFailedFlow(HttpServletRequest request){
        if(StringUtils.isNotEmpty(this.getXnSessId(request)) 
                && "6401".equals(this.getClass().getName().split("Xn")[1].split("Controller")[0])){
            return true;
        }
        return false;
    }
    
    public static final String MEM_CACHE_NEW_LOGINFAIL_KEY = "loginfail_usernofound_";
    public RegLogger log = RegLogger.getLoggerWithType(Xn6401Controller.class.getSimpleName(),UTUtils.authorEmail);
    private static final String DEFAULT_SS = "10125";
    private HashMap<String, Object> loginFailkHm = null;
    
    private String getXnSessId(HttpServletRequest request){
        String fp = request.getParameter("fr");
        request.setAttribute("fpara", fp);
        return fp;
    }
    
    private void printMap(Map<?, ?> map, RegLogger log){
        for(Map.Entry<?, ?> ent : map.entrySet()){
            log.debug("----[weige]---- key=" + ent.getKey() + ", value = " + ent.getValue());
        }
    }
    
	@Override
	protected Map<String, RegForward> setRegMapping(Map<String, RegForward> map) {
		map.put("success", new RegForward("/pages/register/reg/reg_"
				+ StringUtils.substring(getActionId(), 2) + ".jsp"));
		map.put("initcommonreg", new RegForward("/init/xn6205"));
		return map;
	}
}
