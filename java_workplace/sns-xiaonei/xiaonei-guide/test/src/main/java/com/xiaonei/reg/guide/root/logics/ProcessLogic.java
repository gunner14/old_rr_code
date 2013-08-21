package com.xiaonei.reg.guide.root.logics;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.root.models.Unit;

public class ProcessLogic {
	private static ProcessLogic instance = new ProcessLogic();

    public static ProcessLogic getInstance() {
        return instance;
    }

    private ProcessLogic() {
        super();
    }
    
	public void endTheStep(User host, Unit unit) {
		StepStatusLogic.getInstance().updateStepStatus(host.getId(), unit.getBinaryFlag());
		
	}
}
