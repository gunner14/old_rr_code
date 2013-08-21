package com.xiaonei.tools.profiler;

import java.util.Map;
import xce.storm.ObserverPrx;
import com.xiaonei.xce.storm.XceStormAdapter;

public class ProfilerQuery {
	public static void main(String[] args){
		if(args.length!=1){
			System.out.println("ProfilerQuery Usage:domain");
			System.exit(0);
		}
		String domain = args[0];
		Map<String,ObserverPrx[]> prxs = XceStormAdapter.getInstance().query("ProfilerManager-"+domain);
		for(String key:prxs.keySet()){
			System.out.println(key+"========");
			ObserverPrx[] list = prxs.get(key);
			for(ObserverPrx k:list){
				System.out.println(k);
			}
		}
		
		System.exit(0);
	}
}
