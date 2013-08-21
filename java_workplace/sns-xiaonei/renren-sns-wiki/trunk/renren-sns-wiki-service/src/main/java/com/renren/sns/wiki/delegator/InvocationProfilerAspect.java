package com.renren.sns.wiki.delegator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.perf4j.StopWatch;
import org.perf4j.commonslog.CommonsLogStopWatch;

/**
 * 用来监控Delegator的运行状况。 首先实现对Delegator调用用时的统计， TODO:上线运行后再添加动态切服务的功能
 * 
 * @author yi.li@renren-inc.com since 2012-4-13
 * 
 */
@Aspect
public class InvocationProfilerAspect {

    private static final Log logger = LogFactory.getLog(InvocationProfilerAspect.class);

    private static final Log plog = LogFactory.getLog("delegatorPerf");

    private static final String SUCCESS = ".success";

    private static final String FAILURE = ".failure";

    @Around("execution(public * com.renren.sns.wiki.delegator.*.*.*(..))")
    public Object doInvocationProfiler(ProceedingJoinPoint pjp) throws Throwable {
        if (logger.isDebugEnabled()) {
            logger.debug("enter invocation profiler" + pjp.getSignature().getDeclaringTypeName()
                    + " - " + pjp.getSignature().getDeclaringType());
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
            stopWatch.stop(tag + SUCCESS);
        }
    }
}
