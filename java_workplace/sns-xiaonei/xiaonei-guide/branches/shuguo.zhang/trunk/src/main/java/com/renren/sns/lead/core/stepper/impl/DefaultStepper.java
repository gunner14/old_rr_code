package com.renren.sns.lead.core.stepper.impl;

import com.renren.sns.lead.core.stepper.AbstractStepper;
import com.renren.sns.lead.core.stepper.StepperUtil;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;

public class DefaultStepper extends AbstractStepper{

	@Override
	public String doGetToUrl(User host, int step) {
		GuideLogger.printLog(" host:"+ host.getId() + " - start");
		String toUrl = LEAD_COMPLETE_URI;
		try {
			GuideLogger.printLog(" host:"+ host.getId() +" [StepStatus]="+step);
			if(step == 0 || (step & STEP_SEARCH_FRIEND) != STEP_SEARCH_FRIEND){
				toUrl = URL_SEARCH_FRIEND;
			} 
			else if ((step & STEP_FILLINFO) != STEP_FILLINFO) {
				toUrl = URL_FILL_INFO;
			} 
			else if ((step & STEP_PREVIEW) != STEP_PREVIEW) {
				toUrl = URL_PREVIEW;
			}
			//调整顺序试一下 2010-5-26 15:48:21
			else if ((step & STEP_MOREFRIEND) != STEP_MOREFRIEND){
				toUrl = URL_MORE_FRIEND;
			}
			else if (((step & STEP_IMPORT_MSN) != STEP_IMPORT_MSN) && StepperUtil.needImportmsn(host)) {
				toUrl = URL_IMPORT_MSN;
			} 
			else if ((step & STEP_UPLOAD_HEAD) != STEP_UPLOAD_HEAD) {
				toUrl = URL_UPLOAD_HEAD;
			} 
			
			//STEP_IMPORT_MSN

		} catch (Exception e) {
			GuideLogger.printLog(" host:"+ host.getId() + "  error"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		GuideLogger.printLog(" host:"+ host.getId() +" [toUrl]="+toUrl);
		return toUrl;
	}

}
