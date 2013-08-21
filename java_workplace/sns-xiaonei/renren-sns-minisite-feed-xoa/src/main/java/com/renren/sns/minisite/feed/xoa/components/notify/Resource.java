package com.renren.sns.minisite.feed.xoa.components.notify;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;


public class Resource {

    public static Executor HIGH_EXECUTOR = Executors.newFixedThreadPool(100);

    public static Executor LOW_EXECUTOR = Executors.newFixedThreadPool(10);
}
