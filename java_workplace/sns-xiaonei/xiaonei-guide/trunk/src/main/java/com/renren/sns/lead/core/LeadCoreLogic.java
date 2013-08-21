package com.renren.sns.lead.core;

import java.util.List;

import com.renren.sns.lead.core.stepper.AbstractStepper;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.register.logic.pretreatment.RegFromSourceLogic;


public class LeadCoreLogic {
	private static LeadCoreLogic instance = new LeadCoreLogic();

    public static LeadCoreLogic getInstance() {
        return instance;
    }
    
    /**
     * 供GoWhere中，取去向URL的方法 
     * @param host
     * @param fromsource
     * @param stepstatus
     * @return
     */
	public String getRequestUrl(User host, String fromsource,int stepstatus) {
		String ret = "";
		//get a stepper by type
		//do step
		GuideLogger.printLog("lcl host:"+host.getId()+" from:"+fromsource+" step:"+stepstatus);
		AbstractStepper steppter = StepperFactory.getStepper(host,fromsource);
		ret = steppter.getStepUrl(host, stepstatus);
		set_fromsource(fromsource);
		set_stepstatus(stepstatus);
		GuideLogger.printLog(" host:"+host.getId()+" ret:"+ret);
		
		return ret;
	}
	
	public String getLeadCoreFlag(User host, String fromsource){
		//GuideLogger.printLog(" host:"+host.getId()+" from:"+fromsource);
		String flag = "";
		List<String> fslist = RegFromSourceLogic.getFromSoureList(fromsource);
		for (String fs : fslist) {
			if(fs.startsWith("_")){
				flag = fs;
			}
		}
		GuideLogger.printLog(" host:"+host.getId()+" from:"+fromsource+" coreflag:"+flag);
		return flag;
	}
	
	private static final ThreadLocal<String>  _fromsource = new ThreadLocal<String>();
	private static final ThreadLocal<Integer> _stepstatus = new ThreadLocal<Integer>();

	public static String get_fromsource() {
		return _fromsource.get();
	}

	private static void set_fromsource(String _fromsource) {
		LeadCoreLogic._fromsource.set(_fromsource);
	}

	@SuppressWarnings("unused")
	private static Integer get_stepstatus() {
		return _stepstatus.get();
	}

	private static void set_stepstatus(Integer _stepstatus) {
		LeadCoreLogic._stepstatus.set(_stepstatus);
	}
	
	public static void clearThreadLocal(){
	    _fromsource.remove();
	    _stepstatus.remove();
	}

}
