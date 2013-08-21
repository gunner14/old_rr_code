package com.renren.sns.wiki.stat;

/**
 * @author yi.li@renren-inc.com since 2012-4-23
 * 
 */
public interface LogProcessor {

    public void setFrequency(Frequency frequency);

    public boolean support(String logType);

    public void parseLog(String[] params);

    public void finish();
}
