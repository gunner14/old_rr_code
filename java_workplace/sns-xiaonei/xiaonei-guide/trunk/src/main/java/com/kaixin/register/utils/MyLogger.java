package com.kaixin.register.utils;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.log4j.Logger;

import com.kaixin.Globals;


public class MyLogger {
	public static void main(String[] args) {
		String s = "12345";
		String a = s.substring(0,5);
		System.out.println(s.length() + "," +a);
		
	}
	public static void writeLine(String s){
		Date D = new Date();
		SimpleDateFormat sf = new SimpleDateFormat("HH:mm:ss yyyy-MM-dd");
		sf.format(D);
		
		try {
			if(		//要看谁的Log，就把谁的注释解开；要不看谁的，就把它注释掉……
					true
//					|| s.contains("CurrentStepDoneAction")
//					|| s.contains("Control")
			){
				Logger log = Logger.getLogger(Globals.regLogName);
				log.debug(" [wei.cheng@opi-corp.com] "+s);
			}
		} catch (Exception e) {
		}
	}
	public static void writeLine(String s, int type){
		if(type == 1){
			writeLine("~!Error -- "+s);
		}
	}
	public static void writeLine(Object o,String s){
		writeLine(o,s,0);
	}
	
	public static void writeLine(Object o,String s,int type){
		String classname = o.getClass().getSimpleName();
		switch(type){
		case 0:
			writeLine(classname+"::"+s);
			break;
		case 1:
			writeLine("~!Error -- "+classname+"::"+s);
			break;
		}

	}
}