package com.renren.sns.lead.core.stepper.impl;

import com.renren.sns.lead.core.stepper.AbstractStepper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;

public class MSNConnectStepper extends AbstractStepper {

	@Override
	public String doGetToUrl(User host, int stepstatus) {
		GuideLogger.printLog(" host:"+ host.getId() + " - start");
		String toUrl = LEAD_COMPLETE_URI;
		try {
			GuideLogger.printLog(" host:"+ host.getId() +" [StepStatus]="+stepstatus);
			if (stepstatus == 0 ||(stepstatus & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = URL_FILL_INFO;
			} 

		} catch (Exception e) {
			GuideLogger.printLog(" host:"+ host.getId() + "  error"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		GuideLogger.printLog(" host:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}

}
