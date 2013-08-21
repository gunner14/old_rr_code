package com.dodoyo.invite.httpservice.controllers;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.dodoyo.invite.core.logic.LogUtils;


/**
 * @author Administrator
 */
public class ServiceManageAction extends Action {
//    private static final ClientIPDao clientDao = null;//new ClientIPDao();
//
//    private static final ServiceStatusLogic statusLogic = ServiceStatusLogic.getInstance();

    @Override
    public ActionForward execute(ActionMapping mapping,
                                 ActionForm form,
                                 HttpServletRequest request,
                                 HttpServletResponse response) throws Exception {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");

        ActionForward actionForward = null;

        try {
            actionForward = this.hanleRequest(mapping, form, request, response);
        }
        catch (Throwable e) {
            e.printStackTrace();
            LogUtils.logErr(e.getMessage());
        }

        return actionForward;
    }

    protected ActionForward hanleRequest(ActionMapping mapping,
                                         ActionForm form,
                                         HttpServletRequest request,
                                         HttpServletResponse response)
            throws Exception {
        /*String action = request.getParameter("action");
        LogUtils.logDebug("current action = " + action);
        if (!"login".equals(action) && request.getSession().getAttribute("admin") == null) {
            LogUtils.logWarn("not logined, return to login page ...., session = " + request.getSession().getAttribute("admin"));
            return mapping.findForward("login");
        }

        if ("login".equals(action)) {
            LogUtils.logDebug("do login ....");
            String userName = request.getParameter("user_name");
            String password = request.getParameter("password");
            LogUtils.logDebug("userName = " + userName + ", password = " + password);
            if ("admin".equals(userName) && "pass".equals(password)) {
                request.getSession(true).setAttribute("admin", userName);
                LogUtils.logDebug("login success, session = " + request.getSession().getAttribute("admin"));
                return new ActionForward("/ServiceManage.do?action=client_list", true);
            }
            return mapping.findForward("login");
        } else if ("client_list".equals(action)) {
            List<ClientInfo> clientList = clientDao.getClientInfoList();
            request.setAttribute("clientList", clientList);
            request.setAttribute("actionList", statusLogic.getServiceStatus());

            return mapping.findForward("client_list");
        } else if ("logout".equals(action)) {
            request.getSession().removeAttribute("admin");
            return mapping.findForward("login");
        } else if ("add".equals(action)) {
            String clientName = request.getParameter("client_name");
            String clientIp = request.getParameter("client_ip");
            String clientKey = request.getParameter("client_key");
            String type = request.getParameter("type");
            String expireTime = request.getParameter("expire_time");
            ClientInfo ci = new ClientInfo();
            ci.setClientIp(clientIp);
            ci.setClientName(clientName);
            ci.setClientKey(clientKey);
            ci.setType("1".equals(type) ? 1 : 0);
            ci.setExpireTime(Timestamp.valueOf(expireTime));
            clientDao.addClientInfo(ci);
            return new ActionForward("/ServiceManage.do?action=client_list", true);
        } else if ("modshow".equals(action)) {
            String ip = request.getParameter("client_ip");
            ClientInfo ci = clientDao.getClientInfo(ip);
            request.setAttribute("clientInfo", ci);
            return new ActionForward("/ServiceManage.do?action=client_list", true);
        } else if ("modsub".equals(action)) {
            String clientName = request.getParameter("client_name");
            String clientIp = request.getParameter("client_ip");
            String clientKey = request.getParameter("client_key");
            String type = request.getParameter("type");
            String expireTime = request.getParameter("expire_time");

            ClientInfo ci = clientDao.getClientInfo(clientIp);
            ci.setClientIp(clientIp);
            ci.setClientName(clientName);
            ci.setClientKey(clientKey);
            ci.setType("1".equals(type) ? 1 : 0);
            ci.setExpireTime(Timestamp.valueOf(expireTime));

            clientDao.updateClientInfo(ci);

            return new ActionForward("/ServiceManage.do?action=client_list", true);
        } else if ("disable".equals(action)) {
            String clientIp = request.getParameter("client_ip");
            ClientInfo ci = clientDao.getClientInfo(clientIp);
            ci.setType(0);

            clientDao.updateClientInfo(ci);

            return new ActionForward("/ServiceManage.do?action=client_list", true);
        } else if ("enable".equals(action)) {
            String clientIp = request.getParameter("client_ip");
            ClientInfo ci = clientDao.getClientInfo(clientIp);
            ci.setType(1);

            clientDao.updateClientInfo(ci);

            return new ActionForward("/ServiceManage.do?action=client_list", true);
        } else if ("action_status_reload".equals(action)) {
            statusLogic.initServiceStatus();
            return new ActionForward("/ServiceManage.do?action=client_list", true);
        }*/

        return null;
    }

}
