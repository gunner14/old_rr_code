package com.renren.sns.lead.core.stepper;

import com.renren.sns.lead.core.ILeadConstants;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.configs.IStepStatusDefinition;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

public abstract class AbstractStepper implements ILeadConstants,IStepStatusDefinition{
	public abstract String doGetToUrl(User host, int stepstatus);
	
	public String getStepUrl(User host, int stepstatus){
		String ret = "";
		GuideLogger.printLog(" host:"+ host.getId() + " step:"+stepstatus);
		ret = doGetToUrl(host, stepstatus);
		GuideLogger.printLog(" host:"+ host.getId() + " ret:"+ret);
		return ret;
	}
	
	public int getUserLeadType(User host, String fromsource){
		int type = StepProcessLogic.getInstance().getGuideUserType(host, fromsource);
		return type;
	}
	public int getUserLeadType(User host) {
		int type = StepProcessLogic.getInstance().getGuideUserType(host);
		return type;
	}
	
	@Override
	public String toString() {
		return " this is:"+this.getClass().getSimpleName();
	}
}
