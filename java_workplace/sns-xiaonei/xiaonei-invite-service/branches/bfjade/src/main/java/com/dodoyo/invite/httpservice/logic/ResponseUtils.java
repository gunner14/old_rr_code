package com.dodoyo.invite.httpservice.logic;

import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.entity.ExportRequestParam;
import com.xiaonei.importer.Constants;
import com.xiaonei.importer.entity.ContactInfo;
import com.xiaonei.importer.entity.ImporterContacts;

import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

public class ResponseUtils {
    private static ResponseUtils rsp = new ResponseUtils();

    public static ResponseUtils getInstance() {
        return rsp;
    }

    public String getResponseXMLForLinkInvite(String link, int result) {
        StringBuffer sb = new StringBuffer();

        sb.append("<link>").append(link).append("</link>");

        return getResponseXML(sb, result);
    }

    public String getResponseXMLForLinkInviteAll(Map<String, String> linkMap, int result) {
        if (linkMap == null)
            return "";
        StringBuffer sb = new StringBuffer();

        for (String key : linkMap.keySet())
            sb.append("<link group=\"" + key + "\">").append(linkMap.get(key)).append("</link>");

        return getResponseXML(sb, result);
    }

    public String getResponseXMLForMailInvite(String ss, int result) {
        StringBuffer sb = new StringBuffer();

        sb.append("<ss>").append(ss).append("</ss>");

        return getResponseXML(sb, result);
    }

    public String getResponseXMLForExportContacts(ExportRequestParam exportReqParam,
                                                  ImporterContacts ics, int listType, int result) {
        StringBuffer sb = new StringBuffer();

        if (exportReqParam != null) {
            sb.append("<inviter_id>" + exportReqParam.getInviterId() + "</inviter_id>");
            sb.append("<importer>" + exportReqParam.getEmail() + "</importer>");
            sb.append("<app_id>" + exportReqParam.getAppId() + "</app_id>");
        }

        sb.append("<contacts>");

        if (ics != null) {
            LogUtils.logInfo("++++getResponseXMLForExportContacts, ics.size = " + ics.getBuddies().size());
            Map<String, ContactInfo> buddyMap = null;
            if (listType == Constants.CONTACT_NOREG)
                buddyMap = ics.getNoregBuddies();
            else if (listType == Constants.CONTACT_REG_ALL) {
                buddyMap = ics.getRegUsers();
                buddyMap.putAll(ics.getRegFriends());
            } else if (listType == Constants.CONTACT_REG_FRIENDS)
                buddyMap = ics.getRegFriends();
            else if (listType == Constants.CONTACT_REG_NOFRIENDS)
                buddyMap = ics.getRegUsers();
            else
                buddyMap = ics.getBuddies();

            Set<Entry<String, ContactInfo>> kvSet = buddyMap.entrySet();
            for (Entry<String, ContactInfo> e : kvSet) {
                LogUtils.logInfo("++++++email = " + e.getKey());
                sb.append("<contact id=\"" + e.getValue().getUserId() + "\">");
                sb.append("<name>" + e.getValue().getName() + "</name>");
                sb.append("<email>" + e.getValue().getEmail() + "</email>");
                sb.append("</contact>");
            }
        }
        sb.append("</contacts>");

        return getResponseXML(sb, result);
    }


    public String getResponseXML(StringBuffer inviteData, int result) {
        StringBuffer sb = new StringBuffer();
        sb.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        sb.append("<invite_service>");
        sb.append("<result>" + result + "</result>");
        sb.append(inviteData);
        sb.append("</invite_service>");
        return sb.toString();
    }
}