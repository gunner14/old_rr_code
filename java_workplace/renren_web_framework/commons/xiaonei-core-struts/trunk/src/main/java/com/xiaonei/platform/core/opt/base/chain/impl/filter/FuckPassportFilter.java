package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterConfig;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

public class FuckPassportFilter extends AbstractBizFilter {
	
	/**
	 * 是否从request参数中取票
	 */
	private boolean ticketFromParam = false;
	

	/**
	 * 从request参数中取票时的参数名
	 */
	private String ticketParamName = PassportManager.loginKey_ticket;
	
	private String mticketParamName = PassportManager.loginKey_mticket;
	
	/**
	 * 从cookie中验票时，存放票的cookie的name
	 */
	private String ticketCookieName = PassportManager.loginKey_ticket;
	
	@Override
	public void init(BizFilterConfig filterConfig) {
		super.init(filterConfig);
		String s_ticketFromParam = filterConfig.getInitParameter("ticketFromParam");
		if (s_ticketFromParam != null && s_ticketFromParam.equals("true")) {	//可以通过初始化参数来控制是否从参数中取票
			setTicketFromParam(true);
			String _ticketParamName = filterConfig.getInitParameter("ticketParamName");	//可以通过初始化参数来控制存票的参数名
			if (_ticketParamName != null && _ticketParamName.trim().length() > 0) {
				setTicketParamName(_ticketParamName);
			}
		}
		
		String _ticketCookieName = filterConfig.getInitParameter("ticketCookieName");
		if (_ticketCookieName != null && _ticketCookieName.trim().length() > 0) {
			setTicketCookieName(_ticketCookieName);
		}
	}

	@Override
	public boolean executeMe(WebContext webContext) throws BizFilterException {
		HttpServletRequest request = webContext.getRequest();
		HttpServletResponse response = webContext.getResponse();

		//设置当前请求URL到request对象中，有些业务可能会用到这个URL来完成返回操作
		StringBuffer uri = request.getRequestURL();
		if (uri.indexOf("/Login.do") < 0) { //排除login操作的url
			String queryString = request.getQueryString();
			if (queryString != null && queryString.length() > 0) {
				uri.append("?");
				uri.append(BizFilterUtil.urlSecurityEscape(queryString));
			}
			request.setAttribute("origURL", uri.toString()); //老版的action在request对象中放置的origURL是一个String对象，这里toString以便保证兼容性
		}
		
		//为WebPager添加一个cookie来冒充session id
		BizFilterUtil.addCookieForIM(request, response);
		
		//取hostid
		//先判断当前request对象里是否已经有host了，有一些特殊的应用场景，在来到这里之前已经进行过验票操作了，这里判断一下以避免重复验票
		User host = BaseThreadUtil.currentHost(request);
		Integer hostid = null;
		if (host != null) { //如果当前请求中已经有host对象了,就不走通常的取票验票过程了,这种情况极为特殊,目前只发现apps.xiaonei.com有此类用途
			hostid = host.getId();
		} else { //取票验票操作，虽然方法名叫getHostIdFromCookie，但是实际上会和中间层交互
			String ticket = null;
			//从cookie取票
			hostid = PassportManager.getInstance().getHostIdFromCookie(request, response, ticketCookieName);
			if (hostid != null) {	//验票成功，得到这个票是什么
				ticket = CookieManager.getInstance().getCookie(request, ticketCookieName);
			}
			
			if (hostid == null && ticketFromParam) {
				//从request参数中取票
				hostid = PassportManager.getInstance().getHostIdFromRequestParam(request,PassportManager.loginKey_mticket);
				if (hostid != null) {	//验票成功，得到这个票是什么
					ticket = request.getParameter(ticketParamName);
				}
			}
			
			BaseThreadUtil.setTicket(request, ticket);	//把票放在request中，以后会有用
		}
		BizFilterUtil.trackActivity(hostid, request);	//用户行为追踪
		
		if (hostid != null && host == null) { //取User对象，同样判断一下，如果host不为null就不重复取了
			host = WUserAdapter.getInstance().get(hostid);
		}
		if (host == null) { //host==null，则认为此用户未登录，需要登录
			throw new BizFilterException(BizFilterException.CODE_NEED_TO_LOGIN);
		}
		if (host != null && host.getStatus() >= User.statusActivateVerify) { //属于被封禁的用户
			throw new BizFilterException(BizFilterException.CODE_BAD_USER_STATUS);
		}
		
		BaseThreadUtil.setHost(request, host);
		
		//设置两个属性，名字不同值相同，属于历史遗留问题
		request.setAttribute("host", host);
		request.setAttribute("visiter", host);

		return false;
	}

	public void setTicketFromParam(boolean ticketFromParam) {
		this.ticketFromParam = ticketFromParam;
		if (logger.isInfoEnabled()) {
			logger.info("Set verifying ticket from request parameter.");
		}
	}

	public void setTicketParamName(String ticketParamName) {
		this.ticketParamName = ticketParamName;
		if (logger.isInfoEnabled()) {
			logger.info("Set ticketParamName = " + ticketParamName);
		}
	}

	public void setTicketCookieName(String ticketCookieName) {
		this.ticketCookieName = ticketCookieName;
		if (logger.isInfoEnabled()) {
			logger.info("Set ticketCookieName = " + ticketCookieName);
		}
	}
}
