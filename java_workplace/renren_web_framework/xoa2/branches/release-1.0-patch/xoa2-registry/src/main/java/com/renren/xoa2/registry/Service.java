package com.renren.xoa2.registry;

import java.util.List;




/**
 * Service layer
 * 
 * @author Wang Tai (cn.wang.tai@gmail.com)
 * @date Dec 15, 2011 1:03:19 AM
 * 
 */
public class Service {

    private String id;
    
    private List<Node> serviceList;
    
    private List<String> alarmPhones;


	public String getId() {
		return id;
	}

	public List<Node> getServiceList() {
		return serviceList;
	}

	public List<String> getAlarmPhones() {
		return alarmPhones;
	}

	public void setId(String id) {
		this.id = id;
	}

	public void setServiceList(List<Node> serviceList) {
		this.serviceList = serviceList;
	}

	public void setAlarmPhones(List<String> alarmPhones) {
		this.alarmPhones = alarmPhones;
	}
    
}
