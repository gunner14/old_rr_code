package com.renren.commons.tools.madapter.model;

import java.io.Serializable;
import java.util.LinkedList;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.commons.tools.madapter.MSyncAdapterHandler;

/**
 * 
 * @Author xuze(cantellow)
 * @Email ze.xu@renren-inc.com
 * @Time Apr 7, 2012 12:08:24 PM
 * @Who 每个方法对应一个MAdapterQueueModel，这个model包含这个方法在单机web server的全局执行信息
 * @Modify
 */
public class MAdapterQueueModel implements Serializable {

    private final static Log logger = LogFactory.getLog(MSyncAdapterHandler.class);

    private static final long serialVersionUID = 8556967071997972724L;

    private final String methodName;

    //最大连续失败次数，超过这个次数就算失败
    private final int failsThreshold;

    //当前连续失败的次数
    private volatile int failing;

    //超时几秒算一次失败
    private final int timeOut;

    //总共失败的次数，包括不连续的次数
    private volatile int allFailTimes;

    //队列
    private BlockingQueue<MAdapterOneCalledModel> callModels;

    //是否被切断
    private volatile boolean cutOff;

    //因为队列满了没有被塞进队列里的数目
    private volatile int numDiscardedMessages;

    //循环等待处理队列里的线程
    private Thread drainingThread;

    private LinkedList<MAdapterOneCalledModel> drainedModels;

    public MAdapterQueueModel(final int failsThreshold, final int timeOut, final String methodName) {
        this.failsThreshold = failsThreshold;
        this.timeOut = timeOut;
        this.methodName = methodName;
        this.activateOptions();
    }

    private void activateOptions() {
        //一些默认值
        failing = 0;
        allFailTimes = 0;
        callModels = new ArrayBlockingQueue<MAdapterOneCalledModel>(10240);
        cutOff = false;
        numDiscardedMessages = 0;
        drainedModels = new LinkedList<MAdapterOneCalledModel>();

        if (drainingThread != null) {
            try {
                drainingThread.join(10000L);
            } catch (final Exception e) {
                logger.warn("Unexpected error stopping MAdapterQueueModel draining thread", e);
            }
        }
        drainingThread = new Thread(new Dispatcher(), "MAdapter-thread-" + methodName);
        drainingThread.setDaemon(true);
        drainingThread.start();
    }

    private class Dispatcher implements Runnable {

        @Override
        public void run() {
            while (true) {
                if (drainedModels.isEmpty()) {
                    callModels.drainTo(drainedModels, 64);
                    //drainTo is more efficient but it doesn't block, so if we're still empty call take() to block
                    if (drainedModels.isEmpty()) {
                        try {
                            drainedModels.add(callModels.take());
                        } catch (final InterruptedException e) {
                            continue;
                        }
                    }
                }

                while (!drainedModels.isEmpty()) {
                    final MAdapterOneCalledModel model = drainedModels.removeFirst();
                    if (model.isSuccess()) {
                        cutOff = false;
                        failing = 0;
                    } else {
                        failing++;
                        allFailTimes++;
                        if (failing > failsThreshold) {
                            cutOff = true;
                        }
                    }
                    //call time
                    //System.out.println(model.getCalledTime());
                    //System.out.println(model.getUsedMemory());
                }
            }
        }
    }

    /*=====================get and set========================*/

    public int getFailsThreshold() {
        return failsThreshold;
    }

    public int getFailing() {
        return failing;
    }

    public void setFailing(final int failing) {
        this.failing = failing;
    }

    public int getTimeOut() {
        return timeOut;
    }

    public int getAllFailTimes() {
        return allFailTimes;
    }

    public void setAllFailTimes(final int allFailTimes) {
        this.allFailTimes = allFailTimes;
    }

    public BlockingQueue<MAdapterOneCalledModel> getCallModels() {
        return callModels;
    }

    public void setCallModels(final BlockingQueue<MAdapterOneCalledModel> callModels) {
        this.callModels = callModels;
    }

    public boolean isCutOff() {
        return cutOff;
    }

    public void setCutOff(final boolean cutOff) {
        this.cutOff = cutOff;
    }

    public int getNumDiscardedMessages() {
        return numDiscardedMessages;
    }

    public void setNumDiscardedMessages(final int numDiscardedMessages) {
        this.numDiscardedMessages = numDiscardedMessages;
    }

    public String getMethodName() {
        return methodName;
    }

}
