package com.kaixin.register.controllers;

import java.io.PrintWriter;
import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;
import com.xiaonei.platform.core.utility.Escape;

/**
 * ajax检测公司名称输入是否含有违禁词
 * 2008-10-8
 * @author wei.cheng@opi-corp.com
 *
 */
@Path({ "AjxRgsWkp.do", "AjaxRegisterWorkplaceAuth.do" })
public class AjxRgsWkpController{

    @SuppressWarnings("deprecation")
    @Get()
    @Post()
    public String index(Invocation inv) throws SQLException {

        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();
		String field = request.getParameter("authType");
		String checkValue = Escape.trim(request.getParameter("value"));
		if (field != null && field.equalsIgnoreCase("workplace")) {
			
			//不合法的输入校验	
			if (checkValue == null || checkValue.length() == 0) {
				output(response, "公司名称不能为空");
				return null;
			}

			CheckResult cr = null;
			cr = AntiSpamAdapter.getInstance().checkAndFilter(0, CheckType.REGISTER_TYPE,
			        checkValue);
			if (cr != null && cr.getFlag() == CheckResult.PROHIBITED) {
				output(response, "公司名称包含不恰当内容，请检查。");
				return null;
			} else if (cr != null && cr.getFlag() == CheckResult.SYSTEM_ERROR) {
				output(response, "公司名称包含不恰当内容，请检查。");
				return null;
			}
			
			output(response, "OKWORKPLACE");
		}

		return null;
	}

	private void output(HttpServletResponse response, String msg)
	{
		response.setContentType("text/html;charset=utf-8");
		try {
			PrintWriter out = response.getWriter();
			out.print(msg);
		} catch(Exception e) {
			e.printStackTrace(System.err);
		}
	}
}
