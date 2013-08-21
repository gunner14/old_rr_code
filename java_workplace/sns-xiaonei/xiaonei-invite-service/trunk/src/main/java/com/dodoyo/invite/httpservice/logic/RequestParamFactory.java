package com.dodoyo.invite.httpservice.logic;

import com.dodoyo.invite.core.InviteUtil;
import com.dodoyo.invite.httpservice.entity.ExportRequestParam;
import com.dodoyo.invite.httpservice.entity.LinkRequestParam;
import com.dodoyo.invite.httpservice.entity.MailRequestParam;
import com.dodoyo.invite.httpservice.excep.LinkInviteException;
import com.dodoyo.invite.httpservice.excep.MailExportException;
import com.dodoyo.invite.httpservice.excep.MailInviteException;

import javax.servlet.http.HttpServletRequest;
import java.util.HashMap;
import java.util.Map;

public class RequestParamFactory {
    private static RequestParamFactory rpf = new RequestParamFactory();

    public static RequestParamFactory getInstance() {
        return rpf;
    }

    /**
     * generate object for link request
     *
     * @param request
     * @return
     */
    public LinkRequestParam getLinkRequestParam(HttpServletRequest request) {
        LinkRequestParam linkReqObj = new LinkRequestParam();
        try {
            linkReqObj.setInviterId(Integer.parseInt(request.getParameter("inviter_id")));
            linkReqObj.setAppId(request.getParameter("app_id"));
            linkReqObj.setGroupName(request.getParameter("group_name"));
            String baseLink = request.getParameter("base_link");
            linkReqObj.setBaseLink(baseLink);
            linkReqObj.setTimestamp(request.getParameter("timestamp"));
            linkReqObj.setToken(request.getParameter("token"));
            linkReqObj.setClientIp(InviteUtil.getIpAddress(request));
            linkReqObj.setAction(request.getParameter("action"));
            linkReqObj.setSs(request.getParameter("ss"));
            linkReqObj.setRt(request.getParameter("rt"));
//            //添加扩展参数，用来直接接在邀请链接后面。 20100903 by sj
//            linkReqObj.setExp(request.getParameter("exp"));
        }
        catch (Exception e) {
            throw new LinkInviteException("getLinkRequestParam exception", e);
        }

        return linkReqObj;
    }

    /**
     * generate object for mail request
     *
     * @param request
     * @return
     */
    public MailRequestParam getMailRequestParam(HttpServletRequest request) {
        MailRequestParam mailReqObj = new MailRequestParam();
        try {
            mailReqObj.setTemplateId(request.getParameter("template_id"));
            mailReqObj.setInviterId(Integer.parseInt(request.getParameter("inviter_id")));
            mailReqObj.setAppId(request.getParameter("app_id"));
            mailReqObj.setInviteeMail(request.getParameter("invitee_mail"));
            mailReqObj.setTimestamp(request.getParameter("timestamp"));
            mailReqObj.setToken(request.getParameter("token"));
            mailReqObj.setClientIp(InviteUtil.getIpAddress(request));
            mailReqObj.setAction(request.getParameter("action"));
            mailReqObj.setFromer(request.getParameter("importer_mail"));
            Map<String, String> statMap = new HashMap<String, String>();
            statMap.put("invite_type", request.getParameter("invite_type"));
            statMap.put("enter_type", request.getParameter("enter_type"));
            statMap.put("action_type", request.getParameter("action_type"));
            mailReqObj.setStatMap(statMap);
        }
        catch (Exception e) {
            throw new MailInviteException("getMailRequestParam exception", e);
        }
        return mailReqObj;
    }

    /**
     * generate objec for export request
     *
     * @param request
     * @return
     */
    public ExportRequestParam getExportRequestParam(HttpServletRequest request) {
        ExportRequestParam exportReqObj = new ExportRequestParam();
        Map<String, Object> attMap = new HashMap<String, Object>();
        try {
            exportReqObj.setAction(request.getParameter("action"));
            exportReqObj.setEmail(request.getParameter("email"));
            exportReqObj.setHttpCallback(request.getParameter("callback"));
            exportReqObj.setInviterId(Integer.parseInt(request.getParameter("inviter_id")));
            exportReqObj.setMsn("1".equals(request.getParameter("is_msn")));
            String password = request.getParameter("password");
//			if (password != null)
//				password = EncryptUtil.getInstance().decodeBase64(password);
            exportReqObj.setPassword(password);
            exportReqObj.setAppId(request.getParameter("app_id"));
            exportReqObj.setTimestamp(request.getParameter("timestamp"));
            exportReqObj.setToken(request.getParameter("token"));
            exportReqObj.setClientIp(InviteUtil.getIpAddress(request));
            String listType = request.getParameter("list_type");
            if (listType == null || "".equals(listType))
                exportReqObj.setListType(0);
            else
                exportReqObj.setListType(Integer.parseInt(listType));

            //add for parameter extentions
            attMap.put("APP_ID", request.getParameter("app_id"));
            attMap.put("TEMPLATE_ID", request.getParameter("template_id"));
            attMap.put("ACTION_TYPE", request.getParameter("action_type"));
            exportReqObj.setAttMap(attMap);

        }
        catch (Exception e) {
            throw new MailExportException("getExportRequestParam() exception", e);
		}
		
		return exportReqObj;
	}
}