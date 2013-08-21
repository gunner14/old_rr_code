package com.xiaonei.commons.interceptors.userinterface;

import java.lang.annotation.Annotation;
import java.lang.reflect.Method;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.IfParamExists;
import net.paoding.rose.web.annotation.Interceptor;
import net.paoding.rose.web.var.Model;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.Ajax;
import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.unifiedentrance.CheckUnifiedEntrance;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.model.UserConfigInfo;
import com.xiaonei.platform.core.opt.ice.WUserConfigInfoAdapter;

/**
 * {@link UserConfigInfoInterceptor} 拦截对Web控制器的调用，取UserConfigInfo
 * 
 * @author 李伟博
 * 
 */
@Interceptor(oncePerRequest = true)
public class UserConfigInfoInterceptor extends ControllerInterceptorAdapter {

    public static final String HOST_USER_CONFIG_INFO = "HOST_USER_CONFIG_INFO";

    @Autowired
    private HostHolder hostHolder;

    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return Ajax.class;
    }

    @Override
    protected boolean isForAction(Method actionMethod, Class<?> controllerClazz) {
        // async的访问不需要
        if (actionMethod.isAnnotationPresent(IfParamExists.class)) {
            IfParamExists ifParamExists = actionMethod.getAnnotation(IfParamExists.class);
            if (ifParamExists.value().indexOf(CheckUnifiedEntrance.async) != -1
                    || ifParamExists.value().indexOf(CheckUnifiedEntrance.asyncReload) != -1) {
                return false;
            }
        }
        return true;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        User host = hostHolder.getUser();
        if (host == null) {
            return true;
        }
        Model model = inv.getModel();
        //获取UserConfigInfo
        try {
            UserConfigInfo config = WUserConfigInfoAdapter.getInstance()
                    .getUserConfig(host.getId());
            model.add(HOST_USER_CONFIG_INFO, config);
        } catch (Exception e) {
            logger.error("", e);
        }/*catch (Ice.TimeoutException e) {
                    logger.error("[" + getClass().getName() + "] Ice.TimeoutException: " + e.getMessage());
                }*/
        return true;
    }
}
