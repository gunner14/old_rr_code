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
package com.xiaonei.jade.dbplugin.listener.impl;


import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.jade.dbplugin.configuration.ConfigJudge;
import com.xiaonei.jade.dbplugin.listener.DataMonitorListener;
import com.xiaonei.jade.dbplugin.model.DataModel;
import com.xiaonei.jade.dbplugin.writer.DataMonitorWriter;

/**
 * DataMonitorListenerImpl <br>
 * 
 * @author tai.wang@opi-corp.com Jul 14, 2010 - 3:57:07 PM
 */
public class DataMonitorListenerImpl implements DataMonitorListener {

    protected Log logger = LogFactory.getLog(getClass());

    private DataMonitorWriter writer = null;

    private ConfigJudge judge;

    @Override
    public void setConfigJudge(ConfigJudge configJudge) {
        this.judge = configJudge;
    }

    @Override
    public void setWriter(DataMonitorWriter writer) {
        this.writer = writer;
    }

    /*
     * (non-Javadoc)
     * 
     * @see
     * com.xiaonei.jade.dbplugin.listener.DataMonitorListener#listen(net.paoding
     * .rose.jade.provider.jdbctemplate.plugin.model.DataModel)
     * 开始时间|消耗时间|数据源|数据库|SQL|SQL 参数|客户端IP|类名|方法名
     * 2010.5.12.8:31:31.321|0.01|common
     * |ip_log|{“ip”:”127.0.0.1”;”id”:”236851580
     * ”}|127.0.0.1|com.xiaonei.login.dao.IplogDAO|inster
     */
    @Override
    public void listen(DataModel data) {

        if (false == isAllowed(data)) {
            logger.debug("isAllowed(data): false");
            return;
        }
        logger.debug("isAllowed(data): true");
        StringBuilder message = new StringBuilder();
        message.append("");
        message.append("|").append(data.getCostTime());
        message.append("|").append(data.getDataSource());
        message.append("|");
        message.append("|").append(data.getSql());
        //军伟说:不需要params
//        message.append("|").append(data.getSqlParams());
        message.append("|").append(data.getClientIp());
        message.append("|").append(data.getClassName());
        message.append("|").append(data.getMethodName());
        if (logger.isDebugEnabled()) {
            logger.debug("message: " + message.toString());
        }
        writer.write(message.toString());
    }

    private boolean isAllowed(DataModel data) {
        return judge.isAllowed(data);
    }

}
