/*
 * Copyright 2009-2010 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License i distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.xiaonei.jade.dbplugin.writer.executor;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Executor;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * WriterQueueDef <br>
 * 写队列定义
 * 
 * @author tai.wang@opi-corp.com Jul 16, 2010 - 2:14:07 PM
 */
public class WriterQueueDef {

    /** 长度500000 阻塞队列 */
    public static BlockingQueue<Runnable> queue = new LinkedBlockingQueue<Runnable>(500000);

    /** 1个线程的pool */
    public static Executor executor = new DataMonitorExecutorWrapper(new ThreadPoolExecutor(1, 1,
            0L, TimeUnit.MILLISECONDS, queue));

}
