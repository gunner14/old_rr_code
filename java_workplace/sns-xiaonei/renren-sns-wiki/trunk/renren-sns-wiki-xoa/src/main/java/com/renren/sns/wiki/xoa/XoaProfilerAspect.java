package com.renren.sns.wiki.xoa;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.perf4j.StopWatch;
import org.perf4j.commonslog.CommonsLogStopWatch;

/**
 * XOA性能监控Profiler
 * 
 * @author yi.li@renren-inc.com since 2012-6-13
 * 
 */
@Aspect
public class XoaProfilerAspect {

    private static final Log logger = LogFactory.getLog(XoaProfilerAspect.class);

    private static final Log plog = LogFactory.getLog("xoa2profiler");

    private static final String SUCCESS = ".success";

    private static final String FAILURE = ".failure";

    private static final String SLOW = ".slow";

    @Around("execution(public * com.renren.sns.wiki.xoa.WikiXoaServiceImpl.*(..))")
    public Object doInvocationProfiler(ProceedingJoinPoint pjp) throws Throwable {
        if (logger.isDebugEnabled()) {
            logger.debug("enter xoa invocation profiler"
                    + pjp.getSignature().getDeclaringTypeName() + " - "
                    + pjp.getSignature().getDeclaringType());
        }

        StopWatch stopWatch = new CommonsLogStopWatch(plog);
        String tag = pjp.getSignature().getDeclaringType().getSimpleName() + "#"
                + pjp.getSignature().getName();

        stopWatch.start();
        try {
            return pjp.proceed();
        } catch (Throwable t) {
            stopWatch.stop(tag + FAILURE);
            throw t;
        } finally {
            if (stopWatch.getElapsedTime() > 150) {
                stopWatch.stop(tag + SUCCESS + SLOW);
            } else {
                stopWatch.stop(tag + SUCCESS);
            }
        }
    }
}
