package com.renren.sns.wiki.stat.processor;

import java.util.concurrent.atomic.AtomicInteger;

import com.renren.sns.wiki.stat.Frequency;

/**
 * @author yi.li@renren-inc.com since 2012-4-23
 * 
 */
public class PVProcessor extends AbstractProcessor {

    private AtomicInteger pvCount;

    public PVProcessor() {
        pvCount = new AtomicInteger(0);
    }

    @Override
    public boolean support(String logType) {
        return true;
    }

    @Override
    public void parseLog(String[] params) {
        pvCount.incrementAndGet();
    }

    @Override
    public void finish() {
        logger.info("PV count: " + pvCount.get());

        //TODO write to db
        if (this.frequency == Frequency.DAILY) {}
    }

}
