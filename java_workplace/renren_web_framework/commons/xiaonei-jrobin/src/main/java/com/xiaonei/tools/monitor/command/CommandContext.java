package com.xiaonei.tools.monitor.command;

import java.util.HashMap;
import java.util.Map;

public class CommandContext {
	private Map<String,String> context = new HashMap<String,String>();
	public CommandContext(){
		
	}
	public void setContext(String key,String value){
		context.put(key, value);
	}
	public String getContext(String key){
		return context.get(key);
	}
	public void reset(){
		context.clear();
	}
}
