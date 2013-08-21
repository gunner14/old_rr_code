package com.xiaonei.platform.core.opt.base.chain.impl.filter;

import javax.servlet.http.HttpServletRequest;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.base.chain.AbstractBizFilter;
import com.xiaonei.platform.core.opt.base.chain.BizFilterConfig;
import com.xiaonei.platform.core.opt.base.chain.BizFilterException;
import com.xiaonei.platform.core.opt.base.chain.WebContext;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 用于上传等服务的完成验票操作的Filter，这类服务通常不能正常使用cookie，会将票放置在请求参数中，
 * 而为了安全，参数中使用的是加密后的T票。
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-24 下午07:02:23
 */
public class ParamPassportValidationFilter extends AbstractBizFilter {

	/**
	 * 从request参数中取票时的参数名
	 */
	private String ticketParamName = PassportManager.loginKey_ticket;
	
	@Override
	public void init(BizFilterConfig filterConfig) {
		super.init(filterConfig);
		
		String _ticketParamName = filterConfig.getInitParameter("ticketParamName");	//可以通过初始化参数来控制存票的参数名
		if (_ticketParamName != null && _ticketParamName.trim().length() > 0) {
			setTicketParamName(_ticketParamName);
		}
	}

	@Override
	public boolean executeMe(WebContext webContext) throws BizFilterException {
		HttpServletRequest request = webContext.getRequest();
		
		//取hostid
		//先判断当前request对象里是否已经有host了，有一些特殊的应用场景，在来到这里之前已经进行过验票操作了，这里判断一下以避免重复验票
		User host = BaseThreadUtil.currentHost(request);
		Integer hostid = null;
		if (host != null) { //如果当前请求中已经有host对象了,就不走通常的取票验票过程了,这种情况极为特殊,目前只发现apps.xiaonei.com有此类用途
			hostid = host.getId();
		} else {
			String encryptTicket = request.getParameter(ticketParamName);	//从参数中取加密票
			int userId = PassportManager.getInstance().verifyEncryptedTicket(encryptTicket);	//验证加密票，取userId
			if (userId != -1) {	//-1是验证不通过
				hostid = userId;
			}
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

	public void setTicketParamName(String ticketParamName) {
		this.ticketParamName = ticketParamName;
		if (logger.isInfoEnabled()) {
			logger.info("Set ticketParamName = " + ticketParamName);
		}
	}
}
