package com.renren.sns.lead.dataproviders;

import java.util.concurrent.Callable;

import com.xiaonei.reg.guide.util.GuideLogger;

public abstract class AbstractLeadDataCallable<T> implements Callable<T>{
	private String key = null;
	private int hostid = 0;
	public AbstractLeadDataCallable(String key,int hostid){
		this.key = key;
		this.hostid = hostid;
	}
	
	public String getKey() {
		return key;
	}
	
	@Override
	public T call() throws Exception {
		T ret = null;
		long start = System.currentTimeMillis();
		try {
			ret = doCall();
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+hostid +" e:"+e.toString(),GuideLogger.ERROR);
			e.printStackTrace();
		}
		long end = System.currentTimeMillis();
		long cost = end - start;
		GuideLogger.printLog(" host:"+hostid +" key:"+key+" cost:"+cost);
		return ret;
	}


	protected abstract T doCall()throws Exception;
}
