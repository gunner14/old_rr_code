package com.xiaonei.reg.guide.action.a090729;

import com.xiaonei.reg.guide.util.GuideLogger;

public class MyStringUtil {
	public static Object stringToObject(String src,Class<?> type){
		Object obj = null;
		GuideLogger.printLog("stringToObject() - src:"+src+" type:"+type.getName());
		if(type.getName().equals("int") || type.getName().contains("Integer")){
			try {
				obj = Integer.parseInt(src);
			} catch (Exception e) {
				GuideLogger.printLog("error - "+src+type.getName(),GuideLogger.DEBUG);
			}
		}
		if(type.getName().contains("String")){
			obj = src;
		}
		return obj;
	}	
	
	public static  Object getDefaultObject(Class<?> type) {
		Object obj = null;
		GuideLogger.printLog("getDefaultObject() - type:"+type.getName());
		if(type.getName().equals("int") || type.getName().contains("Integer")){
			obj = 0;
		}
		if(type.getName().contains("String")){
			obj = "";
		}
		return obj;
	}
	public static  String getUpperHead(String s){
		if(s==null || s.trim().equals("")){
			return s;
		}
		if(s.length()==1){
			return s.toUpperCase();
		}
		return s.toUpperCase().substring(0,1)+s.substring(1);
	}
	
	public static boolean isEmpty(String str){
		  return str == null || str.length() == 0 || str.trim().length() == 0;
	}
}
