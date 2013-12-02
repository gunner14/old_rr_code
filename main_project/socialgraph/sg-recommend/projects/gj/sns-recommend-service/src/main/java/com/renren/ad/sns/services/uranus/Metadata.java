package com.renren.ad.sns.services.uranus;

import java.util.Map;

/**
 * 
 * @author ivoryxiong
 *
 */
public abstract class Metadata {
	protected Number [] contents;
	protected static Map<String,Integer> fields;
	protected static String[] fieldsIdx;
	
	public Number get(String key){
		Number res = null;
		if( fields != null && fields.containsKey(key)) {
			int idx = fields.get(key);
			res = contents[idx] ;
		}
		return res;
	}

	public boolean set(String key, Number value){
		boolean res = false;
		if( fields != null && fields.containsKey(key)) {
			int idx = fields.get(key);
			contents[idx] = value;
		}
		return res;
	}
	
	public static String contents2String(Number [] numbers){
		StringBuilder res = new StringBuilder("");
		boolean first = true;
		for( Number number : numbers){
			if( ! first ) {
				res.append(",");
			}else{
				first = false;
			}
			res.append(number);
		}
		if( res.length() > 0) {
			return res.toString();
		}
		return null;

	}
	
	public static Number [] string2Contents(String str){
		if( str == null) return null;
		String [] tmps = str.split(",");
		Number [] numbers = new Number[tmps.length];
		for( int i =0; i < tmps.length ; i ++ ){
			numbers[i]=Long.parseLong(tmps[i]);
		}
		return numbers;
		
	}
	
	public Number [] getValues(){
		return contents;
	}
	
	public String [] getKeys(){
		return fieldsIdx;
	}
//	public abstract void set(String key, Number);
}
