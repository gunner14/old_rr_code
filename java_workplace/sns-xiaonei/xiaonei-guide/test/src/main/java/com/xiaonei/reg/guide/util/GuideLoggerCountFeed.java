package com.xiaonei.reg.guide.util;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

import org.apache.log4j.Logger;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.reg.common.constants.Globals;


public class GuideLoggerCountFeed {
	private static Logger inshion_log = Logger.getLogger("com.inshion.MyLogger.countfeed");
	public static final int DEBUG = 5;
	public static final int INFO = 4;
	public static final int WARN = 3;
	public static final int ERROR = 2;
	public static final int FATAL = 1;
	
	private static Map<String,Integer> map = new HashMap<String,Integer>();
	public static void reloadConfigMap(){
		DodoyoResource.close("guidelog");
		DodoyoResource.init("guidelog");
		ResourceBundle resourceBundle = DodoyoResource.getResourceBundle("guidelog");
		
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
				inshion_log.info("MyLogger::guidelog.properties reloaded:"+"<"+key+","+value+","+ivalue+">");
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
	public static void printLog(String msg, int type){
		write(msg,type);
	}
	private static void write(String msg, int type){
		String classname = "";
		String method = "";
		String packagename = "";
		int linenumber = -1;
		try {
			StackTraceElement stack = Thread.currentThread().getStackTrace()[3];
			linenumber = stack.getLineNumber();
			String classnames[] = stack.getClassName().split("\\.");
			int max = classnames.length - 1;
			classname = classnames[max];
			if(classname != null && classname.indexOf('$')>=0){
			    try {
                    classname = classname.split("$")[0];
                } catch (Exception e) {
                    // TODO: handle exception
                }
			}
			if(classname == null){
				classname = "";
			}
			packagename = stack.getClassName().substring(0,stack.getClassName().indexOf(classname)-1);
			method = stack.getMethodName();
			
		} catch (RuntimeException e) {
			e.printStackTrace();
		}
		String message = classname+"::"+method+"("+linenumber+"):"+msg;

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
					inshion_log.info("<D>"+message);
				}
				inshion_log.debug(message);
				break;
			case INFO:
				inshion_log.info(message);
				break;
			case WARN:
				inshion_log.warn(message);
				break;
			case ERROR:
				inshion_log.error(message);
				break;
			case FATAL:
				inshion_log.fatal(message);
				break;
			default:
				inshion_log.debug(message);
			break;
			}
		}
		else{
			switch(type){
			case DEBUG:
				if(Globals.DEBUG_ENV){
					inshion_log.info("<D>"+message);
				}
				inshion_log.debug(message);
				break;
			case INFO:
				inshion_log.info(message);
				break;
			case WARN:
				inshion_log.warn(message);
				break;
			case ERROR:
				inshion_log.error(message);
				break;
			case FATAL:
				inshion_log.fatal(message);
				break;
			default:
				inshion_log.debug(message);
			break;
			}
		}
		
	}
	public static void printLog(String msg){
		write(msg,0);
    }
	

    public static void writeLine(Object o, String s) {
    	write(s,0);
    }

    public static void writeLine(Object o, String s, int type) {
        switch (type) {
            case 0:
            	write(s,0);
                break;
            case 1:
            	write(s,ERROR);
                break;
        }

    }
}