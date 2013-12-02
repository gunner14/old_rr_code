package com.renren.renrengame.utils.config;

import java.util.ArrayList;

import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

public class ConfigXMLGenerator {
	private final Logger log = Logger.getLogger(ConfigXMLGenerator.class.getName());

	public String configXML(ArrayList<ConfigInfo> ciList)
	{
		Document document = DocumentHelper.createDocument();
		Element config = document.addElement("config");	
		
		for (int i=0; i<ciList.size(); ++i)
		{
			log.debug("ConfigXMLGenerator.configXML    name:"+ciList.get(i).name+"    value:"+ciList.get(i).value);

			Element item = config.addElement("item");
			item.addElement("name").setText(ciList.get(i).name);
			item.addElement("value").setText(ciList.get(i).value);
		}
		log.debug("ConfigXMLGenerator.configXML    ciList.size:"+ciList.size()+"    \nconfig:"+config.asXML());

		return config.asXML();
	}
}
