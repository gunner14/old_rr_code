package com.xiaonei.reg.register.controllers.sb;

import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.reg.common.RegUtil;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.register.controllers.base.BaseRegSubmitController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.RegPureIDCommonForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.IRegisterLogic;
import com.xiaonei.reg.register.logic.RegisterLogicFactory;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * 		<action path="/s-c-p-i-reg" type="com.xiaonei.reg.register.action.sb.ShortPureIDCommonRegisterAction" name="regPureIDCommonForm" scope="request" >
			<forward name="success" path="/pages/register/register_ok.jsp" />
			<forward name="success-qq" path="/pages/register/reg/register_ok_1105.jsp" />
			<forward name="err" path="/init/xn5205" />
		</action>
 * @author tai.wang@opi-corp.com
 */
@Path("s-c-p-i-reg")
public class ShortPureIDCommonRegisterController extends BaseRegSubmitController {

    // 需要完成跳转
    private static final String errForward = "err";

    @Override
    protected String getErrForward() {
        return errForward;
    }

    @Override
	public Class<? extends BaseRegForm> getFormClazz() {
        return RegPureIDCommonForm.class;
    }

    @Override
    protected void addUserTraceLogForward(RegisterPojo regPojo, HttpServletRequest request,
            User user, BaseRegForm baseRegForm) {
        try {
            // 邀请链接上的邀请人id
            int ivId = 0;
            String i = request.getParameter("i");
            if (StringUtils.isNotEmpty(i)) {
                ivId = StringUtils.isNumeric(i) ? Integer.parseInt(i) : 0;
            }

            String actionStr = String.valueOf(RegOprDefine.AUTO_MAIL_ACTIVE);
            actionStr = String.valueOf(RegOprDefine.AUTO_XID_ACTIVE);
            String source = request.getParameter("ss");
            UserTraceLogic.getInstance().insertRegHis(request, user, user.getAccount(), actionStr,
                    0, source, null, baseRegForm.getG(), baseRegForm.getB(), ivId, true);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    protected void addUserTraceLogRegSucc(RegisterPojo regPojo, HttpServletRequest request,
            User user, BaseRegForm baseRegForm) {
        String actionStr = request.getParameter("action_id");// 非邀请,对应一般注册的情况
        String source = request.getParameter("ss");
        UserTraceLogic.getInstance().insertRegHis(request, user, regPojo.getRegEmail(), actionStr,
                0, source, null, baseRegForm.getG(), baseRegForm.getB(), 0, true);
        
        //2010.03.25修改
        actionStr = String.valueOf(RegOprDefine.NORMAL_REG_OK);
        if (StringFormat.isValidEmailAddr(user.getAccount())) {
            // do nothing
        } else if (StringFormat.isValidMobile(user.getAccount())) {
            actionStr = String.valueOf(RegOprDefine.NORMAL_MOBILE_REG_OK);
        } else {
            actionStr = String.valueOf(RegOprDefine.NORMAL_ID_REG_OK);
        }
        UserTraceLogic.getInstance().insertRegHis(request, user, regPojo.getRegEmail(),
                actionStr, 0, source, null, baseRegForm.getG(),
                baseRegForm.getB(), 0, true);
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
    protected RegForward succForward(HttpServletRequest request, HttpServletResponse response,
            RegMapping mapping, User user) {
        request.setAttribute("userId", user.getId());
        request.setAttribute("email", user.getAccount());
        request.setAttribute("emailPage", RegUtil.emailLoginUrl(user.getAccount()));
        try {
            ActivateLogic.getInstance().activateUser(user);
            LoginLogic.getInstance().doLoginR(user, request, response, 0, false);
            return new RegForward(Globals.urlWww + "/Home.do", true);
        } catch (Exception e) {}

        return mapping.findForward(succForward.toString());
    }

	@Override
	protected Map<String, RegForward> setRegMapping(Map<String, RegForward> m) {
//		<forward name="success" path="/pages/register/register_ok.jsp" />
//		<forward name="success-qq" path="/pages/register/reg/register_ok_1105.jsp" />
//		<forward name="err" path="/init/xn5205" />
		m.put("success", new RegForward("/pages/register/register_ok.jsp"));
		m.put("success-qq", new RegForward("/pages/register/reg/register_ok_1105.jsp"));
		m.put("err", new RegForward("/init/xn5205"));
		return m;
	}

}
