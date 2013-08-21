package com.dodoyo.invite.httpservice.auth;

import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.Constants;
import com.dodoyo.invite.httpservice.entity.ExportRequestParam;
import com.dodoyo.invite.httpservice.entity.LinkRequestParam;
import com.dodoyo.invite.httpservice.entity.MailRequestParam;

public class AuthLogic {
    private static AuthLogic al = new AuthLogic();

    public static AuthLogic getInstance() {
        return al;
    }

    /**
     * link invite service authentication
     *
     * @param linkReqObj
     * @return
     */
    public int linkAuthLogic(LinkRequestParam linkReqObj) {
        //check remote client server is valid or not
        LogUtils.logInfo("call link invite service, clientIp = " + linkReqObj.getClientIp());
        //String clientKey = ClientAuth.gentInstance().getClientKey(linkReqObj.getClientIp());
        String clientKey = ClientAuth.gentInstance().getClientKeyByAppId(linkReqObj.getAppId());
        if (clientKey == null) {
            LogUtils.logErr("client key not found, client ip is unauthed ....");
            return Constants.INVITE_RESULT_INVALID_CLIENT;
        }


        //check token
        int authRet = TokenAuth.getInstance().linkTokenAuth(linkReqObj, clientKey);
        if (authRet != Constants.TOKEN_RESULT_OK) {
            LogUtils.logErr("token auth failed, return INVITE_RESULT_UNAUTHED");
            return Constants.INVITE_RESULT_UNAUTHED;
        }

        return Constants.INVITE_RESULT_SUCCESS;
    }

    /**
     * mail invite service authentication
     *
     * @param mailReqObj
     * @return
     */
    public int mailAuthLogic(MailRequestParam mailReqObj) {
        //check remote client server is valid or not
        LogUtils.logInfo("call mail invite service, clientIp = " + mailReqObj.getClientIp());
        String clientKey = ClientAuth.gentInstance().getClientKey(mailReqObj.getClientIp());
        if (clientKey == null) {
            LogUtils.logInfo("client key not found, client ip is unauthed ....");
            return Constants.INVITE_RESULT_INVALID_CLIENT;
        }

        //check token
        int authRet = TokenAuth.getInstance().mailTokenAuth(mailReqObj, clientKey);
        if (authRet != Constants.TOKEN_RESULT_OK) {
            LogUtils.logInfo("token auth failed, return INVITE_RESULT_UNAUTHED");
            return Constants.INVITE_RESULT_UNAUTHED;
        }

        return Constants.INVITE_RESULT_SUCCESS;
    }

    /**
     * export request authentication
     *
     * @param exportReqObj
     * @return
     */
    public int mailExportReqAuthLogic(ExportRequestParam exportReqObj) {
        //check remote client server is valid or not
        LogUtils.logInfo("#######call mail export [request] invite service, clientIp = " + exportReqObj.getClientIp());
        String clientKey = ClientAuth.gentInstance().getClientKey(exportReqObj.getClientIp());
        if (clientKey == null) {
            LogUtils.logInfo("client key not found, client ip is un authed ....");
            return Constants.INVITE_RESULT_INVALID_CLIENT;
        }

        //check token
        int authRet = TokenAuth.getInstance().exportReqTokenAuth(exportReqObj, clientKey);
        if (authRet != Constants.TOKEN_RESULT_OK) {
            LogUtils.logInfo("token auth failed, return INVITE_RESULT_UNAUTHED");
            return Constants.INVITE_RESULT_UNAUTHED;
        }

        return Constants.INVITE_RESULT_SUCCESS;
    }

    /**
     * export query authentication
     *
     * @param exportReqObj
     * @return
     */
    public int mailExportQueryAuthLogic(ExportRequestParam exportReqObj) {
        //check remote client server is valid or not
        LogUtils.logInfo("#######call mail export [request] invite service, clientIp = " + exportReqObj.getClientIp());
        String clientKey = ClientAuth.gentInstance().getClientKey(exportReqObj.getClientIp());
        if (clientKey == null) {
            LogUtils.logInfo("client key not found, client ip is un authed ....");
            return Constants.INVITE_RESULT_INVALID_CLIENT;
        }

        //check token
        int authRet = TokenAuth.getInstance().exportQueryTokenAuth(exportReqObj, clientKey);
        if (authRet != Constants.TOKEN_RESULT_OK) {
            LogUtils.logInfo("token auth failed, return INVITE_RESULT_UNAUTHED");
            return Constants.INVITE_RESULT_UNAUTHED;
		}
		
		return Constants.INVITE_RESULT_SUCCESS;
	}
}