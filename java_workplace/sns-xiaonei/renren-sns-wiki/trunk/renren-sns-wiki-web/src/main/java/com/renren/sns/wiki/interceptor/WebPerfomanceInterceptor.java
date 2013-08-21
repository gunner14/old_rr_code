package com.renren.sns.wiki.interceptor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.perf4j.StopWatch;
import org.perf4j.commonslog.CommonsLogStopWatch;
import net.paoding.rose.web.ControllerInterceptorAdapter;
import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.InvocationChain;

/**
 * 用来监控web性能。这里不同于XOA的PerformanceInterceptor，XOA采用了before和after的方式
 * 这里只采用了round方式来做，单纯的检查Controller的执行效率
 * 
 * @author yi.li@renren-inc.com since 2012-5-4
 * @see XOA的PerformanceInterceptor
 * 
 */
public class WebPerfomanceInterceptor extends ControllerInterceptorAdapter {

    private static final Log logger = LogFactory.getLog("controllerPerf");

    @Override
    public int getPriority() {
        return 200;
    }

    @Override
    protected Object round(Invocation inv, InvocationChain chain) throws Exception {
        StringBuilder sb = new StringBuilder();
        sb.append(inv.getRequest().getMethod()).append("|");
        sb.append(inv.getResourceId());

        StopWatch watch = new CommonsLogStopWatch(logger);
        try {
            watch.start();
            return super.round(inv, chain);
        } finally {
            watch.stop(sb.toString());
        }
    }

}
