package com.dodoyo.invite.httpservice.logic;

import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.entity.ExportRequestParam;
import com.dodoyo.invite.httpservice.test.PostHttpRequest;
import com.xiaonei.importer.ImporterCallback;
import com.xiaonei.importer.entity.ImporterContacts;

public class ImporterCallbackImpl implements ImporterCallback {
    private ExportRequestParam exportReqParam;

    protected ImporterCallbackImpl(ExportRequestParam exportReqParam) {
        this.exportReqParam = exportReqParam;
    }

    public void finished(ImporterContacts contacts) {
        LogUtils.logDebug(">>>>>> http callback addr: " + exportReqParam.getHttpCallback());
        LogUtils.logDebug(">>>>>> list_type: " + exportReqParam.getListType());

        String callbackDatas = ResponseUtils.getInstance().getResponseXMLForExportContacts(exportReqParam, contacts, exportReqParam.getListType(), 0);
        LogUtils.logInfo("callbackDatas = " + callbackDatas);

        //now to do http callback
        PostHttpRequest phr = new PostHttpRequest(exportReqParam.getHttpCallback(), callbackDatas);
        phr.send_url();
        LogUtils.logInfo("remote server http callback response: " + phr.getResponse_content());
    }

    public void exception(Exception arg0) {

    }

}