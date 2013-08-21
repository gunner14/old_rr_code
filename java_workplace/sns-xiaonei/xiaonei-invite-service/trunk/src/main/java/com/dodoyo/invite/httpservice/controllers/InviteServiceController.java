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

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.Constants;
import com.dodoyo.invite.httpservice.logic.InviteServiceLogic;
import com.dodoyo.invite.httpservice.logic.ResponseUtils;

@Path( { "InviteService.do", "inviteService.do", "inviteservice.do" })
public class InviteServiceController {
	private static Map<String, String> SERVICE_STATUS_MAP = new HashMap<String, String>();

	private static Properties pp = new Properties();

	static {
		FileInputStream fis = null;
		try {
			fis = new FileInputStream(com.dodoyo.invite.core.Constants.SERVICE_INTERFACE_CONF_PATH);
			pp.load(fis);
			Set<Entry<Object, Object>> kvs = pp.entrySet();
			for (Entry<Object, Object> e : kvs) {
				SERVICE_STATUS_MAP.put((String) e.getKey(), (String) e.getValue());
				LogUtils.logDebug("set service status, key = " + e.getKey() + ", value = " + e.getValue());
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (fis != null) {
				try {
					fis.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

	@Autowired
	private InviteServiceLogic inviteServiceLogic;

	public void setTestInviteServiceLogic(InviteServiceLogic inviteServiceLogic) {
		this.inviteServiceLogic = inviteServiceLogic;
	}

	@Get
    @Post
	public String post(Invocation iv) {
		LogUtils.logDebug("___InviteServiceController post");
		HttpServletRequest request = iv.getRequest();
		HttpServletResponse response = iv.getResponse();
		String action = request.getParameter("action");
		response.setCharacterEncoding("UTF-8");
		response.setContentType("text/xml");
		//check the service action is open or not
		if (!Constants.SERVICE_STATUS_OPEN.equals(SERVICE_STATUS_MAP.get(action))) {
			return "@" + showNotAvailable(/*response, */action);
		}
		if (Constants.SERVICE_ACTION_LINK.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doLinkInvite(request);
		} else if (Constants.SERVICE_ACTION_LINK_ALL.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doLinkInviteAll(request);
		} else if (Constants.SERVICE_ACTION_LINK_BATCH.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doLinkInviteBatch(request);
		} else if (Constants.SERVICE_ACTION_EMAIL.equalsIgnoreCase(action)) {
			LogUtils.logDebug("___inviteServiceLogic.doMailInvite(request)");
			return "@" + inviteServiceLogic.doMailInvite(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_REQUEST.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportReq(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_QUERY.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportQuery(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_CALLBACK.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportWithCallback(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_INVITE_CALLBACK.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportInviteWithCallback(request);
		}
		LogUtils.logErr("REQUEST.SERVICE.ACTION IS NULL ....");
		return "@" + "<error>" + action + "</error>";
	}

	private String showNotAvailable(String service) {
		LogUtils.logDebug("!!!![" + service + "] is not available now ....");
		return ResponseUtils.getInstance().getResponseXML(new StringBuffer(""), Constants.INVITE_RESULT_NOT_AVAILABLE);
	}

	public String fortest(HttpServletRequest request, HttpServletResponse response) {
		LogUtils.logDebug("___InviteServiceController fortest");
		String action = request.getParameter("action");
		response.setCharacterEncoding("UTF-8");
		response.setContentType("text/xml");
		//check the service action is open or not
		if (!Constants.SERVICE_STATUS_OPEN.equals(SERVICE_STATUS_MAP.get(action))) {
			return "@" + showNotAvailable(action);
		}
		if (Constants.SERVICE_ACTION_LINK.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doLinkInvite(request);
		} else if (Constants.SERVICE_ACTION_LINK_ALL.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doLinkInviteAll(request);
		} else if (Constants.SERVICE_ACTION_LINK_BATCH.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doLinkInviteBatch(request);
		} else if (Constants.SERVICE_ACTION_EMAIL.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailInvite(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_REQUEST.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportReq(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_QUERY.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportQuery(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_CALLBACK.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportWithCallback(request);
		} else if (Constants.SERVICE_ACTION_EXPORT_INVITE_CALLBACK.equalsIgnoreCase(action)) {
			return "@" + inviteServiceLogic.doMailExportInviteWithCallback(request);
		}
		LogUtils.logErr("REQUEST.SERVICE.ACTION IS NULL ....");
		return "@" + "<error>" + action + "</error>";
	}
}