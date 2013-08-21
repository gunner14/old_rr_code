package com.xiaonei.platform.scheduleTask;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Properties;

public class PropertyUtil {
	public static void getSysInfo() {
		Properties properties = new Properties();
		// 通过获得系统属性构造属性类 properties
		properties = System.getProperties();
		properties.list(System.out);
	}

	public void loadFile() {
		try {
			Properties properties = new Properties();
			// 文件位置:当前package
			properties.load(getClass().getResourceAsStream("test.property"));
			System.err.println(properties.getProperty("key1")
					+ properties.getProperty("key2"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void loadFileByIO() {
		InputStream in = null;
		try {
			in = new FileInputStream("NodeStatus.property");
			Properties properties = new Properties();
			properties.load(in);
			properties.list(System.out);
			System.err.println(properties.getProperty("key1")
					+ properties.getProperty("key2"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public int getKeyValueloadFileByIO(String filename, String key) {
		/*InputStream in = null;*/
		try {
/*			in = new FileInputStream(filename);
			Properties properties = new Properties();
			properties.load(in);
			String value = properties.getProperty(key);
			if (value == null) {
				return 0;
			} else
				return Integer.parseInt(properties.getProperty(key));
			
			*/
			
			Properties properties = new Properties();
			properties.load(getClass().getResourceAsStream(filename));
			String value = properties.getProperty(key);
			if (value == null) {
				return 0;
			} else
				return Integer.parseInt(properties.getProperty(key));
 
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		return 0;
	}

	public void loadXmlFileByIO() {
		InputStream in = null;
		try {
			in = new FileInputStream("TestSource/com/vs/test/test.xml");
			Properties properties = new Properties();
			properties.loadFromXML(in);
			properties.list(System.out);
			System.err.println(properties.getProperty("key1")
					+ properties.getProperty("key2"));
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void storeXmlFileByIO() {
		OutputStream out = null;
		InputStream in = null;
		try {
			Properties properties = new Properties();
			properties.setProperty("test", "testStore");
			// 写文件时是以覆盖的方式写入的 ,如果不把原文件内容读出来 ,原文件的内容就会丢失
			// 所以要把原文件的信息先读出来
			in = new FileInputStream("TestSource/com/vs/test/test.xml");
			properties.loadFromXML(in);
			out = new FileOutputStream("TestSource/com/vs/test/test.xml");
			properties.storeToXML(out, "writeToXml");
			properties.list(System.out);
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void storeFileByIO() {
		OutputStream out = null;
		InputStream in = null;
		try {
			Properties properties = new Properties();
			properties.setProperty("test", "testStore");
			// 写文件时是以覆盖的方式写入的 ,如果不把原文件内容读出来 ,原文件的内容就会丢失
			// 所以要把原文件的信息先读出来
			in = new FileInputStream("TestSource/com/vs/test/test.property");
			properties.load(in);
			out = new FileOutputStream("TestSource/com/vs/test/test.property");
			properties.store(out, "writeToXml");
			properties.list(System.out);
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String args[]) {
		PropertyUtil one=new PropertyUtil();
		int result =one
				.getKeyValueloadFileByIO(
						"NodeStatus.property",
						"LEVEL_WARN");
		System.out.println("===============" + result);
	}
}