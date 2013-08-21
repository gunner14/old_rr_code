package com.xiaonei.login.logic.status;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts.action.ActionMessage;
import org.apache.struts.action.ActionMessages;
import org.springframework.stereotype.Service;


@Service
public class PassportMessageImpl implements PassportMessage {

	@Override
	public void addErrorMessage(HttpServletRequest request, String message) {
		ActionMessages ams = new ActionMessages();
		ActionMessage am = new ActionMessage("errors.detail", message);
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		request.setAttribute(org.apache.struts.Globals.ERROR_KEY, ams);
	}

	@Override
	public void addWarningMessage(HttpServletRequest request, String message) {
		ActionMessages ams = new ActionMessages();
		ActionMessage am = new ActionMessage("errors.detail", message);
		ams.add(ActionMessages.GLOBAL_MESSAGE, am);
		request.setAttribute(org.apache.struts.Globals.MESSAGE_KEY, ams);
	}

}
