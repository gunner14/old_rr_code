package com.dodoyo.invite.httpservice.auth;

import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.dodoyo.invite.core.logic.LogUtils;
import com.dodoyo.invite.httpservice.dao.ClientIPDao;
import com.dodoyo.invite.httpservice.entity.ClientInfo;
import com.dodoyo.invite.httpservice.excep.ServiceDBException;

public class ClientAuth {
    private static ClientAuth ca = new ClientAuth();

    private static Map<String, ClientInfo> clientMap = new HashMap<String, ClientInfo>();
    
    private static Map<String, String> clientKeyMap = new HashMap<String, String>();
    
    private static ClientIPDao ipDao = new ClientIPDao();
    
    private static long lastUpdate = 0L;
    
    private static final long maxInterval = 5 * 60 * 1000L;
    
    static
    {
    	ca.loadClientInfos();
    }
    
    public static ClientAuth gentInstance() {
        return ca;
    }

    private ClientAuth()
    {
    }
    
    /**
     * check is authed client ip (web server ip calling this service)
     *
     * @param clientIp
     * @return
     */
    public String getClientKey(String clientIp) {
        ClientIPDao dao = new ClientIPDao();
        ClientInfo ci = null;
        try {
            ci = this.getClientInfo(clientIp);
        }
        catch (Exception e) {
            throw new ServiceDBException("getClientInfo() db exception", e);
        }

        if (ci == null || ci.getType() == 0)
            return null;

        if (ci.getExpireTime() != null
                && ci.getExpireTime().before(new Timestamp(new Date().getTime())))
            return null;
        dao = null;

        return ci.getClientKey();
	}
    
    /**
     * get client key by appId
     * @param clientIp
     * @return
     */
    public String getClientKeyByAppId(String appId) {
        if (appId == null || "".equals(appId))
            appId = "0";
        String clientKey = clientKeyMap.get(appId);
        
        return clientKey == null ? "123456" : clientKey;
    }
    
    /**
     * Load client ip list interval
     * return the current TimeMillis 
     * @return
     */
    private synchronized long loadClientInfos()
    {
    	if (System.currentTimeMillis() - lastUpdate < maxInterval)
    	{
    		LogUtils.logInfo("Client info interval less then maxInterval, upate canceled ....");
    		return lastUpdate;
    	}
    	
    	LogUtils.logInfo("Client info need be reload for interval ....");
    	
    	try
    	{
    		List<ClientInfo> clientList = ipDao.getClientInfoList();
    		for (ClientInfo client : clientList)
    		{
    			clientMap.put(client.getClientIp(), client);
    		}
    	}
    	catch(Exception e)
    	{
    	    lastUpdate = System.currentTimeMillis();//to be safe
    		throw new ServiceDBException("loadClientInfos() db exception", e);
    	}
    	
    	lastUpdate = System.currentTimeMillis();
    	
    	LogUtils.logInfo("Client info reload over ....");
    	
    	return lastUpdate;
    }
    
    public ClientInfo getClientInfo(String clientIp)
    {
    	if (System.currentTimeMillis() - lastUpdate > maxInterval)
    		this.loadClientInfos();
    	return clientMap.get(clientIp);
    }
}