package com.xiaonei.reg.register.logic.pretreatment.validate;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.proxy.ProxyMethodLog;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.logic.additional.RegXidLogic;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

public class CheckNicknameId extends BaseRegCheckLogic {

	private static final CheckNicknameId proxyInstance = ProxyMethodLog
			.getLogProxy(CheckNicknameId.class);
	
	public static CheckNicknameId getProxy(){
		return proxyInstance;
	}

	@Override
	public String check(BaseRegForm regForm) {
		IAccType in;
		try {
			in = (IAccType) coverInterface(regForm, IAccType.class);
		} catch (Exception e) {
			e.printStackTrace();
			return OK;
		}
		return checkXId(in.getNicknameId());
	}

	public String checkXId(String xId) {
	    xId = StringUtils.trimToEmpty(xId).toLowerCase();
		if (StringUtils.isEmpty(xId)) {
			return ERR_NIKNAME_FORMAT;
		}
		if (StringFormat.isValidMobile(xId)
				|| StringFormat.isValidEmailAddr(xId)) {
			return ERR_NIKNAME_FORMAT;
		}
		if (RegXidLogic.isInBlackList(xId)) {
			return ERR_NIKNAME_FORMAT;
		}
		
		/* 2010.03.18 需求被改变了，数字也允许id注册啦
		 * try {
		    Long.valueOf(xId); // 不能是数字
		    return ERR_NIKNAME_FORMAT;
		} catch (NumberFormatException e) {
		}*/
		
		// String nickPatten = "^(([0-9]*[a-zA-Z]+)|([a-zA-Z]+[0-9]*))$";
		String nickPatten = "^\\w{6,10}|\\d{6,10}$";
		Pattern p = Pattern.compile(nickPatten);
		Matcher m = p.matcher(xId);
		if (!m.matches()) {
			return ERR_NIKNAME_FORMAT;
		}
		boolean userExsit = false;
		try {
			userExsit = UserAdapter.hasUser(xId);
		} catch (Exception e) {
		}

		if (true == userExsit) {
			RegLogger.getLoggerWithType("MAIL_EXIST").info(xId);
			return ERR_NIKNAME_EXIST;
		}
		return OK;
	}

}
