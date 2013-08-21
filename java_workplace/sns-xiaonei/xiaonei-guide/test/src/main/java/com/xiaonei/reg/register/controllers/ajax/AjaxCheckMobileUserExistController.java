/**
 *
 */
package com.xiaonei.reg.register.controllers.ajax;

import java.io.PrintWriter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.controllers.base.BaseRegAjaxController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.logic.additional.LoginLogic;
import com.xiaonei.reg.usertrace.logic.UserTraceLogic;
import com.xiaonei.reg.usertrace.model.RegOprDefine;

/**
 * 		<action path="/acu" type="com.xiaonei.reg.register.action.AjaxCheckMobileUserExist" />

 * AjaxCheckMobileUserExist
 * 
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-16 - 下午05:51:50
 */
@Path("acu")
public class AjaxCheckMobileUserExistController extends BaseRegAjaxController {

	private static final String MSG_ERR = "0";
	private static final String MSG_NO_USER = "1";
	private static final String MSG_OK = "2";

	@Override
	public RegForward executeRAt(RegMapping mapping, RegForm form,
			HttpServletRequest request, HttpServletResponse response){
		String mobile = StringUtils.trimToEmpty(request.getParameter("mobile"));
		if (StringUtils.isEmpty(mobile)) {
			return output(response, MSG_ERR);
		}

		if (false == UserAdapter.hasUser(mobile)) {
			return output(response, MSG_NO_USER);
		}
		// 自动登录
		try {
			User user = UserAdapter.getUser(mobile, Globals.domain.toString());
			try {
				// 用户行为日志跟踪
				UserTraceLogic.getInstance().insertRegHis(request, null,
						user.getAccount(),
						String.valueOf(RegOprDefine.MOBILE_AUDIT_OK),
						UserAdapter.get(user).getStage(), "", null, null, null,
						0, true);
				// 用户行为日志跟踪
				UserTraceLogic.getInstance().insertRegHis(request, null,
						user.getAccount(),
						String.valueOf(RegOprDefine.UP_CODE_ACTIVE),
						UserAdapter.get(user).getStage(), "", null, null, null,
						0, true);
			} catch (Exception e) {
				e.printStackTrace();
			}
			LoginLogic.getInstance().doLoginR(user, request, response, 0, false);
		} catch (Exception e) {
			// set cookie
			CookieManager.getInstance().saveCookie(response, "_de", mobile);
			addErr(request, "激活成功，请登录");
			// 出错了,去手动登录页面
			e.printStackTrace();
		}
		return output(response, MSG_OK);
	}

	private RegForward output(HttpServletResponse response, String msg) {
		response.setContentType("text/html;charset=utf-8");
		try {
			PrintWriter out = response.getWriter();
			out.print(msg);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}
}
