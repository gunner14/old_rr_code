package com.renren.com.registry.config;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

import com.renren.xoa.registry.XoaService;
import com.renren.xoa.registry.XoaServiceDescriptor;
import com.renren.xoa.registry.XoaServiceDescriptorBase;

/**
 * 
 * xoa配置的工具类
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-2-25 下午05:41:23
 */
public class ConfigUtil {

	/**
	 * 将配置信息转化给XML格式并写道输出流中去
	 * 
	 * @param services
	 * @param os
	 */
	public static void toXml(List<XoaService> services, OutputStream os) {
		
		Document document = DocumentHelper.createDocument();
		
		document.setXMLEncoding("UTF-8");
		
		Element rootNode = document.addElement("xoa");
		Element servicesNode = rootNode.addElement("services");

		//先排序
		sort(services);
		
		for (XoaService svc : services) {
			
			Element serviceNode = servicesNode.addElement("service");
			serviceNode.addAttribute("id", svc.getServiceId());
			Element nodesNode = serviceNode.addElement("nodes");
			for (XoaServiceDescriptor desc : svc.getNodes()) {
				Element nodeNode = nodesNode.addElement("node");
				nodeNode.addAttribute("ip", desc.getIpAddress());
				nodeNode.addAttribute("port", desc.getPort() + "");
				nodeNode
						.addAttribute(
								"status",
								desc.isDisabled() ? XoaServiceDescriptor.STATUS_DISABLED
										: XoaServiceDescriptor.STATUS_ENABLED);
			}
		}
		XMLWriter output = new XMLWriter(new OutputStreamWriter(os));
		try {
			output.write(document);
			output.flush();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				output.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * 排序
	 * @param services
	 */
	public static void sort(List<XoaService> services) {
		Collections.sort(services, XoaService.COMPARATOR);
		for (XoaService svc : services) {
			Collections.sort(svc.getNodes(), XoaServiceDescriptor.COMPARATOR);
		}
	}
	
	/**
	 * 从txt中读取配置
	 * @param in
	 * @return
	 */
	public static List<XoaService> readFromTxt(InputStream in) {
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(in));
		String line;

		Map<String, List<XoaServiceDescriptor>> map = new HashMap<String, List<XoaServiceDescriptor>>();

		try {
			while ((line = reader.readLine()) != null) {
				
				line = line.trim();
				int offset = 0;
				String[] ss = line.split(" ");
				while (ss[offset].trim().length() == 0) {
					offset ++;
				}
				String serviceId = ss[offset].trim();
				
				offset ++;
				while (ss[offset].trim().length() == 0) {
					offset ++;
				}
				String ip = ss[offset].trim();
				
				offset ++;
				while (ss[offset].trim().length() == 0) {
					offset ++;
				}
				int port = Integer.parseInt(ss[offset].trim());
				
				XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
				desc.setServiceId(serviceId);
				desc.setIpAddress(ip);
				desc.setPort(port);
				
				List<XoaServiceDescriptor> list = map.get(serviceId);
				if (list == null) {
					list = new ArrayList<XoaServiceDescriptor>();
					map.put(serviceId, list);
				}
				list.add(desc);
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		
		List<XoaService> services = new ArrayList<XoaService>();
		for (Entry<String, List<XoaServiceDescriptor>> entry : map.entrySet()) {
			XoaService service = new XoaService();
			service.setServiceId(entry.getKey());
			service.setNodes(entry.getValue());
			services.add(service);
		}
		return services;
	}
	
	/**
	 * 从xml中读取配置
	 * @param in
	 * @return
	 */
	@SuppressWarnings("unchecked")
	public static List<XoaService> readFromXml(InputStream in) {
		
		SAXReader reader = new SAXReader();
		
		List<XoaService> services = new ArrayList<XoaService>();
		
		try {
			Document document = reader.read(in);
			Element root = document.getRootElement();
			Element servicesNode = root.element("services");
			List<Element> serviceNodes = servicesNode.elements("service");
			
			
			for (Element serviceNode : serviceNodes) {
			    
			    
			    
				String serviceId = serviceNode.attributeValue("id");
				XoaService service = new XoaService();
				service.setServiceId(serviceId);
				
				Element nodesNode = serviceNode.element("nodes");
				List<Element> nodeNodes = nodesNode.elements("node");
				List<XoaServiceDescriptor> descs = new ArrayList<XoaServiceDescriptor>();
				
				for (Element nodeNode : nodeNodes) {
					XoaServiceDescriptorBase desc = new XoaServiceDescriptorBase();
					desc.setServiceId(serviceId);
					desc.setIpAddress(nodeNode.attributeValue("ip"));
					desc.setPort(Integer.parseInt(nodeNode.attributeValue("port")));
					desc.setDisabled(XoaServiceDescriptor.STATUS_DISABLED
							.equals(nodeNode.attributeValue("status")));
					descs.add(desc);
				}
				Collections.sort(descs, XoaServiceDescriptor.COMPARATOR);
				service.setNodes(descs);
				services.add(service);
			}
			
		} catch (DocumentException e) {
			e.printStackTrace();
		}
		Collections.sort(services, XoaService.COMPARATOR);
		return services;
	}
	
}
