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
package com.xiaonei.jade.dbplugin.listener;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.List;

import javax.sql.DataSource;

import net.paoding.rose.jade.provider.Modifier;
import net.paoding.rose.jade.provider.jdbc.Jdbc;
import net.paoding.rose.jade.provider.jdbc.JdbcWrapper;

import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.RowMapper;

import com.xiaonei.jade.dbplugin.model.DataModel;

/**
 * DataMonitorWrapper <br> {@link Jdbc}的wrapper
 * 
 * @author tai.wang@opi-corp.com Aug 9, 2010 - 8:10:04 PM
 */
public class DataMonitorWrapper implements JdbcWrapper {

    /** 本体 */
    private Jdbc jdbc;

    private DataSource dataSource;

    /** 插入 */
    private DataMonitorPlugin plugin;

    public void setPlugin(DataMonitorPlugin plugin) {
        this.plugin = plugin;
    }

    @Override
    public Jdbc getJdbc() {
        return this.jdbc;
    }

    @Override
    public void setJdbc(Jdbc jdbc) {
        this.jdbc = jdbc;
    }

    @Override
    public Object insertAndReturnId(Modifier modifier, String sql, Object[] parameters) {
        Object num;
        DataModel data = createDataModel();
        try {
            plugin.startPlugin(data, dataSource, sql, modifier, parameters);
            num = this.jdbc.insertAndReturnId(modifier, sql, parameters);
        } finally {
            data.setCostTime((int) (System.currentTimeMillis() - data.getStartTime()));
            plugin.endPlugin(data);
        }
        return num;
    }

    /**
     * createDataModel<br>
     * 
     * @return
     * 
     * @author tai.wang@opi-corp.com Aug 9, 2010 - 8:04:02 PM
     */
    private DataModel createDataModel() {
        DataModel d = new DataModel();
        d.setStartTime(System.currentTimeMillis());
        d.setClientIp(HOST_ADDRESS);
        return d;
    }

    private static String HOST_ADDRESS = getHost();

    private static String getHost() {
        try {
            return InetAddress.getLocalHost().getHostAddress();
        } catch (UnknownHostException e) {
            return "";
        }
    }

    @Override
    public List<?> query(Modifier modifier, String sql, Object[] args, RowMapper rowMapper) {
        DataModel data = createDataModel();
        List<?> list;
        try {
            plugin.startPlugin(data, dataSource, sql, modifier, args);
            list = this.jdbc.query(modifier, sql, args, rowMapper);
        } finally {
            data.setCostTime((int) (System.currentTimeMillis() - data.getStartTime()));

            plugin.endPlugin(data);
        }
        return list;
    }

    @Override
    public void setDataSource(DataSource dataSource) {
        this.dataSource = dataSource;
    }

    @Override
    public int update(Modifier modifier, String sql, Object[] args) {
        DataModel data = createDataModel();
        int upInt;
        try {
            plugin.startPlugin(data, dataSource, sql, modifier, args);
            upInt = this.jdbc.update(modifier, sql, args);
        } finally {
            data.setCostTime((int) (System.currentTimeMillis() - data.getStartTime()));
            plugin.endPlugin(data);
        }
        return upInt;
    }

	@Override
	public int[] batchUpdate(Modifier modifier, String sql, List<Object[]> args)
			throws DataAccessException {
		return this.jdbc.batchUpdate(modifier, sql, args);
	}

}
