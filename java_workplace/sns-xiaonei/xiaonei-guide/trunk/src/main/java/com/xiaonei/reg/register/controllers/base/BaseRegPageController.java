package com.xiaonei.reg.register.controllers.base;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.paoding.rose.web.annotation.AsSuperController;

import com.xiaonei.reg.register.form.base.RegForm;

@AsSuperController
public abstract class BaseRegPageController extends BaseRegController {

	@Override
	public RegForward executeRt(RegMapping mapping, RegForm regForm,
			HttpServletRequest request, HttpServletResponse response) {
		return executePt(mapping, regForm, request, response);
	}

	public abstract RegForward executePt(RegMapping mapping, RegForm regForm,
			HttpServletRequest request, HttpServletResponse response);
}
