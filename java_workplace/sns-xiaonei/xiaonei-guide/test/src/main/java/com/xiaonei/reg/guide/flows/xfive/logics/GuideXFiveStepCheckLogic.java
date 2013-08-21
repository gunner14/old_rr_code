package com.xiaonei.reg.guide.flows.xfive.logics;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.logics.StepStatusLogic;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;

public class GuideXFiveStepCheckLogic {
	private static GuideXFiveStepCheckLogic instance = new GuideXFiveStepCheckLogic();

    public static GuideXFiveStepCheckLogic getInstance() {
        return instance;
    }
    
	public boolean isTheStepDone(int thestep,User host){
        GuideTimeCost cost = GuideTimeCost.begin();
        cost.endStep();
		boolean ret = true;
		if(host == null){
			GuideLogger.printLog(" host null! ",GuideLogger.ERROR);
		}
		else if(thestep == 0){
			GuideLogger.printLog(" host:"+host.getId()+" thestep=0 ",GuideLogger.ERROR);
		}
		else{
			int status = StepStatusLogic.getInstance().getStatusById(host.getId());
			cost.endStep();
			if((status & thestep) == thestep){
				ret = true;
			}
			else{
				ret = false;
			}
			
		}
		cost.endStep();
		cost.endFinally();
		return ret;
	}

	
}
