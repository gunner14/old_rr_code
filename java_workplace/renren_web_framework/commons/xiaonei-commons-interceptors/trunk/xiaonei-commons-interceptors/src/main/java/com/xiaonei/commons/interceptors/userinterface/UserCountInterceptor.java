package com.xiaonei.commons.interceptors.userinterface;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;
import java.util.Map;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.IfParamExists;
import net.paoding.rose.web.annotation.Interceptor;
import net.paoding.rose.web.var.Model;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.annotation.MarkAsFromWap;
import com.xiaonei.commons.interceptors.unifiedentrance.CheckUnifiedEntrance;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.usercount.UserCountMgr;

/**
 * {@link UserCountInterceptor} 拦截对Web控制器的调用，取与导航相关的数据
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class UserCountInterceptor extends ControllerInterceptorAdapter {

    @Autowired
    private HostHolder hostHolder;

    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return Ajax.class;
    }

    @Override
    protected boolean isForAction(Method actionMethod, Class<?> controllerClazz) {
        // async的访问没有navigation
        if (actionMethod.isAnnotationPresent(IfParamExists.class)) {
            IfParamExists ifParamExists = actionMethod.getAnnotation(IfParamExists.class);
            if (ifParamExists.value().indexOf(CheckUnifiedEntrance.async) != -1
                    || ifParamExists.value().indexOf(CheckUnifiedEntrance.asyncReload) != -1) {
                return false;
            }
        }
        // wap的访问没有navigation
        MarkAsFromWap markAsFromWap = actionMethod.getAnnotation(MarkAsFromWap.class);
        if (markAsFromWap == null) {
            markAsFromWap = controllerClazz.getAnnotation(MarkAsFromWap.class);
        }
        if (markAsFromWap != null) {
            return !markAsFromWap.value();
        }
        //
        return true;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        User host = hostHolder.getUser();
        if (host == null) {
            return true;
        }
        Model model = inv.getModel();

        Map<Integer, Integer> userCountMap = UserCountMgr.getCountMap(host.getId());
        if (userCountMap != null) { // 如果ICETimeout，这里就可能是null了
            model.add("userCountMap", userCountMap);
            model.add("newMessageCount", userCountMap.get(UserCountMgr.NEW_MESSAGE));
            model.add("newNotifyCount", userCountMap.get(UserCountMgr.NOTIFY));
            
            //publisher上使用的用户UGC计数，2011-10-10添加
            model.add("photoCount", userCountMap.get(UserCountMgr.UGC_PHOTO));
            model.add("blogCount", userCountMap.get(UserCountMgr.UGC_BLOG));
            model.add("statusCount", userCountMap.get(UserCountMgr.UGC_STATUS));
            model.add("shareCount", userCountMap.get(UserCountMgr.UGC_SHARE));
        }
        return true;
    }
}
