package com.xiaonei.login.client.base;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.action.ActionMessages;
import org.springframework.mock.web.MockHttpServletRequest;

import com.xiaonei.aegis.login.client.CheckLoginClient;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.logic.LoginLogicFactory;
import com.xiaonei.login.task.set.LogTaskSet;
import com.xiaonei.login.util.StatUtil;
import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.core.opt.base.chain.impl.BizFilterUtil;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.platform.core.utility.RegularExpression;
import com.xiaonei.wservice.userscore.login.LoginType;
import com.xiaonei.xce.passport.Account;
import com.xiaonei.xce.passport.Login;
import com.xiaonei.xce.passport.PassportAdapterFactory;

/**
 * 这是一个Struts结构模板Action，便于其他项目继承实现各自特殊的登录逻辑。<br>
 * 改类实现了核心的登录逻辑，这些核心逻辑为：<br>
 * 1.用户名和密码的输入验证<br>
 * 判断是否提交了这两个参数<br>
 * 2.用户ip地址验证<br>
 * 判断用户ip是否被封禁<br>
 * 3.用户名和密码正确性验证<br>
 * 判断用户名和密码是否和数据库记录的匹配<br>
 * 4.用户当前状态验证<br>
 * 判断用户帐号是否被封禁／注销<br>
 * 5.记录用户最后登录时间和登录ip<br>
 * 
 * 以上的每步验证都可能不通过，此时核心逻辑会调用fail方法，<br>
 * 你可以override fail方法来做自己的错误处理，<br>
 * fail方法提供了failCode参数，你可以通过LoginUtil.getErrorMessage(int failCode)来获取对应的错误信息。<br>
 * 以上的核心逻辑在任何登录入口都需要执行，<br>
 * 如果还有特殊的逻辑需要处理则override beforeLogin()和afterLogin()方法<br>
 * beforeLogin会在核心逻辑执行之前被调用，<br>
 * 你可以在这里做自己的输入校验和其他的准备工作，<br>
 * 如果你想在beforeLogin后让整个执行逻辑结束，你可以调用setNextStep()方法<br>
 * afterLogin则在核心逻辑走完后被调用，<br>
 * 你可以在这里做后继处理，<br>
 * 比如种自己的cookie等，如果你想在afterLogin后就结束执行逻辑，你可以调用setNextStep()方法<br>
 * 
 * 如果你的登录逻辑大部分跟web的流程类似，你最好直接继承LoginAction类。<br>
 * 
 * @author zhangjie
 */
public abstract class AbstractStructAction extends LoginAction {

	private static class LogTaskBean {

		public byte loginType;

		private String ip = "";

		private int type;

		private LogTaskBean(String ip, int type, byte loginType) {
			this.ip = ip;
			this.type = type;
			this.loginType = loginType;
		}

	}

	protected Log logger = LogFactory.getLog(this.getClass());
	
	public static List<Login.Type> defaultLogin = new ArrayList<Login.Type>();

	protected static ThreadPoolExecutor executor = new ThreadPoolExecutor(5, 5,
			0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<Runnable>());

	static {
		defaultLogin.add(Login.Type.Account);
		defaultLogin.add(Login.Type.Phone);
		defaultLogin.add(Login.Type.BindedId);
	}

	public static void main(String[] args) {
		AbstractStructAction ac = new AbstractStructAction() {

			@Override
			protected ActionForward afterLogin(ActionMapping mapping,
					ActionForm form, HttpServletRequest request,
					HttpServletResponse response) throws Exception {
				return null;
			}

			@Override
			protected ActionForward beforeLogin(ActionMapping mapping,
					ActionForm form, HttpServletRequest request,
					HttpServletResponse response) throws Exception {
				return null;
			}

			@Override
			protected ActionForward fail(ActionMapping mapping,
					ActionForm form, HttpServletRequest request,
					HttpServletResponse response, int failCode)
					throws Exception {
				return null;
			}

			@Override
			protected String getIdentify() {
				return null;
			}
		};
		ac.setLogTaskBean("1.1.1.1", 3, LoginType.TYPE_CONNET);
		Account account = new Account(1, 1, "account", "", 0, "111");
		MockHttpServletRequest request = new MockHttpServletRequest();
		System.out.println(ac.getLogTask(account, "2.2.2.2", request));
	}

	private ThreadLocal<LogTaskBean> logTaskBean = new ThreadLocal<LogTaskBean>();

	protected final boolean useOldPassport;

	/**
	 * 默认优先使用已经存在的票(即不踢票)
	 */
	public AbstractStructAction() {
		this.useOldPassport = true;
	}

	/**
	 * 构造函数
	 * 
	 * @param useOldPassport
	 *            是否优先沿用旧票(Web实现需要)
	 */
	public AbstractStructAction(boolean useOldPassport) {
		this.useOldPassport = useOldPassport;
	}

	@Override
	public ActionForward execute(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		StatUtil su = new StatUtil(_Tstat);
		String domain = LoginLogicFactory.getDomainOpr().getDomainParam(
				request.getParameter(LoginParamDef.PARAM_DOMAIN), request);

		try {

			ActionForward beforeForward = beforeLogin(mapping, form, request,
					response);
			su.stepTimeCost("1:AbstractStructAction:beforeLogin");
			if (!hasNextStep(request)) {
				su.getWholeTime("LoginGetFinish", getActionName());
				return beforeForward;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		try {
			// 设置最终跳转页面地址(这个很重要)

			// 输入验证
			boolean passwordEncode = request.getAttribute("enpassword") != null ? true
					: false;

			String userName = (String) request.getAttribute("email");
			if (userName == null) {
				su.getWholeTime("LoginInputWrongFailFinish", getActionName());
				return fail(mapping, form, request, response,
						LoginStatusCode.FAILINPUT.getCode());
			}
			userName = Escape.trim(userName);
			// 统一用户名为小写
			userName = RegularExpression.processForLogin(userName)
					.toLowerCase();
			String password = passwordEncode ? request.getAttribute(
					"enpassword").toString() : request.getAttribute("password")
					.toString();
			String autoLogin = request.getParameter(PARAM_AUTOLOGIN);
			boolean isPer = autoLogin != null
					&& Boolean.parseBoolean(autoLogin);
			if (userName == null || password == null || userName.length() == 0
					|| password.length() == 0) {
				su.getWholeTime("LoginInputWrongFailFinish", getActionName());
				return fail(mapping, form, request, response,
						LoginStatusCode.FAILINPUT.getCode());
			}

			// 地址封禁过滤
			String ip = BizFilterUtil.getClientIP(request);
			boolean isBan = LoginLogicFactory.getValidation().isBanIP(ip);
			su.stepTimeCost("2:AbstractStructAction:isBanIP");

			if (isBan) {
				_log.error("LoginIpBanFailFinish" + ip);
				su.getWholeTime("LoginIpBanFailFinish", getActionName());
				return fail(mapping, form, request, response,
						LoginStatusCode.FAILBANIP.getCode());
			}
			// 根据用户名和密码获取用户对象
			// 超找串号原因的代码
			domain = (String) request.getAttribute(PARAM_DOMAIN);
			Account account = PassportAdapterFactory.getWebLogin().login(
					defaultLogin, userName,
					passwordEncode ? password : Encoder.encode(password),
					domain, isPer ? -1 : 6 * 3600, useOldPassport);
			su.stepTimeCost("3:AbstractStructAction:login",
					account != null ? account.getId() : null);

			// 安全登录检查
			try {
				// 默认开启Debug时关闭
				if (!_log.isDebugEnabled()) {
					if (account != null && account.getSuccess() != -1) {
						CheckLoginClient.getInstance().loginCheckTest(
								account.getId(), userName, password, ip,
								account.getSuccess() == 0,
								System.currentTimeMillis(), request);
					} else {
						// 用户不存在的情况
						CheckLoginClient.getInstance().loginCheckTest(-1,
								userName, password, ip, false,
								System.currentTimeMillis(), request);
					}
					su.stepTimeCost("4:AbstractStructAction:checklogin");
				}
			} catch (Exception e) {
				_log.error(e.getMessage());
			}

			if (account == null) {
				return fail(mapping, form, request, response,
						LoginStatusCode.FAILUNKNOWN.getCode());
			}

			if (account.getId() <= 0 || account.getSuccess() < 0) {
				if (account.getId() <= 0 || account.getSuccess() == -1) {
					// 输入用户不存在
					_log.error(getLoginType(request) + "Invalid User Name:"
							+ userName);
					logger.warn(getLoginType(request)
							+ "Invalid User Name:" + userName);
					su.getWholeTime("LoginUserNotExistFailFinish",
							getActionName());
					return fail(mapping, form, request, response,
							LoginStatusCode.FAILUSERNOTEXIST.getCode());
				} else if (account.getSuccess() == -2) {
					// 输入密码错误
					if (_TwrongPW.isInfoEnabled()) {
						_TwrongPW.info(getLoginType(request)
								+ "Wrong Password Login:" + userName);
					}
					su.getWholeTime("LoginPasswordWrongFailFinish",
							getActionName());
					return fail(mapping, form, request, response,
							LoginStatusCode.FAILUSER.getCode());
				}
			}

			request.setAttribute(LoginAction.USER_ID, account.getId());

			// 对当前登录用户状态检验
			LoginStatusCode status = LoginLogicFactory.getValidation()
					.checkUserStatus(account);
			if (status != LoginStatusCode.LOGIN_OK) {
				su.getWholeTime("LoginStatusFailFinish", getActionName());
				return fail(mapping, form, request, response, status.getCode());
			}

			// 判断用户是否有交叉登录的情况发生
			if (!account.getDomain().equalsIgnoreCase(domain)) {
				request.setAttribute(LOGIN_CROSS, Boolean.toString(true));
			}

			// 登录成功
			if (isPer) {
				// 保存一个月的
				LoginLogicFactory.getPassportTicketOpr().savePassport2Cookie(
						account.getId(), request, response,
						account.getTicket(), true);
			} else {
				// 保存session域
				LoginLogicFactory.getPassportTicketOpr().savePassport2Cookie(
						account.getId(), request, response,
						account.getTicket(), false);
			}
			su.stepTimeCost("6:AbstractStructAction:saveCookie");

			// 后继任务
			Runnable logTask = getLogTask(account, ip, request);

			executor.submit(logTask);
		} catch (Exception e) {
			e.printStackTrace();
			su.getWholeTime("LoginUnknownFailFinish", getActionName());
			return fail(mapping, form, request, response,
					LoginStatusCode.FAILUNKNOWN.getCode());
		}
		try {
			ActionForward afterForward = afterLogin(mapping, form, request,
					response);
			su.stepTimeCost("7:AbstractStructAction:afterLogin");
			su.getWholeTime("LoginSuccFinish", getActionName());
			// 全部逻辑走完 扫尾
			if (!hasNextStep(request)) {
				// 额外的跳转
				return afterForward;
			}
		} catch (Exception e) {
			e.printStackTrace();
			if (!hasNextStep(request)) {
				// 出现异常时
				su.getWholeTime("LoginUnknownFailFinish", getActionName());
				return fail(mapping, form, request, response,
						LoginStatusCode.FAILUNKNOWN.getCode());
			}
		}
		su.getWholeTime("LoginUnknownFailFinish", getActionName());
		return mapping.findForward("fail");
	}

	public String getActionName() {
		return "AbstractStructAction";
	}

	private String getLoginType(HttpServletRequest request) {
		String pre = (String) request.getAttribute(LoginAction.LOGINTYPE_PRE);
		if (pre == null) {
			return "";
		}
		return pre;
	}

	private LogTaskSet getLogTask(Account account, String ip,
			HttpServletRequest request) {
		LogTaskSet logTask;
		if (logTaskBean.get() == null) {
			logTask = new LogTaskSet(account.getId(), ip, request.getRequestURL()
					.toString());
		} else {
			logTask = new LogTaskSet(account.getId(), logTaskBean.get().ip,
					logTaskBean.get().type, request.getRequestURL().toString(),
					logTaskBean.get().loginType);
		}
		return logTask;
	}

	// 添加错误提示信息
	protected void addErrorMessage(HttpServletRequest request, String message) {
		ActionMessages ams = new ActionMessages();
		ActionMessage am = new ActionMessage("errors.detail", message);
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		request.setAttribute(org.apache.struts.Globals.ERROR_KEY, ams);
	}

	// 添加警告提示信息
	protected void addWarningMessage(HttpServletRequest request, String message) {
		ActionMessages ams = new ActionMessages();
		ActionMessage am = new ActionMessage("errors.detail", message);
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		request.setAttribute(org.apache.struts.Globals.MESSAGE_KEY, ams);
	}

	abstract protected ActionForward afterLogin(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception;

	abstract protected ActionForward beforeLogin(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception;

	abstract protected ActionForward fail(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response, int failCode) throws Exception;

	/**
	 * 为每个Action增加一个返回标识的方法
	 */
	protected abstract String getIdentify();

	// OPTIMIZE:对于 origURL 的处理还要考虑到是否是非法 如转向到站外地址等情况
	protected String getRedirectUrl(HttpServletRequest request) {
		String homeURL = "http://home." + request.getAttribute(PARAM_DOMAIN)
				+ "/Home.do";
		String origURL = request.getParameter(PARAM_ORIGURL);
		origURL = origURL != null && origURL.length() > 0 ? origURL : homeURL;
		if (origURL.contains(OpiConstants.urlLogin + "/GLogin.do")
		// 暂时删了 || origURL.contains("SysHome.do") // 不跳回主页
				|| origURL.contains(OpiConstants.urlPassport + "/PLogin.do")) {
			origURL = homeURL;
		}

		return origURL;
	}

	/**
	 * 判断是否有下一步的执行逻辑 每次都会清除上次的状态
	 * 
	 * @param request
	 * @return
	 */
	protected boolean hasNextStep(HttpServletRequest request) {
		boolean next = request.getAttribute("nextstep") == null
				|| (Boolean) request.getAttribute("nextstep");
		request.setAttribute("nextstep", null);
		return next;
	}

	/**
	 * 设置目标domain
	 */
	protected void setDomain(HttpServletRequest request, String domain) {
		request.setAttribute(LoginAction.PARAM_DOMAIN, domain);
	}

	protected void setLogTaskBean(String ip, int logType, byte loginType) {
		ip = null == ip ? "" : ip;
		logTaskBean.set(new LogTaskBean(ip, logType, loginType));
	}

	/**
	 * setNextStep方法提供给你结束执行逻辑的控制能力，如果想让执行逻辑在模板方法中返回，则设置一个false即可
	 * 
	 * @param request
	 * @param flag
	 */
	protected void setNextStep(HttpServletRequest request, Boolean flag) {
		request.setAttribute("nextstep", flag);
	}

	/**
	 * 把明文密码设置到处理的上下文中
	 * 
	 * @param request
	 * @param password
	 */
	protected void setPassword(HttpServletRequest request, String password) {
		setPassword(request, password,
				request.getParameter(PARAM_ENPWD) != null);
	}

	/**
	 * 把密码设置到处理的上下文中，如果密码已经被加密(md5)，则encode参数设置为true
	 * 
	 * @param request
	 * @param password
	 * @param encode
	 *            密码是明文还是密文，true代表密文，false代表明文
	 */
	protected void setPassword(HttpServletRequest request, String password,
			boolean encode) {
		if (encode)
			request.setAttribute("enpassword", password);
		request.setAttribute("password", password);
	}

	/**
	 * 设置用户名
	 * 
	 * @param request
	 * @param userName
	 */
	protected void setUserName(HttpServletRequest request, String userName) {
		request.setAttribute("email", userName);
	}

}
