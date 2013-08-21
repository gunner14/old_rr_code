package com.renren.xoa2.registry;

import java.util.List;




/**
 * Service layer
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @date Dec 15, 2011 1:03:19 AM
 * 
 */
public interface Service {

    public String getId();
    
    public ServiceGroup getServiceGroup();
    
    public List<String> getAlarmPhones();
}
