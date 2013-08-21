package com.xiaonei.platform.component.tools;

import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.opt.base.action.ProtectAction;


public class LoginForwardAction extends ProtectAction {

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws SQLException {
        String path = mapping.getParameter();
        if(path == null)
        {
            return null;
        } else
        {
            ActionForward retVal = new ActionForward(path);
            retVal.setContextRelative(true);
            return retVal;
        }

	}
	
}

