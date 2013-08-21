package com.renren.xoa.mock.backend;

import java.util.Properties;
import java.util.Set;
import java.util.Map.Entry;

import com.renren.xoa.Method;
import com.renren.xoa.XoaResponse;

public class BackenManager {

	private static BackenManager instance =  new BackenManager();
	
	public static BackenManager getInstance() {
		return instance;
	}
	
	public XoaResponse handle(Method method) {
		
		//method.get
		
		return null;
	}
	
	public static void main(String[] args) {

		//System.out.println(System.getenv());
		
		System.out.println(System.getProperties());
		/*
		Properties ps = System.getProperties();
		Set<Entry<Object, Object>> es = ps.entrySet();
		for (Entry<Object, Object> entry : es) {
			System.out.println(entry.getKey() + "=" + entry.getValue());
		}*/
	}
}
