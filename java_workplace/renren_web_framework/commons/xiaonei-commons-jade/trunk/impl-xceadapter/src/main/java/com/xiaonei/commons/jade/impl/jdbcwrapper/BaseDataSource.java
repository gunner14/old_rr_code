package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.io.PrintWriter;
import java.sql.SQLException;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 继承: BaseDataSource 定制数据源。
 * 
 * @author han.liao
 */
public abstract class BaseDataSource implements DataSource {

    /** 供派生类使用 */
    protected final Log logger = LogFactory.getLog(this.getClass());

    @Override
    public int getLoginTimeout() throws SQLException {
        return 0;
    }

    @Override
    public void setLoginTimeout(int seconds) throws SQLException {
        throw new UnsupportedOperationException("DataSource#setLoginTimeout not supported.");
    }

    @Override
    public PrintWriter getLogWriter() throws SQLException {
        throw new UnsupportedOperationException("DataSource#getLogWriter not supported.");
    }

    @Override
    public void setLogWriter(PrintWriter out) throws SQLException {
        throw new UnsupportedOperationException("DataSource#setLogWriter not supported.");
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        return DataSource.class.equals(iface);
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {

        if (!DataSource.class.equals(iface)) {
            throw new SQLException("DataSource [" + getClass().getName()
                    + "] can't be unwrapped as [" + iface.getName() + "].");
        }

        return iface.cast(this);
    }
}
