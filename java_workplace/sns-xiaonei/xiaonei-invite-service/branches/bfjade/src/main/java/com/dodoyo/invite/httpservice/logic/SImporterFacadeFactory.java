package com.dodoyo.invite.httpservice.logic;

import com.dodoyo.invite.core.logic.LogUtils;
import com.xiaonei.importer.ImporterFacade;
import com.xiaonei.importer.impl.ImporterFacadeFactory;

public class SImporterFacadeFactory {

    /**
     * get msn importer or email importer
     *
     * @param isMsn
     * @return
     */
    public static ImporterFacade getImporterFacade(boolean isMsn) {
        if (isMsn) {
            LogUtils.logDebug("using msnImporterFacade ....");
            return ImporterFacadeFactory.getMsnImporterFacade();
        } else {
            LogUtils.logDebug("using emailImporterFacade ....");
            return ImporterFacadeFactory.getEmailImporterFacade();
		}
	}
}