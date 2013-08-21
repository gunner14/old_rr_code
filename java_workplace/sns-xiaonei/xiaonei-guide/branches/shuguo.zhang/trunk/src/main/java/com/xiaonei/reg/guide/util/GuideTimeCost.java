package com.xiaonei.reg.guide.util;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

import org.apache.log4j.Logger;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.reg.common.constants.Globals;

public class GuideTimeCost {
    private static Logger logger = Logger.getLogger("com.inshion.MyTimeCostLogger");

    private final long beginTime = System.currentTimeMillis();
    private final String name;
    private long stepTime;
    private boolean alreadyEnd = false;
    private StackTraceElement stack;

    public static final int DEBUG = 5;
	public static final int INFO = 4;
	public static final int WARN = 3;
	public static final int ERROR = 2;
	public static final int FATAL = 1;
	
	private static Map<String,Integer> map = new HashMap<String,Integer>();
	public static void reloadConfigMap(){
		DodoyoResource.close("guidecosttime");
		DodoyoResource.init("guidecosttime");
		ResourceBundle resourceBundle = DodoyoResource.getResourceBundle("guidecosttime");
		
		map = new HashMap<String, Integer>();
		Enumeration<?> es = resourceBundle.getKeys();
		while (es.hasMoreElements()) {
			String key = (String) es.nextElement();
			String value = null;
			try {
				value = resourceBundle.getString(key);
			} catch (Exception e) {
				e.printStackTrace();
			}
			
			if (value == null) {
				//
			}
			else{
				int ivalue = getLevelInteger(value);
				map.put(key, ivalue);
				logger.info("MyTimeCost::guidecosttime.properties reloaded:"+"<"+key+","+value+","+ivalue+">");
			}
		}
	}
	public static int getLevelInteger(String level){
		if("DEBUG".equals(level)){
			return DEBUG;
		}
		if("INFO".equals(level)){
			return INFO;
		}
		if("WARN".equals(level)){
			return WARN;
		}
		if("ERROR".equals(level)){
			return ERROR;
		}
		if("FATAL".equals(level)){
			return FATAL;
		}
		return DEBUG;
	}
	static{
		reloadConfigMap();
	}
	
	private static void write(String msg, int type){
		if((null != map.get("STATUS")) && map.get("STATUS")==DEBUG){
			//logger.debug("logger off");
			return ;
		}
		String classname = "";
		String packagename = "";
		try {
			StackTraceElement stack = Thread.currentThread().getStackTrace()[3];
			String classnames[] = stack.getClassName().split("\\.");
			int max = classnames.length - 1;
			classname = classnames[max];
			packagename = stack.getClassName().substring(0,stack.getClassName().indexOf(classname)-1);
		} catch (RuntimeException e) {
			e.printStackTrace();
		}
//		String message = classname+"::"+method+"("+linenumber+"):"+msg;
		String message = msg;
		
		if(type == 0){
			Integer level = 0;
			if(map != null){
				level = map.get(classname);
			}
			if(level == null || level == 0){
				if(map != null){
					level = map.get(packagename);
				}
			}
			
			if(level == null || level == 0){
				level = DEBUG;
			}

			switch(level){
			case DEBUG:
				if(Globals.DEBUG_ENV){
					logger.info("<D>"+message);
				}
				logger.debug(message);
				break;
			case INFO:
				logger.info(message);
				break;
			case WARN:
				logger.warn(message);
				break;
			case ERROR:
				logger.error(message);
				break;
			case FATAL:
				logger.fatal(message);
				break;
			default:
				logger.debug(message);
			break;
			}
		}
		else{
			switch(type){
			case DEBUG:
				if(Globals.DEBUG_ENV){
					logger.info("<D>"+message);
				}
				logger.debug(message);
				break;
			case INFO:
				logger.info(message);
				break;
			case WARN:
				logger.warn(message);
				break;
			case ERROR:
				logger.error(message);
				break;
			case FATAL:
				logger.fatal(message);
				break;
			default:
				logger.debug(message);
			break;
			}
		}
		
	}
	
    public static GuideTimeCost begin(Object name) {
        return new GuideTimeCost(String.valueOf(name));
    }

    public static GuideTimeCost begin() {
        return new GuideTimeCost();
    }

    private GuideTimeCost(String name) {
        this.stepTime = this.beginTime;
        this.name = name;
    }

    private GuideTimeCost() {
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
//        logger.info(message);
		write(message, 0);
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
        write(message, 0);
//        logger.info(message);
        return endStepTime;
    }

    public void endFinally() {
        if (alreadyEnd) {
            return;
        }
        alreadyEnd = true;
        String message = name + " - Cost: "
                + (System.currentTimeMillis() - beginTime);
        write(message, 0);
//        logger.info(message);
    }
}
