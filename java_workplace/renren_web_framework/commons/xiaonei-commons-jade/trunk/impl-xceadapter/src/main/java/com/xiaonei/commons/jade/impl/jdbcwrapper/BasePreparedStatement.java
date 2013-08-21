package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.io.InputStream;
import java.io.Reader;
import java.math.BigDecimal;
import java.net.URL;
import java.sql.Array;
import java.sql.Blob;
import java.sql.Clob;
import java.sql.Connection;
import java.sql.Date;
import java.sql.NClob;
import java.sql.ParameterMetaData;
import java.sql.PreparedStatement;
import java.sql.Ref;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.RowId;
import java.sql.SQLException;
import java.sql.SQLXML;
import java.sql.Time;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;

/**
 * 继承: BasePreparedStatement 定制参数化数据库查询语句。
 * 
 * @author han.liao
 */
public abstract class BasePreparedStatement // NL 
        extends BaseStatement<PreparedStatement> implements PreparedStatement {

    // 保存设置的参数, 延迟至执行时设置。
    protected ArrayList<ParameterSetter> setters = new ArrayList<ParameterSetter>();

    @Override
    public void close() throws SQLException {

        setters.clear();

        super.close();
    }

    @Override
    public void addBatch() throws SQLException {

        fillParam();

        statement.addBatch();
    }

    @Override
    public boolean execute() throws SQLException {

        fillParam();

        return statement.execute();
    }

    @Override
    public ResultSet executeQuery() throws SQLException {

        fillParam();

        return statement.executeQuery();
    }

    @Override
    public int executeUpdate() throws SQLException {

        fillParam();

        return statement.executeUpdate();
    }

    @Override
    public ResultSetMetaData getMetaData() throws SQLException {

        initStatement();

        return statement.getMetaData();
    }

    @Override
    public ParameterMetaData getParameterMetaData() throws SQLException {

        initStatement();

        return statement.getParameterMetaData();
    }

    @Override
    public void setArray(final int parameterIndex, final Array x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Array 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setArray(parameterIndex, x);
            }
        });
    }

    @Override
    public void setAsciiStream(final int parameterIndex, final InputStream x) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setAsciiStream(parameterIndex, x);
            }
        });
    }

    @Override
    public void setAsciiStream(final int parameterIndex, final InputStream x, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setAsciiStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setAsciiStream(final int parameterIndex, final InputStream x, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setAsciiStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setBigDecimal(final int parameterIndex, final BigDecimal x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBigDecimal(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBinaryStream(final int parameterIndex, final InputStream x) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBinaryStream(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBinaryStream(final int parameterIndex, final InputStream x, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBinaryStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setBinaryStream(final int parameterIndex, final InputStream x, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBinaryStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setBlob(final int parameterIndex, final Blob x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Blob 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBlob(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBlob(final int parameterIndex, final InputStream inputStream)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBlob(parameterIndex, inputStream);
            }
        });
    }

    @Override
    public void setBlob(final int parameterIndex, final InputStream inputStream, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBlob(parameterIndex, inputStream, length);
            }
        });
    }

    @Override
    public void setBoolean(final int parameterIndex, final boolean x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBoolean(parameterIndex, x);
            }
        });
    }

    @Override
    public void setByte(final int parameterIndex, final byte x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setByte(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBytes(final int parameterIndex, final byte[] x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setBytes(parameterIndex, x);
            }
        });
    }

    @Override
    public void setCharacterStream(final int parameterIndex, final Reader reader)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setCharacterStream(parameterIndex, reader);
            }
        });
    }

    @Override
    public void setCharacterStream(final int parameterIndex, final Reader reader, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setCharacterStream(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setCharacterStream(final int parameterIndex, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setCharacterStream(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setClob(final int parameterIndex, final Clob x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Clob 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setClob(parameterIndex, x);
            }
        });
    }

    @Override
    public void setClob(final int parameterIndex, final Reader reader) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setClob(parameterIndex, reader);
            }
        });
    }

    @Override
    public void setClob(final int parameterIndex, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setClob(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setDate(final int parameterIndex, final Date x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setDate(parameterIndex, x);
            }
        });
    }

    @Override
    public void setDate(final int parameterIndex, final Date x, final Calendar cal)
            throws SQLException {

        // TODO: NEED-FIX 希望在后面  Calendar 不会被修改。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setDate(parameterIndex, x, cal);
            }
        });
    }

    @Override
    public void setDouble(final int parameterIndex, final double x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setDouble(parameterIndex, x);
            }
        });
    }

    @Override
    public void setFloat(final int parameterIndex, final float x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setFloat(parameterIndex, x);
            }
        });
    }

    @Override
    public void setInt(final int parameterIndex, final int x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(parameterIndex, x);
            }
        });
    }

    @Override
    public void setLong(final int parameterIndex, final long x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setLong(parameterIndex, x);
            }
        });
    }

    @Override
    public void setNCharacterStream(final int parameterIndex, final Reader value)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNCharacterStream(parameterIndex, value);
            }
        });
    }

    @Override
    public void setNCharacterStream(final int parameterIndex, final Reader value, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNCharacterStream(parameterIndex, value, length);
            }
        });
    }

    @Override
    public void setNClob(final int parameterIndex, final NClob value) throws SQLException {

        // TODO: NEED-FIX 如果之后  Clob 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNClob(parameterIndex, value);
            }
        });
    }

    @Override
    public void setNClob(final int parameterIndex, final Reader reader) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNClob(parameterIndex, reader);
            }
        });
    }

    @Override
    public void setNClob(final int parameterIndex, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNClob(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setNString(final int parameterIndex, final String value) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNString(parameterIndex, value);
            }
        });
    }

    @Override
    public void setNull(final int parameterIndex, final int sqlType) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNull(parameterIndex, sqlType);
            }
        });
    }

    @Override
    public void setNull(final int parameterIndex, final int sqlType, final String typeName)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setNull(parameterIndex, sqlType, typeName);
            }
        });
    }

    @Override
    public void setObject(final int parameterIndex, final Object x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setObject(parameterIndex, x);
            }
        });
    }

    @Override
    public void setObject(final int parameterIndex, final Object x, final int targetSqlType)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setObject(parameterIndex, x, targetSqlType);
            }
        });
    }

    @Override
    public void setObject(final int parameterIndex, final Object x, final int targetSqlType,
            final int scaleOrLength) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setObject(parameterIndex, x, targetSqlType, scaleOrLength);
            }
        });
    }

    @Override
    public void setRef(final int parameterIndex, final Ref x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setRef(parameterIndex, x);
            }
        });
    }

    @Override
    public void setRowId(final int parameterIndex, final RowId x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setRowId(parameterIndex, x);
            }
        });
    }

    @Override
    public void setSQLXML(final int parameterIndex, final SQLXML xmlObject) throws SQLException {

        // TODO: NEED-FIX 如果之后  SQLXML 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setSQLXML(parameterIndex, xmlObject);
            }
        });
    }

    @Override
    public void setShort(final int parameterIndex, final short x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setShort(parameterIndex, x);
            }
        });
    }

    @Override
    public void setString(final int parameterIndex, final String x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(parameterIndex, x);
            }
        });
    }

    @Override
    public void setTime(final int parameterIndex, final Time x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setTime(parameterIndex, x);
            }
        });
    }

    @Override
    public void setTime(final int parameterIndex, final Time x, final Calendar cal)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setTime(parameterIndex, x, cal);
            }
        });
    }

    @Override
    public void setTimestamp(final int parameterIndex, final Timestamp x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setTimestamp(parameterIndex, x);
            }
        });
    }

    @Override
    public void setTimestamp(final int parameterIndex, final Timestamp x, final Calendar cal)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setTimestamp(parameterIndex, x, cal);
            }
        });
    }

    @Override
    public void setURL(final int parameterIndex, final URL x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setURL(parameterIndex, x);
            }
        });
    }

    @Override
    @Deprecated
    public void setUnicodeStream(final int parameterIndex, final InputStream x, final int length)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setUnicodeStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void clearParameters() throws SQLException {

        setters.clear();
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {

        if (PreparedStatement.class.equals(iface)) {
            return true;
        }

        return super.isWrapperFor(iface);
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {

        if (PreparedStatement.class.equals(iface)) {
            iface.cast(this);
        }

        return super.unwrap(iface);
    }

    /**
     * 实现查询参数设置的委托接口。
     * 
     * @author han.liao
     */
    protected static abstract class ParameterSetter {

        /**
         * 委托设置单条查询参数。
         * 
         * @param statement - 设置的查询语句。
         * 
         * @throws SQLException
         */
        public abstract void setParam(PreparedStatement ps) throws SQLException;
    }

    /**
     * 设置参数化查询语句的参数: ParameterSetter 委托。 <br>
     * 
     * @param parameterIndex - 参数索引, 1, 2, 3, 4, etc
     * 
     * @param setter - 设置参数的委托调用。
     */
    protected void setParam(int parameterIndex, ParameterSetter setter) {

        setters.ensureCapacity(parameterIndex);

        while (setters.size() < parameterIndex) {
            setters.add(null);
        }

        setters.set(parameterIndex - 1, setter);
    }

    @Override
    public Connection getConnection() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    /**
     * 批量应用参数化查询语句的参数列表。 <br>
     * 
     * @throws SQLException
     */
    protected void fillParam() throws SQLException {

        initStatement(); // 获得查询语句。

        // 输出日志。
        if (logger.isDebugEnabled()) {
            logger.debug("Filling in " + setters.size() + " parameters.");
        }

        // 设置查询参数。
        for (ParameterSetter setter : setters) {

            if (setter != null) {
                setter.setParam(statement);
            }
        }
    }
}
