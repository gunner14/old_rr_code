/*package com.dodoyo.invite.httpservice.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

public class InviteServiceAction extends ServiceAbstractAction {
//    private static Map<String, String> SERVICE_STATUS_MAP = new HashMap<String, String>();
//    ;

//    private static Properties pp = new Properties();

    static {
        try {
            pp.load(new FileInputStream(com.dodoyo.invite.core.Constants.SERVICE_INTERFACE_CONF_PATH));
            Set<Entry<Object, Object>> kvs = pp.entrySet();
            for (Entry e : kvs) {
                SERVICE_STATUS_MAP.put((String) e.getKey(), (String) e.getValue());
                LogUtils.logDebug("set service status, key = " + e.getKey() + ", value = " + e.getValue());
            }
        }
        catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

//    private static final InviteServiceLogic service = null;//InviteServiceLogicImpl.getInstance();

    @Override
    protected ActionForward hanleRequest(ActionMapping mapping, ActionForm form, HttpServletRequest request, HttpServletResponse response)
            throws Exception {
       // String action = request.getParameter("action");

        //check the service action is open or not
        if (!Constants.SERVICE_STATUS_OPEN.equals(SERVICE_STATUS_MAP.get(action))) {
            showNotAvailable(response, action);
            return null;
        }

        if (Constants.SERVICE_ACTION_LINK.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doLinkInvite(request));
            return null;
        } else if (Constants.SERVICE_ACTION_LINK_ALL.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doLinkInviteAll(request));
            return null;
        } else if (Constants.SERVICE_ACTION_LINK_BATCH.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doLinkInviteBatch(request));
            return null;
        } else if (Constants.SERVICE_ACTION_EMAIL.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doMailInvite(request));
            return null;
        } else if (Constants.SERVICE_ACTION_EXPORT_REQUEST.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doMailExportReq(request));
            return null;
        } else if (Constants.SERVICE_ACTION_EXPORT_QUERY.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doMailExportQuery(request));
            return null;
        } else if (Constants.SERVICE_ACTION_EXPORT_CALLBACK.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doMailExportWithCallback(request));
            return null;
        } else if (Constants.SERVICE_ACTION_EXPORT_INVITE_CALLBACK.equalsIgnoreCase(action)) {
            response.getWriter().print(service.doMailExportInviteWithCallback(request));
            return null;
        }

        response.getWriter().println("<error>" + action + "</error>");
        LogUtils.logErr("REQUEST.SERVICE.ACTION IS NULL ....");
        return null;
    }

    private void showNotAvailable(HttpServletResponse response, String service) throws Exception {
        response.getWriter().print(ResponseUtils.getInstance().getResponseXML(new StringBuffer(""),
                Constants.INVITE_RESULT_NOT_AVAILABLE));
        LogUtils.logDebug("!!!![" + service + "] is not available now ....");
    }
}*/