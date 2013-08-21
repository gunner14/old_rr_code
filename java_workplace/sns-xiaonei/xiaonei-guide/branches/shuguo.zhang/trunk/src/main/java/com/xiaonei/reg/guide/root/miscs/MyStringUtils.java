package com.xiaonei.reg.guide.root.miscs;

import com.xiaonei.commons.interceptors.MD5;
import com.xiaonei.reg.guide.util.GuideLogger;

/**
 * MyStringUtils.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-9-6 添加 containsIgnoreCase
 * 2010-9-13 添加 String headToUpper
 */
public class MyStringUtils {
	public static void main(String[] args) {
//		System.out.println("aa".contains(null));
		
		System.out.println(contains("", ""));
		System.out.println(getSubstring("", 2));
		System.out.println(getSubstring(null, 2));
		System.out.println(getSubstring("1", 2));
		System.out.println(getSubstring("12", 2));
		System.out.println(getSubstring("123", 2));
		System.out.println(getSubstring("中", 2));
		System.out.println(getSubstring("中国", 2));
		System.out.println(getSubstring("中国人", 2));
		System.out.println(MD5.digest("81120"));
		System.out.println(stringToObjectnew("1234", Integer.class));
		System.out.println(stringToObjectnew(null, Integer.class));
		System.out.println(stringToObjectnew(null, String.class));
		
		System.out.println(hasChi("中"));
		System.out.println(hasChi("123dgh"));
		System.out.println(("中".getBytes().length == "中".length())?"s1无汉字":"s1有汉字");  
		System.out.println(("dfdf".getBytes().length == "dfdf".length())?"s1无汉字":"s1有汉字");
		
		/*Object o1 = (true ? new Integer(123) :  new String("1.0")); 
		Object o11 = (true ? new Character('1') :  new Integer(1));
		
		char c1 = (char)((int)1);
		System.out.println(o1.getClass()+"中"+o1);
		System.out.println(o11.getClass()+"中11"+o11);

		Object o2 = (false ? new Integer(1) : new Double(1.0)); 

		int a = 99;
		//Object b = (true ? new Double(123.0) :  new String("1.0"))+123;//.compareTo(null);
		System.out.println(o2.getClass());
		//System.out.println(b.getClass()+""+b);*/
		
/*        Object o1 = (true ? new Integer(5) : new Double(1.0));
        Object o2 = (false ? new Integer(6) : new Double(8.0));
        System.out.println(o1.getClass() + "," + ((Double) o1).intValue());//class java.lang.Double,5
        System.out.println(o2.getClass() + "," + ((Double) o2).intValue());//class java.lang.Double,8 
*/		System.out.println(stringToObjectnew(null, String.class));
	}
	
	public static boolean equalsIgnoreCase(String a,String b){
	    if(a == null) a = "";
	    if(b == null) b = "";
	    a = a.toLowerCase();
	    b = b.toLowerCase();
	    return equals(a, b);
	}
	
	public static String getSubstring(String s,int length){
		String ret = null;
		if(s == null){
			ret = "";
		}
		else if(s.length()<=length){
			ret = s;
		}
		else{
			ret = s.substring(0, length);
		}
		return ret;
	}
	public static boolean contains(String a,String b){
		boolean ret = false;
		if(a == null || b == null){
			ret = false;
		}
		else if(a.contains(b)){
			ret = true;
		}
		return ret;
	}
	
	public static boolean containsIgnoreCase(String a,String b){
	    boolean ret = false;
        if(a == null || b == null){
            ret = false;
        }
        else {
            a = a.toLowerCase();
            b = b.toLowerCase();
            if(a.contains(b)){
                ret = true;
            }
        }
        return ret;
	}
	public static boolean isNullOrEmpty(String s){
		if(s == null || "".equals(s)){
			return true;
		}
		return false;
	}
	public static boolean notEmpty(String s){
		return !isNullOrEmpty(s);
	}
	
	public static String getUpperHead(String s){
		if(s==null || s.trim().equals("")){
			return s;
		}
		if(s.length()==1){
			return s.toUpperCase();
		}
		return s.toUpperCase().substring(0,1)+s.substring(1);
	}
	
	public static Object stringToObject(String src,Class<?> type){
		Object obj = null;
		GuideLogger.printLog("stringToObject() - src:"+src+" type:"+type.getName());
		if(type.getName().equals("int") || type.getName().contains("Integer")){
			try {
				obj = Integer.parseInt(src);
			} catch (Exception e) {
				GuideLogger.printLog("error - "+src+type.getName()+" e:"+e.toString(),GuideLogger.DEBUG);
				obj = 0;
			}
		}
		if(type.getName().contains("String")){
			obj = src;
		}
		return obj;
	}
	public static String getNotNullString(String string) {
		return string == null?"":string.trim();
	}
	public static boolean equals(String a, String b) {
		if(a == null && b == null)return true;
		if(a != null ){
			return a.equals(b);
		}
		return false;
	}	
	
	public static int stringToInt(String string){
		int ret = -1;
		try {
			int a = Integer.parseInt(string);
			ret = a;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return ret;
	}
	@SuppressWarnings("unchecked")
	public static <T extends Object> T stringToObjectnew(String src,Class<?> type){
		T obj = null;
		GuideLogger.printLog(" src:"+src+" type:"+type.getName());
		if(type.getName().equals("int") || type.getName().contains("Integer")){
			try {
				Integer i = Integer.parseInt(src);
				obj = (T)i;
			} catch (Exception e) {
				GuideLogger.printLog("error - "+src+type.getName()+" e:"+e.toString(),GuideLogger.DEBUG);
				obj = (T)Integer.valueOf(0);
			}
		}
		if(src!=null && type.getName().contains("String")){
			obj = (T)src;
		}
		return obj;
	}
	
	public static boolean hasChi(String str) {
		boolean ret = false;
		if(str!=null){
			ret = ((str.getBytes().length == str.length())?false:true);
		}
		return ret;
	}

   public static String headToUpper(String s) {
        if (s == null) return "";
        StringBuilder sb = new StringBuilder();
        if (s.length() >= 1) {
            sb.append(s.substring(0, 1).toUpperCase());
            sb.append(s.substring(1).toLowerCase());
        } else {
            sb.append(s.toUpperCase());
        }
        return sb.toString();
    }
}
