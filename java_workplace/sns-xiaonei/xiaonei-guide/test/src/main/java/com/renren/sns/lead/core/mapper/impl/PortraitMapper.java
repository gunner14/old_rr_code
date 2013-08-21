package com.renren.sns.lead.core.mapper.impl;

import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.util.GuideLogger;

public class PortraitMapper extends AbstractLeadPageMapper{

	@Override
	public String getThisJspUrl(User host, String fromsource) {
		String ret = LeadConfigUtil.getPageUri(host, fromsource, ILeadConstants.STEP_KEY_PORTRAIT);
		String pageurifrom = "config";
		GuideLogger.printLog(" host:"+host.getId()+" urif:"+pageurifrom+" topg:"+ret);
		return ret;
	}

	@Override
	public int getThisStep() {
		return IStepStatusDefinition.STEP_UPLOAD_HEAD;
	}

}
