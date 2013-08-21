package com.xiaonei.platform.core.utility;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;

public class VerifyCode {
	private static VerifyCode instance = null;
	private static char code[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g'
		,'h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D'
		,'E','F','G','H','I','G','M','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	private static Random ran = new Random() ;
	static {
		instance = new VerifyCode();
	}	
	public static VerifyCode getInstance() {
		return instance;
	}	
	public static String getVerifyCode() {
		Random ran = new Random();
		StringBuffer sb=new StringBuffer();
		for (int i=0;i<20;i++){
			sb.append(ran.nextInt(10));
		}
		return sb.toString();
	}
	/**
	 * 获取长度是  length 的 [0-9][a-z][A-Z]的字符串
	 * 
	 * @param length
	 * @return
	 */
	public static String getVerifyCode(final int length) {
		Random ran=new Random();
		StringBuffer buf=new StringBuffer();
		for (int i = 0; i < length; i++) {
			buf.append(code[ran.nextInt(code.length)]);
		}
		return buf.toString();
	}
	
	
	public String getVerifyCode(final String key)
	{
		 MemCacheFactory fac = MemCacheFactory.getInstance() ;
		 MemCacheManager cache = fac.getManagerByPoolName(MemCacheKeys.pool_icode);	//校内开心有差异，咨询过张洁，统一改成校内的
		 return (String)cache.get(key);		
	}
	public static void main(String args[]){
		Map mp = new HashMap() ;
		for(int i =0 ;i<1000000000;i++){
			String code = getVerifyCode(10) ;
		    mp.put(code, code) ;
			System.out.println(code) ;
			System.out.println(mp.size()) ;
		}
	}
	
}
