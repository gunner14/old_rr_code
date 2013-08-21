package com.renren.sns.wiki.utils.concurrent;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 一个默认的listener的实现，没有复杂的逻辑，只会输出简单的log
 * 
 * @author yi.li@renren-inc.com since 2011-12-5
 * 
 */
public class GenericAsynTaskListener implements AsynTaskListener {

    private static final int WARN_QUEUE_SIZE = 1000;

    private static final String MONITOR_THREAD_PREFIX = "AsynMonitorThread";

    private final Log logger = LogFactory.getLog(getClass());

    private ExecutorService monitorExecutor;

    public GenericAsynTaskListener() {
        monitorExecutor = Executors.newFixedThreadPool(1, new AsynTaskExecutor.AsynThreadFactory(
                MONITOR_THREAD_PREFIX));
    }

    @Override
    public void onTaskAdded(AsynTask task) {
        int currentSize = task.getExecutor().getTaskCount();

        if (logger.isDebugEnabled()) {
            logger.debug(String.format("AsynTask %s added to taskQueue", task.toString()));
        }
        if (currentSize > WARN_QUEUE_SIZE) {
            logger.warn("AsynTaskQueue size exceed warning size, current size " + currentSize);
        }
    }

    @Override
    public void onTaskStart(final AsynTask task) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("AsynTask %s start", task.toString()));
        }

        monitorExecutor.execute(new Runnable() { // 单线程对任务的执行情况进行监控，对超时的任务进行取消

                    @Override
                    public void run() {
                        Future<?> future = task.getFuture();
                        try {
                            if (task.getTimeToExe() == 0L || future == null) {
                                return;
                            }
                            if (future.isDone()) {
                                String fstatus = "done";
                                if (future.isCancelled()) {
                                    fstatus = "cancelled";
                                }
                                if (logger.isDebugEnabled()) {
                                    logger.debug(String.format("Asyntask %s finished[%s]",
                                            task.toString(), fstatus));
                                }
                                return;
                            }

                            long curTime = System.currentTimeMillis();
                            long leftTime = task.getTaskStartTime() + task.getTimeToExe() - curTime;
                            if (leftTime > 0) {
                                future.get(leftTime, TimeUnit.MILLISECONDS);
                            } else {
                                task.getExecutor().onTaskTimeOut(task);
                                future.cancel(true);
                            }
                        } catch (InterruptedException e) {
                            logger.error(String.format("AsynTask %s interrupted", task.toString()),
                                    e);
                        } catch (ExecutionException e) {
                            logger.error(
                                    String.format("AsynTask %s execute error", task.toString()), e);
                            task.setThrowable(e);
                            task.getExecutor().onTaskError(task);
                        } catch (TimeoutException e) {
                            task.getExecutor().onTaskTimeOut(task);
                            future.cancel(true);
                        } catch (Exception e) {
                            logger.error(
                                    String.format("AsynTask %s unknownException", task.toString()),
                                    e);
                            task.setThrowable(e);
                            task.getExecutor().onTaskError(task);
                        }

                        if (logger.isDebugEnabled()) {
                            logger.debug(String.format("AsynTask %s execute check finished",
                                    task.toString()));
                        }
                    }
                });
    }

    @Override
    public void onTaskEnd(AsynTask task) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("AsynTask %s finished", task.toString()));
        }

        task.getExecutor().removeTaskFromSet(task);
    }

    @Override
    public void onTaskTimeOut(AsynTask task) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("AsynTask %s execute timeout expected time(%d ms)",
                    task.toString(), task.getTimeToExe()));
        }
        task.getExecutor().removeTaskFromSet(task);
    }

    @Override
    public void onTaskError(AsynTask task) {
        logger.error(String.format("AsynTask %s execute error.", task.toString()),
                task.getThrowable());
        task.getExecutor().removeTaskFromSet(task);
    }

    @Override
    public void onTaskReady(AsynTask task) {
        if (logger.isDebugEnabled()) {
            logger.debug(String.format("AsynTask %s ready", task.toString()));
        }
        task.getExecutor().addTaskToSet(task);
    }

}
