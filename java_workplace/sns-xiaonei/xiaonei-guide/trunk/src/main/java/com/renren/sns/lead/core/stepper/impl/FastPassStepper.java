package com.renren.sns.lead.core.stepper.impl;

import com.renren.sns.lead.core.stepper.AbstractStepper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;

public class FastPassStepper extends AbstractStepper {

	@Override
	public String doGetToUrl(User host, int step) {
		GuideLogger.printLog(" host:"+ host.getId() + " - start");
		String toUrl = LEAD_COMPLETE_URI;
		try {
			GuideLogger.printLog(" host:"+ host.getId() +" [StepStatus]="+step);
			if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = URL_PREVIEW;
			}

		} catch (Exception e) {
			GuideLogger.printLog(" host:"+ host.getId() + "  error"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		GuideLogger.printLog(" host:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}
	
}
