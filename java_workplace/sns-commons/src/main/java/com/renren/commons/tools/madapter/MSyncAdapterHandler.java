package com.renren.commons.tools.madapter;

import java.lang.reflect.Method;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Map.Entry;
import java.util.concurrent.Callable;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.perf4j.StopWatch;
import org.perf4j.commonslog.CommonsLogStopWatch;

import com.renren.commons.tools.madapter.model.MAdapterOneCalledModel;
import com.renren.commons.tools.madapter.model.MAdapterQueueModel;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Mar 6, 2012 2:01:16 PM
 * @Who 同步的调用处理器
 * @Modify
 */
public class MSyncAdapterHandler implements MethodInterceptor {

    private final static Log logger = LogFactory.getLog(MSyncAdapterHandler.class);

    private final static int minutes_20 = 20 * 60 * 1000;

    //用来执行这些调用的线程池
    private final static ThreadPoolExecutor failExecutor = new ThreadPoolExecutor(20, 300, 10, TimeUnit.SECONDS, new LinkedBlockingQueue<Runnable>(99999));

    //用来执行这些调用的线程池
    private final static ThreadPoolExecutor executor = new ThreadPoolExecutor(50, 500, 10, TimeUnit.SECONDS, new LinkedBlockingQueue<Runnable>(99999));

    //用来存放单个web server每个方法执行的情况
    private final static Map<String, MAdapterQueueModel> methodQueues = new ConcurrentHashMap<String, MAdapterQueueModel>();

    //目标对象  
    private final Object targetObject;

    private final int failsThreshold;

    private final int timeOut;

    private final InvocationFailedListener invocationFailedListener;

    private Log perfLogger;

    //默认返回值
    private final Object value;

    static {
        //启动一个定时线程，每隔一段时间将那些已经切断的访问恢复正常，试着去重试
        final Timer timer = new Timer("Reset cutOff", true);
        timer.scheduleAtFixedRate(new TimerTask() {

            @Override
            public void run() {
                for (final Entry<String, MAdapterQueueModel> e : methodQueues.entrySet()) {
                    final MAdapterQueueModel queueModel = e.getValue();
                    if (queueModel.isCutOff()) {
                        queueModel.setCutOff(false);
                        queueModel.setFailing(0);
                    }
                }
            }
        }, minutes_20, minutes_20);
    }

    public MSyncAdapterHandler(final Object targetObject, final int failsThreshold, final int timeOut, final Object value, final Log perfLogger,
            final InvocationFailedListener invocationFailedListener) {
        this.targetObject = targetObject;
        this.failsThreshold = failsThreshold;
        this.timeOut = timeOut;
        this.value = value;
        this.perfLogger = perfLogger;
        this.invocationFailedListener = invocationFailedListener;
    }

    @Override
    public Object intercept(final Object obj, final Method method, final Object[] args, final MethodProxy proxy) throws Throwable {
        //有些方法是不需要代理的
        if (!this.isAgentMethod(obj, method)) {
            //调用目标对象的方法
            return proxy.invokeSuper(obj, args);
        }
        //先判断是否超过了预定的最大失败次数
        final String methodName = targetObject.getClass().getName() + "." + method.getName();
        MAdapterQueueModel queueModel = methodQueues.get(methodName);
        if (queueModel == null) {
            queueModel = new MAdapterQueueModel(failsThreshold, timeOut, methodName);
            methodQueues.put(methodName, queueModel);
        }
        if (queueModel.isCutOff()) {
            return this.invocationFailed(args, methodName);
        }
        final MAdapterOneCalledModel oneCalledModel = new MAdapterOneCalledModel();
        //否则开始执行正常的流程
        final Callable<Object> callable = new Callable<Object>() {

            @Override
            public Object call() {
                try {
                    if (perfLogger == null) {
                        perfLogger = LogFactory.getLog(StopWatch.DEFAULT_LOGGER_NAME);
                    }
                    final StopWatch perfStopWatch = new CommonsLogStopWatch(perfLogger);
                    //final long beforeMem = JVMMemoryUtil.getUsedMemory();
                    //调用目标对象的方法
                    final Object superResult = proxy.invokeSuper(obj, args);
                    //oneCalledModel.setUsedMemory(JVMMemoryUtil.getUsedMemory() - beforeMem);
                    perfStopWatch.stop(methodName);
                    oneCalledModel.setCalledTime(perfStopWatch.getElapsedTime());
                    return superResult;
                } catch (final Throwable e) {
                    //记录下失败的证据
                    logger.error("proxy.invokeSuper exception:", e);
                    return null;
                }
            }
        };
        final Future<?> future = executor.submit(callable);
        Object result = null;
        try {
            result = future.get(timeOut, TimeUnit.SECONDS);
            oneCalledModel.setSuccess(true);
            this.pushModeOntoQueue(queueModel, oneCalledModel);
        } catch (final Exception e) {
            logger.error("future.get exception:", e);
            oneCalledModel.setSuccess(false);
            future.cancel(true);
            executor.purge();
            this.pushModeOntoQueue(queueModel, oneCalledModel);
            return value;
        }
        return result;
    }

    private void pushModeOntoQueue(final MAdapterQueueModel queueModel, final MAdapterOneCalledModel oneCalledModel) {
        //将指定的元素插入到此队列的尾部（如果立即可行且不会超过该队列的容量），在成功时返回 true，如果此队列已满，则返回 false。
        if (!queueModel.getCallModels().offer(oneCalledModel)) {
            queueModel.setNumDiscardedMessages(queueModel.getNumDiscardedMessages() + 1);
            logger.error("BlockingQueue is full,queue size:" + queueModel.getCallModels().size());
        }
    }

    /**
     * @Author xuze(cantellow)
     * @Time Mar 7, 2012 1:51:57 PM
     * @param obj
     * @param method
     * @return
     * @Todo 检查一下哪些方法需要代理哪些不需要代理
     * @Modify
     */
    private boolean isAgentMethod(final Object obj, final Method method) {
        final MAdaperAgentMethod ram = method.getAnnotation(MAdaperAgentMethod.class);
        if (ram == null) {
            return false;
        }
        return true;
    }

    /**
     * @Author xuze(cantellow)
     * @Time Mar 7, 2012 1:52:11 PM
     * @param targetObject
     * @param methodName
     * @return
     * @Todo 失败返回默认值，并调用之前设定好的监听器，采用异步的方式是防止调用者大范围阻塞返回默认值线程
     * @Modify
     */
    private Object invocationFailed(final Object targetObject, final String methodName) {
        if (invocationFailedListener != null) {
            failExecutor.submit(new Runnable() {

                @Override
                public void run() {
                    invocationFailedListener.invocationFailed(targetObject, methodName, failsThreshold);
                }
            });
        }
        return value;
    }

    public static ThreadPoolExecutor getExecutor() {
        return executor;
    }

    public static Map<String, MAdapterQueueModel> getMAdapterQueueModels() {
        return methodQueues;
    }
}
