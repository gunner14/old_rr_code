package com.xiaonei.sns.platform.core.rose.cache.provider;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;

/**
 * 取得User对象
 * 
 * @author lookis(comic.liu@gmail.com)
 * @author xylz(xylz@live.cn)
 * @since 20091102
 * 
 */
public class UserProvider extends AbstractProvider<User> {
    
    private static UserProvider _instance = new UserProvider();

    private UserProvider() {
    }

    public static UserProvider getInstance() {
        return _instance;
    }

    public static final String KEY_PREF = UserProvider.class.getName();

   /**
    * get User Object from cache<br />
    * To get the current User from cache, the <code>params</code> must be null or empty array;
    * otherwise the User Object is fetched by XCE adapter.
    * @param params the User id
    * @return an User Object or null
    */
    public User get(CacheManager cm, Object... params) {
        User ret = null;
        
        if (params == null || params.length == 0 || !(params[0] instanceof Integer || ((Integer)params[0]) == 0)){
            return BaseThreadUtil.currentHost(cm.getHolder());
        }

        int id = ((Integer) params[0]).intValue();
        ret = (User) cm.get(KEY_PREF + id);
        if (ret == null) {//cache里没有
            ret = SnsAdapterFactory.getUserAdapter().get(id);
            cm.put(KEY_PREF + id, ret);
        }
        return ret;
    }

}
