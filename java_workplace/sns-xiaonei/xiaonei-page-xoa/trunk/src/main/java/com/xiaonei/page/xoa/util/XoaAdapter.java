package com.xiaonei.page.xoa.util;

import org.apache.log4j.Logger;

import com.renren.xoa.commons.bean.XoaBizErrorBean;
import com.renren.xoa.lite.ServiceFuture;
import com.xiaonei.page.xoa.listener.PageXoaBizErrorListener;

/**
 * 
 * 封装xoa的调用。
 * 
 * @author happyshelocks@gmail.com
 * 
 */
public class XoaAdapter {

    private final static Logger logger = Logger.getLogger("PAGEXOA");

    private static final long TIMEOUTMILLIS = 500;

    /**
     * 
     * @param sf
     * @return 成功，返回实际的对象；否则，返回{@link XoaBizErrorBean}
     */
    public static <T> T doSubmit(ServiceFuture<T> sf) {
        return doSubmit(sf, TIMEOUTMILLIS);
    }

    /**
     * 
     * @param sf
     * @param timeoutMillis 超时参数
     * @return 成功，返回实际的对象；否则，返回{@link XoaBizErrorBean}
     */
    public static <T> T doSubmit(ServiceFuture<T> sf, long timeoutMillis) {
        PageXoaBizErrorListener listener = new PageXoaBizErrorListener();
        sf.addListener(listener);
        sf.submit();

        if (sf.awaitUninterruptibly(timeoutMillis)) {
            if (sf.isSuccess()) {
                if (logger.isDebugEnabled()) {
                    logger.info("invoke success!");
                }
                return sf.getContent();
            } else {
                try {
                    XoaBizErrorBean errorBean = listener.getReturn();
                    //记录出错信息
                    if (errorBean != null) {
                        logger.error(String.format("errorMessage:%s,errorCode:%d",
                                errorBean.getMessage(), errorBean.getErrorCode()));
                    }else{
                        logger.info("errorBean is null.But xoa invoke failed!");
                    }
                } catch (RuntimeException e) {
                    logger.error("未知服务出错", e);
                } catch (InterruptedException e) {
                    logger.error("调用状态XOA服务出错");
                }
            }
        } else {
            logger.error("状态服务XOA超时");
        }
        return null;
    }
}
