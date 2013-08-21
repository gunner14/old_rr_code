/**
 * 
 */
package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.io.InputStream;
import java.io.Reader;
import java.math.BigDecimal;
import java.net.URL;
import java.sql.Array;
import java.sql.Blob;
import java.sql.CallableStatement;
import java.sql.Clob;
import java.sql.Date;
import java.sql.NClob;
import java.sql.ParameterMetaData;
import java.sql.Ref;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.RowId;
import java.sql.SQLException;
import java.sql.SQLXML;
import java.sql.Time;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;

/**
 * 继承: BaseCallableStatement 定制参数化数据库存储过程。
 * 
 * @author han.liao
 */
public abstract class BaseCallableStatement extends BaseStatement<CallableStatement> implements
        CallableStatement {

    // 保存注册的输出参数, 延迟至执行时设置。
    protected HashMap<String, ParameterSetter> outParams = new HashMap<String, ParameterSetter>();

    // 保存设置的参数, 延迟至执行时设置。
    protected HashMap<String, ParameterSetter> inParams = new HashMap<String, ParameterSetter>();

    @Override
    public void close() throws SQLException {

        // 清除存储过程输出参数。
        outParams.clear();

        // 清除存储过程调用参数。
        inParams.clear();

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
    public ParameterMetaData getParameterMetaData() throws SQLException {

        initStatement();

        return statement.getParameterMetaData();
    }

    @Override
    public ResultSetMetaData getMetaData() throws SQLException {

        initStatement();

        return statement.getMetaData();
    }

    @Override
    public boolean wasNull() throws SQLException {

        initStatement();

        return statement.wasNull();
    }

    @Override
    public Array getArray(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getArray(parameterIndex);
    }

    @Override
    public Array getArray(String parameterName) throws SQLException {

        initStatement();

        return statement.getArray(parameterName);
    }

    @Override
    public BigDecimal getBigDecimal(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getBigDecimal(parameterIndex);
    }

    @Override
    public BigDecimal getBigDecimal(String parameterName) throws SQLException {

        initStatement();

        return statement.getBigDecimal(parameterName);
    }

    @Override
    @Deprecated
    public BigDecimal getBigDecimal(int parameterIndex, int scale) throws SQLException {

        initStatement();

        return statement.getBigDecimal(parameterIndex, scale);
    }

    @Override
    public Blob getBlob(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getBlob(parameterIndex);
    }

    @Override
    public Blob getBlob(String parameterName) throws SQLException {

        initStatement();

        return statement.getBlob(parameterName);
    }

    @Override
    public boolean getBoolean(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getBoolean(parameterIndex);
    }

    @Override
    public boolean getBoolean(String parameterName) throws SQLException {

        initStatement();

        return statement.getBoolean(parameterName);
    }

    @Override
    public byte getByte(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getByte(parameterIndex);
    }

    @Override
    public byte getByte(String parameterName) throws SQLException {

        initStatement();

        return statement.getByte(parameterName);
    }

    @Override
    public byte[] getBytes(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getBytes(parameterIndex);
    }

    @Override
    public byte[] getBytes(String parameterName) throws SQLException {

        initStatement();

        return statement.getBytes(parameterName);
    }

    @Override
    public Reader getCharacterStream(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getCharacterStream(parameterIndex);
    }

    @Override
    public Reader getCharacterStream(String parameterName) throws SQLException {

        initStatement();

        return statement.getCharacterStream(parameterName);
    }

    @Override
    public Clob getClob(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getClob(parameterIndex);
    }

    @Override
    public Clob getClob(String parameterName) throws SQLException {

        initStatement();

        return statement.getClob(parameterName);
    }

    @Override
    public Date getDate(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getDate(parameterIndex);
    }

    @Override
    public Date getDate(String parameterName) throws SQLException {

        initStatement();

        return statement.getDate(parameterName);
    }

    @Override
    public Date getDate(int parameterIndex, Calendar cal) throws SQLException {

        initStatement();

        return statement.getDate(parameterIndex, cal);
    }

    @Override
    public Date getDate(String parameterName, Calendar cal) throws SQLException {

        initStatement();

        return statement.getDate(parameterName, cal);
    }

    @Override
    public double getDouble(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getDouble(parameterIndex);
    }

    @Override
    public double getDouble(String parameterName) throws SQLException {

        initStatement();

        return statement.getDouble(parameterName);
    }

    @Override
    public float getFloat(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getFloat(parameterIndex);
    }

    @Override
    public float getFloat(String parameterName) throws SQLException {

        initStatement();

        return statement.getFloat(parameterName);
    }

    @Override
    public int getInt(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getInt(parameterIndex);
    }

    @Override
    public int getInt(String parameterName) throws SQLException {

        initStatement();

        return statement.getInt(parameterName);
    }

    @Override
    public long getLong(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getLong(parameterIndex);
    }

    @Override
    public long getLong(String parameterName) throws SQLException {

        initStatement();

        return statement.getLong(parameterName);
    }

    @Override
    public Reader getNCharacterStream(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getNCharacterStream(parameterIndex);
    }

    @Override
    public Reader getNCharacterStream(String parameterName) throws SQLException {

        initStatement();

        return statement.getNCharacterStream(parameterName);
    }

    @Override
    public NClob getNClob(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getNClob(parameterIndex);
    }

    @Override
    public NClob getNClob(String parameterName) throws SQLException {

        initStatement();

        return statement.getNClob(parameterName);
    }

    @Override
    public String getNString(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getNString(parameterIndex);
    }

    @Override
    public String getNString(String parameterName) throws SQLException {

        initStatement();

        return statement.getNString(parameterName);
    }

    @Override
    public Object getObject(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getObject(parameterIndex);
    }

    @Override
    public Object getObject(String parameterName) throws SQLException {

        initStatement();

        return statement.getObject(parameterName);
    }

    @Override
    public Object getObject(int parameterIndex, Map<String, Class<?>> map) throws SQLException {

        initStatement();

        return statement.getObject(parameterIndex, map);
    }

    @Override
    public Object getObject(String parameterName, Map<String, Class<?>> map) throws SQLException {

        initStatement();

        return statement.getObject(parameterName, map);
    }

    @Override
    public Ref getRef(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getRef(parameterIndex);
    }

    @Override
    public Ref getRef(String parameterName) throws SQLException {

        initStatement();

        return statement.getRef(parameterName);
    }

    @Override
    public RowId getRowId(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getRowId(parameterIndex);
    }

    @Override
    public RowId getRowId(String parameterName) throws SQLException {

        initStatement();

        return statement.getRowId(parameterName);
    }

    @Override
    public SQLXML getSQLXML(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getSQLXML(parameterIndex);
    }

    @Override
    public SQLXML getSQLXML(String parameterName) throws SQLException {

        initStatement();

        return statement.getSQLXML(parameterName);
    }

    @Override
    public short getShort(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getShort(parameterIndex);
    }

    @Override
    public short getShort(String parameterName) throws SQLException {

        initStatement();

        return statement.getShort(parameterName);
    }

    @Override
    public String getString(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getString(parameterIndex);
    }

    @Override
    public String getString(String parameterName) throws SQLException {

        initStatement();

        return statement.getString(parameterName);
    }

    @Override
    public Time getTime(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getTime(parameterIndex);
    }

    @Override
    public Time getTime(String parameterName) throws SQLException {

        initStatement();

        return statement.getTime(parameterName);
    }

    @Override
    public Time getTime(int parameterIndex, Calendar cal) throws SQLException {

        initStatement();

        return statement.getTime(parameterIndex, cal);
    }

    @Override
    public Time getTime(String parameterName, Calendar cal) throws SQLException {

        initStatement();

        return statement.getTime(parameterName, cal);
    }

    @Override
    public Timestamp getTimestamp(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getTimestamp(parameterIndex);
    }

    @Override
    public Timestamp getTimestamp(String parameterName) throws SQLException {

        initStatement();

        return statement.getTimestamp(parameterName);
    }

    @Override
    public Timestamp getTimestamp(int parameterIndex, Calendar cal) throws SQLException {

        initStatement();

        return statement.getTimestamp(parameterIndex, cal);
    }

    @Override
    public Timestamp getTimestamp(String parameterName, Calendar cal) throws SQLException {

        initStatement();

        return statement.getTimestamp(parameterName, cal);
    }

    @Override
    public URL getURL(int parameterIndex) throws SQLException {

        initStatement();

        return statement.getURL(parameterIndex);
    }

    @Override
    public URL getURL(String parameterName) throws SQLException {

        initStatement();

        return statement.getURL(parameterName);
    }

    @Override
    public void registerOutParameter(final int parameterIndex, final int sqlType)
            throws SQLException {

        setOutParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.registerOutParameter(parameterIndex, sqlType);
            }
        });
    }

    @Override
    public void registerOutParameter(final String parameterName, final int sqlType)
            throws SQLException {

        setOutParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.registerOutParameter(parameterName, sqlType);
            }
        });
    }

    @Override
    public void registerOutParameter(final int parameterIndex, final int sqlType, final int scale)
            throws SQLException {

        setOutParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.registerOutParameter(parameterIndex, sqlType, scale);
            }
        });
    }

    @Override
    public void registerOutParameter(final int parameterIndex, final int sqlType,
            final String typeName) throws SQLException {

        setOutParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.registerOutParameter(parameterIndex, sqlType, typeName);
            }
        });
    }

    @Override
    public void registerOutParameter(final String parameterName, final int sqlType, final int scale)
            throws SQLException {

        setOutParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.registerOutParameter(parameterName, sqlType, scale);
            }
        });
    }

    @Override
    public void registerOutParameter(final String parameterName, final int sqlType,
            final String typeName) throws SQLException {

        setOutParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.registerOutParameter(parameterName, sqlType, typeName);
            }
        });
    }

    @Override
    public void setAsciiStream(final String parameterName, final InputStream x) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setAsciiStream(parameterName, x);
            }
        });
    }

    @Override
    public void setAsciiStream(final String parameterName, final InputStream x, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setAsciiStream(parameterName, x, length);
            }
        });
    }

    @Override
    public void setAsciiStream(final String parameterName, final InputStream x, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setAsciiStream(parameterName, x, length);
            }
        });
    }

    @Override
    public void setBigDecimal(final String parameterName, final BigDecimal x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBigDecimal(parameterName, x);
            }
        });
    }

    @Override
    public void setBinaryStream(final String parameterName, final InputStream x)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBinaryStream(parameterName, x);
            }
        });
    }

    @Override
    public void setBinaryStream(final String parameterName, final InputStream x, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBinaryStream(parameterName, x, length);
            }
        });
    }

    @Override
    public void setBinaryStream(final String parameterName, final InputStream x, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBinaryStream(parameterName, x, length);
            }
        });
    }

    @Override
    public void setBlob(final String parameterName, final Blob x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Blob 被  free, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBlob(parameterName, x);
            }
        });
    }

    @Override
    public void setBlob(final String parameterName, final InputStream inputStream)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBlob(parameterName, inputStream);
            }
        });
    }

    @Override
    public void setBlob(final String parameterName, final InputStream inputStream, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBlob(parameterName, inputStream, length);
            }
        });
    }

    @Override
    public void setBoolean(final String parameterName, final boolean x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBoolean(parameterName, x);
            }
        });
    }

    @Override
    public void setByte(final String parameterName, final byte x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setByte(parameterName, x);
            }
        });
    }

    @Override
    public void setBytes(final String parameterName, final byte[] x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setBytes(parameterName, x);
            }
        });
    }

    @Override
    public void setCharacterStream(final String parameterName, final Reader reader)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setCharacterStream(parameterName, reader);
            }
        });
    }

    @Override
    public void setCharacterStream(final String parameterName, final Reader reader, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setCharacterStream(parameterName, reader);
            }
        });
    }

    @Override
    public void setCharacterStream(final String parameterName, final Reader reader,
            final long length) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setCharacterStream(parameterName, reader, length);
            }
        });
    }

    @Override
    public void setClob(final String parameterName, final Clob x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Clob 被  free, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setClob(parameterName, x);
            }
        });
    }

    @Override
    public void setClob(final String parameterName, final Reader reader) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setClob(parameterName, reader);
            }
        });
    }

    @Override
    public void setClob(final String parameterName, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setClob(parameterName, reader, length);
            }
        });
    }

    @Override
    public void setDate(final String parameterName, final Date x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setDate(parameterName, x);
            }
        });
    }

    @Override
    public void setDate(final String parameterName, final Date x, final Calendar cal)
            throws SQLException {

        // TODO: NEED-FIX 希望在后面  Calendar 不会被修改。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setDate(parameterName, x, cal);
            }
        });
    }

    @Override
    public void setDouble(final String parameterName, final double x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setDouble(parameterName, x);
            }
        });
    }

    @Override
    public void setFloat(final String parameterName, final float x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setFloat(parameterName, x);
            }
        });
    }

    @Override
    public void setInt(final String parameterName, final int x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setInt(parameterName, x);
            }
        });
    }

    @Override
    public void setLong(final String parameterName, final long x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setLong(parameterName, x);
            }
        });
    }

    @Override
    public void setNCharacterStream(final String parameterName, final Reader value)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNCharacterStream(parameterName, value);
            }
        });
    }

    @Override
    public void setNCharacterStream(final String parameterName, final Reader value,
            final long length) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNCharacterStream(parameterName, value, length);
            }
        });
    }

    @Override
    public void setNClob(final String parameterName, final NClob value) throws SQLException {

        // TODO: NEED-FIX 如果之后  NClob 被  free, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNClob(parameterName, value);
            }
        });
    }

    @Override
    public void setNClob(final String parameterName, final Reader reader) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNClob(parameterName, reader);
            }
        });
    }

    @Override
    public void setNClob(final String parameterName, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNClob(parameterName, reader, length);
            }
        });
    }

    @Override
    public void setNString(final String parameterName, final String value) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNString(parameterName, value);
            }
        });
    }

    @Override
    public void setNull(final String parameterName, final int sqlType) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNull(parameterName, sqlType);
            }
        });
    }

    @Override
    public void setNull(final String parameterName, final int sqlType, final String typeName)
            throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setNull(parameterName, sqlType, typeName);
            }
        });
    }

    @Override
    public void setObject(final String parameterName, final Object x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setObject(parameterName, x);
            }
        });
    }

    @Override
    public void setObject(final String parameterName, final Object x, final int targetSqlType)
            throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setObject(parameterName, x, targetSqlType);
            }
        });
    }

    @Override
    public void setObject(final String parameterName, final Object x, final int targetSqlType,
            final int scale) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setObject(parameterName, x, targetSqlType, scale);
            }
        });
    }

    @Override
    public void setRowId(final String parameterName, final RowId x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setRowId(parameterName, x);
            }
        });
    }

    @Override
    public void setSQLXML(final String parameterName, final SQLXML xmlObject) throws SQLException {

        // TODO: NEED-FIX 如果之后  SQLXML 被  free, 延迟的设置将出错。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setSQLXML(parameterName, xmlObject);
            }
        });
    }

    @Override
    public void setShort(final String parameterName, final short x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setShort(parameterName, x);
            }
        });
    }

    @Override
    public void setString(final String parameterName, final String x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setString(parameterName, x);
            }
        });
    }

    @Override
    public void setTime(final String parameterName, final Time x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setTime(parameterName, x);
            }
        });
    }

    @Override
    public void setTime(final String parameterName, final Time x, final Calendar cal)
            throws SQLException {

        // TODO: NEED-FIX 希望在后面  Calendar 不会被修改。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setTime(parameterName, x, cal);
            }
        });
    }

    @Override
    public void setTimestamp(final String parameterName, final Timestamp x) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setTimestamp(parameterName, x);
            }
        });
    }

    @Override
    public void setTimestamp(final String parameterName, final Timestamp x, final Calendar cal)
            throws SQLException {

        // TODO: NEED-FIX 希望在后面  Calendar 不会被修改。

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setTimestamp(parameterName, x, cal);
            }
        });
    }

    @Override
    public void setURL(final String parameterName, final URL val) throws SQLException {

        setParam(parameterName, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {

                cs.setURL(parameterName, val);
            }
        });
    }

    @Override
    public void setArray(final int parameterIndex, final Array x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Array 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setArray(parameterIndex, x);
            }
        });
    }

    @Override
    public void setAsciiStream(final int parameterIndex, final InputStream x) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setAsciiStream(parameterIndex, x);
            }
        });
    }

    @Override
    public void setAsciiStream(final int parameterIndex, final InputStream x, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setAsciiStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setAsciiStream(final int parameterIndex, final InputStream x, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setAsciiStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setBigDecimal(final int parameterIndex, final BigDecimal x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBigDecimal(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBinaryStream(final int parameterIndex, final InputStream x) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBinaryStream(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBinaryStream(final int parameterIndex, final InputStream x, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBinaryStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setBinaryStream(final int parameterIndex, final InputStream x, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBinaryStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void setBlob(final int parameterIndex, final Blob x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Blob 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBlob(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBlob(final int parameterIndex, final InputStream inputStream)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBlob(parameterIndex, inputStream);
            }
        });
    }

    @Override
    public void setBlob(final int parameterIndex, final InputStream inputStream, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBlob(parameterIndex, inputStream, length);
            }
        });
    }

    @Override
    public void setBoolean(final int parameterIndex, final boolean x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBoolean(parameterIndex, x);
            }
        });
    }

    @Override
    public void setByte(final int parameterIndex, final byte x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setByte(parameterIndex, x);
            }
        });
    }

    @Override
    public void setBytes(final int parameterIndex, final byte[] x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setBytes(parameterIndex, x);
            }
        });
    }

    @Override
    public void setCharacterStream(final int parameterIndex, final Reader reader)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setCharacterStream(parameterIndex, reader);
            }
        });
    }

    @Override
    public void setCharacterStream(final int parameterIndex, final Reader reader, final int length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setCharacterStream(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setCharacterStream(final int parameterIndex, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setCharacterStream(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setClob(final int parameterIndex, final Clob x) throws SQLException {

        // TODO: NEED-FIX 如果之后  Clob 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setClob(parameterIndex, x);
            }
        });
    }

    @Override
    public void setClob(final int parameterIndex, final Reader reader) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setClob(parameterIndex, reader);
            }
        });
    }

    @Override
    public void setClob(final int parameterIndex, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setClob(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setDate(final int parameterIndex, final Date x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setDate(parameterIndex, x);
            }
        });
    }

    @Override
    public void setDate(final int parameterIndex, final Date x, final Calendar cal)
            throws SQLException {

        // TODO: NEED-FIX 希望在后面  Calendar 不会被修改。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setDate(parameterIndex, x, cal);
            }
        });
    }

    @Override
    public void setDouble(final int parameterIndex, final double x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setDouble(parameterIndex, x);
            }
        });
    }

    @Override
    public void setFloat(final int parameterIndex, final float x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setFloat(parameterIndex, x);
            }
        });
    }

    @Override
    public void setInt(final int parameterIndex, final int x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setInt(parameterIndex, x);
            }
        });
    }

    @Override
    public void setLong(final int parameterIndex, final long x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setLong(parameterIndex, x);
            }
        });
    }

    @Override
    public void setNCharacterStream(final int parameterIndex, final Reader value)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNCharacterStream(parameterIndex, value);
            }
        });
    }

    @Override
    public void setNCharacterStream(final int parameterIndex, final Reader value, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNCharacterStream(parameterIndex, value, length);
            }
        });
    }

    @Override
    public void setNClob(final int parameterIndex, final NClob value) throws SQLException {

        // TODO: NEED-FIX 如果之后  Clob 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNClob(parameterIndex, value);
            }
        });
    }

    @Override
    public void setNClob(final int parameterIndex, final Reader reader) throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNClob(parameterIndex, reader);
            }
        });
    }

    @Override
    public void setNClob(final int parameterIndex, final Reader reader, final long length)
            throws SQLException {

        // TODO: NEED-FIX 如果之后流被关闭, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNClob(parameterIndex, reader, length);
            }
        });
    }

    @Override
    public void setNString(final int parameterIndex, final String value) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNString(parameterIndex, value);
            }
        });
    }

    @Override
    public void setNull(final int parameterIndex, final int sqlType) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNull(parameterIndex, sqlType);
            }
        });
    }

    @Override
    public void setNull(final int parameterIndex, final int sqlType, final String typeName)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setNull(parameterIndex, sqlType, typeName);
            }
        });
    }

    @Override
    public void setObject(final int parameterIndex, final Object x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setObject(parameterIndex, x);
            }
        });
    }

    @Override
    public void setObject(final int parameterIndex, final Object x, final int targetSqlType)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setObject(parameterIndex, x, targetSqlType);
            }
        });
    }

    @Override
    public void setObject(final int parameterIndex, final Object x, final int targetSqlType,
            final int scaleOrLength) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setObject(parameterIndex, x, targetSqlType, scaleOrLength);
            }
        });
    }

    @Override
    public void setRef(final int parameterIndex, final Ref x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setRef(parameterIndex, x);
            }
        });
    }

    @Override
    public void setRowId(final int parameterIndex, final RowId x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setRowId(parameterIndex, x);
            }
        });
    }

    @Override
    public void setSQLXML(final int parameterIndex, final SQLXML xmlObject) throws SQLException {

        // TODO: NEED-FIX 如果之后  SQLXML 被  free, 延迟的设置将出错。

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setSQLXML(parameterIndex, xmlObject);
            }
        });
    }

    @Override
    public void setShort(final int parameterIndex, final short x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setShort(parameterIndex, x);
            }
        });
    }

    @Override
    public void setString(final int parameterIndex, final String x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setString(parameterIndex, x);
            }
        });
    }

    @Override
    public void setTime(final int parameterIndex, final Time x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setTime(parameterIndex, x);
            }
        });
    }

    @Override
    public void setTime(final int parameterIndex, final Time x, final Calendar cal)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setTime(parameterIndex, x, cal);
            }
        });
    }

    @Override
    public void setTimestamp(final int parameterIndex, final Timestamp x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setTimestamp(parameterIndex, x);
            }
        });
    }

    @Override
    public void setTimestamp(final int parameterIndex, final Timestamp x, final Calendar cal)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setTimestamp(parameterIndex, x, cal);
            }
        });
    }

    @Override
    public void setURL(final int parameterIndex, final URL x) throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setURL(parameterIndex, x);
            }
        });
    }

    @Override
    @Deprecated
    public void setUnicodeStream(final int parameterIndex, final InputStream x, final int length)
            throws SQLException {

        setParam(parameterIndex, new ParameterSetter() {

            @Override
            public void setParam(CallableStatement cs) throws SQLException {
                cs.setUnicodeStream(parameterIndex, x, length);
            }
        });
    }

    @Override
    public void clearParameters() throws SQLException {

        inParams.clear();
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {

        if (CallableStatement.class.equals(iface)) {
            return true;
        }

        return super.isWrapperFor(iface);
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {

        if (CallableStatement.class.equals(iface)) {
            iface.cast(this);
        }

        return super.unwrap(iface);
    }

    /**
     * 实现存储过程参数设置的委托接口。
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
        public abstract void setParam(CallableStatement cs) throws SQLException;
    }

    /**
     * 设置存储过程的参数: ParameterSetter 委托。 <br>
     * 
     * @param parameterName - 参数名称
     * 
     * @param setter - 设置参数的委托调用。
     */
    protected void setOutParam(int parameterIndex, ParameterSetter setter) {

        // TODO: FIX-LATER 如果混用索引和名称设置, 可能会导致错误覆盖。
        outParams.put(String.valueOf('@') + parameterIndex, setter);
    }

    /**
     * 设置存储过程的参数: ParameterSetter 委托。 <br>
     * 
     * @param parameterName - 参数名称
     * 
     * @param setter - 设置参数的委托调用。
     */
    protected void setOutParam(String parameterName, ParameterSetter setter) {

        outParams.put(parameterName, setter);
    }

    /**
     * 设置存储过程的参数: ParameterSetter 委托。 <br>
     * 
     * @param parameterName - 参数名称
     * 
     * @param setter - 设置参数的委托调用。
     */
    protected void setParam(int parameterIndex, ParameterSetter setter) {

        // TODO: FIX-LATER 如果混用索引和名称设置, 可能会导致错误覆盖。
        inParams.put(String.valueOf('@') + parameterIndex, setter);
    }

    /**
     * 设置存储过程的参数: ParameterSetter 委托。 <br>
     * 
     * @param parameterName - 参数名称
     * 
     * @param setter - 设置参数的委托调用。
     */
    protected void setParam(String parameterName, ParameterSetter setter) {

        inParams.put(parameterName, setter);
    }

    /**
     * 批量应用存储过程的参数列表。 <br>
     * 
     * @throws SQLException
     */
    protected void fillParam() throws SQLException {

        initStatement(); // 获得查询语句。

        // 记录日志。
        if (logger.isDebugEnabled()) {
            logger.debug("Filling in " + inParams.size() + " parameters");
        }

        // 写入存储过程调用参数。
        for (ParameterSetter setter : inParams.values()) {

            if (setter != null) {
                setter.setParam(statement);
            }
        }

        // 记录日志。
        if (logger.isDebugEnabled()) {
            logger.debug("Registering " + outParams.size() + " out-parameters");
        }

        // 写入存储过程返回参数。
        for (ParameterSetter setter : outParams.values()) {

            if (setter != null) {
                setter.setParam(statement);
            }
        }
    }
}
