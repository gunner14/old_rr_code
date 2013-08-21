package com.xiaonei.reg.guide.action.netbarflow;

import java.sql.SQLException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.action.ProtectAction;

public class FillinfoInitAction extends ProtectAction{

	private final static String INIT = "init";
	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
		NetbarUtil.setInitAttribute(request);
		return mapping.findForward(INIT);
	}

}
