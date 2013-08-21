package com.dodoyo.invite.httpservice.controllers;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

//import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.ReqMapping;

import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.Constants;
import com.dodoyo.invite.httpservice.logic.InviteServiceLogic;
import com.dodoyo.invite.httpservice.logic.InviteServiceLogicImpl;
import com.dodoyo.invite.httpservice.logic.ResponseUtils;

@ReqMapping(path={"InviteService.do","inviteService.do","inviteservice.do"})
public class InviteServiceController
{
    private static Map<String, String> SERVICE_STATUS_MAP = new HashMap<String, String>();

    private static Properties pp = new Properties();

    static {
        try {
            pp.load(new FileInputStream(com.dodoyo.invite.core.Constants.SERVICE_INTERFACE_CONF_PATH));
            Set<Entry<Object, Object>> kvs = pp.entrySet();
            for (Entry<Object, Object> e : kvs) {
                SERVICE_STATUS_MAP.put((String) e.getKey(), (String) e.getValue());
                LogUtils.logInfo("set service status, key = " + e.getKey() + ", value = " + e.getValue());
            }
        }
        catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static final InviteServiceLogic service = InviteServiceLogicImpl.getInstance();

/*    @Autowired
    private Invocation iv;*/
    public String get(Invocation iv){return post(iv);}
    
    public String post(Invocation iv)
    {
    	HttpServletRequest request = iv.getRequest();
    	HttpServletResponse response = iv.getResponse();
        String action = request.getParameter("action");
        
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/xml");
//        if (!"POST".equalsIgnoreCase(request.getMethod())) {
//            LogUtils.logDebug("A request from " + InviteUtil.getIpAddress(request) + " with " + request.getRequestURL() + " had been rejected.");
//            return "@<warning>the mothed current used is not supported by this service</warning>";
//        }
        
        //check the service action is open or not
        if (!Constants.SERVICE_STATUS_OPEN.equals(SERVICE_STATUS_MAP.get(action))) {
            return "@" + showNotAvailable(response, action);
        }

        if (Constants.SERVICE_ACTION_LINK.equalsIgnoreCase(action)) {
            return "@" + service.doLinkInvite(request);
        } else if (Constants.SERVICE_ACTION_LINK_ALL.equalsIgnoreCase(action)) {
            return "@" + service.doLinkInviteAll(request);
        } else if (Constants.SERVICE_ACTION_LINK_BATCH.equalsIgnoreCase(action)) {
            return "@" + service.doLinkInviteBatch(request);
        } else if (Constants.SERVICE_ACTION_EMAIL.equalsIgnoreCase(action)) {
            return "@" + service.doMailInvite(request);
        } else if (Constants.SERVICE_ACTION_EXPORT_REQUEST.equalsIgnoreCase(action)) {
            return "@" + service.doMailExportReq(request);
        } else if (Constants.SERVICE_ACTION_EXPORT_QUERY.equalsIgnoreCase(action)) {
            return "@" + service.doMailExportQuery(request);
        } else if (Constants.SERVICE_ACTION_EXPORT_CALLBACK.equalsIgnoreCase(action)) {
            return "@" + service.doMailExportWithCallback(request);
        } else if (Constants.SERVICE_ACTION_EXPORT_INVITE_CALLBACK.equalsIgnoreCase(action)) {
            return "@" + service.doMailExportInviteWithCallback(request);
        }

        LogUtils.logErr("REQUEST.SERVICE.ACTION IS NULL ....");
        return "@" + "<error>" + action + "</error>";
    }

    private String showNotAvailable(HttpServletResponse response, String service)
    {
    	LogUtils.logInfo("!!!![" + service + "] is not available now ....");
    	return ResponseUtils.getInstance().getResponseXML(new StringBuffer(""),
                Constants.INVITE_RESULT_NOT_AVAILABLE);
    }
}