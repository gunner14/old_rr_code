/**
 * 
 */
package com.xiaonei.reg.register.logic.additional;

import java.util.LinkedList;
import java.util.List;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.utility.StringFormat;

/**
 * @author wangtai
 * 
 */
public class RegQQLogic {
	private static final MemCacheManager mem = MemCacheFactory.getInstance()
			.getManagerByPoolName(MemCacheKeys.pool_user_space);
	private static final String key = "mem_reg_qq_activate";

	public synchronized static void set(String qqE) {
		if (!StringFormat.isValidEmailAddr(qqE)) {
			return;
		}
		String qq = qqE.split("@")[0];
		if (!StringUtils.isNumeric(qq)) {
			return;
		}
		Object o = mem.get(key);
		List<String> qqList = null;
		if (o == null) {
			qqList = new LinkedList<String>();
		} else {
			qqList = cast(o);
		}
		qqList.add(qq);
		mem.set(key, qqList,(int)MemCacheKeys.hour*3);
	}
	public synchronized static void rm(String qq) {
		if (!StringUtils.isNumeric(qq)) {
			return;
		}
		Object o = mem.get(key);
		List<String> qqList = null;
		if (o == null) {
			qqList = new LinkedList<String>();
		} else {
			qqList = cast(o);
		}
		qqList.remove(qq);
		mem.set(key, qqList);
	}
	
	public static List<String> get(){
		Object o = mem.get(key);
		if(null == o){
			return new LinkedList<String>();
		}else{
			return cast(o);
		}
	}

	@SuppressWarnings("unchecked")
	private static List<String> cast(Object o) {
		return (List<String>) o;
	}

}
