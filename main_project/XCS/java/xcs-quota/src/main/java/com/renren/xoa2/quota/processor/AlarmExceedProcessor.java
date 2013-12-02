package com.renren.xoa2.quota.processor;

import java.util.concurrent.atomic.AtomicInteger;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xcs.Configuration;
import com.renren.xcs.ServiceRegistryAccessor;
import com.renren.xoa2.quota.checker.DefaultQuotaChecker;
import com.renren.xoa2.quota.utils.IPAddrUtil;
import com.renren.xoa2.quota.utils.SMSNotifyUtil;

/**
 * 超过配额报警处理器(初期磨合配额系统的策略，对超过配额的情况不直接拒绝访问，而是积累至一定的threshold后向调用者发报警邮件和短信)
 * @author Xue Wenxin Oct 18, 2012 3:35:11 PM
 */
public class AlarmExceedProcessor implements ExceedProcessor {

	private ServiceRegistryAccessor accessor;
	
	private Configuration config;
	
	private AtomicInteger number = new AtomicInteger(0);
	
	private static final int THRESHOLD = 1000;
	
	private Log logger = LogFactory.getLog(DefaultQuotaChecker.class);

	public AlarmExceedProcessor(ServiceRegistryAccessor accessor, Configuration config) {
		super();
		this.accessor = accessor;
		this.config = config;
	}
	
	@Override
	public void process() {
		if(number.incrementAndGet() % THRESHOLD == 0){
			smsNotify(accessor.getService(), accessor.getVersion(), config.getClientUsername());
			logger.warn("Quota Exceeded "+THRESHOLD+" times For Service["+accessor.getService()+"],Version["+accessor.getVersion()+"]");
			number.set(0);
		}	
	}

    private void smsNotify(String serviceId, int versionId, String clientId) {
    	String msg = "ClientId:" + clientId + "[" + IPAddrUtil.getLocalhostIp() +"]"+" Exceeded Quota"+THRESHOLD+" times For Service["+serviceId+"],Version["+versionId+"]";
        SMSNotifyUtil.smsSend(msg, SMSNotifyUtil.EXCEED_QUOTA);
    }
	
	public int getNumber() {
		return number.get();
	}
}
