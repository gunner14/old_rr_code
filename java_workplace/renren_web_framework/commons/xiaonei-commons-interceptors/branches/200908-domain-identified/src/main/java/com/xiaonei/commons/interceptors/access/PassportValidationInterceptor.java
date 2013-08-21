package com.xiaonei.commons.interceptors.access;

import javax.servlet.http.HttpServletRequest;

import mop.hi.oce.adapter.AdapterFactory;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * {@link PassportValidationInterceptor}
 * 拦截对Web控制器的调用，如果该请求中含有合法的ticket，则把这个ticket对应的用户设置到request的attribute中
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class PassportValidationInterceptor extends ControllerInterceptorAdapter {

    private HostHolder hostHolder;

    public PassportValidationInterceptor() {
        setPriority(29900);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        HttpServletRequest request = inv.getRequest();

        // 之前已经设置过了的访问者
        User guester = hostHolder.getUser();

        // 如果之前已经设置过访问者，既然有了，那就算了
        // (这种情况存在于jsp的forward、include或velcotiy的$import.read语句)
        if (guester != null) {
            return true;
        }

        // 获取HTTP请求中的ticket Cookie，验证其合法性，并从ICE服务中获取对应的用户对象
        String ticket = PassportManager.getInstance().getCookie(request,
                PassportManager.loginKey_ticket);
        if (ticket != null && ticket.length() != 0 && !ticket.equals("null")) { // cookie中有票，从中间层验票
            Integer guesterId = AdapterFactory.getPassportAdapter().verifyTicket(ticket);
            if (guesterId != null && guesterId > 0) {
                guester = WUserAdapter.getInstance().get(guesterId);
            }
        }

        // 最后，将获取的访客用户对象设置到请求属性中
        if (guester != null) {
            hostHolder.setUser(guester);
        }
        return true;
    }
}
