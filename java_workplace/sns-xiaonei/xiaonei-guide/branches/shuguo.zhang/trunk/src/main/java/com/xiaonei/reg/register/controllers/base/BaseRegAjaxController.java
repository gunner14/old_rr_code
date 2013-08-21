package com.xiaonei.reg.register.controllers.base;

import java.io.PrintWriter;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.AsSuperController;

import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.register.form.base.RegForm;

@AsSuperController
public abstract class BaseRegAjaxController extends BaseRegController{

	@Override
	public RegForward executeRt(RegMapping RegMapping,
			RegForm RegForm, HttpServletRequest httpservletrequest,
			HttpServletResponse httpservletresponse) {
		return executeRAt(RegMapping, RegForm, httpservletrequest,
				httpservletresponse);
	}

	public abstract RegForward executeRAt(RegMapping mapping,
			RegForm form, HttpServletRequest request,
			HttpServletResponse response);

	/**
	 * AJAX返返回数据
	 * 
	 * @param request
	 * @param response
	 * @param msg
	 * @return
	 */
	protected RegForward output(HttpServletRequest request,
			HttpServletResponse response, String msg) {
		response.setContentType("text/html;charset=utf-8");
		try {
			PrintWriter out = response.getWriter();
			out.print(msg);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}
	protected RegForward output(HttpServletRequest request,
			HttpServletResponse response, int msg) {
		return output(request, response, ""+msg);
	}

	/**
	 * ajax 回调函数
	 * 
	 * @param request
	 * @param response
	 * @param msg
	 * @return
	 */
	protected RegForward callback(HttpServletRequest request,
			HttpServletResponse response, String msg) {
		response.setContentType("text/html;charset=utf-8");
		try {
			PrintWriter out = response.getWriter();
			out
					.print("<script>document.domain=\""+Globals.domain+"\";parent.callback(\""
							+ msg + "\");</script>");
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}
	@Override
	final protected Map<String, RegForward> setRegMapping(Map<String, RegForward> m) {
		return null;
	}

}
