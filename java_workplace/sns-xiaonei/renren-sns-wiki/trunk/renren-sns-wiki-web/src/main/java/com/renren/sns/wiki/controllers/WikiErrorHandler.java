package com.renren.sns.wiki.controllers;

import java.util.Arrays;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.web.util.WebUtils;

import com.renren.sns.wiki.exception.WikiException;
import com.renren.sns.wiki.exception.WikiExceptionType;
import com.renren.sns.wiki.utils.JSONResultUtil;
import com.xiaonei.commons.interceptors.Ajax;

import net.paoding.rose.web.ControllerErrorHandler;
import net.paoding.rose.web.Invocation;

/**
 * 统一的ErrorHandler实现。
 * <p>
 * 有了该ErrorHandler，Controller中不再需要做try{}catch(e){}操作。在这里我也建议除非业务需要<br>
 * （如：异常情况下要跳转到别的页面）否则不建议在Controller中编写try{}catch(e){}代码
 * 
 * @author yi.li@renren-inc.com since 2012-5-17
 * 
 */
public class WikiErrorHandler implements ControllerErrorHandler {

    private static final Log logger = LogFactory.getLog(WikiErrorHandler.class);

    @Override
    public Object onError(Invocation inv, Throwable ex) throws Throwable {
        WikiException wikiEx = extractException(ex);

        // 异常发生时Rose会设置Servlet规范中<9.9 ErrorHandling>的request attributes
        // Tomcat拿到这些属性后会将code转成500这个地方要避免这种情况发生，所以清除这些属性
        // resin 4.0不清除这个数据不能返回500页
        WebUtils.clearErrorRequestAttributes(inv.getRequest());
        if (isAjax(inv)) {
            String result = JSONResultUtil.buildJSONResult(wikiEx.getCode(),
                    wikiEx.getDisplayMsg(), null);
            
            return "@" + result;
        }

        // TODO 还需要考虑Portal的情况，因为暂时没有用到Portal暂时不考虑

        return "error:500";
    }

    private boolean isAjax(Invocation inv) {
        Ajax annotation = inv.getAnnotation(Ajax.class); // 该方法的内容部实现是现获取Method的Annotation再获取ControllerClass的Annotation
        if (logger.isDebugEnabled()) {
            logger.debug("have ajax annotation: " + (annotation == null) + " != "
                    + (null != annotation) + " inv: " + inv.getClass() + " ann: "
                    + Arrays.toString(inv.getMethod().getAnnotations()));
        }

        if (null != annotation) {
            return true;
        }

        return false;
    }

    private WikiException extractException(Throwable ex) {
        if (ex instanceof WikiException) {
            return (WikiException) ex;
        }

        logger.error("Catch none wiki exception in WikiErrorHandler. ", ex);
        WikiException wikiEx = new WikiException(WikiExceptionType.UNKNOWN_EXCEPTION, ex);

        return wikiEx;
    }

}
