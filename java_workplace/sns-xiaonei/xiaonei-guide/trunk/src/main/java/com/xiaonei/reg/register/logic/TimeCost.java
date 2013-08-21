package com.xiaonei.reg.register.logic;

import org.apache.log4j.Logger;

public class TimeCost {
    private static Logger logger = Logger.getLogger("regTimeCost");

    private final long beginTime = System.currentTimeMillis();
    private final String name;
    private long stepTime;
    private boolean alreadyEnd = false;
    private StackTraceElement stack;

    public static TimeCost begin(Object name) {
        return new TimeCost(String.valueOf(name));
    }

    public static TimeCost begin() {
        return new TimeCost();
    }

    private TimeCost(String name) {
        this.stepTime = this.beginTime;
        this.name = name;
    }

    private TimeCost() {
        this.stepTime = this.beginTime;
        stack = Thread.currentThread().getStackTrace()[3];
        this.name = stack.getClassName() + "." + stack.getMethodName();
    }

    public long endStep(String stepName) {
        if (alreadyEnd)
            return -1l;
        long endStepTime = System.currentTimeMillis() - stepTime;
        stepTime = System.currentTimeMillis();
		String message = name + " - Step " + stepName + " - Cost: "
                + endStepTime;
        logger.info(message);
        return endStepTime;
    }

    public long endStep() {
        if (alreadyEnd)
            return -1l;
        long endStepTime = System.currentTimeMillis() - stepTime;
        stepTime = System.currentTimeMillis();
        stack = Thread.currentThread().getStackTrace()[2];
        String message = name + " - Step " + " line:" + stack.getLineNumber()
                + " - Cost: " + (endStepTime);
        stepTime = System.currentTimeMillis();
        logger.info(message);
        return endStepTime;
    }

    public void endFinally() {
        if (alreadyEnd) {
            return;
        }
        alreadyEnd = true;
        String message = name + " - Cost: "
                + (System.currentTimeMillis() - beginTime);
        logger.info(message);
    }
    
}
