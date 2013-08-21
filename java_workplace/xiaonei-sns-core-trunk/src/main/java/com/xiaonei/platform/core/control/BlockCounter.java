package com.xiaonei.platform.core.control;

import java.io.Serializable;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

/**
 * a count for splitting some blockers
 * 
 * @author xylz (xylz@live.cn)
 * @since 20091022
 */
public class BlockCounter implements Serializable {

    private static final long serialVersionUID = 8329983481842030452L;

    public static final int UNLIMITED = -1;

    private String name;

    private final AtomicInteger counter = new AtomicInteger(0);

    private int maxBlocked = UNLIMITED;

    /** Total call times */
    private AtomicLong executeTimes = new AtomicLong(0);


    /** The service is running ok */
    private final AtomicBoolean enable = new AtomicBoolean(true);

    public BlockCounter(String name, int maxBlocked) {
        super();
        this.name = name;
        setMaxBlocked(maxBlocked);
    }
    public BlockCounter(BlockCounter bc) {
        this.name=bc.name;
        this.counter.set(bc.getCurrent());
        this.enable.set(bc.enable.get());
        this.executeTimes.set(bc.executeTimes.get());
        this.maxBlocked=bc.getMaxBlocked();
    }
    public BlockCounter(String name) {
        this(name, UNLIMITED);
    }

    public BlockCounter() {
        this(UUID.randomUUID().toString());
    }

    /**
     * check access
     * 
     * @return true while maxBlocked is <code>{@link #UNLIMITED}</code> or
     *         current access count is less then maxBlocked and false for
     *         other
     * @see #outer()
     */
    public boolean enter() {
        executeTimes.incrementAndGet();
        if (maxBlocked == UNLIMITED) return true;
        //if (!enable.get()) return false;//service is down...
        if (maxBlocked > counter.get()) return counter.incrementAndGet() > 0;
        return false;
    }

    /**
     * check the service is ok
     * 
     * @return true while the service is ok or false for other
     */
    public boolean checkEnabled() {
        return enable.get();
    }

    /**
     * get the number of current threads
     * 
     * @return thread number
     */
    public int getCurrent() {
        return counter.get();
    }

    /**
     * out of check access progress<br>
     * Decrease the counter will maxBlocked is not
     * <code>{@link #UNLIMITED}</code>
     */
    public void outer() {
        if (maxBlocked > UNLIMITED && counter.get() > 0) counter.decrementAndGet();
    }

    /**
     * check the block status of service
     * 
     * @return true when
     *         <code>{@link #getMaxBlocked()} &lt; {@link #getCurrent()} and
	 * {@link #getMaxBlocked()} != {@link #UNLIMITED}
     */
    public boolean isBlock() {
        return (maxBlocked > UNLIMITED && counter.get() >= maxBlocked);
    }

    /**
     * set the max block count
     * 
     * @param maxBlocked the max block count
     * @throws IllegalArgumentException the 'maxBlocked' is not
     *         <code>{@link #UNLIMITED} and positive integer
     */
    public void setMaxBlocked(int maxBlocked) {
        if (maxBlocked != UNLIMITED && maxBlocked <= 0) throw new IllegalArgumentException(
                "the 'maxBlocked' value must be positive integer or 'UNLIMITED':" + maxBlocked);
        this.maxBlocked = maxBlocked;
    }

    /**
     * the max block count
     */
    public int getMaxBlocked() {
        return maxBlocked;
    }

    @Override
    public String toString() {
        return "BlockCounter [name=" + name + ", maxBlocked=" + maxBlocked + ", counter=" + counter
                + ", executeTimes=" + executeTimes + "]" + (isBlock() ? "BLOCK" : "");
    }

    public String getName() {
        return name;
    }

}
