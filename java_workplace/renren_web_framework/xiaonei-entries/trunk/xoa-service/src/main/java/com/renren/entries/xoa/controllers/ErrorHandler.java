package com.renren.entries.xoa.controllers;

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
        logger.error("", ex);
        return new XoaBizErrorBean(ex.getMessage());
    }

}
