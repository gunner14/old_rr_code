package com.xiaonei.commons.interceptors.access.loginrequired;

import java.lang.annotation.Annotation;
import java.util.BitSet;
import java.util.List;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.NoSuchBeanDefinitionException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.platform.core.model.User;

/**
 * {@link LoginRequiredInterceptor} 拦截对XOA中使用的Web控制器的调用，确认被标注为
 * {@link LoginRequired} 的控制器类或方法只有在登录的情况下才能被访问
 * 
 * @author zhiliang.wang@opi-corp.com
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-9-2 下午03:37:17
 */
public class LoginRequiredInterceptor extends ControllerInterceptorAdapter {

    @Autowired
    private HostHolder hostHolder;

    @Autowired
    private LoginRequiredHandler defaultLoginRequiredHandler;

    public LoginRequiredInterceptor() {
        setPriority(LoginRequired.LOGIN_REQUIRED_PRIV);
    }

    @Override
    protected final List<Class<? extends Annotation>> getRequiredAnnotationClasses() {
        return createList(1).add(LoginRequired.class).getList();
    }

    @Override
    protected BitSet getAnnotationScope(Class<? extends Annotation> annotationType) {
        if (LoginRequired.class == annotationType) {
            return AnnotationScope.getMethodAndClassAndAnnotationScope();
        }
        return AnnotationScope.getMethodAndClassScope();
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        //
        User guester = hostHolder.getUser();
        if (guester != null) {
            return true;
        }

        ApplicationContext ctx = inv.getApplicationContext();
        LoginRequiredHandler handler = null;
        while (ctx != null) {
            String[] beansOfType = ctx.getBeanNamesForType(LoginRequiredHandler.class);
            if (beansOfType.length == 1) {
                handler = (LoginRequiredHandler) ctx.getBean(beansOfType[0]);
                break;
            } else if (beansOfType.length > 1) {
                throw new NoSuchBeanDefinitionException(LoginRequiredHandler.class,
                        "expected single bean but found " + beansOfType.length);
            }
            ctx = ctx.getParent();
        }

        if (handler == null) {
            handler = defaultLoginRequiredHandler;
        }

        return handler.deny(inv);
    }
}
