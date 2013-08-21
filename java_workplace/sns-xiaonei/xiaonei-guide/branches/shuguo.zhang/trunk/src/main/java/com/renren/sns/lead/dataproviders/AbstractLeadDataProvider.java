package com.renren.sns.lead.dataproviders;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.guide.util.GuideLogger;
public abstract class AbstractLeadDataProvider {
	private static ExecutorService es = new ThreadPoolExecutor(10, 10,
            0L, TimeUnit.MILLISECONDS,
            new LinkedBlockingQueue<Runnable>(1));
	//private static AtomicInteger statint = new AtomicInteger(0);
	public <T>T getData(final User host,final long timeout,final Object ...param){
		T ret = null;
		if(host == null){
			GuideLogger.printLog(" host:0",GuideLogger.WARN);
		}
		else{
			/*if(statint.get() >= 30){
				GuideLogger.printLog(" host:"+host.getId()+" statint:"+statint,GuideLogger.WARN);
			}
			else{
				GuideLogger.printLog(" host:"+host.getId()+" statint:"+statint);
			}
			statint.incrementAndGet();*/
			Future<T> mayret = null;
			try {
				mayret = es.submit(new Callable<T>(){
					@SuppressWarnings("unchecked")
					@Override
					public T call() throws Exception {
						T data = (T)doGetData(host,timeout,param);
						return data;
					}});
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" "+this.getClass().getSimpleName()+" "+e.toString(),GuideLogger.ERROR);
			}
			
			
			try {
				if(mayret != null){
					ret = (T) mayret.get(timeout, TimeUnit.MILLISECONDS);
				}
				else{
					GuideLogger.printLog(" host:"+host.getId()+" "+this.getClass().getSimpleName()+" submitfailed",GuideLogger.WARN);
				}
			} catch (InterruptedException e) {
				GuideLogger.printLog(" host:"+host.getId()+" "+this.getClass().getSimpleName()+" "+e.toString(),GuideLogger.ERROR);
			} catch (ExecutionException e) {
				GuideLogger.printLog(" host:"+host.getId()+" "+this.getClass().getSimpleName()+" "+e.toString(),GuideLogger.ERROR);
			} catch (TimeoutException e) {
				GuideLogger.printLog(" host:"+host.getId()+" "+this.getClass().getSimpleName()+" "+e.toString(),GuideLogger.ERROR);
			} catch (Exception e) {
				GuideLogger.printLog(" host:"+host.getId()+" "+this.getClass().getSimpleName()+" "+e.toString(),GuideLogger.ERROR);
				e.printStackTrace();
			}
			finally{
				//statint.decrementAndGet();
			}
		}
		return ret;
	}
	
	protected abstract <T>T doGetData(User host,final long timeout,final Object ...param);
	
	public static void main(String[] args) {
		new AbstractLeadDataProvider(){

			@Override
			protected <T> T doGetData(User host, long timeout, Object... param) {
				// TODO Auto-generated method stub
				return null;
			}};
	}
	
}
