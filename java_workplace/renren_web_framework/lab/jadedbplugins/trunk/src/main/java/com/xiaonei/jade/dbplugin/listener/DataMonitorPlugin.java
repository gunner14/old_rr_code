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
/**
 * 
 */
package com.xiaonei.jade.dbplugin.listener;

import javax.sql.DataSource;

import net.paoding.rose.jade.provider.Modifier;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.jade.datasource.XnDataSource;
import com.xiaonei.jade.dbplugin.model.DataModel;


/**
 * DataMonitorPlugin <br>
 * 把这个东西注册到jade中
 * 
 * @author tai.wang@opi-corp.com May 13, 2010 - 8:27:05 PM
 */
public class DataMonitorPlugin  {

    private DataMonitorListener listener = null;

    public DataMonitorPlugin() {
    }

    public void setDataMonitorListener(DataMonitorListener listener) {
        this.listener = listener;
    }

    protected void startPlugin(DataModel data, DataSource dataSource, String sqlString,
            Modifier modifier, Object[] parameters) {
        data.setClassName(modifier.getDefinition().getName());
        if (dataSource instanceof XnDataSource) {
            data.setDataSource(((XnDataSource) dataSource).getBizName());
        } else {
            data.setDataSource("");
        }
        data.setMethodName(modifier.getMethod().getName());
        data.setSql(sqlString);
        StringBuilder buffer = new StringBuilder("");
        for (Object p : parameters) {
            if (null != p) {
                String par = p.toString();
                par = StringUtils.replace(par, ",", "\\,");
                buffer.append(par).append(",");
            }
        }
        data.setSqlParams(buffer.toString());
    }

    protected void endPlugin(DataModel data) {
        listener.listen(data);
    }

    @Override
    public String toString() {
        return DataMonitorPlugin.class.getSimpleName() + " " + super.toString();
    }
}
