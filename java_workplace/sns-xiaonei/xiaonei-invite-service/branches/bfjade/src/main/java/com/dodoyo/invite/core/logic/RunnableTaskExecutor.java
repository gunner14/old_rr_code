package com.dodoyo.invite.core.logic;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class RunnableTaskExecutor {
    private RunnableTaskExecutor() {
    }

    private static final RunnableTaskExecutor rte = new RunnableTaskExecutor();

    private static final Executor executor = Executors.newCachedThreadPool();

    public static RunnableTaskExecutor getInstance() {
        return rte;
    }

    public void runTask(Runnable task) {
        LogUtils.logDebug("######## A new sub task coming ....");
        executor.execute(task);
    }

}