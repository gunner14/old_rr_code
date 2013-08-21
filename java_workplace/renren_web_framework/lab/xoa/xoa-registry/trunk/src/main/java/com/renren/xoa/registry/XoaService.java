package com.renren.xoa.registry;

import java.util.Comparator;
import java.util.List;

/**
 * 封装一个XOA服务的配置信息
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 */
public class XoaService {

	private String serviceId;
	
	private List<XoaServiceDescriptor> nodes;

	public String getServiceId() {
		return serviceId;
	}

	public void setServiceId(String serviceId) {
		this.serviceId = serviceId;
	}

	public List<XoaServiceDescriptor> getNodes() {
		return nodes;
	}

	public void setNodes(List<XoaServiceDescriptor> nodes) {
		this.nodes = nodes;
	}
	
	public static final Comparator<XoaService> COMPARATOR = new Comparator<XoaService>() {
		@Override
		public int compare(XoaService o1, XoaService o2) {
			return o1.getServiceId().compareTo(o2.getServiceId());
		}
	};
	
}
