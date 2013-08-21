package com.xiaonei.platform.core.opt.base;

import java.sql.SQLException;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

import com.xiaonei.platform.core.model.ApiKey;
import com.xiaonei.platform.core.opt.jdbc.dao.ApiKeyDAO;

/**
 * app's api_key 的管理器<br>
 * 缓存了 app_basic 表的api_key
 * @author fusong.li@opi-corp.com
 * 2008-4-1 上午10:14:44
 */
public class ApiKeyMgr {

    private static ApiKeyMgr instance = new ApiKeyMgr();

    public static ApiKeyMgr getInstance() {
        if (instance == null) {
            synchronized (ApiKeyMgr.class) {
                if (instance == null) {
                    instance = new ApiKeyMgr();
                }
            }
        }
        return instance;
    }

    /**
     * @param args
     */
    public static void main(final String[] args) {


    }

    private final ConcurrentHashMap<String, ApiKey> apikeyMap = new ConcurrentHashMap<String, ApiKey>();


    static{
        instance.init();
    }



    protected ApiKeyMgr() {
    }

    /**
     * 到cache中得到一个apiKey对应的ApiKey对象。
     * @param apiKey
     * @return
     */
    public ApiKey get(final String apiKey){
        return this.apikeyMap.get(apiKey);
    }
    /**
     * 向Cache中放入一个ApiKey对象
     * @param apiKey
     * @param key
     */
    public void put(final String apiKey,final ApiKey key){
        this.apikeyMap.put(apiKey, key);
    }


    /**
     * 移除Cache中的一个ApiKey对象
     * @param apiKey
     */
    public void remove(final String apiKey){
        this.apikeyMap.remove(apiKey);
    }


    private void init(){
        try {
            final List<ApiKey> list = ApiKeyDAO.getInstance().getAll();
            if((list!=null) && (list.size()>0)){
                for (final Iterator<ApiKey> iter = list.iterator(); iter.hasNext();) {
                    final ApiKey item = iter.next();
                    this.apikeyMap.put(item.getApiKey(), item);
                }
            }
        } catch (final SQLException e) {
            e.printStackTrace();
        }
    }
}

