package com.xiaonei.commons.interceptors.unifiedentrance;

import java.lang.reflect.Method;
import java.net.URLEncoder;

import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Interceptor;

import org.springframework.beans.factory.annotation.Autowired;

import com.xiaonei.commons.interceptors.access.HostHolder;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginRequired;
import com.xiaonei.commons.interceptors.access.loginrequired.LoginStatus;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * 如果控制器标注了 {@link CheckUnifiedEntrance}
 * 并且设置value为true，标志所在的控制器或方法必须符合“统一访问入口”规范，<br>
 * 即如果参数不含有__view的请求将被重定向到www.reneren.com/home#url_hash的地址。
 * 
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
@Interceptor(oncePerRequest = true)
public class CheckUnifiedEntranceInterceptor extends ControllerInterceptorAdapter {

    private static final String PARAM_VIEW = "__view"; // 2个下划线

    @Autowired
    private HostHolder hostHolder;

    public CheckUnifiedEntranceInterceptor() {
    	//优先级比PassportValidationInterceptor低，因为要取userId，
    	//也要比LoginRequiredInterceptor低，因为要先跳登录页再跳首页，否则url hash会丢失
        this.setPriority(LoginRequired.LOGIN_REQUIRED_PRIV - 2); 
    }

    @Override
    protected Object before(final Invocation inv) throws Exception {

        if (this.logger.isDebugEnabled()) {
            this.logger.debug(this.getClass().getSimpleName() + ".before() entered");
        }

        if (OpiConstants.isKaixin()) {
            return true;
        }

        final User host = this.hostHolder.getUser();

        final CheckUnifiedEntrance checkEntrance = inv.getMethod().getAnnotation(
                CheckUnifiedEntrance.class);
        if (checkEntrance.checkWhen() != LoginStatus.ANY) {
            if (host == null) {
                if (checkEntrance.checkWhen() == LoginStatus.LOGIN) {
                    if (this.logger.isDebugEnabled()) {
                        this.logger.debug("skip, it's not login " + inv.getControllerClass().getName()
                                + "#" + inv.getMethod().getName());
                    }
                    return Boolean.TRUE;
                }
            } else {
                if (checkEntrance.checkWhen() == LoginStatus.LOGOUT) {
                    if (this.logger.isDebugEnabled()) {
                        this.logger.debug("skip, it's login " + inv.getControllerClass().getName() + "#"
                                + inv.getMethod().getName());
                    }
                    return Boolean.TRUE;
                }
            }
        }
        final String view = inv.getParameter(PARAM_VIEW);
        if (view == null) {
            if (this.logger.isDebugEnabled()) {
                this.logger.debug("it shall be redirect to the unified entrance,"
                        + " becase not exists param '" + PARAM_VIEW + "'");
            }
            final String queryString = inv.getRequest().getQueryString();
            final String hashed = UnifiedEntranceUtils.urlhash(inv.getRequest().getRequestURL()
                    .toString());

            final StringBuilder ret = new StringBuilder();
            ret.append("r:");
            if ((host != null) && !host.getUserStateInfo().isGuideDone()) {//guide用户跳guide页
                ret.append(OpiConstants.urlGuide);
                ret.append("/guide");
            } else { //非guide用户跳www
                ret.append(OpiConstants.urlWww);
                ret.append("/home");
            }

            //如果没有hash就不带#，解决IE可能存在的页面刷新问题
            if ((hashed == null) || (hashed.length() == 0)) {
                //do nothing
            } else {
               String reurlStr=hashed;
                if ((queryString != null) && (queryString.length() > 0)) {
                	reurlStr+="?";
                	reurlStr+=queryString;
                	reurlStr=URLEncoder.encode(reurlStr, "UTF-8");
                }
                ret.append("?reurl="+reurlStr);
                ret.append("#");
                ret.append(hashed);
            }
            if ((queryString != null) && (queryString.length() > 0)) {
                ret.append("?");
                ret.append(queryString);
            }
            return ret.toString();
        } else {
            if (this.logger.isDebugEnabled()) {
                this.logger.debug("Condition not matched: __view=" + view);
            }
        }
        if ((view != null) && !view.startsWith("async")) {
            this.logger.warn("bad request, invalid value '" + view + "' for param '" + PARAM_VIEW + "'");
            return "error:400;bad request, invalid value '" + view + "' for param '" + PARAM_VIEW
                    + "'";
        }
        return true;
    }

    @Override
    protected boolean isForAction(final Method actionMethod, final Class<?> controllerClazz) {
        final CheckUnifiedEntrance checkEntrance = actionMethod.getAnnotation(CheckUnifiedEntrance.class);
        return checkEntrance == null ? false : checkEntrance.value();
    }
}
