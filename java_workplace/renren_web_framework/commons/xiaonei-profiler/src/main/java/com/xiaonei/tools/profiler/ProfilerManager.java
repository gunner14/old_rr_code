package com.xiaonei.tools.profiler;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import com.xiaonei.xce.storm.XceStormAdapter;
import com.xiaonei.xce.storm.XceStormListener;
public class ProfilerManager extends XceStormListener{
	private static final long serialVersionUID = -2947569724642533181L;
	@Override
	public void handle(Map<String, String> properties) {
		if(properties.get("enable")!=null){
			if(enable){
				enable = false;
			}else{
				enable = true;
			}
		}
		if(properties.get("time")!=null){
			try{
				int time = Integer.parseInt(properties.get("time"));
				ProfilerPoint.setTimeLogThread(time);
			}catch(NumberFormatException ex){
				ex.printStackTrace();
			}
		}
	}

	private volatile boolean enable = true;
	private final String domain;
	private static ConcurrentHashMap<String,ProfilerManager> managers = new ConcurrentHashMap<String,ProfilerManager>();
	private ConcurrentHashMap<String, ProfilerPoint> points = new ConcurrentHashMap<String, ProfilerPoint>(16);
	
	private ProfilerManager(String domain){
		this.domain = domain;
	}
	public void subscribe(){
		XceStormAdapter.getInstance().subscribe("ProfilerManager-xiaonei.com", ProfilerManager.this);
	}
	
	public static ProfilerManager getInstanceByDomain(String domain){
		ProfilerManager manager = managers.get(domain);
		if(manager==null){
			manager = new ProfilerManager(domain);
			ProfilerManager old = managers.putIfAbsent(domain, manager);
			if(old!=null)return old;
			manager.subscribe();
		}
		return manager;
	}
	
	public void setEnable(boolean enable){
		this.enable = enable;
	}
	public boolean isEnable(){
		return enable;
	}
	public String getDomain(){
		return domain;
	}
	
	public ProfilerPoint getProfilerPoint(String name,String domain){
		ProfilerPoint point = points.get(name);
		if(point==null){
			point  = new ProfilerPoint(name,domain);
			ProfilerPoint old = points.putIfAbsent(name, point);
			if(old!=null)return old;
		}
		return point;
	}
	
	public void clear(){
		points.clear();
	}
	
	public java.util.Iterator<ProfilerPoint> profilerIterator(){
		return points.values().iterator();
	}
}
	