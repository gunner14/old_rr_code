package com.renren.sns.minisite.feed.xoa.services.broadcast;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;

public class AutoRunBroadcastService implements BroadcastService,
		InitializingBean {

	@Autowired
	@Qualifier("broadcastService")
	private BroadcastService broadcastService;

	private ScheduledExecutorService scheduled;
	
	private final static int BROADCAST_THREAD_NUM =1;

	protected Log logger = LogFactory.getLog(this.getClass());

	@Override
	public void afterPropertiesSet() throws Exception {
		this.scheduled = Executors.newScheduledThreadPool(BROADCAST_THREAD_NUM);
		this.doBroadcast();
	}

	@Override
	public void doBroadcast() {
		final Runnable runnable = new Runnable() {

			@Override
			public void run() {
				if (AutoRunBroadcastService.this.logger.isDebugEnabled()) {
					AutoRunBroadcastService.this.logger
							.debug("AutoRunBroadcastService.doBroadcast().new Runnable() {...}.run():");
				}
				try {
					AutoRunBroadcastService.this.broadcastService.doBroadcast();
				} catch (Exception e) {
					System.out.println("=======Exception!!!====");
					e.printStackTrace();
					// TODO: handle exception
				}
				if (AutoRunBroadcastService.this.logger.isDebugEnabled()) {
					AutoRunBroadcastService.this.logger
							.debug("AutoRunBroadcastService.doBroadcast().new Runnable() {...}.run():end");
				}
			}
		};
		// this.scheduled.scheduleAtFixedRate(runnable, 0, 1, TimeUnit.SECONDS);
		for(int i=0; i < BROADCAST_THREAD_NUM ;i++){
			this.scheduled.scheduleAtFixedRate(runnable, 0,	1, TimeUnit.SECONDS);
		}
			
	}

}
