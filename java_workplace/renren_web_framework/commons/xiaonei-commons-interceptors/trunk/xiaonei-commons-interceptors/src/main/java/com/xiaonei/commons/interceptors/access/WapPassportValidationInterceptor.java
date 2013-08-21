package com.xiaonei.commons.interceptors.access;

import java.lang.annotation.Annotation;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.annotation.IgnorePassportValidation;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.BaseThreadUtil;
import com.xiaonei.platform.core.opt.base.CookieManager;
import com.xiaonei.platform.core.opt.base.PassportManager;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;

/**
 * 
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class WapPassportValidationInterceptor extends ControllerInterceptorAdapter {

    private HostHolder hostHolder;

    /**
     * 从request参数中取票时的参数名
     */

    private String ticketParamName = "sid";

    /**
     * 从cookie中验票时，存放票的cookie的name
     */
    private String ticketCookieName = PassportManager.loginKey_mticket;

    public WapPassportValidationInterceptor() {
        setPriority(29900);
    }

    @Autowired
    public void setHostHolder(HostHolder hostHolder) {
        this.hostHolder = hostHolder;
    }

    @Override
    protected Class<? extends Annotation> getDenyAnnotationClass() {
        return IgnorePassportValidation.class;
    }

    @Override
    public Object before(Invocation inv) throws Exception {
        HttpServletRequest request = inv.getRequest();

        // 之前已经设置过了的访问者
        User guester = hostHolder.getUser();

        // 如果之前已经设置过访问者，既然有了，那就算了
        // (这种情况存在于jsp的forward、include或velcotiy的$import.read语句)
        if (guester != null) {
            if (logger.isDebugEnabled()) {
                logger.debug("=======guester is not null");
            }
            return true;
        }
        if (!MarkingHelper.isFromWapAndMark(inv)) {
            if (logger.isDebugEnabled()) {
                logger.debug("=======continue; cause: invocation is not from wap");
            }
            return true;
        }

        // 获取HTTP请求中的ticket Cookie，验证其合法性，并从ICE服务中获取对应的用户对象取hostid
        // 先判断当前request对象里是否已经有host了，有一些特殊的应用场景，在来到这里之前已经进行过验票操作了，这里判断一下以避免重复验票
        //取票验票操作，虽然方法名叫getHostIdFromCookie，但是实际上会和中间层交互
        String ticket = null;
        //从cookie取票
        Integer hostid = PassportManager.getInstance().getHostIdFromCookieForWap(request,
                inv.getResponse(), ticketCookieName);
        if (hostid != null && hostid != -1) { //验票成功，得到这个票是什么
            ticket = CookieManager.getInstance().getCookie(request, ticketCookieName);
            if (logger.isDebugEnabled()) {
                logger.debug("passport validation succ from cookie:" + ticketCookieName + "="
                        + ticket);
            }
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("passport validation failed from cookie:" + ticketCookieName + "="
                        + CookieManager.getInstance().getCookie(request, ticketCookieName));
            }
        }

        if (hostid == null || hostid == -1) {
            //从request参数中取票
            hostid = PassportManager.getInstance().getHostIdFromRequestParamForWap(request,
                    ticketParamName);
            if (hostid != null && hostid != -1) { //验票成功，得到这个票是什么
                ticket = request.getParameter(ticketParamName);
                if (logger.isDebugEnabled()) {
                    logger.debug("passport validation succ from request parameter:"
                            + ticketParamName + "=" + ticket);
                }
            } else {
                if (logger.isDebugEnabled()) {
                    logger.debug("passport validation failed from request parameter:"
                            + ticketParamName + "=" + request.getParameter(ticketParamName));
                }
            }
        }

        if (hostid != null && hostid > 0) {
            guester = WUserAdapter.getInstance().get(hostid);
        }

        // 最后，将获取的访客用户对象设置到请求属性中
        if (guester != null) {
            if (logger.isDebugEnabled()) {
                logger.debug("==========set guester=" + guester);
            }
            BaseThreadUtil.setTicket(request, ticket); //把票放在request中，以后会有用
            hostHolder.setUser(guester);
        } else {
            if (logger.isDebugEnabled()) {
                logger.debug("======guester is  null");
            }
        }
        return true;
    }
}
