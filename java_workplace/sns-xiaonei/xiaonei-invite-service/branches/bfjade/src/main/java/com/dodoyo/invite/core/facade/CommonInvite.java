package com.dodoyo.invite.core.facade;

import com.dodoyo.invite.core.entity.InviteParams;
import com.dodoyo.invite.core.logic.InviteMailLogic;
import com.dodoyo.invite.core.logic.UUIDUtils;

import java.util.Map;

public class CommonInvite {
    protected CommonInvite() {
    }

    protected String sendInviteMail(String importerMail, int inviterId, String appId, String templateId, String inviteeMail, Map<String, String> statMap) {
        InviteParams ip = new InviteParams();
        ip.setInviterId(inviterId);
        ip.setAppId(appId);
        if (templateId == null || "".equals(templateId))
            ip.setTemplateId(appId);
        else
            ip.setTemplateId(templateId);
        ip.setInviteeMail(inviteeMail);
        ip.setImporterMail(importerMail);
        ip.setStatMap(statMap);

        return InviteMailLogic.getInstance().sendInviteMail(ip);
    }

    protected String getURLForLinkInvite(String baseLink, int inviterId, String appId, String groupName, String ss, String rt) {
        return baseLink + UUIDUtils.getInstance().genUUIDForLinkInvite(inviterId, appId, groupName, ss, rt);
    }

    protected Map<String, String> getURLForLinkInviteAll(String baseLink, int inviterId, String appId) {
        return UUIDUtils.getInstance().genUUIDForLinkInviteAll(inviterId, appId);
    }
}