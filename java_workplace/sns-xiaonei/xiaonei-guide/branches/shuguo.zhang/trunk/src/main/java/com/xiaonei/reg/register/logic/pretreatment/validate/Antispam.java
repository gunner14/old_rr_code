package com.xiaonei.reg.register.logic.pretreatment.validate;

import com.xiaonei.antispam.AntiSpamAdapter;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.antispam.model.CheckType;

public class Antispam {
	private static final int REGISTER_TYPE = CheckType.REGISTER_TYPE;

	@SuppressWarnings("deprecation")
    public static CheckResult checkAndFilterCR(int uId, String content) {
		CheckResult cr = null;
		try {
			cr = AntiSpamAdapter.getInstance().checkAndFilter(uId, REGISTER_TYPE,content);
		} catch (Exception e) {
		}
		if(cr == null){
			cr = new CheckResult(CheckResult.SAFE, content, content, "AntiSpam gua le ba?[xiaonei_reg]");
		}
		return cr;
	}

	@SuppressWarnings("deprecation")
    public static CheckResult checkAndFilterCR(String content) {
		return AntiSpamAdapter.getInstance().checkAndFilter(0, REGISTER_TYPE,
				content);
	}
}
