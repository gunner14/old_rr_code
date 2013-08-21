package com.xiaonei.login.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.xiaonei.login.controllers.base.AbstractController;

public class TestAbstractController extends AbstractController {

    @Override
    protected String afterLogin(final HttpServletRequest request, final HttpServletResponse response) {
        return "@ok";
    }

    @Override
    protected String beforeLogin(final HttpServletRequest request,
            final HttpServletResponse response) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    protected String fail(final HttpServletRequest request, final HttpServletResponse response,
            final int failCode) {
        return "@fail " + failCode;
    }

    @Override
    protected String getIdentify() {
        // TODO Auto-generated method stub
        return null;
    }

	@Override
	protected int getAegisLoginType() {
		// TODO Auto-generated method stub
		return 0;
	}

}
