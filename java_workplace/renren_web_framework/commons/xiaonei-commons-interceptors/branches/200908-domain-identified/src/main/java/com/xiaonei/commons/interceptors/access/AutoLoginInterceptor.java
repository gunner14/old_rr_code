package com.xiaonei.commons.interceptors.access;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import mop.hi.oce.adapter.AdapterFactory;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.utility.Encoder;

/**
 * {@link AutoLoginInterceptor} 拦截对Web控制器的调用，如果该请求的Cookie中设置了自动登录信息，执行自动登录
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class AutoLoginInterceptor extends ControllerInterceptorAdapter {

    private HostHolder hostHolder;

    public AutoLoginInterceptor() {
        setPriority(29800);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        HttpServletRequest request = inv.getRequest();
        HttpServletResponse response = inv.getResponse();

        // 之前已经设置过了的访问者
        User guester = hostHolder.getUser();

        // 如果之前已经设置过访问者，既然有了，那就算了
        // (这种情况存在于前面的拦截器分析了ticket，或因为使用jsp的forward、include或velcotiy的$import.read语句)
        if (guester != null) {
            return true;
        }

        // 检查是否设置了k1Cookie，以及在格式上是否合法(必须能够分出2个字符串出来)
        String klCookie = PassportManager.getInstance().getCookie(request,
                PassportManager.loginKey_last);
        if (klCookie != null) {
            String k1Splits[] = klCookie.split(PassportManager.spl);

            if (k1Splits.length == 2) {
                // 从ICE服务获取Cookie所声明的自己，计算其加密值和cookie传上来的加密值进行比对
                // 比对成功表示他所声明的的确就是自己；否则就是“偷盗”或者因为密码等更改导致的过时
                String guesterIdInCookie = k1Splits[1].trim();
                User userDeclared = WUserAdapter.getInstance().get(
                        Integer.parseInt(guesterIdInCookie));
                if (userDeclared != null) {
                    String encodeKl = Encoder.encode(userDeclared.getPassword()
                            + PassportManager.secret + guesterIdInCookie)
                            + PassportManager.spl + guesterIdInCookie;
                    Integer guesterId = encodeKl.equals(klCookie) ? userDeclared.getId() : -1;
                    if (guesterId != null && guesterId > 0) {
                        hostHolder.setUser(userDeclared);
                        String ticket = AdapterFactory.getPassportAdapter().createTicket(guesterId,
                                true);
                        PassportManager.getInstance().putTicketToCookie(response, ticket, true,
                                PassportManager.loginKey_ticket);
                        encodeKl = Encoder.encode(userDeclared.getPassword()
                                + PassportManager.secret + guesterId)
                                + PassportManager.spl + guesterId;
                        CookieManager.getInstance().saveCookie(response,
                                PassportManager.loginKey_last, encodeKl, 30 * 24 * 60 * 60, "/");
                    } else {
                        PassportManager.getInstance().clearCookie(request, response,
                                PassportManager.loginKey_last);
                        PassportManager.getInstance().clearCookie(request, response,
                                PassportManager.loginKey_ticket);
                    }
                }
            }
        }
        return true;
    }
}
