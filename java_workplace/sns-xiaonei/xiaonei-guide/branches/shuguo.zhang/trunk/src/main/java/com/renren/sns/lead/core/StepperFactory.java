package com.renren.sns.lead.core;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.renren.sns.lead.core.stepper.AbstractStepper;
import com.renren.sns.lead.core.stepper.impl.AutoAAJStepper;
import com.renren.sns.lead.core.stepper.impl.ClassInviteStepper;
import com.renren.sns.lead.core.stepper.impl.DefaultStepper;
import com.renren.sns.lead.core.stepper.impl.KouxiangtangStepper;
import com.renren.sns.lead.core.stepper.impl.MSNConnectStepper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.logics.StepProcessLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;

public class StepperFactory {
	private static final Map<String,AbstractStepper> map = new HashMap<String, AbstractStepper>();
	static{
		reloadStepMap();
	}
	private static void reloadStepMap() {
		map.put("DEFAULT", new DefaultStepper());
		map.put("FROMSOURCE_AAJ", new AutoAAJStepper());
		map.put("Kouxiangtang", new KouxiangtangStepper());
		map.put("MSNConnect", new MSNConnectStepper());
		map.put("ClassInvite", new ClassInviteStepper());
		
	}
	public static AbstractStepper getStepper(User host, String fromsource){
		
		int type = getUserLeadType(host,fromsource);
		GuideLogger.printLog("sf host:"+host.getId()+""+" from:"+fromsource+" type:"+type);     
		String key = "TYPE_"+type;
		AbstractStepper stepper = map.get(key);
		
		if(stepper == null){
			String steppername = LeadConfigUtil.getStepperName(host,fromsource);
			stepper = map.get(steppername);
			GuideLogger.printLog(" host:"+host.getId()+""+" from:"+fromsource+" sf_coreflag:"+stepper);
		}
		
		if(stepper == null){ //自动匹配
			List<String> fslist = RegFromSourceLogic.getFromSoureList(fromsource);
			for (String fsstring : fslist) {
				key = "FROMSOURCE"+fsstring;
				stepper = map.get(key);
				if(stepper != null){
					GuideLogger.printLog(" host:"+host.getId()+""+" from:"+fromsource+" sf_auto:"+stepper);		
					break;
				}
			}
		}
		if(stepper == null){ //还为空？没办法了…兑一个吧
			stepper = map.get("DEFAULT");
		}
		GuideLogger.printLog(" host:"+host.getId()+""+" stepper:"+stepper);		
		return stepper;
	}
	
	public static int getUserLeadType(User host, String fromsource){
		int type = StepProcessLogic.getInstance().getGuideUserType(host, fromsource);
		return type;
	}
	
	public static void main(String[] args) {
		AbstractStepper stepper = map.get("");
		System.out.println(stepper);
		try {
			Class<?> c = Class.forName("StepperFactory");
			c.newInstance();
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InstantiationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
