package com.renren.sns.wiki.stat.processor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.sns.wiki.stat.Frequency;
import com.renren.sns.wiki.stat.LogProcessor;

/**
 * @author yi.li@renren-inc.com since 2012-4-24
 * 
 */
public abstract class AbstractProcessor implements LogProcessor {

    protected Frequency frequency = Frequency.DAILY;

    protected Log logger = LogFactory.getLog(this.getClass());

    @Override
    public void setFrequency(Frequency frequency) {
        this.frequency = frequency;
    }
}
