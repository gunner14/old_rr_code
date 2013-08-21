package com.xiaonei.tools.profiler;

import java.util.Iterator;
import java.util.Map;

import com.xiaonei.xce.storm.XceStormAdapter;
import com.xiaonei.xce.storm.XceStormListener;
public class ProfilerSubscribe extends XceStormListener {
	private static final long serialVersionUID = 524987463180016456L;
	@Override
	public void handle(Map<String, String> context) {
		Iterator<String> ite = context.keySet().iterator();
		while(ite.hasNext()){
			String key = ite.next();
			System.out.println(key+"=>"+context.get(key));
		}
	}

	public static void main(String[] args){
		if(args.length!=1){
			System.out.println("ProfilerSubscribe Usage:domain");
			System.exit(0);
		}
		
		XceStormListener listener = new ProfilerSubscribe();
		XceStormAdapter.getInstance().subscribe("ProfilerManager-"+args[0], listener);
	}
}
