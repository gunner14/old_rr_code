package com.xiaonei.reg.register.controllers.ajax;

import java.io.PrintWriter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.Path;

import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.utility.Escape;
import com.xiaonei.reg.register.controllers.base.BaseRegAjaxController;
import com.xiaonei.reg.register.controllers.base.RegForward;
import com.xiaonei.reg.register.controllers.base.RegMapping;
import com.xiaonei.reg.register.form.base.RegForm;

/**
 * 		<action path="/AjaxRegisterNameAuth" type="com.xiaonei.reg.register.action.AjaxRegisterNameAction" scope="request" />

 * @author wangtai
 *
 */
@Path("AjaxRegisterNameAuth")
public class AjaxRegisterNameController extends BaseRegAjaxController {
	public RegForward executeRAt(RegMapping mapping, RegForm form,
			HttpServletRequest request, HttpServletResponse response) {

		// if (request.getMethod().equalsIgnoreCase("post")) {

		String field = request.getParameter("authType");
		String checkValue = Escape.trim(request.getParameter("value"));
		if (field != null && field.equalsIgnoreCase("name")) {

			if (checkValue == null || checkValue.length() == 0) {
				output(response, "请输入真实姓名，方便朋友查找。");
				return null;
			}
			if (checkValue.length() > 12) {
				output(response, "姓名不能多于6个汉字或者12个字符。");
				return null;
			}
			// 不合法的输入校验&百家姓校验
			AntiSpamHome ash = AntiSpamHome.getInstance();
			CheckResult cr = null;
			cr = ash.checkRealName(0, checkValue);
			if (cr.getFlag() == CheckResult.PROHIBITED) {
				output(response, "您的输入中可能含有不恰当内容，请检查。");
				return null;
			} else if (cr.getFlag() == CheckResult.CONTENT_NEED_AUDIT) {
				output(response, "请输入真实姓名，方便朋友查找。");
				return null;
			} else if (cr.getFlag() == CheckResult.SYSTEM_ERROR) {
				output(response, "您的输入中可能含有不恰当内容，请检查。");
				return null;
			}
			output(response, "OKNAME");
		}
		// }

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
