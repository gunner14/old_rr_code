package com.dodoyo.invite.httpservice.action;

import com.dodoyo.invite.httpservice.logic.ResponseUtils;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.Date;

public class GetTimestampAction extends ServiceAbstractAction {

    @Override
    protected ActionForward hanleRequest(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response)
            throws Exception {

        String timestampString = "<timestamp>" + new Date().getTime() + "</timestamp>";
        timestampString = ResponseUtils.getInstance().getResponseXML(new StringBuffer(timestampString), 0);
        response.getWriter().print(timestampString);
        return null;
    }

}