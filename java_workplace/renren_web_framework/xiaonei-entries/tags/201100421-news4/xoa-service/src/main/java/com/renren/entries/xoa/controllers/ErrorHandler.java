package com.renren.entries.xoa.controllers;

import javax.servlet.http.HttpServletRequest;

import net.paoding.rose.web.Invocation;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.xiaonei.commons.interceptors.errorhandler.XoaErrorHandlerBase;

/**
 * xoa接口错误处理
 * 
 * @author yanghe.liang@opi-corp.com
 */
public class ErrorHandler extends XoaErrorHandlerBase {

    private Log logger = LogFactory.getLog(ErrorHandler.class);

    @Override
    public XoaBizErrorBean wrapError(Invocation inv, Throwable ex) {
        HttpServletRequest request = inv.getRequest();
        StringBuilder sb = new StringBuilder(128);
        sb.append(request.getMethod());
        sb.append(" ");
        sb.append(request.getRequestURI());
        sb.append(" ");
        sb.append(request.getRemoteAddr());
        sb.append(" : ");
        Throwable root = ex;
        while(root.getCause() != null){
        	root = root.getCause();
        }
    	sb.append(root.getClass().getName());
        sb.append(ex.getMessage());
        
        /*StackTraceElement[] stackTraceElements = ex.getStackTrace();
        if (stackTraceElements != null && stackTraceElements.length > 0) {
            sb.append("\r\n\tat ");
            sb.append(stackTraceElements[0].toString());
            if (stackTraceElements.length > 1) {
                sb.append("\r\n\tat ");
                sb.append(stackTraceElements[1].toString());
            }
        }*/
        logger.error(sb.toString(), ex);
        return new XoaBizErrorBean(ex.getMessage());
    }

}
