package com.renren.sns.guide.rose.controllers;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.regex.Pattern;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.paoding.rose.web.portal.Portal;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.sns.platform.core.rose.cache.CacheManager;
import com.xiaonei.sns.platform.core.rose.cache.provider.UserProvider;

/**
 * @author: wei.xiang   
 * @E-mail: wei.xiang@opi-corp.com
 * @create time：2010-8-19 下午03:46:39
 */
@Path({ "ajaxGuideSetMemcacheWithExpiry.do", "ajaxGuideSetMemcacheWithExpiry"})
public class AjaxGuideSetMemcacheWithExpiryController {
    
    public static Properties p = new Properties();
    
    @Get
    @Post
    public String index(Invocation inv, Portal portal, CacheManager cm) throws InterruptedException {
        String memKey = inv.getRequest().getParameter("memKey");
        User host = UserProvider.getInstance().get(cm);
        Integer userId = 0;
        if (host != null) {
            userId = host.getId();
        }
        String strUserId = userId.toString();
        String[] memKeyArrays = new String[] {};
        String memKeyStart = "";
        String memKeyEnd = "";
        if (memKey != null) {
            memKeyArrays = memKey.split("_");
        }
        if (memKeyArrays.length > 1) {
            memKeyStart = memKeyArrays[0] + "_";
            memKeyEnd = memKeyArrays[1];
        }
        Map<String, String> keyMap = new HashMap<String, String>();
        if (p.isEmpty()) {
            try {
                p.load(this.getClass().getClassLoader().getResourceAsStream("memcache.properties"));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        for (Object k : p.keySet()) {
            String key = (String) k;
            String value = p.getProperty(key);
            try {
                keyMap.put(key, value);
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }
        if (keyMap.containsValue(memKeyStart) && strUserId.equals(memKeyEnd)) {
            String memExpiry = inv.getRequest().getParameter("memExpiry");
            Pattern pattern = Pattern.compile("[0-9]*");
            if(!pattern.matcher(memExpiry).matches()){
                return "@";
            }
            int intMemExpiry = Integer.parseInt(memExpiry);
            MemCacheManager mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_guide);
            mem.set(memKey, "clicked", (int) com.xiaonei.platform.core.cache.MemCacheKeys.day * intMemExpiry);
        }
        return "@";
    }

}
 