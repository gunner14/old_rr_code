package com.xiaonei.reg.register.controllers.ajax;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.register.constants.IRegMobileWebInfoMap;
import com.xiaonei.reg.register.controllers.base.BaseRegAjaxController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.dao.RegMobileWebInfoMapDAO;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;

/**		<action path="/ajax-mobile-code" type="com.xiaonei.reg.register.action.AjaxMobileRegisterCodeAction" />

 * @author wangtai
 *
 */
@Path("ajax-mobile-code")
public class AjaxMobileRegisterCodeController extends BaseRegAjaxController {

	private static final int OPT_SEND_SMS = 1;

	private static final String PARA_MOBILE_NUMBER = "mn";
	private static final String PARA_OPTION = "opt";

	@Override
	public RegForward executeRAt(RegMapping mapping, RegForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int opt = StringUtils.isNumeric(request.getParameter(PARA_OPTION)) ? Integer
				.valueOf(request.getParameter(PARA_OPTION))
				: -1;
		switch (opt) {
		case OPT_SEND_SMS:
			String mobile = request.getParameter(PARA_MOBILE_NUMBER);
			RegLogger.getLoggerWithType(this.getClass()).info("mobile register code::: mo=" + mobile);
			String downCode = sendSms(mobile);
			saveSmsCodeInfo(downCode, mobile);
			userTraceSendSms(request, mobile);
			break;
		}
		return null;
	}

	private void userTraceSendSms(HttpServletRequest request, String mobile) {
		UserTraceLogic.getInstance().insertRegHis(request, null, mobile,
				"236218", 0, null, null, null, null, 0);
	}

	private void saveSmsCodeInfo(String downCode, String mobile) {
		if (null == downCode) {
			return;
		}
		try {
			RegMobileWebInfoMapDAO.getInstance().insertRegMobileInfo(
					IRegMobileWebInfoMap.check_new, mobile, downCode);
		} catch (final SQLException e) {
			e.printStackTrace();
		}
	}

	private String sendSms(String mobile) {
		try {
			return null;//RegMobileLogic.getInstance().sendSms(mobile);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
}
