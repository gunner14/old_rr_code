package com.xiaonei.commons.interceptors.unifiedentrance;

import java.net.URL;

public class UnifiedEntranceUtils {

    public static String urlhash(String requestURL) throws Exception {
        URL url = new URL(requestURL);
        String protocol = null;
        String hostname = null;
        int port = -1;

        if (!"http".equals(url.getProtocol())) {
            protocol = url.getProtocol();
        }
        if (!"renren.com".equals(url.getHost())) {
            hostname = url.getHost().replace(".renren.com", "");
        }
        if ("http".equals(protocol) && url.getPort() != 80) {
            port = url.getPort();
        }
        if ("https".equals(protocol) && url.getPort() != 443) {
            port = url.getPort();
        }

        StringBuilder hash = new StringBuilder();
        if (protocol != null) {
            hash.append(protocol).append("://");
        }
        if (hostname != null) {
            hash.append(protocol != null ? "" : "//").append(hostname);
        }
        if (port > 0) {
            hash.append(":").append(port);
        }
        if (!url.getPath().startsWith("/")) {
            hash.append('/');
        }
        hash.append(url.getPath());
        
        String temp = hash.toString();
        
        //对home做特殊处理
        if (temp.equalsIgnoreCase("//www/home.do")
        		|| temp.equalsIgnoreCase("//home/home.do")) {	
        	hash.setLength(0);	//删掉原来的内容
        	//hash.append("/home");	//替换
        } 
        
        //对首部多出的www部分滤除  by david on 2012-05-15
        else if (temp.startsWith("//www/")) {
            hash = hash.replace(1, 6, "");
         }
        
        
        //如果上层传递的参数是inv.getRequest().getRequestURL(),
        //那么这里拿到的query就是null
        String query = url.getQuery();
        if (query != null) {
            hash.append("?").append(url.getQuery());
        }

        return hash.toString();
    }

    /*public static int mod = 2;
    public static int[] remainders = {0};
    
    *//**
     * 判断是否对指定用户应用unifiedEntrance策略
     * @param host
     * @return
     *//*
    public static boolean isUnifiedEntrance(User host) {
		
    	if (host == null) {
    		return false;
    	}
    	
    	//guide用户看不到新版
    	if (!host.getUserStateInfo().isGuideDone()) {
    		return false;
    	}
    	
    	if (mod == 0 || remainders == null ) {
    		return false;
    	}
    	
    	int remainder = host.getId() % mod;
    	for (int r : remainders) {
    		if (r == remainder) {
    			return true;
    		}
    	}
    	
    	return UnifiedEntranceUtils.isInWhiteList(host.getId());
    	return host.getId() % mod == remainder || UnifiedEntranceUtils
						.isInWhiteList(host.getId());
    }
    
    private static Set<Integer> whiteList = new HashSet<Integer>();
    static {
    	whiteList.add(1556200763);
    	whiteList.add(242679045);
    	whiteList.add(224931222);
    	whiteList.add(32715654);
    	whiteList.add(33501183);
    	whiteList.add(318826069);
    	whiteList.add(258601212);
    	whiteList.add(4402);
    	whiteList.add(142525482);
    	whiteList.add(116710603);
    	whiteList.add(244861456);
    }
    public static boolean isInWhiteList(int userId) {
    	return whiteList.contains(userId);
    }*/
}
