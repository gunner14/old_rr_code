package com.xiaonei.reg.register.logic.pretreatment.validate;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.platform.component.mobile.logic.MobileLogic;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.proxy.ProxyMethodLog;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckRegEmail extends BaseRegCheckLogic {

	private static final CheckRegEmail proxyInstance = ProxyMethodLog
			.getLogProxy(CheckRegEmail.class);

	public static CheckRegEmail getProxy() {
		return proxyInstance;
	}

	@Override
	public String check(BaseRegForm regForm) {
		IAccType ir;
		try {
			ir = (IAccType) coverInterface(regForm, IAccType.class);
		} catch (Exception e) {
			e.printStackTrace();
			return OK;
		}
		return check(ir);
	}

	/**
	 * @param ir
	 * @param stage
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-4-7 - 下午01:11:15
	 */
	private String check(IAccType ir) {
		if (ir.getAccType() == IAccountType.MOBILE) {
			return checkEmailMobile(ir.getRegEmail());
		}
		return checkEmail(ir.getRegEmail());
	}

	public String check(String email, int accType) {
		if (accType == IAccountType.MOBILE) {
			return checkEmailMobile(email);
		}
		return checkEmail(email);
	}

	private String checkEmailMobile(String email) {
		if (StringFormat.isValidEmailAddr(email)) {
			return checkEmail(email);
		}
		return checkMobile(email);
	}

	/**
	 * email判断
	 * 
	 * @param email
	 * @return
	 */
	private String checkEmail(String email) {
		email = StringUtils.trimToEmpty(email).toLowerCase();
		if (email == null || email.length() == 0) {
			return ERR_MAIL_NOT_EMPTY;
		}
		if (email.length() > 50) {
			return ERR_MAIL_LENGTH_EXEED;
		}
		if (!StringFormat.isValidEmailAddr(email)) {
			return ERR_MAIL_NOT_VALID;
		}
		if (!checkEmailDomainIsValid(email)) {
			return ERR_MAIL_NOT_VALID;
		}
		if (hasUser(email) == true) {
			return ERR_MAIL_EXIST;
		}
		return OK;
	}

	/**
	 * 检查domain合法性
	 * 
	 * @param email
	 * @return
	 */
	private boolean checkEmailDomainIsValid(String email) {
		// check blocked domain
		String domain;
		try {
			domain = email.split("@")[1];
		} catch (Exception e) {
			return false;
		}
		String key = "email.domain.blocked." + domain;
		String basename = "check-rules";
		String value = DodoyoResource.getString(key, basename);
		if (!DodoyoResource.isNotValue(key, value)) {
			return false;
		}
		// check corporation domain
		String cdomain;
		String corp;
		try {
			cdomain = domain.split("\\.")[1];
			corp = domain.split("\\.")[0];
		} catch (Exception e) {
			return false;
		}
		key = "email.corporation.domain.right." + corp;
		value = DodoyoResource.getString(key, basename);
		if (!StringUtils.equals(cdomain, value)
				&& !DodoyoResource.isNotValue(key, value)) {
			return false;
		}
		return true;
	}

	/**
	 * 是否已经注册
	 * 
	 * @param email
	 * @return
	 */
	private boolean hasUser(String email) {
		return UserAdapter.hasUser(email);
	}

	private String checkMobile(String mobile) {
		if (mobile == null || mobile.length() == 0) {
			return ERR_MOBILE_NOT_EMPTY;
		}
		// 判断手机号
		if (false == StringFormat.isValidMobile(mobile)) {
			return ERR_MOBILE_NOT_VALID;
		}
		if (MobileLogic.getInstance().getBindedUserIdByMobile(mobile) > 0) {
			return ERR_MOBILE_BIND;
		}
		boolean userExsit = false;
		try {
			userExsit = UserAdapter.hasUser(mobile);
		} catch (Exception e) {
		}
		if (userExsit == true) {
			return ERR_MAIL_MOBILE_EXIST;
		}

		return OK;
	}
}
