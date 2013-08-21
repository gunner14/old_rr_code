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

import java.util.concurrent.Executor;
import java.util.concurrent.ThreadPoolExecutor;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.jade.dbplugin.writer.impl.DataMonitorWriterImpl;

/**
 * DataMonitorExecutorWrapper <br>
 * wrapper一个Executor {@link Executor},可以打个log啥的,监控一下队列里面的情况
 * 
 * @author tai.wang@opi-corp.com Jul 16, 2010 - 2:16:13 PM
 */
public class DataMonitorExecutorWrapper implements Executor {

    protected final Log logger = LogFactory.getLog(getClass());

    private Executor target;

    public DataMonitorExecutorWrapper(Executor target) {
        this.target = target;
    }

    @Override
    public void execute(Runnable command) {
        this.target.execute(command);
        if (logger.isDebugEnabled()) {
            StringBuilder buf = new StringBuilder();
            if (target instanceof ThreadPoolExecutor) {
                ThreadPoolExecutor pe = ((ThreadPoolExecutor) target);
                buf.append(pe.getQueue().size());
            }
            if (command instanceof DataMonitorWriterImpl) {
                DataMonitorWriterImpl dw = (DataMonitorWriterImpl) command;
                buf.append("|" + dw.getContent());
            }
            logger.debug(buf.toString());
        }
    }

}
