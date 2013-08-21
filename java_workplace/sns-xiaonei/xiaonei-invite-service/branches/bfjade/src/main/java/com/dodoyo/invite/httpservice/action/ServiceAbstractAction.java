package com.dodoyo.invite.httpservice.action;

import com.dodoyo.invite.core.InviteUtil;
import com.dodoyo.invite.core.logic.LogUtils;
import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


/**
 * POST method only
 *
 * @author Administrator
 */
public abstract class ServiceAbstractAction extends Action {
    @Override
    public ActionForward execute(ActionMapping mapping,
                                 ActionForm form,
                                 HttpServletRequest request,
                                 HttpServletResponse response) throws Exception {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/xml");
        if (!"POST".equalsIgnoreCase(request.getMethod())) {
            LogUtils.logDebug("A request from " + InviteUtil.getIpAddress(request) + " with " + request.getRequestURL() + " had been rejected.");
            response.getWriter().print("<warning>the mothed current used is not supported by this service</warning>");
            return null;
        }

        long costTm = System.currentTimeMillis();

        ActionForward actionForward = null;

        try {
            actionForward = this.hanleRequest(mapping, form, request, response);
        }
        catch (Throwable e) {
            e.printStackTrace();
            LogUtils.logErr(e.getMessage());
        }

        long costTm2 = System.currentTimeMillis();
        LogUtils.logInfo("service abstract action cost time = " + (costTm2 - costTm));

        return actionForward;
    }

    protected abstract ActionForward hanleRequest(ActionMapping mapping,
                                                  ActionForm form,
                                                  HttpServletRequest request,
                                                  HttpServletResponse response)
            throws Exception;

}
