/**
 * 
 */
package com.xiaonei.click.business.work;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import com.xiaonei.click.constants.Constants;

/**
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public class BossImpl extends Boss {

    private ExecutorService workerPool = Executors.newFixedThreadPool(Constants.BOSS_SIZE);

    protected BossImpl() {

    }

    public <T> Future<T> submit(Job<T> job) {
        return workerPool.submit(job);
    }
}
