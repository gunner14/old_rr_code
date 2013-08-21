package com.xiaonei.reg.register.controllers.base;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.AsSuperController;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.form.base.RegForm;
import com.xiaonei.reg.register.logic.additional.RegPortraitLogic;

@AsSuperController
public abstract class BaseRegInitProfileController extends
		BaseRegInitController {
	@Override
	public RegForward executeRIt(RegMapping mapping, RegForm form,
			final HttpServletRequest request, HttpServletResponse response) {
		int userId;
		try {
			userId = Integer.valueOf(StringUtils.isEmpty(request
					.getParameter("id")) ? (String) request.getAttribute("id")
					: request.getParameter("id"));
		} catch (NumberFormatException e) {
			return new RegForward("/init/xn6205");
		}
		final User user;
		int selected = 0;
		try {
			user = (User) UserAdapter.getUser(userId);
			selected = UserAdapter.get(user).getSelected();
		} catch (Exception e) {
			return new RegForward("/init/xn6205");
		}
		if (user == null) {
			return mapping.findForward("success");
		}
		request.setAttribute("user_id", userId);
		request.setAttribute("inviterId", userId);
		request.setAttribute("user_name", user.getName());
		request.setAttribute("is_star", selected);
		request.setAttribute("is_head_url",
				RegPortraitLogic.hasUploaded(user) ? "true" : "");
		RegForward rt = callBackExecute(mapping, form, request, response);
		if (null != rt) {
			return rt;
		}
		return mapping.findForward("success");
	}

	@SuppressWarnings({ "unused"})
	private void setUserProfileInfo(HttpServletRequest request, User user) {
	}

	protected abstract RegForward callBackExecute(RegMapping mapping,
			RegForm form, final HttpServletRequest request,
			HttpServletResponse response);

}
