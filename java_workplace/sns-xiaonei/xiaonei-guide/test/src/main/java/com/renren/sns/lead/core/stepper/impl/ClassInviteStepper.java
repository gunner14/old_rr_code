package com.renren.sns.lead.core.stepper.impl;

import com.renren.sns.lead.core.stepper.AbstractStepper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;

public class ClassInviteStepper extends AbstractStepper {

	@Override
	public String doGetToUrl(User host, int stepstatus) {
		GuideLogger.printLog(" host:"+ host.getId() + " - start");
		String toUrl = LEAD_COMPLETE_URI;
		try {
			GuideLogger.printLog(" host:"+ host.getId() +" [StepStatus]="+stepstatus);
			if (stepstatus == 0 ||(stepstatus & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
				toUrl = URL_UPLOAD_HEAD;
			} 
			else if ((stepstatus & STEP_COMPLETE_GUIDE) != STEP_COMPLETE_GUIDE) {
				toUrl = URL_GOTO_FS;
			}

		} catch (Exception e) {
			GuideLogger.printLog(" host:"+ host.getId() + "  error"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		GuideLogger.printLog(" host:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}

}
