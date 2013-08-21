package com.renren.com.registry.config;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.List;

import com.renren.xoa.registry.XoaService;

/**
 * 
 * 将从DB到处的txt格式配置转化给xml给是
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-2-25 下午06:44:49
 */
public class TxtToXmlConvertor {

	public static void main(String[] args) throws Exception {
		
		List<XoaService> services = ConfigUtil.readFromTxt(new FileInputStream(
				"conf/xoa-services.txt"));
		System.out.println(services.size() + " services loaded");

		ConfigUtil.toXml(services,
				new FileOutputStream("conf/xoa-services.xml"));
		System.out.println("done");
	}
	
}
