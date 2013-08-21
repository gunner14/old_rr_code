package com.renren.sns.lead.core.stepper;

import com.xiaonei.platform.component.application.utils.MD5;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.permission.define.Stage;
import com.xiaonei.reg.guide.flows.xfive.logics.GuideXFiveUseIdentityLogic;
import com.xiaonei.reg.guide.util.GuideLogger;

public class StepperUtil {
	public static boolean needImportmsn(User host) {
		if(host == null)return false;
		GuideLogger.printLog(" host:"+ host.getId() +" [getStage]="+host.getStage());
		int identity = GuideXFiveUseIdentityLogic.getInstance().getIdentity(host.getId());
		GuideLogger.printLog(" host:"+ host.getId() +" [getIdentity]="+identity);
		if(identity == GuideXFiveUseIdentityLogic.IDENTITY_WORK){
			return true;
		}
		if(host.getStage() == Stage.STAGE_MAJOR){
			return true;
		}
		return false;
	}
	
	public static void main(String[] args) {
		
		//String m = "c20b082efdd04af2d548eeb8aed71ca7";
		System.out.println("renren="+MD5.digest("renren"));
		System.out.println("kaixin="+MD5.digest("kaixin"));
	}
}
