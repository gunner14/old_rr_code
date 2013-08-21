package com.renren.sns.lead.core.mapper.impl;

import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;

public class MergeuserMapper extends AbstractLeadPageMapper  {
	private static final String INIT_DEFAULT = "/views/guide/lead/mergeuser_gum.jsp";
	
	private static final String INIT_LEAD_AAJ = "/views/guide/flowxfivedarkcyan/fillinfo_darkcyan.jsp"; 
	
	
	@Override
	public String getThisJspUrl(User host, String fromsource) {
		int type = getUserLeadType(host,fromsource);
		String ret = "@";
		
		String pageurifrom = "switch";
		switch(type){
		case IGuideUserLeadType.Lead_AAJ:
			//String aairet = doAAISearch(inv, request,host);
			//return aairet;
			ret = INIT_LEAD_AAJ;
			break;
		default:
			ret = LeadConfigUtil.getPageUri(host, fromsource, ILeadConstants.STEP_KEY_MERGEUSER);
			pageurifrom = "config";
		break;
		}
		if(MyStringUtils.isNullOrEmpty(ret)){
			pageurifrom = "default";
			ret = INIT_DEFAULT;
		}
		
		GuideLogger.printLog(" host:"+host.getId()+" type:"+type+" urif:"+pageurifrom+" topg:"+ret);
		return ret;
	}

	@Override
	public int getThisStep() {
		return 0;
	}
	
	public static void main(String[] args) {
		
	}

}
