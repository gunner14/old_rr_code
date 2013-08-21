package com.renren.sns.lead.dataproviders;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
import com.xiaonei.reg.guide.util.GuideTimeCost;



class LeadDataThreadPoolExecutor extends ThreadPoolExecutor{
	public LeadDataThreadPoolExecutor(int corePoolSize, int maximumPoolSize,long keepAliveTime, TimeUnit unit, BlockingQueue<Runnable> workQueue) {
		super(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue);
	}
	
	public <T> Future<T> submit(Callable<T> task, int userId, String logkey) {
		Future<T> ret = null;
		try {
			GuideLogger.printLog(" host:"+userId+" logkey:"+logkey);
			ret = super.submit(task);
		} catch (Exception e) {
			GuideLogger.printLog(" host:"+userId+" logkey:"+logkey+" e:"+e.toString(),GuideLogger.ERROR);
		}
		return ret;
	}
}
public class LeadDataCallableSubmitter {
	private LeadDataThreadPoolExecutor es = new LeadDataThreadPoolExecutor(10, 10,
            0L, TimeUnit.MILLISECONDS,
            new LinkedBlockingQueue<Runnable>(1));
	private Class<?> clazz = null;
	private String invokerName = null;
	
	public LeadDataCallableSubmitter(Class<?> clazz) {
		this.clazz = clazz;
	}
	
	public LeadDataCallableSubmitter(String invokerName) {
		this.invokerName = invokerName;
	}
	
	private String getInvokerName(){
		String ret = "";
		if(invokerName != null){
			ret = invokerName;
		}
		else if(clazz == null){
			
		}
		else{
			ret = clazz.getName();
		}
		return ret;
	}

	@SuppressWarnings("unchecked")
	public <T>T getData(final User host,final long timeout,AbstractLeadDataCallable<T>task){
		T data = null;
		if(task != null){
			List<AbstractLeadDataCallable<? extends Object>>tasks = new ArrayList<AbstractLeadDataCallable<? extends Object>>();
			tasks.add(task);
			Map<String,Object> datas = (Map<String,Object>)getDatas(host, timeout, tasks, true);
			data = (T)datas.get(task.getKey());
		}
		return data;
	}

	public Map<String,Object> getDatas(final User host,final long timeout,List<AbstractLeadDataCallable<? extends Object>>tasks, boolean ordered){
		GuideTimeCost tc = GuideTimeCost.begin();
		if(host == null){
			GuideLogger.printLog(" host:null"+" timeout:"+timeout);
			return null;
		}
		int userId = host.getId();
		if(tasks == null){
			GuideLogger.printLog(" host:"+userId+" timeout:"+timeout+" tasks:null");
			return null;
		}
		Map<String,Object> retmap = new HashMap<String, Object>();
		List<Future<? extends Object>> futures = new ArrayList<Future<? extends Object>>();
		List<String> keylist = new ArrayList<String>();
		tc.endStep();
		int cur = 0;
		for (AbstractLeadDataCallable<? extends Object> task : tasks) {
			Future<? extends Object> f = null;
			f = es.submit(task,host.getId(),getInvokerName()+"-"+task.getKey());
			if(f!=null){
				futures.add(f);
				keylist.add(task.getKey());
			}
			cur ++;
		}
		tc.endStep();
		
		long left = timeout;
		cur = 0;
		StringBuilder sb = new StringBuilder();
		sb.append(" host:"+userId + "");
		for (Future<? extends Object> future : futures) {
			String runkey = keylist.get(cur);
			sb.append(" [("+cur+")"+runkey+"|v:");
			Object value = null;
			long start = System.currentTimeMillis();
			try {
				if(future != null){
					tc.endStep("before future.get-"+runkey);
					value = future.get(left, TimeUnit.MILLISECONDS);
					tc.endStep("get-"+runkey);
				}
				else{
					GuideLogger.printLog(" host:"+userId+" runkey:"+runkey+"("+cur+")"+" future.null",GuideLogger.WARN);
					value = "";
				}
			} catch (InterruptedException e) {
				GuideLogger.printLog(" host:"+userId+" runkey:"+runkey+"("+cur+")"+" e:"+e.toString(),GuideLogger.ERROR);
			} catch (ExecutionException e) {
				GuideLogger.printLog(" host:"+userId+" runkey:"+runkey+"("+cur+")"+" e:"+e.toString(),GuideLogger.ERROR);
			} catch (TimeoutException e) {
				GuideLogger.printLog(" host:"+userId+" runkey:"+runkey+"("+cur+")"+" e:"+e.toString(),GuideLogger.ERROR);
			} catch (RuntimeException e) {
				GuideLogger.printLog(" host:"+userId+" runkey:"+runkey+"("+cur+")"+" e:"+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
            } catch (Exception e) {
            	GuideLogger.printLog(" host:"+userId+" runkey:"+runkey+"("+cur+")"+" e:"+e.toString(),GuideLogger.ERROR);
            	e.printStackTrace();
			}
			long end = System.currentTimeMillis(); 
			long delta = end - start;
			left = left - delta;
			if(left<0)left = 0;
			if(value == null){
				//value = "";
			}
			else{
				retmap.put(runkey, value);
			}
			sb.append(value+"]<"+delta+"> ");
			cur ++;
			tc.endStep();
			if(ordered && value != null) {
				break;
			}
		}
		tc.endFinally();
		GuideLogger.printLog(" host:"+userId+" timeout:"+timeout+" task:"+tasks.size()+" ret:"+sb);
		return retmap;
	}
	
	
	public static void main(String[] args) {
	}
	
}
