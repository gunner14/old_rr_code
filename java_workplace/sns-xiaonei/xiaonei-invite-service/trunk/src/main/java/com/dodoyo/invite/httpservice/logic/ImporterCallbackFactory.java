package com.dodoyo.invite.httpservice.logic;

import com.dodoyo.invite.httpservice.entity.ExportRequestParam;
import com.xiaonei.importer.ImporterCallback;

public class ImporterCallbackFactory {
    private static ImporterCallbackFactory callbackFactory = new ImporterCallbackFactory();

    public static ImporterCallbackFactory getInstance() {
        return callbackFactory;
    }

    public ImporterCallback getImporterCallback(ExportRequestParam exportParam) {
        if (exportParam.getHttpCallback() == null || "".equals(exportParam.getHttpCallback()))
            return null;
        return new ImporterCallbackImpl(exportParam);
    }
}