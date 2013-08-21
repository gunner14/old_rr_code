package com.renren.sns.lead.dataproviders.impl;

import java.util.ArrayList;
import java.util.List;

import com.renren.sns.guide.model.GuideIRelation;
import com.renren.sns.lead.dataproviders.AbstractLeadDataProvider;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveRelationsPreviewLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

public class PreviewProfileProvider extends AbstractLeadDataProvider{

	private static final int RESULT_LIMIT = 45;

	@SuppressWarnings("unchecked")
	@Override
	protected <T>T doGetData(User host, long timeout,final Object ...param) {
	    List<GuideIRelation> list = GuideXFiveRelationsPreviewLogic.getInstance().getProfileResults(RESULT_LIMIT, host, -1);
	    if(list == null){
	    	list = new ArrayList<GuideIRelation>();
	    }
	    else{
	    	
	    }
	    GuideLogger.printLog(" host:"+host.getId()+" list:"+list.size()+" RESULT_LIMIT:"+RESULT_LIMIT);
		return (T) list;
	}


}
