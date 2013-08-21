package com.renren.xoa.proxy.client;

import java.net.InetSocketAddress;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import com.renren.xoa.commons.exception.IllegalArgumentException;
import com.renren.xoa.commons.exception.XoaConnectException;
import com.renren.xoa.proxy.FrameProxy;

public class FrameProxyManager {

	private static FrameProxyManager instance = new FrameProxyManager();
	
	private Map<String, FrameProxy> clientPool = new HashMap<String, FrameProxy>();
	
	public static FrameProxyManager getInstance() {
		return instance;
	}
	
	private Random random = new Random();
	
	private FrameProxyManager() {
		
	}
	
	/**
	 * 获取一个连接指定host的client
	 * @param host
	 * @return
	 */
	public FrameProxy getClient(String host){
        if (clientPool.get(host) == null) { //DCL
            //构造新的Service
            synchronized (host.intern()) {
                if (clientPool.get(host) == null) {
                    FrameProxy xoaService = initClient(host);
                    clientPool.put(host, xoaService);
                }
            }
        }
        return clientPool.get(host);
    }
	
	/**
	 * 初始化一个到质量host的client
	 * @param host
	 * @return
	 */
	private FrameProxy initClient(String host) {
		String[] ss = host.split(":");
		if (ss.length != 2) {
			throw new IllegalArgumentException(host);
		}
		String hostname = ss[0];
		int port = Integer.parseInt(ss[1]);
		SpdyFrameClient spdyClient = new SpdyFrameClient(new InetSocketAddress(hostname, port));
		spdyClient.init();
        return spdyClient;
	}
}
