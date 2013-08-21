package com.xiaonei.tools.profiler;

import java.util.HashMap;
import java.util.Map;

import com.xiaonei.xce.storm.XceStormAdapter;

public class ProfilerNotify {
	public static void main(String[] args){
		if(args.length<2 ||args.length%2!=1){
			System.out.println("ProfilerNotify Usage:domain properties");
			System.exit(0);
		}
		String domain = "ProfilerManager-"+args[0];
		Map<String,String> properties = new HashMap<String,String>();
		for(int i=1;i<args.length;i=i+2){
			properties.put(args[i], args[i+1]);
		}
		System.out.println(domain);
		System.out.println(properties);
		XceStormAdapter.getInstance().notify(domain,properties);
		System.out.println("notify over");
		System.exit(0);
	}
}
