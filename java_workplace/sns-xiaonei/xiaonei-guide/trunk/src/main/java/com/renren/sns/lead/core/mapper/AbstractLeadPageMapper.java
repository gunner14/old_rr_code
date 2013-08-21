package com.renren.sns.lead.core.mapper;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;

public abstract class AbstractLeadPageMapper {
	public abstract int getThisStep();
	public abstract String getThisJspUrl(User host, String fromsource);
	public int getUserLeadType(User host, String fromsource){
		int type = StepProcessLogic.getInstance().getGuideUserType(host, fromsource);
		return type;
	}
	/*public int getUserLeadType(User host) {
		int type = StepProcessLogic.getInstance().getGuideUserType(host);
		return type;
	}*/
	//public abstract List<String> getDataWindows(User host);
	//public abstract doingAAF(Invocation inv,Portal portal, User host, CacheManager cm);
}
