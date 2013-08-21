package com.xiaonei.reg.register.controllers.sb;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegCommForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegAuditLogic;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegAuditLogicFactory;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.additional.RegXidLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegInviteLinkLogic;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * <action path="/commonreg"
 * type="com.xiaonei.reg.register.action.sb.CommonRegisterAction"
 * name="regCommForm" scope="request" > <forward name="success"
 * path="/pages/register/register_ok.jsp" /> <forward name="common"
 * path="/initcommonreg.do" /> <forward name="auditUniv" path="/auditUniv.do"
 * redirect="false" /> <forward name="auditHs" path="/auditHs.do"
 * redirect="false" /> </action> 普通注册的action 2008-9-24
 * 
 * @author wei.cheng@opi-corp.com
 * @author tai.wang@opi-corp.com
 */
@Path("commonreg")
public class CommonRegisterController extends BaseRegSubmitController {
	// 用户行为日志跟踪增加
	private static final int noAudit = 2;
	private static final int needAudit = 1;
	// 需要完成跳转
	private static final String succForward = "success";
	private static final String errForward = "common";
	private static final String auditUnivForward = "auditUniv";

	public RegForward executeRSt(RegMapping mapping, BaseRegForm form,
			HttpServletRequest request, HttpServletResponse response) {

		TimeCost cost = TimeCost.begin();

		RegCommForm regForm = (RegCommForm) form;
		String hostIp = getFromIp(request);
		cost.endStep();

		/* 2,业务逻辑,生成用户啥的... */
		int stage = Integer.valueOf(regForm.getStage());
		// 注册时返会的错误代码
		int regCode = IRegisterLogic.err_noerr;
		// 白领
		if (stage == Stage.STAGE_MAJOR) {
			cost.endStep();
			RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
			cost.endStep();
			IRegisterLogic reg = RegisterLogicFactory.getMajorCommon();
			cost.endStep();
			try {
				regCode = reg.register(regPojo);
				cost.endStep();
			} catch (Exception e) {
				return errForward(mapping, request, regForm, e, "系统错误，请稍后重试");
			}
			if (regCode < IRegisterLogic.err_noerr) {
				User user = null;
				try {
					user = UserAdapter.getUser(-regCode);
					cost.endStep();
				} catch (Exception ex) {
					ex.printStackTrace();
				}
				request.setAttribute("userId", -regCode);
				/* 增加用户行为跟踪日志 */
				addUserTraceLog(regPojo, request, user, noAudit, regForm);
				cost.endStep();
				regCode = IRegisterLogic.err_noerr;
			}
		}
		// 高中
		else if (stage == Stage.STAGE_HEIGHSCHOOL) {
			cost.endStep();
			RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
			cost.endStep();
			if (StringFormat.isValidMobile(regForm.getRegEmail())) {
				cost.endStep();
				/* 增加用户行为跟踪日志 */
				addUserTraceLog(regPojo, request, null, 1, regForm);
				cost.endStep();
				// 去高中审核页面
				request.setAttribute("init", "init");
				return mapping.findForward("auditHs");
			}
			IRegisterLogic reg = RegisterLogicFactory.getHSCommon();
			cost.endStep();
			try {
				regCode = reg.register(regPojo);
				cost.endStep();
			} catch (Exception e) {
				return errForward(mapping, request, regForm, e, "系统错误，请稍后重试");
			}
			if (regCode < IRegisterLogic.err_noerr) {
				User user = null;
				cost.endStep();
				try {
					user = UserAdapter.getUser(-regCode);
					cost.endStep();
				} catch (Exception ex) {
					ex.printStackTrace();
				}
				request.setAttribute("userId", -regCode);
				cost.endStep();
				/* 增加用户行为跟踪日志 */
				addUserTraceLog(regPojo, request, user, noAudit, regForm);
				cost.endStep();
				regCode = IRegisterLogic.err_noerr;
			}
		}
		// 大学
		else if (stage == Stage.STAGE_UNIV) {
			cost.endStep();
			// 判断此用户是注册还是审核
			RegAuditLogicFactory rAuditFac = RegAuditLogicFactory.getInstance();
			cost.endStep();
			IRegAuditLogic rAuditLogic = rAuditFac
					.getAudit(RegAuditLogicFactory.reg_audit_univ);
			cost.endStep();
			int isRegOrAudit = rAuditLogic.shouldBeAudit(regForm, hostIp); // 此处添加判断审核还是注册
			cost.endStep();
			// 注册
			if (IRegAuditLogic.isReg == isRegOrAudit) {
				RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
				cost.endStep();
				IRegisterLogic reg = RegisterLogicFactory.getUnivCommon();
				cost.endStep();
				try {
					regCode = reg.register(regPojo);
					cost.endStep();
				} catch (Exception e) {
					return errForward(mapping, request, regForm, e,
							"系统错误，请稍后重试");
				}
				if (regCode < IRegisterLogic.err_noerr) {
					cost.endStep();
					User user = null;
					try {
						user = UserAdapter.getUser(-regCode);
						cost.endStep();
					} catch (Exception ex) {
						ex.printStackTrace();
					}
					request.setAttribute("userId", -regCode);
					cost.endStep();
					/* 增加用户行为跟踪日志 */
					addUserTraceLog(regPojo, request, user, noAudit, regForm);
					cost.endStep();
					regCode = IRegisterLogic.err_noerr;
				} else {
					return errForward(mapping, request, regForm, null,
							"系统错误，请稍后重试");
				}
			}
			// 去大学审核页面
			else if (IRegAuditLogic.isAudit == isRegOrAudit) {
				RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
				cost.endStep();
				/* 增加用户行为跟踪日志 */
				addUserTraceLog(regPojo, request, null, needAudit, regForm);
				cost.endStep();
				request.setAttribute("init", "init");
				return mapping.findForward(auditUnivForward);
			}
			// 出错
			else {
				String errMsg;
				if (IRegAuditLogic.err_invalid_email == isRegOrAudit) {
					errMsg = "请输入你学校的电子邮箱";

					regCode = IRegisterLogic.err_unknown;
				} else if (IRegAuditLogic.err_no_choice_univ == isRegOrAudit) {
					errMsg = "请选择大学";
					regCode = IRegisterLogic.err_unknown;
				} else if (IRegAuditLogic.err_no_such_univ == isRegOrAudit) {
					errMsg = "没有这个大学";
					regCode = IRegisterLogic.err_unknown;
				} else {
					errMsg = "系统错误，请稍后重试";
					regCode = IRegisterLogic.err_unknown;
				}
				return errForward(mapping, request, regForm, null, errMsg);
			}
		}
		// 以上都不是
		else if (stage == Stage.STAGE_NONE) {
			cost.endStep();
			RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
			cost.endStep();
			IRegisterLogic reg = RegisterLogicFactory.getOthersCommon();
			cost.endStep();
			try {
				regCode = reg.register(regPojo);
				cost.endStep();
			} catch (Exception e) {
				return errForward(mapping, request, regForm, e, "系统错误，请稍后重试");
			}
			if (regCode < IRegisterLogic.err_noerr) {
				User user = null;
				try {
					user = UserAdapter.getUser(-regCode);
					cost.endStep();
				} catch (Exception ex) {
					ex.printStackTrace();
				}
				request.setAttribute("userId", -regCode);
				/* 增加用户行为跟踪日志 */
				addUserTraceLog(regPojo, request, user, noAudit, regForm);
				cost.endStep();
				regCode = IRegisterLogic.err_noerr;
			} else {
				return errForward(mapping, request, regForm, null, "系统错误，请稍后重试");
			}
		}
		// 未知
		else {
			regCode = IRegisterLogic.err_unknown;
			return errForward(mapping, request, regForm, null, "系统错误，请稍后重试");
		}
		request.setAttribute("stage", regForm.getStage());
		request.setAttribute("email", regForm.getRegEmail());
		request.setAttribute("emailPage", RegUtil.emailLoginUrl(regForm
				.getRegEmail()));
		/* 3,跳转 */
		// 判断是否xid注册
		cost.endStep();
		if (RegXidLogic.isXid(regForm.getAccType(), regForm.getNicknameId())) {
			cost.endStep();
			User user = null;
			try {
				user = (User) UserAdapter.getUser(-regCode);
				cost.endStep();
			} catch (Exception e) {
				e.printStackTrace();
			}
			if (user != null) {
				try {
					ActivateLogic.getInstance().activateUser(user);
					cost.endStep();
					this.addUserTraceLog_autoActivate(regForm, request, user);
					cost.endStep();
					LoginLogic.getInstance().doLoginR(user, request, response,
							0, false);
					cost.endStep();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			cost.endFinally();
			return new RegForward(Globals.urlWww + "/Home.do", true);
		}
		cost.endFinally();
		return mapping.findForward(succForward);
	}

	/**
	 * 增加用户行为跟踪日志
	 * 
	 * @param regPojo
	 * @param request
	 * @param oceUser
	 */
	private void addUserTraceLog(RegisterPojo regPojo,
			HttpServletRequest request, User oceUser, int flag, RegCommForm form) {

		if (StringUtils
				.equals(form.getFrom(), RegInviteLinkLogic.REG_ADS_HSDOG)) {
			UserTraceLogic.getInstance().insertRegHis(request, null,
					regPojo.getRegEmail(),
					String.valueOf(RegOprDefine.MSN_TAB_REG),
					Stage.STAGE_HEIGHSCHOOL, RegOprDefine.ssHsDog, null,
					form.getG(), form.getB(), 0, true);
			return;
		}
		String actionStr = request.getParameter("action_id");// 非邀请,对应一般注册的情况
		if (!StringUtils.equals(actionStr, "" + RegOprDefine.STD_REG)) {
			if (StringFormat.isValidEmailAddr(regPojo.getRegEmail())) {
				actionStr = "20" + StringUtils.substring(actionStr, 2);
			} else if (StringFormat.isValidMobile(regPojo.getRegEmail())) {
				actionStr = "22" + StringUtils.substring(actionStr, 2);
			} else {
				actionStr = "21" + StringUtils.substring(actionStr, 2);
			}
		}
		String source = request.getParameter("ss");
		UserTraceLogic.getInstance().insertRegHis(request, oceUser,
				regPojo.getRegEmail(), actionStr,
				Integer.valueOf(regPojo.getStage()), source, null, form.getG(),
				form.getB(), 0, true);
		if (flag == noAudit) {		    
		    
		    //2010.03.25修改
	        actionStr = String.valueOf(RegOprDefine.NORMAL_REG_OK);
	        if (StringFormat.isValidEmailAddr(oceUser.getAccount())) {
	            // do nothing
	        } else if (StringFormat.isValidMobile(oceUser.getAccount())) {
	            actionStr = String.valueOf(RegOprDefine.NORMAL_MOBILE_REG_OK);
	        } else {
	            actionStr = String.valueOf(RegOprDefine.NORMAL_ID_REG_OK);
	        }
	        
			UserTraceLogic.getInstance().insertRegHis(request, oceUser,
					regPojo.getRegEmail(),
					actionStr,
					Integer.valueOf(regPojo.getStage()), source, null,
					form.getG(), form.getB(), 0, true);
		}

	}

	/**
	 * 增加用户行为跟踪日志自动激活
	 * 
	 * @param regPojo
	 * @param request
	 * @param oceUser
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-11-7 下午08:30:34
	 */
	private void addUserTraceLog_autoActivate(BaseRegForm regForm,
			HttpServletRequest request, User oceUser) {
		try {
			// 邀请链接上的邀请人id
			int ivId = 0;
			String i = request.getParameter("i");
			if (StringUtils.isNotEmpty(i)) {
				ivId = StringUtils.isNumeric(i) ? Integer.parseInt(i) : 0;
			}

			String actionStr = String.valueOf(RegOprDefine.AUTO_MAIL_ACTIVE);
			if (StringFormat.isValidEmailAddr(oceUser.getAccount())) {
			} else if (StringFormat.isValidMobile(oceUser.getAccount())) {
			} else {
				actionStr = String.valueOf(RegOprDefine.AUTO_XID_ACTIVE);
			}
			String source = request.getParameter("ss");
			UserTraceLogic.getInstance().insertRegHis(request, oceUser,
					oceUser.getAccount(), actionStr,
					UserAdapter.get(oceUser).getStage(), source, null,
					regForm.getG(), regForm.getB(), ivId, true);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	protected String getErrForward() {
		return errForward;
	}

	@Override
	public Class<? extends BaseRegForm> getFormClazz() {
		return RegCommForm.class;
	}

	@Override
	protected void addUserTraceLogForward(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {

	}

	@Override
	protected void addUserTraceLogRegSucc(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {

	}

	@Override
	protected RegForward before(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response) {
		return executeRSt(mapping, baseRegForm, request, response);
	}

	@Override
	protected IRegisterLogic getRegisterLogic() {
		return null;
	}

	@Override
	protected RegForward after(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response, User user) {
		return null;
	}

	@Override
	protected RegForward succForward(HttpServletRequest request,
			HttpServletResponse response, RegMapping mapping, User user) {
		return null;
	}

	@Override
	protected Map<String, RegForward> setRegMapping(Map<String, RegForward> map) {
		// <forward name="success" path="/pages/register/register_ok.jsp" />
		// <forward name="common" path="/initcommonreg.do" />
		// <forward name="auditUniv" path="/auditUniv.do" redirect="false" />
		// <forward name="auditHs" path="/auditHs.do" redirect="false" />
		map.put("success", new RegForward("/pages/register/register_ok.jsp"));
		map.put("common", new RegForward("/init/xn6205"));
		map.put("auditUniv", new RegForward("/auditUniv.do",false));
		map.put("auditHs", new RegForward("/auditHs.do",false));
		return map;
	}

}
