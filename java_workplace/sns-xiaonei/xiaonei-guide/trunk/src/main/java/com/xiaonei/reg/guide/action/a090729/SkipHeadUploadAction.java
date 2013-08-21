package com.xiaonei.reg.guide.action.a090729;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.chain.impl.struts.action.BasePageAction;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.guide.util.GuideLogger;

public class SkipHeadUploadAction extends BasePageAction{

	@Override
	public ActionForward executePt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		MyTimeCost cost = MyTimeCost.begin();
		User host = BaseThreadUtil.currentHost(request);
		cost.endStep();
		GuideLogger.printLog("host:"+host.getId());
		cost.endStep();
		String headurl = Globals.urlGuide + "/hu-090729.do"+"?skip=1";
		cost.endFinally();
		return new ActionForward(headurl, true);
	}

}
