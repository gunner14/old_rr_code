package com.xiaonei.reg.register.controllers.sb;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RunnableTaskExecutor;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegMobileLogic;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * <action path="/s-m-reg"
 * type="com.xiaonei.reg.register.action.sb.ShortMobileRegisterAction"
 * name="regShortCommonForm" scope="request" > <forward name="success"
 * path="/pages/register/register_ok.jsp" /> <forward name="err"
 * path="/init/xn6218" /> </action>
 * 
 * @author tai.wang@opi-corp.com
 */
@Path("/s-m-reg")
public class ShortMobileRegisterController extends BaseRegSubmitController {

	@Override
	public Class<? extends BaseRegForm> getFormClazz() {
		return RegShortCommonForm.class;
	}

	protected IRegisterLogic getRegisterLogic() {
		return RegisterLogicFactory.getRegShortCommon();
	}

	@Override
	protected void addUserTraceLogForward(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		UserTraceLogic.getInstance().insertRegHis(request, user,
				regPojo.getRegEmail(), String.valueOf(290041), 0, null, null,
				baseRegForm.getG(), baseRegForm.getB(), 0, true);
	}

	@Override
	protected void addUserTraceLogRegSucc(RegisterPojo regPojo,
			HttpServletRequest request, User user, BaseRegForm baseRegForm) {
		String actionStr = "22"
				+ request.getParameter("action_id").substring(2);
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
	protected RegForward before(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response) {
		return null;
	}

	@Override
	protected RegForward after(RegMapping mapping, BaseRegForm baseRegForm,
			HttpServletRequest request, HttpServletResponse response,
			final User user) {
		Thread t = new Thread() {
			@Override
			public void run() {
				RegMobileLogic.getInstance().bindMobileAndCreateMMSAlbum(user);
			};
		};
		RunnableTaskExecutor.getInstance().runTask(t);
		return null;
	}

	@Override
	protected RegForward succForward(final HttpServletRequest request,
			final HttpServletResponse response, RegMapping mapping,
			final User user) {
		final ConcurrentHashMap<String, Thread> tasksMap = new ConcurrentHashMap<String, Thread>();
		tasksMap.put("activate user", new Thread() {
			@Override
			public void run() {
				ActivateLogic.getInstance().activateUser(user);
			}
		});
		tasksMap.put("login tiket", new Thread() {
			@Override
			public void run() {
				LoginLogic.getInstance().doLoginR(user, request, response, 0,
						false);
			}
		});
		RunnableTaskExecutor.getInstance().runConcurrentTasks(
				"short mobile activate", tasksMap);
		return new RegForward(Globals.urlWww + "/Home.do", true);
	}

	@Override
	protected Map<String, RegForward> setRegMapping(
			Map<String, RegForward> mapping) {
		// <forward name="success" path="/pages/register/register_ok.jsp" />
		// <forward name="err" path="/init/xn6218" />
		mapping.put("success",
				new RegForward("/pages/register/register_ok.jsp"));
		mapping.put("err", new RegForward("/init/xn6218"));
		return mapping;
	}

}
