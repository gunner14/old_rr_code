package com.renren.sns.wiki.utils.event.impl;

import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.utils.event.Event;
import com.renren.sns.wiki.utils.event.EventHandler;
import com.renren.sns.wiki.utils.event.exception.LongEventPathException;
import com.renren.sns.wiki.utils.event.exception.LoopEventException;
import com.renren.sns.wiki.utils.event.impl.thread.InvocationPathUtil;

/**
 * 事件分发器
 * 
 * @author yi.li@renren-inc.com since 2012-3-29
 * 
 */
@SuppressWarnings("rawtypes")
public class EventDispatcher {

    private EventRegisterTable registerTable;

    private LinkedBlockingQueue<Runnable> workerQueue;

    private ExecutorService executor;

    private boolean isInited;

    private Object initLock = new Object();

    private static final String THREAD_PREFIX = "WIKI-EVENT";

    private static final int WARN_QUEUE_SIZE = 1000;

    private static final int MAX_ALLOWED_PATH_LEN = 5;

    private Log logger = LogFactory.getLog(EventDispatcher.class);

    public EventDispatcher(EventRegisterTable registerTable) {
        this.registerTable = registerTable;
        this.isInited = false;
    }

    private void init() {
        synchronized (initLock) {
            if (!isInited) {
                logger.info("Event Dispatcher begin to initail...");

                workerQueue = new LinkedBlockingQueue<Runnable>();

                int threadSize = Runtime.getRuntime().availableProcessors();
                executor = new ThreadPoolExecutor(threadSize, threadSize, 0L,
                        TimeUnit.MILLISECONDS, workerQueue, new EventThreadFactory(THREAD_PREFIX));

                isInited = true;
                logger.info("Event Dispatcher initail finished." + threadSize
                        + "event worker thread(s) have been created.");
            }
        }
    }

    public <T extends Event> void dipatcherEvent(T event) {
        if (!isInited) {
            init();
        }

        List<EventHandler> handlers = registerTable.getHandlers(event.getClass());
        if (handlers.size() == 0) {
            logger.warn("No handler found for event " + event.getClass());
            return;
        }

        EventWrapper eventWrapper = buildEventWrapper(event);
        EventWorker worker = new EventWorker(eventWrapper, handlers);
        // TODO can add listener
        executor.submit(worker);

        int queueSize = workerQueue.size();
        if (queueSize > WARN_QUEUE_SIZE) {
            logger.warn("Event queue size at WARNNING SIZE(" + WARN_QUEUE_SIZE + ")"
                    + " current size: " + queueSize);
        }
    }

    private EventWrapper buildEventWrapper(Event event) {
        String invocationPath = InvocationPathUtil.getCurrentInvocationPath();
        String eventName = event.getClass().getSimpleName();

        if (null == eventName || "".equals(eventName)) {
            logger.warn("Build Event Wrapper no simple name found for event-" + event.getClass());
        } else {
            String pathSection = InvocationPathUtil.INV_PATH_SEPERATOR + eventName
                    + InvocationPathUtil.INV_PATH_SEPERATOR;
            String _tempInvocationPath = invocationPath + eventName
                    + InvocationPathUtil.INV_PATH_SEPERATOR;

            if (invocationPath.contains(pathSection)) { //例如.AAA., .A. 那么实际上是不冲突
                throw new LoopEventException(_tempInvocationPath);
            }

            String pathes[] = invocationPath.split(InvocationPathUtil.REG_INV_PATH_SEPERATOR);
            if (pathes.length > 0 && (pathes.length - 1) >= MAX_ALLOWED_PATH_LEN) {
                throw new LongEventPathException(_tempInvocationPath, MAX_ALLOWED_PATH_LEN);
            }
            invocationPath = _tempInvocationPath;
        }

        EventWrapper wrapper = new EventWrapper(event, invocationPath);
        return wrapper;
    }

    /**
     * 线程工厂类
     * 
     * @author yi.li@renren-inc.com since 2012-3-29
     * 
     */
    private static class EventThreadFactory implements ThreadFactory {

        private String namePrefix;

        private final AtomicInteger threadNum = new AtomicInteger();

        public EventThreadFactory(String namePrefix) {
            this.namePrefix = namePrefix;
        }

        @Override
        public Thread newThread(Runnable r) {
            String threadName = namePrefix + threadNum.incrementAndGet();
            Thread newThread = new Thread(r, threadName);
            newThread.setDaemon(false);
            newThread.setPriority(Thread.NORM_PRIORITY);
            return newThread;
        }

    }
}
