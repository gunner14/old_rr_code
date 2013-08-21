package com.dodoyo.invite.httpservice.logic;

import com.dodoyo.invite.core.facade.InviteFacade;
import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.Constants;
import com.dodoyo.invite.httpservice.auth.AuthLogic;
import com.dodoyo.invite.httpservice.entity.ExportRequestParam;
import com.dodoyo.invite.httpservice.entity.LinkRequestParam;
import com.dodoyo.invite.httpservice.entity.MailRequestParam;
import com.xiaonei.importer.entity.ImporterContacts;

import javax.servlet.http.HttpServletRequest;
import java.util.Map;

public class InviteServiceLogicImpl implements InviteServiceLogic {
    private InviteServiceLogicImpl() {
    }

    private static InviteServiceLogicImpl isl = new InviteServiceLogicImpl();

    public static InviteServiceLogicImpl getInstance() {
        return isl;
    }

    /**
     * get link invitation URL
     *
     * @param request
     * @return
     */
    public String doLinkInvite(HttpServletRequest request) {
        String link = "";

        try {
            //get parameters
            LinkRequestParam linkReqObj = RequestParamFactory.getInstance().getLinkRequestParam(request);
            LogUtils.logDebug("link request datas: " + linkReqObj);

            //authentications
            int authRet = AuthLogic.getInstance().linkAuthLogic(linkReqObj);
            if (authRet != Constants.INVITE_RESULT_SUCCESS) {
                return ResponseUtils.getInstance().getResponseXMLForLinkInvite(link, authRet);
            }

            //send link request with core facade
            link = InviteFacade.getInviteFacade().getURLForLinkInvite(linkReqObj.getBaseLink(),
                    linkReqObj.getInviterId(), linkReqObj.getAppId(), linkReqObj.getGroupName(),
                    linkReqObj.getSs(), linkReqObj.getRt());
        }
        catch (Exception e) {
            LogUtils.logErr("doLinkInvite exception ....");
            LogUtils.logExceptionStack(e);
            e.printStackTrace(System.err);
            return ResponseUtils.getInstance().getResponseXMLForLinkInvite(link, Constants.INVITE_RESULT_EXCP);
        }

        return ResponseUtils.getInstance().getResponseXMLForLinkInvite(link, Constants.INVITE_RESULT_SUCCESS);
    }

    /**
     * get all link invitation URL
     */
    public String doLinkInviteAll(HttpServletRequest request) {
        Map<String, String> linkMap = null;

        try {
            //get parameters
            LinkRequestParam linkReqObj = RequestParamFactory.getInstance().getLinkRequestParam(request);
            LogUtils.logDebug("link request datas: " + linkReqObj);

            //authentications
            int authRet = AuthLogic.getInstance().linkAuthLogic(linkReqObj);
            if (authRet != Constants.INVITE_RESULT_SUCCESS) {
                return ResponseUtils.getInstance().getResponseXMLForLinkInviteAll(linkMap, authRet);
            }

            //send link request with core facade
            linkMap = InviteFacade.getInviteFacade().getURLForLinkInviteAll(linkReqObj.getBaseLink(), linkReqObj.getInviterId(), linkReqObj.getAppId());
        }
        catch (Exception e) {
            LogUtils.logErr("doLinkInvite exception ....");
            LogUtils.logExceptionStack(e);
            e.printStackTrace(System.err);
            return ResponseUtils.getInstance().getResponseXMLForLinkInviteAll(linkMap, Constants.INVITE_RESULT_EXCP);
        }

        return ResponseUtils.getInstance().getResponseXMLForLinkInviteAll(linkMap, Constants.INVITE_RESULT_SUCCESS);
    }

    /**
     * get batch link invitation URLS
     */
    public String doLinkInviteBatch(HttpServletRequest request) {
        return null;
    }

    /**
     * sending mail invitation
     *
     * @param request
     * @return
     */
    public String doMailInvite(HttpServletRequest request) {
        String ss = "";
        try {
            //get paramters
            MailRequestParam mailReqObj = RequestParamFactory.getInstance().getMailRequestParam(request);
            LogUtils.logDebug("mail request datas: " + mailReqObj);

            //authentications
            int authRet = AuthLogic.getInstance().mailAuthLogic(mailReqObj);
            if (authRet != Constants.INVITE_RESULT_SUCCESS) {
                return ResponseUtils.getInstance().getResponseXMLForMailInvite(ss, authRet);
            }

            //send mail request with core facade
            long dt = System.currentTimeMillis();
            String[] mailArr = mailReqObj.getInviteeMail().split(";");
            for (String mail : mailArr) {
                if ("".equals(mail)) continue;
                ss = InviteFacade.getInviteFacade().sendInviteMailForImport(mailReqObj.getFromer(), mailReqObj.getInviterId(),
                        mailReqObj.getAppId(), mailReqObj.getTemplateId(), mail, mailReqObj.getStatMap());
            }
            LogUtils.logDebug("invite mail of core cost:" + (System.currentTimeMillis() - dt));

        }
        catch (Exception e) {
            e.printStackTrace(System.err);
            LogUtils.logExceptionStack(e);
            LogUtils.logInfo("do mail invite service exeception, return INVITE_RESULT_EXCP");
            return ResponseUtils.getInstance().getResponseXMLForMailInvite(ss, Constants.INVITE_RESULT_EXCP);
        }

        return ResponseUtils.getInstance().getResponseXMLForMailInvite(ss, Constants.INVITE_RESULT_SUCCESS);
    }

    /**
     * for page using, step1
     *
     * @param request
     * @return
     */
    public String doMailExportReq(HttpServletRequest request) {
        ExportRequestParam exportReqObj = null;
        try {
            exportReqObj = RequestParamFactory.getInstance().getExportRequestParam(request);
            LogUtils.logDebug("doMailExportReq datas: " + exportReqObj);

            return sendExportRequest(exportReqObj, false, false);
        }
        catch (Exception e) {
            e.printStackTrace(System.err);
            LogUtils.logExceptionStack(e);
            LogUtils.logInfo("do mail export request service exeception, return INVITE_RESULT_EXCP");
            return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, null, 0, Constants.INVITE_RESULT_EXCP);
        }

    }

    /**
     * for page using, step2
     *
     * @param request
     * @return
     */
    public String doMailExportQuery(HttpServletRequest request) {
        ImporterContacts ics = null;
        ExportRequestParam exportReqObj = null;
        try {
            //get paramters
            exportReqObj = RequestParamFactory.getInstance().getExportRequestParam(request);
            LogUtils.logInfo("doMailExportQuery datas: " + exportReqObj);

            //authentications
            int authRet = AuthLogic.getInstance().mailExportQueryAuthLogic(exportReqObj);
            if (authRet != Constants.INVITE_RESULT_SUCCESS) {
                return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, ics, exportReqObj.getListType(), authRet);
            }

            //send export request with importer
            long dt = System.currentTimeMillis();
            ics = SImporterFacadeFactory.getImporterFacade(exportReqObj.isMsn()).
                    getResponse(exportReqObj.getInviterId(), exportReqObj.getEmail(), 2, exportReqObj.getAttMap());
            LogUtils.logDebug("export query of core cost:" + (System.currentTimeMillis() - dt));

        }
        catch (Exception e) {
            e.printStackTrace(System.err);
            LogUtils.logExceptionStack(e);
            LogUtils.logErr("do mail export query service exeception, return INVITE_RESULT_EXCP");
            return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, ics, 0, Constants.INVITE_RESULT_EXCP);
        }

        return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, ics, exportReqObj.getListType(), Constants.INVITE_RESULT_SUCCESS);
    }

    /**
     * export contacts, send invite and provide http callback
     *
     * @param request
     * @return
     */
    public String doMailExportInviteWithCallback(HttpServletRequest request) {
        ExportRequestParam exportReqObj = null;
        try {
            exportReqObj = RequestParamFactory.getInstance().getExportRequestParam(request);
            LogUtils.logDebug("map.appId = " + exportReqObj.getAttMap().get("APP_ID"));
            LogUtils.logDebug("map.actionType = " + exportReqObj.getAttMap().get("ACTION_TYPE"));
            LogUtils.logInfo("doMailExportInviteWithCallback datas: " + exportReqObj);
            return sendExportRequest(exportReqObj, true, exportReqObj.getHttpCallback() != null && !"".equals(exportReqObj.getHttpCallback()));
        }
        catch (Exception e) {
            e.printStackTrace(System.err);
            LogUtils.logExceptionStack(e);
            LogUtils.logErr("do mail export and invite service exeception, return INVITE_RESULT_EXCP");
            return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, null, 0, Constants.INVITE_RESULT_EXCP);
        }
    }

    /**
     * export contacts and provide http callback
     *
     * @param request
     * @return
     */
    public String doMailExportWithCallback(HttpServletRequest request) {
        ExportRequestParam exportReqObj = null;
        try {
            exportReqObj = RequestParamFactory.getInstance().getExportRequestParam(request);
            LogUtils.logInfo("doMailExportWithCallback datas: " + exportReqObj);
            return sendExportRequest(exportReqObj, false, exportReqObj.getHttpCallback() != null && !"".equals(exportReqObj.getHttpCallback()));
        }
        catch (Exception e) {
            e.printStackTrace(System.err);
            LogUtils.logExceptionStack(e);
            LogUtils.logErr("do mail export service exeception, return INVITE_RESULT_EXCP");
            return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, null, 0, Constants.INVITE_RESULT_EXCP);
        }
    }

    /**
     * call importer send request
     *
     * @param exportReqObj
     * @param isSendInvite
     * @param isCallback
     * @return
     */
    private String sendExportRequest(ExportRequestParam exportReqObj, boolean isSendInvite, boolean isCallback) {
        //authentications
        int authRet = AuthLogic.getInstance().mailExportReqAuthLogic(exportReqObj);
        if (authRet != Constants.INVITE_RESULT_SUCCESS) {
            return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, null, 0, authRet);
        }

        //send export invite callback with importer
        long dt = System.currentTimeMillis();

        if (isSendInvite && isCallback)
            SImporterFacadeFactory.getImporterFacade(exportReqObj.isMsn()).sendRequestAndInvite(exportReqObj.getInviterId(), exportReqObj.getEmail(), exportReqObj.getPassword(), exportReqObj.getListType(), ImporterCallbackFactory.getInstance().getImporterCallback(exportReqObj), exportReqObj.getAttMap());
        else if (isSendInvite && !isCallback)
            SImporterFacadeFactory.getImporterFacade(exportReqObj.isMsn()).sendRequestAndInvite(exportReqObj.getInviterId(), exportReqObj.getEmail(), exportReqObj.getPassword(), exportReqObj.getAttMap());
        else if (!isSendInvite && isCallback)
            SImporterFacadeFactory.getImporterFacade(exportReqObj.isMsn()).sendRequest(exportReqObj.getInviterId(), exportReqObj.getEmail(), exportReqObj.getPassword(), exportReqObj.getListType(), ImporterCallbackFactory.getInstance().getImporterCallback(exportReqObj), exportReqObj.getAttMap());
        else if (!isSendInvite && !isCallback)
            SImporterFacadeFactory.getImporterFacade(exportReqObj.isMsn()).sendRequest(exportReqObj.getInviterId(), exportReqObj.getEmail(), exportReqObj.getPassword(), exportReqObj.getAttMap());

        LogUtils.logDebug("commonMailExportInviteWithCallback of importer cost:" + (System.currentTimeMillis() - dt));

        return ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqObj, null, exportReqObj.getListType(), Constants.INVITE_RESULT_SUCCESS);
	}
}