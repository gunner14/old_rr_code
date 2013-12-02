package com.renren.ad.sns.services.uranus.util;


public class ExceptionUtil {

    public static String getStackTraceString(Exception exp){
        StringBuffer result = new StringBuffer();
        if (exp.getMessage() != null){
            result.append(exp.getMessage()+"\n");
        }
        StackTraceElement[] steArr = exp.getStackTrace();
        for(StackTraceElement ste : steArr){
            result.append(ste.toString() + "\n");
        }
        return result.toString();
    }
}
