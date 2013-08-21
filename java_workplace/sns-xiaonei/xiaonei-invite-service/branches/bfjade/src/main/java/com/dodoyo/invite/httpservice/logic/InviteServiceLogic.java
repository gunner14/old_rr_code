package com.dodoyo.invite.httpservice.logic;

import javax.servlet.http.HttpServletRequest;

public interface InviteServiceLogic {
    /**
     * get link invitation URL
     *
     * @param request
     * @return
     */
    public String doLinkInvite(HttpServletRequest request);

    /**
     * get all link invitation URL
     *
     * @param request
     * @return
     */
    public String doLinkInviteAll(HttpServletRequest request);

    /**
     * get link invitation URL for batch
     *
     * @param request
     * @return
     */
    public String doLinkInviteBatch(HttpServletRequest request);

    /**
     * sending mail invitation
     *
     * @param request
     * @return
     */
    public String doMailInvite(HttpServletRequest request);

    /**
     * for page using, step1
     *
     * @param request
     * @return
     */
    public String doMailExportReq(HttpServletRequest request);

    /**
     * for page using, step2
     *
     * @param request
     * @return
     */
    public String doMailExportQuery(HttpServletRequest request);

    /**
     * export contacts, send invite and provide http callback
     *
     * @param request
     * @return
     */
    public String doMailExportInviteWithCallback(HttpServletRequest request);

    /**
     * export contacts and provide http callback
     *
     * @param request
     * @return
     */
	public String doMailExportWithCallback(HttpServletRequest request);
}