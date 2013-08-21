package com.dodoyo.invite.core.logic;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.log4j.PropertyConfigurator;

import java.io.IOException;
import java.util.Properties;

/**
 * This class, log target can be switched as log4j, commons.logling or other log writer
 *
 * @author Administrator
 */
public class LogUtils {
    static {
        Properties pp = new Properties();
        try {
            pp.load(LogUtils.class.getResourceAsStream("/log4j.properties"));
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        PropertyConfigurator.configure(pp);
    }

    private static Log logger = LogFactory.getLog("service");

    public static void logInfo(Object o) {
        logger.info(o);
//		System.out.println(o);
    }

    public static void logErr(Object o) {
        logger.error(o);
//		System.out.println(o);
    }

    public static void logDebug(Object o) {
        logger.debug(o);
//		System.out.println(o);
    }

    public static void logWarn(Object o) {
        logger.warn(o);
//		System.out.println(o);
    }


    public static void logExceptionStack(Exception e) {
        StackTraceElement[] ste = e.getStackTrace();
        StringBuffer sb = new StringBuffer();
        sb.append(e.getMessage() + "\n");
        for (int i = 0; i < ste.length; i++) {
            sb.append(ste[i].toString() + "\n");
        }
        logErr(sb);
    }

    public static void main(String[] args) {
        LogUtils.logInfo("1234" + System.currentTimeMillis());

        LogUtils.logDebug("1234345DEBUG" + System.currentTimeMillis());
    }
}