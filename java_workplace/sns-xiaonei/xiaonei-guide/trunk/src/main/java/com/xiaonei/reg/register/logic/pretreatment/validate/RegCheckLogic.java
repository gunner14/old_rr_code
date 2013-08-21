package com.xiaonei.reg.register.logic.pretreatment.validate;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.icode.verify.CodeUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.util.BaseActionUtil;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.utility.CharCode;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.form.RegEduForm;
import com.xiaonei.reg.register.form.RegForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

/**
 * @deprecated 数据校验使用RegCheckManager 注册表单验证 2008-9-23
 * 
 * @author wei.cheng@opi-corp.com
 */
public class RegCheckLogic extends BaseRegCheckLogic {

	private static RegCheckLogic _instance = new RegCheckLogic();

	public static RegCheckLogic getInstance() {
		return _instance;
	}

	/**
	 * 表单验证
	 * 
	 * @param req
	 *            HttpServletRequest
	 * @param form
	 *            ActionForm
	 * @param response
	 * @return
	 */
	public String checkForm(final HttpServletRequest req,
			com.xiaonei.reg.register.form.base.RegForm form,
			HttpServletResponse response) {
		return RegCheckManager.getInstance().check((BaseRegForm) form, req,
				response);
	}

	/**
	 * 检查4399注册表单
	 * 
	 * @param req
	 * @param form
	 * @return
	 */
	public String check4399(final HttpServletRequest req,
			com.xiaonei.reg.register.form.base.RegForm form) {
		if ("post".equalsIgnoreCase(req.getMethod())) {
			RegForm regForm = (RegForm) form;
			// 判断电子邮箱
			String mailCheck = this.checkEmail(regForm.getRegEmail(), regForm
					.getStage());
			if (!OK.equals(mailCheck)) {
				return mailCheck;
			}
			AntiSpamHome ash = AntiSpamHome.getInstance();
			boolean canReg;
			if (StringFormat.isValidMobile(regForm.getRegEmail()) == false) {
				try {
					canReg = ash.canRegister(0,regForm.getName(), regForm
							.getRegEmail(), regForm.getPwd(), BaseActionUtil
							.getClientIP(req));
				} catch (Exception e) {
					e.printStackTrace();
					canReg = true;
				}
			} else {
				canReg = true;
			}
			if (false == canReg) {
				return ERR_FILTER_MAJIA;
			}

			// 验证码判断(先判断)
			if (req.getParameter("icode") != null) {
				String failure = this.checkICode(req);
				if (!OK.equals(failure)) {
					return failure;
				}
			}

			// 判断密码的输入
			String pwdCheck = this.checkNoRepeatPwd(regForm.getPwd());
			if (!OK.equals(pwdCheck)) {
				return pwdCheck;
			}
			// 姓名判断
			String nameChk = this.checkName(regForm.getName());
			if (!OK.equals(nameChk)) {
				return nameChk;
			}
			// 性别判断
			String gender = Escape.stringToHTMLString(regForm.getGender());
			if (gender == null || !gender.equals("男生") && !gender.equals("女生")) {
				return ERR_SEX_NOT_EMPTY;
			}
			// 阶段判断
			String stageChk = this.check4399Stage(regForm.getStage());
			if (!OK.equals(stageChk)) {
				return stageChk;
			}
		} else {
			return ERR_REG_SYSTEM;
		}
		return OK;

	}

	/**
	 * 检查手机引导上来的fillinfo表单
	 * 
	 * @param user
	 * @param newName
	 * @param request
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-3 - 下午05:48:47
	 */
	public String checkMobileStageFillinfo(User user, String newName,
			HttpServletRequest request) {
		String ic = checkICodeSigned(request, user);
		if (!StringUtils.equals(OK, ic)) {
			return ic;
		}
		String rn = checkName(newName);
		if (!StringUtils.equals(OK, rn)) {
			return rn;
		}
		return OK;
	}

	/**
	 * 教育部注册
	 * 
	 * @param request
	 * @param regEduForm
	 * @return
	 * @author wang-tai(tai.wang@opi-corp.com) 2009-3-25 - 下午12:37:34
	 * @param response
	 */
	public String checkEducation(HttpServletRequest request,
			RegEduForm regEduForm, HttpServletResponse response) {
		return RegCheckManager.getInstance().check(regEduForm, request,
				response);
	}

	@Override
	public String check(BaseRegForm regForm) {
		return null;
	}

	public String getErrorValue(String errCode) {
		return RegCheckManager.getInstance().getErrorValue(errCode);
	}

	// ====================================

	/**
	 * 手机号判断
	 * 
	 * @param regEmail
	 * @return
	 */
	private String checkMobile(String regEmail) {
		if (true == StringFormat.isValidMobile(regEmail)) {
			if (com.xiaonei.platform.component.mobile.logic.MobileLogic
					.getInstance().getBindedUserIdByMobile(regEmail) > 0) {
				return ERR_MOBILE_BIND;
			} else {
				return OK;
			}
		} else {
			return ERR_MOBILE_NOT_VALID;
		}
	}

	/**
	 * 只有stage值，没有具体的学校
	 * 
	 * @param stage
	 * @return
	 */
	private String check4399Stage(final int stage) {
		switch (stage) {
		case Stage.STAGE_MAJOR:
			break;
		case Stage.STAGE_UNIV:
			break;
		case Stage.STAGE_HEIGHSCHOOL:
			break;
		default:
			return ERR_INVALID_STAGE;
		}

		return OK;
	}

	/**
	 * email判断
	 * 
	 * @param email
	 * @param stage
	 * @return
	 */
	private String checkEmail(String email, int stage) {
		if (email == null || email.length() == 0) {
			return ERR_MAIL_NOT_EMPTY;
		}
		if (email.length() > 50) {
			return ERR_MAIL_LENGTH_EXEED;
		}
		try {
			Long.valueOf(email);
			// 现在只针对高中生
			if (stage == Stage.STAGE_HEIGHSCHOOL) {
				// 判断手机号
				String mailMobile = this.checkMobile(email);
				if (!OK.equals(mailMobile)) {
					return mailMobile;
				}
			} else {
				return ERR_MAIL_NOT_VALID;
			}
		} catch (NumberFormatException e1) {
			if (!StringFormat.isValidEmailAddr(email)
					|| email.substring(email.indexOf("@")).equalsIgnoreCase(
							"@aravensoft.com")
					|| email.substring(email.indexOf("@")).equalsIgnoreCase(
							"@nybella.com")) {
				return ERR_MAIL_NOT_VALID;
			}
		}
		boolean userExsit = false;
		try {
			userExsit = UserAdapter.hasUser(email);
		} catch (Exception e) {
		}
		if (userExsit == true) {
			if (stage == Stage.STAGE_HEIGHSCHOOL)
				return ERR_MAIL_MOBILE_EXIST;
			return ERR_MAIL_EXIST;
		}

		return OK;
	}

	/**
	 * 没有重复密码
	 * 
	 * @param pwd
	 * @return
	 */
	private String checkNoRepeatPwd(final String pwd) {
		if (pwd == null || pwd.length() == 0) {
			return ERR_PWD_NOT_EMPTY;
		}
		if (pwd.length() < 6) {
			return ERR_PWD_NOT_ENOUGH;
		}
		if (pwd.length() > 20) {
			return ERR_PWD_TOO_LONG;
		}
		return OK;
	}

	/**
	 * 用户输入的真实姓名检测
	 * 
	 * @param name
	 * @return
	 */
	private String checkName(String name) {
		if (name == null || name.length() == 0) {
			return ERR_NAME_NOT_EMPTY;
		}
		AntiSpamHome ash = AntiSpamHome.getInstance();
		CheckResult cr = null;
		cr = ash.checkRealName(0, name);
		if (cr.getM_type() == CheckResult.PROHIBITED) {
			return ERR_NAME_NOT_SAFE;
		} else if (cr.getM_type() == CheckResult.CONTENT_NEED_AUDIT) {
			return ERR_NAME_NOT_REAL;
		} else if (cr.getM_type() == CheckResult.SYSTEM_ERROR) {
			return OK;
		} else if (cr.getM_type() == CheckResult.NEED_REAL_NAME) {
			return ERR_NAME_NOT_REAL;
		}
		name = cr.getM_content();
		try {
			if (CharCode.isLengthExeed(name, 12)) {
				return ERR_NAME_TOO_LONG;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return ERR_NAME_NOT_SAFE;
		}

		return OK;
	}

	/**
	 * 验证码判断
	 * 
	 * @param request
	 * @return
	 */
	private String checkICode(HttpServletRequest req) {
		// 邀请注册不检查验证码
		// 处理验证码
		String key_id = req.getParameter("key_id");
		if (null != BaseThreadUtil.currentHost(req)) {
			return ERR_USER_SIGNED;
		}
		if (!CodeUtil.getInstance().validCode(req.getParameter("icode"),
				"register_" + key_id,
				CookieManager.getInstance().getCookie(req, "ick"))) {
			return ERR_CODE_ERR;
		}
		return OK;
	}

	/**
	 * 验证码判断，不判断是否登录
	 * 
	 * @param req
	 * @return
	 */
	private String checkICodeSigned(HttpServletRequest req, User host) {

		String icode = req.getParameter("icode");
		String postfix = "mfillinfoSt";
		if (icode == null
				|| !CodeUtil.getInstance().validCode(icode, postfix,
						host.getId())) {
			return ERR_CODE_ERR;
		}
		return OK;
	}

}
