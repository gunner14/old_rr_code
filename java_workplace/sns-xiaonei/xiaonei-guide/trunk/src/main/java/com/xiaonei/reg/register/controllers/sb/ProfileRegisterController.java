package com.xiaonei.reg.register.controllers.sb;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegProfilePageForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.RegEmailLogic;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * <action path="/p-reg"
 * type="com.xiaonei.reg.register.action.sb.ProfileRegisterAction"
 * name="regProfilePageForm" scope="request" > <forward name="success"
 * path="/pages/register/register_ok.jsp" /> <forward name="success-qq"
 * path="/pages/register/reg/register_ok_1021.jsp" /> <forward name="err"
 * path="/init/xn6209" /> </action>
 * 
 * @author wangtai
 * 
 */
@Path("p-reg")
public class ProfileRegisterController extends BaseRegSubmitController {

	@Override
	protected String getErrForward() {
		return "err";
	}

	@Override
	public Class<? extends BaseRegForm> getFormClazz() {
		return RegProfilePageForm.class;
	}

	@Override
	protected void addUserTraceLogForward(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {

	}

	@Override
	protected void addUserTraceLogRegSucc(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		String actionStr = request.getParameter("action_id");// 非邀请,对应一般注册的情况
		if (!StringUtils.equals(actionStr, "" + RegOprDefine.STD_REG)) {
			actionStr = "20" + StringUtils.substring(actionStr, 2);
		}
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
	protected RegForward after(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response, User user) {
		request.setAttribute("userId", user.getId());
		return null;
	}

	@Override
	protected RegForward before(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response) {
		return null;
	}

	@Override
	protected IRegisterLogic getRegisterLogic() {
		return RegisterLogicFactory.getRegProfile();
	}

	@Override
	protected RegForward succForward(HttpServletRequest request,
			HttpServletResponse response, RegMapping mapping, User user) {
		/* 激活 */
		try {
			// 发送激活信
			RegEmailLogic.getInstance().sendActivateEmail(user);
		} catch (Exception e) {
			e.printStackTrace();
		}

		request.setAttribute("email", user.getAccount());
		request.setAttribute("emailPage", RegUtil.emailLoginUrl(user
				.getAccount()));
		return mapping.findForward(succForward.toString());
	}

	@Override
	protected Map<String, RegForward> setRegMapping(Map<String, RegForward> map) {
		// <forward name="success" path="/pages/register/register_ok.jsp" />
		// <forward name="success-qq"
		// path="/pages/register/reg/register_ok_1021.jsp" />
		// <forward name="err" path="/init/xn6209" />
		map.put("success", new RegForward("/pages/register/register_ok.jsp"));
		map.put("success-qq", new RegForward("/pages/register/reg/register_ok_1021.jsp"));
		map.put("err", new RegForward("/init/xn6209"));
		return map;
	}
}
