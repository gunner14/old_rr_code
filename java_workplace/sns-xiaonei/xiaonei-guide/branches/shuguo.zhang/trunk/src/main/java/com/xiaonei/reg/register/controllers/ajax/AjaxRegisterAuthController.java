package com.xiaonei.reg.register.controllers.ajax;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.controllers.base.BaseRegAjaxController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckName;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckNicknameId;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckRegEmail;
import com.xiaonei.reg.thread.timelimit.TimeLimitConfiguration;
import com.xiaonei.reg.thread.timelimit.TimeLimitController;
import com.xiaonei.reg.thread.timelimit.TimeLimitThread;

/**
 * 		<action path="/AjaxRegisterAuth" type="com.xiaonei.reg.register.action.AjaxRegisterAuthAction" scope="request" />

 * @author wangtai
 *
 */
@Path("AjaxRegisterAuth")
public class AjaxRegisterAuthController extends BaseRegAjaxController {
	private static final String OK = IRegCheckCodeDef.OK;
	private static final String OKNAME = "OKNAME";

	@Override
	public RegForward executeRAt(RegMapping mapping, RegForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String field = request.getParameter("authType");
		String checkValue = Escape.trim(request.getParameter("value"));
		String stage = StringUtils.trimToEmpty(request.getParameter("stage"));

		if (StringUtils.equals(field, "email")) {
			return output(request, response, checkRegEmail(checkValue, stage, request));

		} else if (StringUtils.equals(field, "name")
				&& !StringUtils.isEmpty(checkValue)) {
			// 检查名字是否是百家姓中的
			String rt = checkName(checkValue);
			if (IRegCheckCodeDef.ERR_NAME_NOT_EMPTY.equals(rt)) {
				return output(request, response,
						"请输入<font style=\"color: red;font-weight: bold\">真实中文姓名</font>，方便朋友查找。");
			} else if (OK.equals(rt)) {
				return output(request, response, OKNAME);
			} else {
				return output(request, response, rt);
			}
		} else if (StringUtils.equals(field, "xid")) {
			return output(request, response, checkNicknameId(checkValue));
		} else
			return null;
	}

	private String checkNicknameId(final String checkValue) {
		TimeLimitConfiguration tl = TimeLimitController.getTimeLimit(10);
		tl.setSingleConfig("checkNicknameId", new TimeLimitThread(tl) {
			@Override
			public Object work() {
				return new CheckNicknameId().checkXId(checkValue);
			}
		});
		try {
			return (String) tl.getSingleReturn();
		} catch (Throwable e) {
			e.printStackTrace();
			return OK;
		}
	}

	private String checkName(final String checkValue) {
		TimeLimitConfiguration tl = TimeLimitController.getTimeLimit(10);
		tl.setSingleConfig("checkName", new TimeLimitThread(tl) {
			@Override
			public Object work() {
				return new CheckName().checkName(checkValue);
			}
		});
		try {
			return (String) tl.getSingleReturn();
		} catch (Throwable e) {
			e.printStackTrace();
			return OKNAME;
		}
	}

	private String checkRegEmail(final String checkValue, final String stage,
			HttpServletRequest request) {
		TimeLimitConfiguration tl = TimeLimitController.getTimeLimit(10);
		tl.setSingleConfig("checkRegEmail", new TimeLimitThread(tl) {
			@Override
			public Object work() {
				return new CheckRegEmail().check(checkValue, RegUtil
						.strToInt(stage));
			}
		});
		try {
			String rt = (String) tl.getSingleReturn();
			if (IRegCheckCodeDef.ERR_MAIL_EXIST.equals(rt)) {
				RegLogger.getLoggerWithType("MAIL_EXIST").info(
						checkValue + " session_id:"
								+ request.getSession().getId());
			}
			return rt;
		} catch (Throwable e) {
			e.printStackTrace();
			return OK;
		}
	}

}
