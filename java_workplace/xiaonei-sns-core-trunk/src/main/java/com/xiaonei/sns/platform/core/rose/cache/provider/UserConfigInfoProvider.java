/**
 * 
 */
package com.xiaonei.sns.platform.core.rose.cache.provider;

import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * 取得UserConfigInfo对象
 * 
 * @author lookis (comic.liu@gmail.com)
 * 
 */
public class UserConfigInfoProvider extends AbstractProvider<UserConfigInfo>{
    
    private static UserConfigInfoProvider _instance = new UserConfigInfoProvider();

    private UserConfigInfoProvider() {
    }

    public static UserConfigInfoProvider getInstance() {
        return _instance;
    }
    
    public static final String KEY_PREF = UserConfigInfoProvider.class.getName();
    
    /**
     * @param params 要取的用户ID
     */
    @Override
    public UserConfigInfo get(CacheManager cm, Object... params) {
        UserConfigInfo ret = null;
        int id = 0;
        if (params == null || params.length == 0 || !(params[0] instanceof Integer || ((Integer)params[0]) == 0)){
            id = BaseThreadUtil.currentHost(cm.getHolder()).getId();
        }else{
            id = (Integer) params[0];
        }
        ret = (UserConfigInfo) cm.get(KEY_PREF + id);
        if (ret == null) {//cache里没有
            ret = SnsAdapterFactory.getUserConfigInfoAdapter().getUserConfig(id);
            cm.put(KEY_PREF + id, ret);
        }
        return ret;
        
    }
}
