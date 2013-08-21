package com.xiaonei.reg.register.controllers.sb;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.pojo.RegFromSourcePojo;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * <action path="/s-c-reg"
 * type="com.xiaonei.reg.register.action.sb.ShortCommonRegisterAction"
 * name="regShortCommonForm" scope="request"> <forward name="success"
 * path="/pages/register/register_ok.jsp" /> <forward name="success-qq"
 * path="/pages/register/reg/register_ok_1105.jsp" /> <forward name="err"
 * path="/init/xn6205" /> </action>
 * 
 * @author tai.wang@opi-corp.com
 */
@Path("s-c-reg")
public class ShortCommonRegisterController extends BaseRegSubmitController {

	@Override
	protected String getErrForward() {
		return "err";
	}

	@Override
	public Class<? extends BaseRegForm> getFormClazz() {
		return RegShortCommonForm.class;
	}

	@Override
	protected void addUserTraceLogForward(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		String actionStr = request.getParameter("action_id");// 非邀请,对应一般注册的情况
		String source = request.getParameter("ss");
		UserTraceLogic.getInstance().insertRegHis(request, user,
				regPojo.getRegEmail(), actionStr, 0, source, null,
				baseRegForm.getG(), baseRegForm.getB(), 0, true);
		
		//2010.03.25修改
        actionStr = String.valueOf(RegOprDefine.NORMAL_REG_OK);
        if (StringFormat.isValidEmailAddr(user.getAccount())) {
            // do nothing
        } else if (StringFormat.isValidMobile(user.getAccount())) {
            actionStr = String.valueOf(RegOprDefine.NORMAL_MOBILE_REG_OK);
        } else {
            actionStr = String.valueOf(RegOprDefine.NORMAL_ID_REG_OK);
        }
		UserTraceLogic.getInstance().insertRegHis(request, user,
				regPojo.getRegEmail(),
				actionStr, 0, source, null,
				baseRegForm.getG(), baseRegForm.getB(), 0, true);

	}

	@Override
	protected void addUserTraceLogRegSucc(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {

	}

	@Override
	protected RegForward after(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response, User user) {
		return null;
	}

	@Override
	protected RegForward before(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response) {
		return null;
	}

	@Override
	protected IRegisterLogic getRegisterLogic() {
		return RegisterLogicFactory.getRegShortCommon();
	}

	@Override
	protected RegForward succForward(HttpServletRequest request,
			HttpServletResponse response, RegMapping mapping, User user) {
		request.setAttribute("email", user.getAccount());
		request.setAttribute("emailPage", RegUtil.emailLoginUrl(user
				.getAccount()));
		return mapping.findForward(succForward.toString());
	}

	@Override
	protected RegFromSourcePojo getFormSourcePojo(int userId,
			HttpServletRequest request) {
		String uuid = request.getParameter("uuid");
		String actionId = request.getParameter("action_id");
		String hostIp = getFromIp(request);
		return super.getFormSourcePojo(userId, request).setPageUuid(uuid)
				.setActionId(actionId).setIp(hostIp);
	}

	@Override
	protected Map<String, RegForward> setRegMapping(Map<String, RegForward> map) {
		map.put("success", new RegForward("/pages/register/register_ok.jsp"));
		//20100125去掉QQ注册的激活提示
		/*map.put("success-qq", new RegForward(
				"/pages/register/reg/register_ok_1105.jsp"));*/
		//登录失败，特殊激活页面
		map.put("success-6401", new RegForward("/pages/register/reg/register_ok_6401.jsp"));
		//游戏注册，特殊激活页面
		map.put("success-5226", new RegForward("/pages/register/reg/register_ok_5226.jsp"));
		map.put("err", new RegForward("/init/xn6205"));
		return map;
	}
}
