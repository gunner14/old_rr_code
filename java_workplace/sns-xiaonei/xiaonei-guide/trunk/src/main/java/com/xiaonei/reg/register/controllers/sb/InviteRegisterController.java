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
import com.xiaonei.reg.register.form.RegInviteForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegAuditLogic;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegAuditLogicFactory;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.reg.register.logic.additional.RegInviteTypeLogic;
import com.xiaonei.reg.register.logic.additional.RegXidLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegInviteLinkLogic;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * <action path="/invitereg"
 * type="com.xiaonei.reg.register.action.sb.InviteRegisterAction"
 * name="regInviteForm" scope="request"> <forward name="success"
 * path="/pages/register/register_ok.jsp" /> <forward name="common"
 * path="/initinvitereg.do" /> <forward name="commonapp"
 * path="/initinvitereg.do" redirect="false" /> <forward name="auditUniv"
 * path="/auditUniv.do" redirect="false" /> <forward name="auditHs"
 * path="/auditHs.do" redirect="false" /> <forward name="wait"
 * path="/please-wait-a-moment.do" redirect="true" /> </action>
 * 
 * @author wangtai
 * 
 */
@Path("invitereg")
public class InviteRegisterController extends BaseRegSubmitController {

	// 需要完成跳转
	private static final String errForward = "common";

	private static final String auditUnivForward = "auditUniv";

	// 用户行为日志跟踪增加
	private static final int noAudit = 2;

	private static final int needAudit = 1;

	private static final int errHostId = -1;

	@Override
	protected String getErrForward() {
		return errForward;
	}

	@Override
	public Class<? extends BaseRegForm> getFormClazz() {
		return RegInviteForm.class;
	}

	private int univProc(RegInviteForm regForm, String hostIp,
			HttpServletRequest request, RegMapping mapping) {
		// 判断此用户是注册还是审核
		RegAuditLogicFactory rAuditFac = RegAuditLogicFactory.getInstance();
		IRegAuditLogic rAuditLogic = rAuditFac
				.getAudit(RegAuditLogicFactory.reg_audit_univ);
		int isRegOrAudit = rAuditLogic.shouldBeAudit(regForm, hostIp); // 此处添加判断审核还是注册
		// 注册
		RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
		int hostId = errHostId;
		if (IRegAuditLogic.isReg == isRegOrAudit) {
			IRegisterLogic reg = RegisterLogicFactory.getUnivInvite();
			int regCode;
			try {
				regCode = reg.register(regPojo);
			} catch (Exception e) {
				e.printStackTrace();
				regCode = IRegisterLogic.err_noerr;
			}
			if (regCode < IRegisterLogic.err_noerr) {
				User user = null;
				hostId = -regCode;
				try {
					user = UserAdapter.getUser(-regCode);
				} catch (Exception ex) {
					ex.printStackTrace();
				}
				request.setAttribute("userId", -regCode);
				/* 增加用户行为跟踪日志 */
				addUserTraceLog(regPojo, request, user, noAudit, regForm
						.getFrom(), regForm);

				regCode = IRegisterLogic.err_noerr;
			}
		}
		// 去大学审核页面
		else if (IRegAuditLogic.isAudit == isRegOrAudit) {
			request.setAttribute("init", "init");
			addUserTraceLog(regPojo, request, null, needAudit, regForm
					.getFrom(), regForm);
			return errHostId;
		}
		// 出错
		else if (IRegAuditLogic.err_invalid_email == isRegOrAudit) {
			addErr(request, "请输入你学校的电子邮箱");
		} else if (IRegAuditLogic.err_no_choice_univ == isRegOrAudit) {
			addErr(request, "请选择大学");
		} else if (IRegAuditLogic.err_no_such_univ == isRegOrAudit) {
			addErr(request, "没有这个大学");
		} else {
		}
		return hostId;
	}

	private int hsProc(RegInviteForm regForm, String hostIp,
			HttpServletRequest request) {
		// 注册
		RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
		IRegisterLogic reg = RegisterLogicFactory.getHSInvite();
		int hostId = errHostId;
		int regCode;
		try {
			regCode = reg.register(regPojo);
		} catch (Exception e) {
			e.printStackTrace();
			regCode = IRegisterLogic.err_noerr;
		}
		if (regCode < IRegisterLogic.err_noerr) {
			hostId = -regCode;
			User user = null;
			try {
				user = UserAdapter.getUser(-regCode);
			} catch (Exception ex) {
				ex.printStackTrace();
			}
			request.setAttribute("userId", -regCode);
			/* 增加用户行为跟踪日志 */
			addUserTraceLog(regPojo, request, user, noAudit, hostIp, regForm);
		}
		return hostId;
	}

	private int majorProc(HttpServletRequest request, RegInviteForm regForm,
			String hostIp) {
		RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
		IRegisterLogic reg = RegisterLogicFactory.getMajorInvite();
		int regCode;
		int hostId = errHostId;
		try {
			regCode = reg.register(regPojo);
		} catch (Exception e) {
			e.printStackTrace();
			regCode = IRegisterLogic.err_noerr;
		}
		if (regCode < IRegisterLogic.err_noerr) {
			hostId = -regCode;
			User user = null;
			try {
				user = UserAdapter.getUser(-regCode);
			} catch (Exception ex) {
				ex.printStackTrace();
			}
			request.setAttribute("userId", -regCode);
			/* 增加用户行为跟踪日志 */
			addUserTraceLog(regPojo, request, user, noAudit, hostIp, regForm);
		}
		return hostId;
	}

	/**
	 * 增加用户行为跟踪日志
	 * 
	 * @param regForm
	 * @param request
	 * @param oceUser
	 */
	@SuppressWarnings("unused")
	private void addUserTraceLog_submitLog(RegInviteForm regForm,
			HttpServletRequest request) {
		try {
			String actionStr = request.getParameter("action_id");
			;
			String source = request.getParameter("ss");
			int appId = regForm.getAppId();
			UserTraceLogic.getInstance().insertRegHis(request, null,
					regForm.getRegEmail(), actionStr, regForm.getStage(),
					source, String.valueOf(appId), regForm.getG(),
					regForm.getB(), regForm.getInviterId(), true);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 增加用户行为跟踪日志自动激活
	 * 
	 * @param regForm
	 * @param request
	 * @param oceUser
	 * @author (wang-tai)tai.wang@opi-corp.com 2008-11-7 下午08:30:34
	 */
	private void addUserTraceLog_autoActivate(RegInviteForm regForm,
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

	/**
	 * 增加用户行为跟踪日志
	 * 
	 * @param regPojo
	 * @param request
	 * @param oceUser
	 * @param form
	 */
	private void addUserTraceLog(RegisterPojo regPojo,
			HttpServletRequest request, User oceUser, int flag, String from,
			BaseRegForm form) {

		if (StringUtils.equals(from, RegInviteLinkLogic.REG_ADS_HSDOG)) {
			UserTraceLogic.getInstance().insertRegHis(request, null,
					regPojo.getRegEmail(),
					String.valueOf(RegOprDefine.MSN_TAB_REG),
					Stage.STAGE_HEIGHSCHOOL, RegOprDefine.ssHsDog, null, null,
					null, 0, true);
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
		RegInviteForm regForm = (RegInviteForm) baseRegForm;
		String hostIp = getFromIp(request);

		String url = request.getParameter("fromUrl");
		request.setAttribute("fromUrl", url);

		/* 2,业务逻辑,生成用户啥的... */
		int stage = regForm.getStage();
		int hostId = errHostId;

		// 邀请类型 （必须在邀请链接失效前获得）
		RegisterPojo regPojo = new RegisterPojo(regForm, hostIp);
		int inviteType = RegInviteTypeLogic.getInstance().checkInviteType(
				regPojo.getInviterId(), regPojo.getInviteeEmail(),
				regPojo.getRegEmail());
		// 白领
		if (stage == Stage.STAGE_MAJOR || stage == Stage.STAGE_NONE) {
			hostId = majorProc(request, regForm, hostIp);
		}
		// 高中
		else if (stage == Stage.STAGE_HEIGHSCHOOL) {
			if (StringFormat.isValidMobile(regForm.getRegEmail())) {
				/* 增加用户行为跟踪日志 */
				addUserTraceLog(regPojo, request, null, needAudit, regForm
						.getFrom(), baseRegForm);
				// 去高中审核页面
				request.setAttribute("init", "init");
				return mapping.findForward("auditHs");
			}
			hostId = hsProc(regForm, hostIp, request);
		}
		// 大学
		else if (stage == Stage.STAGE_UNIV) {
			hostId = univProc(regForm, hostIp, request, mapping);
			if (hostId == errHostId) {
				return mapping.findForward(auditUnivForward);
			}
		}
		/* 3,跳转 */
		boolean isAutoActivate = false;
		// 判断是否xid注册
		if (RegXidLogic.isXid(regForm.getAccType(), regForm.getNicknameId())) {
			isAutoActivate = true;
		}
		// 判断是否自动激活
		if (inviteType == RegInviteTypeLogic.REG_INVITE_LINK) {
			isAutoActivate = true;
		}
		/* 激活 */
		try {
			User user = (User) UserAdapter.getUser(hostId);
			if (isAutoActivate && null != user && hostId != errHostId) {
				ActivateLogic.getInstance().activateUser(user);
				this.addUserTraceLog_autoActivate(regForm, request, user);
				LoginLogic.getInstance().doLoginR(user, request, response, 0,
						false);
				return new RegForward(Globals.urlWww + "/Home.do", true);
			} else {
				// 发送激活信
				RegEmailLogic.getInstance().sendActivateEmail(user);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		request.setAttribute("stage", regForm.getStage());
		request.setAttribute("email", regForm.getRegEmail());
		request.setAttribute("emailPage", RegUtil.emailLoginUrl(regForm
				.getRegEmail()));
		return mapping.findForward(succForward.toString());
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
		// <forward name="common" path="/initinvitereg.do" />
		// <forward name="commonapp" path="/initinvitereg.do" redirect="false"
		// />
		// <forward name="auditUniv" path="/auditUniv.do" redirect="false" />
		// <forward name="auditHs" path="/auditHs.do" redirect="false" />
		// <forward name="wait" path="/please-wait-a-moment.do"
		// redirect="true" />
		map.put("success", new RegForward("/pages/register/register_ok.jsp"));
		map.put("common", new RegForward("/init/xn6105"));
		map.put("commonapp", new RegForward("/initinvitereg.do",true));
		map.put("auditUniv", new RegForward("/auditUniv.do",true));
		map.put("auditHs", new RegForward("/auditHs.do",true));
		map.put("wait", new RegForward("/please-wait-a-moment.do",true));
		return map;
	}

}
