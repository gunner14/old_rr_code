package com.dodoyo.invite.core.facade;

import com.dodoyo.invite.core.logic.LinkConfUtils;

import java.util.Map;

public class InviteFacade {
    private InviteFacade() {
    }

    private static InviteFacade inviteFacade = new InviteFacade();

    private static CommonInvite commonInvite = new CommonInvite();

    public static InviteFacade getInviteFacade() {
        return inviteFacade;
    }

    /**
     * For common or app invite mail
     * if templateId is not available, appId will be set as templateId.
     * TemplateId has high priority
     *
     * @param inviterId
     * @param appId
     * @param templateId
     * @param inviteeMail
     * @return
     */
    public String sendInviteMail(int inviterId, String appId, String templateId, String inviteeMail) {
        return commonInvite.sendInviteMail(null, inviterId, appId, templateId, inviteeMail, null);
    }

    public String sendInviteMail(int inviterId, String appId, String templateId, String inviteeMail, Map<String, String> statMap) {
        return commonInvite.sendInviteMail(null, inviterId, appId, templateId, inviteeMail, statMap);
    }

    /**
     * For app invite mail
     *
     * @param inviterId
     * @param appId
     * @param inviteeMail
     * @return
     */
    public String sendInviteMail(int inviterId, String appId, String inviteeMail) {
        return commonInvite.sendInviteMail(null, inviterId, appId, appId, inviteeMail, null);
    }

    public String sendInviteMail(int inviterId, String appId, String inviteeMail, Map<String, String> statMap) {
        return commonInvite.sendInviteMail(null, inviterId, appId, appId, inviteeMail, statMap);
    }

    /**
     * for import send mail using
     *
     * @param importerMail
     * @param inviterId
     * @param appId
     * @param templateId
     * @param inviteeMail
     * @return
     */
    public String sendInviteMailForImport(String importerMail, int inviterId, String appId, String templateId, String inviteeMail) {
        return commonInvite.sendInviteMail(importerMail, inviterId, appId, templateId, inviteeMail, null);
    }

    public String sendInviteMailForImport(String importerMail, int inviterId, String appId, String templateId, String inviteeMail, Map<String, String> statMap) {
        return commonInvite.sendInviteMail(importerMail, inviterId, appId, templateId, inviteeMail, statMap);
    }

    /**
     * for app import send mail using
     *
     * @param importerMail
     * @param inviterId
     * @param appId
     * @param inviteeMail
     * @return
     */
    public String sendInviteMailForImport(String importerMail, int inviterId, String appId, String inviteeMail) {
        return commonInvite.sendInviteMail(importerMail, inviterId, appId, appId, inviteeMail, null);
    }

    public String sendInviteMailForImport(String importerMail, int inviterId, String appId, String inviteeMail, Map<String, String> statMap) {
        return commonInvite.sendInviteMail(importerMail, inviterId, appId, appId, inviteeMail, statMap);
    }

    /**
     * get link's uuid for link copy invite
     *
     * @param inviterId
     * @param appId
     * @param groupName
     * @return
     */
    public String getUUIDForLinkInvite(int inviterId, String appId, String groupName, String ss, String rt) {
        return commonInvite.getURLForLinkInvite("", inviterId, appId, groupName, ss, rt);
    }

    /**
     * get full link for link copy invite
     *
     * @param inviterId
     * @param appId
     * @param groupName
     * @return
     */
    public String getURLForLinkInvite(int inviterId, String appId, String groupName, String ss, String rt) {
        return commonInvite.getURLForLinkInvite(LinkConfUtils.getInstance().getInviteBaseLink(), inviterId, appId, groupName, ss, rt);
    }

    /**
     * get full link for link copy invite
     *
     * @param inviterId
     * @param appId
     * @param groupName
     * @return
     */
    public String getURLForLinkInvite(String baseLink, int inviterId, String appId, String groupName, String ss, String rt) {
        if (baseLink == null || "".equals(baseLink))
            return this.getURLForLinkInvite(inviterId, appId, groupName, ss, rt);
        return commonInvite.getURLForLinkInvite(baseLink, inviterId, appId, groupName, ss, rt);
    }

    public Map<String, String> getURLForLinkInviteAll(String baseLink, int inviterId, String appId)
	{
		return commonInvite.getURLForLinkInviteAll(baseLink, inviterId, appId);
	}
}