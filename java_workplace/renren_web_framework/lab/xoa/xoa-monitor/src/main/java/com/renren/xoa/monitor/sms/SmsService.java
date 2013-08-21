package com.renren.xoa.monitor.sms;


/**
 * 抽象短信接口
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-8-25 下午04:27:13
 */
public interface SmsService {

    /**
     * 发送短信
     * 
     * @param mobilePhone
     * @param hostport
     * @param serviceName
     * @param messageContent
     * @return
     */
    public boolean sendSms(String mobilePhone, String hostport, String serviceName, String state, String messageContent);
    
    
    
    
}
