package com.xiaonei.click.business.work;

import java.util.concurrent.Future;

/**
 * 工作线程调度，为了不占用做NIO的主线程
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public abstract class Boss {

    private static Boss _instance = new BossImpl();

    protected Boss() {
    }

    protected static void setBoss(Boss boss) {
        _instance = boss;
    }

    public static Boss getInstance() {
        return _instance;
    }

    public abstract <T> Future<T> submit(Job<T> job);

}
