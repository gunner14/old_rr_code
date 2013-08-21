package com.renren.sns.wiki.utils.concurrent;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import org.apache.commons.lang.NullArgumentException;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * <p>
 * 异步执行器，用来添加异步执行的任务，能够在避免短时间内的重复任务执行<br>
 * 但由于任务执行队列与判断任务是否重复的集合不是一个，所以不能从根本上避免重复<br>
 * 任务的发生。原因是为了实现timeout特性而做出的牺牲。
 * 
 * <p>
 * Usage:<tt>AsynTaskExecutor</tt>的用法示例
 * 
 * <pre>
 * AsynTaskExecutor executor = new AsynTaskExecutor();
 * executor.addTask(&quot;testtask&quot;, &quot;01&quot;, new Runnable() {
 * 
 *     public void run() {
 *         System.out.println(&quot;Hello&quot;);
 *     }
 * });
 * </pre>
 * 
 * @author yi.li@renren-inc.com since 2011-12-2
 * 
 */
public class AsynTaskExecutor implements AsynTaskListener {

    private final Log logger = LogFactory.getLog(getClass());

    private static final String THREAD_PREFIX = "AsynTaskThread";

    private static final long DEFAULT_EXE_TIME = 500L;

    private List<AsynTaskListener> listeners;

    private BlockingQueue<Runnable> taskQueue; // thread-safe

    private Set<AsynTask> taskSet; // used for discard duplicate task

    private Object taskSetLock = new Object();

    private ExecutorService executor;

    private boolean started = false;

    public AsynTaskExecutor() {
        init();
    }

    private void init() {
        if (logger.isDebugEnabled()) {
            logger.debug("AsynTaskExecutor initialize start...");
        }
        listeners = new ArrayList<AsynTaskListener>();
        taskQueue = new LinkedBlockingQueue<Runnable>();
        taskSet = new HashSet<AsynTask>();
        int threadCount = Runtime.getRuntime().availableProcessors();
        executor = new ThreadPoolExecutor(threadCount, threadCount, 0L, TimeUnit.MILLISECONDS,
                taskQueue, new AsynThreadFactory(THREAD_PREFIX));
        this.addListener(new GenericAsynTaskListener());
        if (logger.isDebugEnabled()) {
            logger.debug("AsynTaskExecutor initialize finished. [threadCount: " + threadCount + "]");
        }
    }

    /**
     * 添加异步任务，如果在任务队列中存在相同的任务（taskName和taskNo相同）后加入的任务将不会执行
     * 
     * @param taskName 任务名
     * @param taskNo 任务流水号，可以是任何字符序列，相同的任务不会同时存在于任务队列中
     * @param task 需要被执行的任务
     * @param timeToExe 任务执行的时间(ms)，时间小于等于零时执行没有时间限制
     */
    public void addTask(String taskName, String taskNo, Runnable task, long timeToExe) {
        if (!started) {
            started = true;
        }

        if (null == task || null == taskName) {
            throw new NullArgumentException("AsynTaskExecutor: null task or taskName exception.");
        }
        AsynTask asynTask = new AsynTask(taskName, taskNo, task, this, timeToExe);
        if (containTask(asynTask)) {
            logger.warn("AsynTaskExecutor already have this task" + asynTask.toString());
            return;
        }

        this.onTaskReady(asynTask);
        try {
            Future<?> future = executor.submit(asynTask);
            asynTask.setFuture(future);
        } catch (Exception e) {
            asynTask.setThrowable(e);
            this.onTaskError(asynTask);
            return;
        }

        this.onTaskAdded(asynTask);
    }

    /**
     * 添加异步执行任务，如果在任务队列中存在相同的任务（taskName和taskNo相同）后加入的任务将不会执行
     * <p>
     * <strong>默认的执行时间是DEFAULT_EXE_TIME ms</strong>
     * 
     * @param taskName 任务名
     * @param taskNo 任务流水号
     * @param task 需要被执行的任务
     */
    public void addTask(String taskName, String taskNo, Runnable task) {
        addTask(taskName, taskNo, task, DEFAULT_EXE_TIME);
    }

    public void addListener(AsynTaskListener listener) {
        if (listener == null) {
            return;
        }

        synchronized (listeners) {
            if (started) {
                throw new RuntimeException(
                        "AsynTaskExecutor is started. listener can not be added.");
            }

            listeners.add(listener);
        }
    }

    public int getTaskCount() {
        return taskQueue.size();
    }

    public void addTaskToSet(AsynTask task) {
        synchronized (taskSetLock) {
            taskSet.add(task);
        }
    }

    public void removeTaskFromSet(AsynTask task) {
        synchronized (taskSetLock) {
            taskSet.remove(task);
        }
    }

    public boolean containTask(AsynTask task) {
        synchronized (taskSetLock) {
            return taskSet.contains(task);
        }
    }

    @Override
    public void onTaskAdded(AsynTask task) {
        for (AsynTaskListener l : listeners) {
            try {
                l.onTaskAdded(task);
            } catch (Exception e) {
                logger.error("", e);
            }
        }
    }

    @Override
    public void onTaskStart(AsynTask task) {
        for (AsynTaskListener l : listeners) {
            try {
                l.onTaskStart(task);
            } catch (Exception e) {
                logger.error("", e);
            }
        }
    }

    @Override
    public void onTaskEnd(AsynTask task) {
        for (AsynTaskListener l : listeners) {
            try {
                l.onTaskEnd(task);
            } catch (Exception e) {
                logger.error("", e);
            }
        }
    }

    @Override
    public void onTaskTimeOut(AsynTask task) {
        for (AsynTaskListener l : listeners) {
            try {
                l.onTaskTimeOut(task);
            } catch (Exception e) {
                logger.error("", e);
            }
        }
    }

    @Override
    public void onTaskError(AsynTask task) {
        for (AsynTaskListener l : listeners) {
            try {
                l.onTaskError(task);
            } catch (Exception e) {
                logger.error("", e);
            }
        }
    }

    @Override
    public void onTaskReady(AsynTask task) {
        for (AsynTaskListener l : listeners) {
            try {
                l.onTaskReady(task);
            } catch (Exception e) {
                logger.error("", e);
            }
        }
    }

    /**
     * customer thread factory
     */
    public static class AsynThreadFactory implements ThreadFactory {

        private String namePrefix;

        private final AtomicInteger threadNum = new AtomicInteger(1);

        public AsynThreadFactory(String namePrefix) {
            this.namePrefix = namePrefix;
        }

        @Override
        public Thread newThread(Runnable r) {
            String threadName = namePrefix + threadNum.getAndIncrement();
            Thread newThread = new Thread(r, threadName);
            newThread.setDaemon(false);
            newThread.setPriority(Thread.NORM_PRIORITY);
            return newThread;
        }

    }

}
