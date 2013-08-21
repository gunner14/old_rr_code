package com.renren.com.registry.config;

import java.io.FileOutputStream;
import java.util.List;

import com.renren.xoa.registry.XoaService;
import com.renren.xoa.registry.impl.zookeeper.ZookeeperBasedRegistry;

/**
 * 将Registry中的配置导出
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-2-25 下午06:38:43
 */
public class RegistryToXmlExporter {
	
	public static void main(String[] args) throws Exception{
		
		ZookeeperBasedRegistry reg = new ZookeeperBasedRegistry();
		reg.init();
		List<XoaService> services = reg.getServices();
		ConfigUtil.toXml(services, new FileOutputStream("conf/xoa-services_exported.xml"));
		
		System.out.println("done");
	}
	
}
