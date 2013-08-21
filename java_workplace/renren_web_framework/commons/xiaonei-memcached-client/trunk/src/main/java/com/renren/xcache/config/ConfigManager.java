package com.renren.xcache.config;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.Map.Entry;

/**
 * 配置管理
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-11-30 下午07:24:46
 */
public class ConfigManager {

	/**
	 * 配置文件的文件名
	 */
	public static final String CONFIG_FILE_NAME = "xcache.properties";
	
	private static ConfigManager instance = new ConfigManager();

	public static ConfigManager getInstance() {
		return instance;
	}
	
	Properties config = new Properties();
	
	private ConfigManager() {
		loadConfig();
	}

	private void loadConfig() {
		InputStream is = null;
		try {
			is = this.getClass().getClassLoader().getResourceAsStream(CONFIG_FILE_NAME);
			if (is == null) {
				throw new RuntimeException("XCache config file not found. Please make sure 'xcache.properties' is in in your classpath");
			}
			config.load(is);
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

	public String getConfigRootPath() {
		return config.getProperty("zookeeper.rootpath");
	}
	
	public List<String> getZkServers() {
		List<String> servers = new ArrayList<String>();
		for (Entry<Object, Object> entry : config.entrySet()) {
			String key = (String)entry.getKey();
			if (key.startsWith("zookeeper.server.")) {
				String value = (String)entry.getValue();
				servers.add(value);
			}
			
		}
		return servers;
	}
	
	
	public static void main(String[] args) {
		ConfigManager mgr = new ConfigManager();
		System.out.println(mgr.config);;
		System.out.println(mgr.getConfigRootPath());
		System.out.println(mgr.getZkServers());
	}
}
