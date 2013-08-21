package com.xiaonei.reg.register.controllers.ajax;

import java.io.PrintWriter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.controllers.base.BaseRegAjaxController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.logic.pretreatment.validate.CheckRegEmail;

/**
 * 		<action path="/AjaxRegisterEmailAuth" type="com.xiaonei.reg.register.action.AjaxRegisterEmailAction" scope="request" />

 * @author wangtai
 *
 */
@Path("AjaxRegisterEmailAuth")
public class AjaxRegisterEmailController extends BaseRegAjaxController {
	
	public RegForward executeRAt(RegMapping mapping, RegForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String field = request.getParameter("authType");
		String checkValue = Escape.trim(request.getParameter("value"));

		if (field != null && field.equalsIgnoreCase("email")) {
			String cn = (new CheckRegEmail().check(checkValue, IAccountType.EMAIL));
			if (!IRegCheckCodeDef.OK.equals(cn)) {
				output(response, cn);
			}
			output(response, "OKEMAIL");
		}

		return null;
	}

	private void output(HttpServletResponse response, String msg) {
		response.setContentType("text/html;charset=utf-8");
		try {
			PrintWriter out = response.getWriter();
			out.print(msg);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
}