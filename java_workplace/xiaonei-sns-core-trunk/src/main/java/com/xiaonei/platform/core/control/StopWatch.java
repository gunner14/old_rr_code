package com.xiaonei.platform.core.control;

import java.io.Serializable;
import java.util.Random;

import org.apache.commons.logging.Log;

/**
 * A time watcher <br />
 * <p>
 * Usage: <code>
 * <pre>
 * StopWatch tc = new StopWatch().start();
 * doMethod1(); // execute step 1
 * tc.stepLog(&quot;Main&quot;, &quot;step1&quot;); //log step 1 with keyword 'Main' and 'step1'
 * doMethod2();
 * tc.stepLog(&quot;Main&quot;, &quot;step2&quot;);
 * doMethod3();
 * tc.stepLog(&quot;Main&quot;, &quot;step3&quot;);
 * doMethod4();
 * tc.endLog(&quot;Main&quot;, &quot;step_over&quot;);//log over with keyword 'Main' and 'step_over'
 * </pre>
 * </code>
 * </p>
 * The log style:
 * 
 * <pre>
 * TimeStep-4626|   0|   0|Main|Test1
 * TimeStep-4626| 124| 124|Main|Test2
 * TimeStep-4626| 234| 110|Main|Test3
 * TimeOver-4626| 343| 109|Main|Test_end
 * </pre>
 * 
 * The log depends the Logger {@link TimeCostUtil#logger}. Reset the logger
 * with {@link #setLogger(Log)} if you want.
 * 
 * @author Liu Yan
 * @since 20091208
 */
public class StopWatch implements Serializable {

    private static final long serialVersionUID = 397574800599500448L;

    /** TimeCounter create time */
    private long createTime = System.currentTimeMillis();

    /** TimeCounter start time */
    private long startTime = -1;

    private long stopTime = -1;

    /** How much time cost */
    private long slipTime = 0;

    /** detected flag for running */
    private boolean starting = false;

    /** last step time flag */
    private long lastStepTime = 0;

    private Log logger = TimeCostUtil.logger;

    private int id = 0;

    static Random rnd = new Random(System.currentTimeMillis());

    public StopWatch() {
    }

    /** starting the counter (ignoring if it's running) */
    public StopWatch start() {
        if (!isStarting()) {
            startTime = System.currentTimeMillis();
            lastStepTime = startTime;
            starting = true;
            id = rnd.nextInt(10000);
        }
        return this;
    }

    //	/** stoping the counter (ignoring if it's stopping) */
    //	protected StopWatch stop() {
    //		if (isStarting()) {
    //			starting = false;
    //			stopTime = System.currentTimeMillis();
    //			slipTime += (stopTime - startTime);
    //		}
    //		return this;
    //	}

    public StopWatch stepLog(String key1, String key2) {
        if (isStarting()) {
            if (logger.isDebugEnabled()) {
                long now = System.currentTimeMillis();
                logger.debug(String.format("TimeStep-%1$4d|%2$4d|%3$4d|%4$s|%5$s"//
                        , id//the key for log
                        , now - startTime//total cost time, from starting to now
                        , now - lastStepTime//step time
                        , "" + key1//user key1
                        , "" + key2)//user key2
                        );
                lastStepTime = now;
            }
        }

        return this;
    }

    public void endLog(String key1, String key2) {
        if (isStarting()) {
            starting = false;
            stopTime = System.currentTimeMillis();
            slipTime += (stopTime - startTime);
            if (logger.isDebugEnabled()) {
                long now = stopTime;
                logger.debug(String.format("TimeOver-%1$4d|%2$4d|%3$4d|%4$s|%5$s"//
                        , id//the key for log
                        , now - startTime//total cost time, from starting to now
                        , now - lastStepTime//step time
                        , "" + key1//user key1
                        , "" + key2)//user key2
                        );
                lastStepTime = now;
            }
        }
    }

    /** check running state of the timer counter */
    public boolean isStarting() {
        return starting;
    }

    /** get the created time of the counter(ms) */
    public long getCreateTime() {
        return createTime;
    }

    /** get the last stopping time of the counter(ms) */
    public long getStopTime() {
        return stopTime;
    }

    /** get the sliping time of the counter(ms) */
    public long getSlipTime() {
        return slipTime;
    }

    /** get the cost time between create time and stop time (ms) */
    public long getCreateStopTime() {
        return stopTime == -1 ? -1 : stopTime - createTime;
    }

    /** get the sliping time of the counter(seconds) */
    public long getSlipTimeSeconds() {
        return getSlipTime() / 1000L;
    }

    /** get the sliping time of the counter(minute) */
    public long getSlipTimeMinutes() {
        return getSlipTimeSeconds() / 60;
    }

    public String getSlipString() {
        return getSlipTimeMinutes() + " minutes, " + (getSlipTimeSeconds() % 60) + " seconds";
    }

    public static void main(String[] args) throws Exception {
        StopWatch tc = new StopWatch().start();
        tc.stepLog("Main", "Test1");
        Thread.sleep(100);
        tc.stepLog("Main", "Test2");
        Thread.sleep(100);
        tc.stepLog("Main", "Test3");
        Thread.sleep(100);
        tc.endLog("Main", "Test_end");
    }

    public String toString() {
        return getSlipString();
    }

    public void setLogger(Log logger) {
        this.logger = logger;
    }
}
