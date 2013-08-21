package com.renren.xoa2.registry.impl;

import java.util.List;

import com.renren.xoa2.registry.Service;
import com.renren.xoa2.registry.ServiceGroup;

/**
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @date Dec 15, 2011 1:26:02 AM
 * 
 */
public class ServiceImpl implements Service {

    private String id;
    
    private ServiceGroup activeServiceGroup;
    
    private List<String> alarmPhones;
    
    @Override
    public String getId() {
        return id;
    }

    @Override
    public ServiceGroup getServiceGroup() {
        return activeServiceGroup;
    }

    @Override
    public List<String> getAlarmPhones() {
    	return alarmPhones;
    }
    
    public ServiceGroup getActiveServiceGroup() {
        return activeServiceGroup;
    }
    
    public void setActiveServiceGroup(ServiceGroup activeServiceGroup) {
        this.activeServiceGroup = activeServiceGroup;
    }
    
    public void setId(String id) {
        this.id = id;
    }
    
    public void setAlarmPhones(List<String> alarmPhones) {
        this.alarmPhones = alarmPhones;
    }
    
    
}
