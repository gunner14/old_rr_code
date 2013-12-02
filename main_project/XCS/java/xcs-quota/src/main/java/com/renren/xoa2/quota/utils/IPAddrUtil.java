package com.renren.xoa2.quota.utils;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 *
 * @author Xue Wenxin Oct 26, 2012 6:29:55 PM
 */
public class IPAddrUtil {
	
	private static Log logger = LogFactory.getLog(IPAddrUtil.class);
	
	private static String localhostIp;
	static {
		localhostIp = "127.0.0.1";
		try{
			for(Enumeration<NetworkInterface> ni =NetworkInterface.getNetworkInterfaces();ni.hasMoreElements();){
				NetworkInterface eth=ni.nextElement();
				for(Enumeration<InetAddress> add = eth.getInetAddresses();add.hasMoreElements();){
					InetAddress i=add.nextElement();
					if(i instanceof Inet4Address){
						if(i.isSiteLocalAddress()){
							localhostIp = i.getHostAddress();
						}
					}
				}
			}
		}catch(SocketException e){
			logger.warn("get local ip address failed:"+e.getMessage());
		}
	}
	public static String getLocalhostIp() {
		return localhostIp;
	}
}
