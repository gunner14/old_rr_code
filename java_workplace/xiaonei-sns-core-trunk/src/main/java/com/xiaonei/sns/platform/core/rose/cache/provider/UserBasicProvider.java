/**
 * 
 */
package com.xiaonei.sns.platform.core.rose.cache.provider;

import com.xiaonei.platform.core.model.UserBasic;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;


/**
 * @author lookis (comic.liu@gmail.com)
 *
 */
public class UserBasicProvider extends AbstractProvider<UserBasic> {
    private static UserBasicProvider _instance = new UserBasicProvider();

    private UserBasicProvider() {
    }

    public static UserBasicProvider getInstance() {
        return _instance;
    }
    

    public static final String KEY_PREF = UserBasicProvider.class.getName();

    /**
     * params 为 null或者0，都为取当前登陆用户
     * 
     * @param params 要取的用户ID
     */
    @Override
    public UserBasic get(CacheManager cm, Object... params) {
        UserBasic ret = null;

        int id = 0;
        if (params == null || params.length == 0 || !(params[0] instanceof Integer || ((Integer)params[0]) == 0)){
            id = BaseThreadUtil.currentHost(cm.getHolder()).getId();
        }else{
            id = (Integer) params[0];
        }
        ret = (UserBasic) cm.get(KEY_PREF + id);
        if (ret == null) {//cache里没有
            ret = SnsAdapterFactory.getUserBasicAdapter().get(id);
            cm.put(KEY_PREF + id, ret);
        }
        return ret;
    }
}
