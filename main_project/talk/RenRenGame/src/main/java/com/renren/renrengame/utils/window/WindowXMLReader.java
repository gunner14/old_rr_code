package com.renren.renrengame.utils.window;

import java.io.FileInputStream;
import java.io.InputStream;
import java.net.URL;

import org.apache.log4j.Logger;

import com.renren.renrengame.utils.RRGameConstants;

public class WindowXMLReader {

	private final Logger log = Logger.getLogger(WindowXMLReader.class.getName());
	
	public String getWindowXml() {
		return getStringFromXml(RRGameConstants.XML);
	}

	private String getStringFromXml(String filePath) {
		URL resourceURL = this.getClass().getClassLoader()
				.getResource(filePath);
		String xml = "";
		if (resourceURL == null) {
			log.error("WindowXMLReader.getStringFromXml ---->Cannot find xml.");
			return xml;
		}
		String path = resourceURL.getPath();
		log.debug("WindowXMLReader.getStringFromXml ----> xmlPath:" + path);
		try {
			InputStream in = new FileInputStream(path);
			byte[] b = new byte[1024];
			int len = -1;
			while((len = in.read(b)) != -1) {
				xml += new String(b, 0 , len);
			}
			in.close();
		} catch (Exception e) {
			log.warn("WindowXMLReader.getStringFromXml---> Get data from file..");
			e.printStackTrace();
		}
		return xml;
	}
}
