package com.renren.xoa2.demo.twitter;

import java.io.File;

import com.renren.xoa2.server.Bootstrap;

public class SimpleTwitterService {
	final static private String confFile = ".\\src\\main\\webapp\\WEB-INF\\xoa.xml";
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Bootstrap bootstrap = new Bootstrap();
		String localConf = confFile.replace("\\", File.separator);
		
		bootstrap.startServices(localConf);
		try {
          Thread.currentThread().join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}	
}
