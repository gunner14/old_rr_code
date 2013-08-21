package com.renren.sns.stat.log.appender;

import java.util.Enumeration;

import org.apache.log4j.Appender;
import org.apache.log4j.AppenderSkeleton;
import org.apache.log4j.helpers.AppenderAttachableImpl;
import org.apache.log4j.spi.AppenderAttachable;
import org.apache.log4j.spi.LoggingEvent;

public class RandomAppender extends AppenderSkeleton implements AppenderAttachable {

    private static final int DEFAULT_RANDOM = 1;

    private final AppenderAttachableImpl appenders;

    private int random = DEFAULT_RANDOM;

    public RandomAppender() {
        this.appenders = new AppenderAttachableImpl();
    }

    public RandomAppender(final int random) {
        this.appenders = new AppenderAttachableImpl();
        this.setRandom(random);
    }

    @Override
    public void addAppender(final Appender newAppender) {
        synchronized (this.appenders) {
            this.appenders.addAppender(newAppender);
        }
    }

    @Override
    public void close() {
        return;
    }

    @Override
    public Enumeration<?> getAllAppenders() {
        synchronized (this.appenders) {
            return this.appenders.getAllAppenders();
        }
    }

    @Override
    public Appender getAppender(final String name) {
        synchronized (this.appenders) {
            return this.appenders.getAppender(name);
        }
    }

    /**
     * @return the random
     */
    public int getRandom() {
        return this.random;
    }

    @Override
    public boolean isAttached(final Appender appender) {
        synchronized (this.appenders) {
            return this.appenders.isAttached(appender);
        }
    }

    @Override
    public void removeAllAppenders() {
        synchronized (this.appenders) {
            this.appenders.removeAllAppenders();
        }
    }

    @Override
    public void removeAppender(final Appender appender) {
        synchronized (this.appenders) {
            this.appenders.removeAppender(appender);
        }
    }

    @Override
    public void removeAppender(final String name) {
        synchronized (this.appenders) {
            this.appenders.removeAppender(name);
        }
    }

    @Override
    public boolean requiresLayout() {
        return false;
    }

    /**
     * @param random the random to set
     */
    public void setRandom(final int random) {
        this.random = random < 0 ? 0 : random;
    }

    @Override
    protected void append(final LoggingEvent event) {
        if ((System.currentTimeMillis() % 100) < this.random) {
            synchronized (this.appenders) {
                this.appenders.appendLoopOnAppenders(event);
            }
        }
    }

}
