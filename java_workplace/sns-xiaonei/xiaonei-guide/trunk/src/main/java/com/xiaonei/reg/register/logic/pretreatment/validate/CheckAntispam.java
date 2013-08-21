package com.xiaonei.reg.register.logic.pretreatment.validate;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.proxy.ProxyMethodLog;
import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.form.base.userganerate.IInvite;
import com.xiaonei.reg.register.form.base.userganerate.IName;
import com.xiaonei.reg.register.form.base.userganerate.IPwd;
import com.xiaonei.reg.register.logic.additional.LianMengLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckAntispam extends BaseRegCheckLogic {

	private static final String CHANNEL_SUCC_COOKIE = "rgsu";

	private static final CheckAntispam _INSTANCE = ProxyMethodLog
			.getLogProxy(CheckAntispam.class);

	//private static final RegisterLeagueCheckClient ANTISPAM_CHEAT = AntiSpamAdapter.getInstance().getRegisterLeagueCheckClient();

	public static CheckAntispam getInstance() {
		return _INSTANCE;
	}

	@Override
	public String check(final BaseRegForm regForm) {
		return OK;
	}

	public String checkAntispam(final BaseRegForm regForm,
			final HttpServletRequest request, final HttpServletResponse response) {
		if (null == request) {
			return ERR_REG_SYSTEM;
		}
		if (!"post".equalsIgnoreCase(request.getMethod())) {
			return ERR_REG_SYSTEM;
		}
		String email = null;
		IAccType iat = null;
		try {
			iat = (IAccType) coverInterface(regForm, IAccType.class);
		} catch (Exception e1) {
			e1.printStackTrace();
			return OK;
		}
		if (null == iat) {
			return OK;
		}
		if (iat.getAccType() == IAccountType.EMAIL) {
			try {
				email = iat.getRegEmail();
			} catch (Exception e) {
				return OK;
			}
		} else if (iat.getAccType() == IAccountType.XID) {
			try {
				email = iat.getNicknameId();
			} catch (Exception e11) {
				return OK;
			}
		} else {
			// account type 不存在
			if (StringUtils.isNotEmpty(iat.getRegEmail())) {
				// 只有邮件存在
				return new CheckRegEmail().check(regForm);
			} else if (StringUtils.isNotEmpty(iat.getNicknameId())) {
				// 只有xid存在
			    logger.info("[checkAntispam.xid] xid=" + iat.getNicknameId() + ",X-Forwarded-For::" + request.getHeader("X-Forwarded-For"));
				return new CheckNicknameId().check(regForm);
			} else {
				return OK;
			}
		}
		IInvite ii;
		int inviterId;
		try {
			ii = (IInvite) coverInterface(regForm, IInvite.class);
			inviterId = ii.getInviterId();
		} catch (Exception e) {
			inviterId = 0;
		}
		IName in;
		try {
			in = (IName) coverInterface(regForm, IName.class);
		} catch (Exception e) {
			return OK;
		}
		String name = in.getName();
		IPwd ipw;
		try {
			ipw = (IPwd) coverInterface(regForm, IPwd.class);
		} catch (Exception e) {
			return OK;
		}
		String pwd = ipw.getPwd();
		String ip;
		ip = BaseActionUtil.getClientIP(request);
		String rt = antispam(email, inviterId, name, pwd, ip);
		if (!OK.equals(rt)) {
			return rt;
		}
		return cheatCheck(request, response);
	}

	@SuppressWarnings("deprecation")
	private String antispam(String email, int inviterId, String name,
			String pwd, String ip) {
		if (StringUtils.isEmpty(email) || StringUtils.isEmpty(name)
				|| StringUtils.isEmpty(pwd) || StringUtils.isEmpty(ip)) {
			logger.info("not in antispam " + email + " " + (name) + " " + (pwd)
					+ " " + (ip));
			return OK;
		}
		logger.info(email + " " + (name) + " " + (pwd) + " " + (ip));
		AntiSpamHome ash = AntiSpamHome.getInstance();
		int canRegStatus;
		if (StringFormat.isValidMobile(email) == false) {
			logger.debug("in antispam " + inviterId + " " + email + " "
					+ (name) + " " + (pwd) + " " + (ip));
			try {
				inviterId = inviterId > 0 ? inviterId : -1;
				canRegStatus = ash
						.canRegister1(inviterId, name, email, pwd, ip);
				logger.info("antispam over " + inviterId + " " + email + " "
						+ (name) + " " + (pwd) + " " + (ip) + " "
						+ canRegStatus);
			} catch (Exception e) {
				e.printStackTrace();
				logger.debug("antispam exception " + inviterId + " " + email
						+ " " + (name) + " " + (pwd) + " " + (ip));
				canRegStatus = CheckResult.REGISTER_SAFE;
			}
		} else {
			logger.debug("antispam mobile " + inviterId + " " + email + " "
					+ (name) + " " + (pwd) + " " + (ip));
			canRegStatus = CheckResult.REGISTER_SAFE;
		}
		if (CheckResult.REGISTER_SPAM == canRegStatus) {
			return ERR_FILTER_MAJIA;
		} else if (CheckResult.REGISTER_SPAM_IP == canRegStatus) {
			return ERR_FILTER_MAJIA_SAMEIP;
		}
		return OK;
	}

	private String cheatCheck(HttpServletRequest request,
			HttpServletResponse response) {
		if (!LianMengLogic.isHaoyeLink(request)) {
			return OK;
		}
		String rt = ipCheck(request);
		if (isNotOk(rt)) {
			return rt;
		}
		return cookieCheck(request, response);
	}

	private String ipCheck(HttpServletRequest request) {

		//String ip = BaseActionUtil.getClientIP(request);
		boolean ipcheck = true;//ANTISPAM_CHEAT.ipCheck(ip);
		if (!ipcheck) {
			logger.info("spamer !");
			return ERR_FILTER_MAJIA;
		}
		return OK;
	}

	private String cookieCheck(HttpServletRequest request,
			HttpServletResponse response) {
		String s = CookieManager.getInstance().getCookie(request,
				CHANNEL_SUCC_COOKIE);
		logger
				.debug("[Inshion]RegCheckLogic::checkCookieForAntiCheat() --start");
		if (s == null || "null".equalsIgnoreCase(s)) {
			logger.debug("[Inshion]cookie for rgsu is null");
			setCookieForAntiCheat(request, response);
		} else {
			return ERR_FILTER_MAJIA;
		}
		return OK;
	}

	// 注册提交，写cookie
	public static void setCookieForAntiCheat(HttpServletRequest request,
			HttpServletResponse response) {
		String v = request.getParameter("ss");
		if (v == null) {
			v = "DN";
		}
		Cookie ck = new Cookie(CHANNEL_SUCC_COOKIE, v);
		ck.setDomain("." + Globals.domain);
		ck.setMaxAge(60 * 60 * 24);
		response.addCookie(ck);
		return;
	}

}
