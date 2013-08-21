package com.dodoyo.invite.httpservice.auth;

import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.Constants;
import com.dodoyo.invite.httpservice.dao.TokenDao;
import com.dodoyo.invite.httpservice.entity.ExportRequestParam;
import com.dodoyo.invite.httpservice.entity.LinkRequestParam;
import com.dodoyo.invite.httpservice.entity.MailRequestParam;
import com.dodoyo.invite.httpservice.entity.Token;
import com.dodoyo.invite.httpservice.excep.ServiceDBException;
import com.dodoyo.invite.httpservice.excep.ServiceException;

import java.sql.SQLException;
import java.util.Date;

public class TokenAuth {
    private static TokenAuth ta = new TokenAuth();

    public static TokenAuth getInstance() {
        return ta;
    }

    /**
     * check the token is expired or not
     *
     * @param timestamp
     * @return
     */
    private boolean isExpiredToken(String timestamp) {
        if (true)//暂时取消timestamp验证。
            return false;
        long tm = 0l;
        try {
            tm = Long.parseLong(timestamp);
        }
        catch (Exception e) {
            throw new ServiceException("Paramter \"timestamp\"  parsing exception", e);
        }
        long currentTm = new Date().getTime();

        return (currentTm > tm + Constants.TOKEN_EXPIRE_SENCONDS * 1000);

    }

    /**
     * check the token is used or not
     *
     * @param token
     * @return
     */
    @SuppressWarnings("unchecked")
    private boolean isUsedToken(String token, String clientIp, String action, Object obj) {
        if (true) return false;
        if (token == null)
            return true;
        TokenDao tokenDao = new TokenDao();
        Token tkObj = null;
        try {
            tkObj = tokenDao.getToken(token);
        }
        catch (SQLException e) {
            throw new ServiceDBException("getToken() db exception", e);
        }

        if (tkObj == null) {
            Token newTk = new Token();
            newTk.setClientIp(clientIp);
            newTk.setAction(action);
            newTk.setToken(token);
            newTk.setParams(obj.toString());

            try {
                tokenDao.addToken(newTk);
            }
            catch (SQLException e) {
                throw new ServiceDBException("addToken() db exception", e);
            }
            return false;
        }

        return true;
    }

    /**
     * check link invite request parameters is valid or not
     *
     * @param inviterId
     * @param appId
     * @param groupName
     * @param baseLink
     * @param timestamp
     * @param token
     * @return
     */
    public int linkTokenAuth(LinkRequestParam linkReqObj, String clientKey) {
        if (isExpiredToken(linkReqObj.getTimestamp())) {
            LogUtils.logDebug("TOKEN_RESULT_EXPIRED");
            return Constants.TOKEN_RESULT_EXPIRED;
        }

        if (isUsedToken(linkReqObj.getToken(), linkReqObj.getClientIp(), linkReqObj.getAction(), linkReqObj)) {
            LogUtils.logDebug("TOKEN_RESULT_USED");
            return Constants.TOKEN_RESULT_USED;
        }

        String correctToken = linkReqObj.getInviterId() + "-" + linkReqObj.getAppId() + "-" + linkReqObj.getGroupName() + "-"
                + linkReqObj.getBaseLink() + "-" + linkReqObj.getTimestamp() + "-" + clientKey;
        correctToken = EncryptUtil.getInstance().md5Str(correctToken);

        LogUtils.logDebug("correctToken = " + correctToken + ", post token = " + linkReqObj.getToken());

        return correctToken.equalsIgnoreCase(linkReqObj.getToken()) ? Constants.TOKEN_RESULT_OK : Constants.TOKEN_RESULT_INVALID;
    }

    /**
     * check mail invite request parameters is valid or not
     *
     * @param inviterId
     * @param appId
     * @param inviteeMail
     * @param timestamp
     * @param token
     * @param clientKey
     * @return
     */
    public int mailTokenAuth(MailRequestParam mailReqObj, String clientKey) {
        if (isExpiredToken(mailReqObj.getTimestamp()))
            return Constants.TOKEN_RESULT_EXPIRED;

        if (isUsedToken(mailReqObj.getToken(), mailReqObj.getClientIp(), mailReqObj.getAction(), mailReqObj))
            return Constants.TOKEN_RESULT_USED;

        String correctToken = mailReqObj.getInviterId() + "-" + mailReqObj.getAppId() + "-" + mailReqObj.getInviteeMail() +
                "-" + mailReqObj.getTimestamp() + "-" + clientKey;
        correctToken = EncryptUtil.getInstance().md5Str(correctToken);

        return correctToken.equalsIgnoreCase(mailReqObj.getToken()) ? Constants.TOKEN_RESULT_OK : Constants.TOKEN_RESULT_INVALID;
    }

    /**
     * check export request parameter is valid or not
     *
     * @param exportReqObj
     * @param clientKey
     * @return
     */
    public int exportReqTokenAuth(ExportRequestParam exportReqObj, String clientKey) {
        if (isExpiredToken(exportReqObj.getTimestamp()))
            return Constants.TOKEN_RESULT_EXPIRED;

        if (isUsedToken(exportReqObj.getToken(), exportReqObj.getClientIp(), exportReqObj.getAction(), exportReqObj))
            return Constants.TOKEN_RESULT_USED;

        String correctToken = exportReqObj.getEmail() + "-" + exportReqObj.getPassword() + "-" +
                exportReqObj.getInviterId() + "-" + exportReqObj.getAppId() + "-" +
                exportReqObj.getTimestamp() + "-" + clientKey;
        correctToken = EncryptUtil.getInstance().md5Str(correctToken);

        return correctToken.equalsIgnoreCase(exportReqObj.getToken()) ? Constants.TOKEN_RESULT_OK : Constants.TOKEN_RESULT_INVALID;
    }

    /**
     * export query auth token
     *
     * @param exportReqObj
     * @param clientKey
     * @return
     */
    public int exportQueryTokenAuth(ExportRequestParam exportReqObj, String clientKey) {
        if (isExpiredToken(exportReqObj.getTimestamp()))
            return Constants.TOKEN_RESULT_EXPIRED;

        if (isUsedToken(exportReqObj.getToken(), exportReqObj.getClientIp(), exportReqObj.getAction(), exportReqObj))
            return Constants.TOKEN_RESULT_USED;

        String correctToken = exportReqObj.getEmail() + "-" +
                exportReqObj.getInviterId() + "-" + exportReqObj.getTimestamp() + "-" + clientKey;
        correctToken = EncryptUtil.getInstance().md5Str(correctToken);

        return correctToken.equalsIgnoreCase(exportReqObj.getToken()) ? Constants.TOKEN_RESULT_OK : Constants.TOKEN_RESULT_INVALID;
    }

    public static void main(String[] args)
	{
		boolean b = TokenAuth.getInstance().isExpiredToken(System.currentTimeMillis() - 3 + "");
		System.out.print(b);
	}
}