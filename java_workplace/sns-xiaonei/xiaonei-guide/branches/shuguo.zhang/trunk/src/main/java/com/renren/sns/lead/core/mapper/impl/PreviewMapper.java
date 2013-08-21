package com.renren.sns.lead.core.mapper.impl;

import com.renren.sns.lead.core.ILeadConstants;
import com.renren.sns.lead.core.LeadConfigUtil;
import com.renren.sns.lead.core.mapper.AbstractLeadPageMapper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.logic.StepStatusLogic;
import com.xiaonei.reg.guide.root.interfaces.IGuideUserLeadType;
import com.xiaonei.reg.guide.root.miscs.MyStringUtils;
import com.xiaonei.reg.guide.util.GuideLogger;

public class PreviewMapper extends AbstractLeadPageMapper  {
    //private static final String SKIP_DEFAULT = "f:lead";
	//private static final String SUCC_DEFAULT = "/views/guide/flowxfive/preview.jsp";
	private static final String SUCC_CORAL = "f:pvcoral";
	private static final String SUCC_BROWN = "/views/guide/flowxfivedark/preview_dark.jsp";
	
	private static final String INIT_LEAD_AAA = "/views/guide/flowxfivedarkchoco/preview_darkchoco.jsp"; //通讯录 曾纯强
	private static final String INIT_LEAD_AAF = "/views/guide/flowxfivedarkcoral/preview_darkcoral.jsp"; //口香糖
	private static final String INIT_LEAD_AAH = "/views/guide/flowxfivedarkcorn/preview_darkcorn.jsp"; //无头带邀请好友引导
	private static final String INIT_LEAD_AAI = "/views/guide/flowxfivedarkcrimson/preview_darkcrimson.jsp"; //一步快速引导
	private static final String INIT_LEAD_AAJ = "/views/guide/flowxfivedarkcyan/preview_darkcyan.jsp"; //一步快速引导
	
	@Override
	public String getThisJspUrl(User host, String fromsource) {
		int type = getUserLeadType(host,fromsource);
		String ret = "@";
		
		String pageurifrom = "switch";
		switch(type){
		case IGuideUserLeadType.XFiveGeneral:
			//把默认的都牵到SUCC_BROWN 主要是为了9的 2010年3月29日17:18:49
			/*doDefault(request, host);
			return SUCC_DEFAULT;*/
			ret = SUCC_BROWN;		
			break;
		case IGuideUserLeadType.XFiveCoral:
		case IGuideUserLeadType.XFiveCoralB:
			ret = SUCC_CORAL;
			break;
		case IGuideUserLeadType.XFiveBrown:
			ret = SUCC_BROWN;
			break;
		case IGuideUserLeadType.Lead_AAA:
		case IGuideUserLeadType.Lead_AAE:
		    //String ret = doAAASearch(inv, request,host);
		    //return ret;
			ret = INIT_LEAD_AAA;
			break;
		case IGuideUserLeadType.Lead_AAF:
			//String aafret = doAAFSearch(inv, request,host);
		    //return aafret;
			ret = INIT_LEAD_AAF;
			break;
		case IGuideUserLeadType.Lead_AAH:
			//String aahret = doAAHSearch(inv, request,host);
		    //return aahret;
			ret = INIT_LEAD_AAH;
			break;
		case IGuideUserLeadType.Lead_AAI:
			//String aairet = doAAISearch(inv, request,host);
			//return aairet;
			ret = INIT_LEAD_AAI;
			break;
		case IGuideUserLeadType.Lead_AAJ:
			ret = INIT_LEAD_AAJ;
			break;
		default:
			//2010-3-12 14:42:17 为网吧引导同步
			/*doDefault(request, host);
			return SUCC_DEFAULT;*/
			ret = LeadConfigUtil.getPageUri(host, fromsource, ILeadConstants.STEP_KEY_PREVIEW);
			pageurifrom = "config";
		break;
		}
		
		if(MyStringUtils.isNullOrEmpty(ret)){
			pageurifrom = "default";
			ret = SUCC_BROWN;
		}
		
		/*switch(type){
		case IGuideUserLeadType.Lead_AAI:
			ret = INIT_LEAD_AAI;
			break;
		case IGuideUserLeadType.Lead_AAF:
			ret = INIT_LEAD_AAF;
			break;
		case IGuideUserLeadType.Lead_AAH:
			ret = INIT_LEAD_AAH;
			break;
		default:
			//2010-3-12 14:42:17 为网吧引导同步
			doDefault(request, host);
			return SUCC_DEFAULT;
			ret =  INIT_LEAD_AAF;
		}*/
		GuideLogger.printLog(" host:"+host.getId()+" type:"+type+" urif:"+pageurifrom+" topg:"+ret);
		return ret;
	}

	@Override
	public int getThisStep() {
		return StepStatusLogic.STEP_PREVIEW;
	}

/*	public List<String> getDataWindows(User host) {
		int type = getUserLeadType(host);
		List<String> ret = new ArrayList<String>();
		switch(type){
		case IGuideUserLeadType.Lead_AAI:
			ret.add("guideGetProfileFriends");
			ret.add("guideGetOfferFriends");
			break;
		case IGuideUserLeadType.Lead_AAJ:
			ret.add("guideGetInviterFriends");
			ret.add("guideGetInviterFriendsCluster");
			break;
		default:
			ret = null;
			break;
		}
			
		return ret;
	}*/
	

}
