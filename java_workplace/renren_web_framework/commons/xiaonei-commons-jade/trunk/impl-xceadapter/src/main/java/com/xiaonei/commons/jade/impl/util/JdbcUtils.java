package com.xiaonei.commons.jade.impl.util;

import java.math.BigDecimal;
import java.math.BigInteger;
import java.sql.Blob;
import java.sql.Clob;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Time;
import java.sql.Timestamp;
import java.sql.Types;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 定义简单的: JDBC 静态方法。
 * 
 * @author han.liao
 */
public class JdbcUtils {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(JdbcUtils.class);

    /**
     * 从: JDBC 类型获取名称, 这个方法只用于生成日志信息。
     * 
     * @param jdbcType - JDBC 类型
     * 
     * @return JDBC 类型的名称。
     */
    public static String getTypeName(int jdbcType) {

        switch (jdbcType) {

            case Types.ARRAY:
                return "ARRAY";

            case Types.BIGINT:
                return "BIGINT";

            case Types.BINARY:
                return "BINARY";

            case Types.BIT:
                return "BIT";

            case Types.BLOB:
                return "BLOB";

            case Types.BOOLEAN:
                return "BOOLEAN";

            case Types.CHAR:
                return "CHAR";

            case Types.CLOB:
                return "CLOB";

            case Types.DATALINK:
                return "DATALINK";

            case Types.DATE:
                return "DATE";

            case Types.DECIMAL:
                return "DECIMAL";

            case Types.DISTINCT:
                return "DISTINCT";

            case Types.DOUBLE:
                return "DOUBLE";

            case Types.FLOAT:
                return "FLOAT";

            case Types.INTEGER:
                return "INTEGER";

            case Types.JAVA_OBJECT:
                return "JAVA_OBJECT";

            case Types.LONGNVARCHAR:
                return "LONGNVARCHAR";

            case Types.LONGVARBINARY:
                return "LONGVARBINARY";

            case Types.LONGVARCHAR:
                return "LONGVARCHAR";

            case Types.NCHAR:
                return "NCHAR";

            case Types.NCLOB:
                return "NCLOB";

            case Types.NULL:
                return "NULL";

            case Types.NUMERIC:
                return "NUMERIC";

            case Types.NVARCHAR:
                return "NVARCHAR";

            case Types.OTHER:
                return "OTHER";

            case Types.REAL:
                return "REAL";

            case Types.REF:
                return "REF";

            case Types.ROWID:
                return "ROWID";

            case Types.SMALLINT:
                return "SMALLINT";

            case Types.SQLXML:
                return "SQLXML";

            case Types.STRUCT:
                return "STRUCT";

            case Types.TIME:
                return "TIME";

            case Types.TIMESTAMP:
                return "TIMESTAMP";

            case Types.TINYINT:
                return "TINYINT";

            case Types.VARBINARY:
                return "VARBINARY";

            case Types.VARCHAR:
                return "VARCHAR";

            default:
                return String.valueOf('[') + jdbcType + ']';
        }
    }

    /**
     * 检查: JDBC 类型与提供的: Java 类型是否不匹配。
     * 
     * @param jdbcType - JDBC 类型
     * 
     * @param clazz - Java 类型
     * 
     * @return 不匹配返回: <code>true</code>
     */
    public static boolean isTypeMismatch(int jdbcType, Class<?> clazz) {

        switch (jdbcType) {

            case Types.BIT:
            case Types.BOOLEAN:
                return !clazz.isAssignableFrom(Boolean.class);

            case Types.TINYINT:
                // 可能是整数或者字节
                return !(clazz.isAssignableFrom(Integer.TYPE)
                        || clazz.isAssignableFrom(Integer.class)
                        || clazz.isAssignableFrom(Byte.TYPE) || clazz.isAssignableFrom(Byte.class));

            case Types.SMALLINT:
                // 可能是整数或者短整数
                return !(clazz.isAssignableFrom(Integer.TYPE)
                        || clazz.isAssignableFrom(Integer.class)
                        || clazz.isAssignableFrom(Short.TYPE) || clazz
                        .isAssignableFrom(Short.class));

            case Types.INTEGER:
                // 根据数据长度可能是整数或者长整数
                return !(clazz.isAssignableFrom(Integer.TYPE)
                        || clazz.isAssignableFrom(Integer.class)
                        || clazz.isAssignableFrom(Long.TYPE) || clazz.isAssignableFrom(Long.class));

            case Types.BIGINT:
                // 根据数据长度可能是长整数或者  BigInteger
                return !(clazz.isAssignableFrom(Long.TYPE) || clazz.isAssignableFrom(Long.class) || clazz
                        .isAssignableFrom(BigInteger.class));

            case Types.DECIMAL:
            case Types.NUMERIC:
                return !clazz.isAssignableFrom(BigDecimal.class);

            case Types.REAL:
                return !(clazz.isAssignableFrom(Float.TYPE) || clazz.isAssignableFrom(Float.class));

            case Types.FLOAT:
            case Types.DOUBLE:
                return !(clazz.isAssignableFrom(Double.TYPE) || clazz
                        .isAssignableFrom(Double.class));

            case Types.CHAR:
            case Types.VARCHAR:
            case Types.LONGVARCHAR:
                // 接受  java.lang.String 作为参数
                return !(clazz.isAssignableFrom(String.class) || clazz.isAssignableFrom(Clob.class));

            case Types.BINARY:
            case Types.VARBINARY:
            case Types.LONGVARBINARY:
                // 接受  java.lang.String 作为参数
                return !(clazz.isAssignableFrom(String.class) || clazz.isAssignableFrom(Blob.class));

            case Types.DATE:
                // 接受  java.util.Date 作为参数
                return !(clazz.isAssignableFrom(java.sql.Date.class));

            case Types.TIME:
                // 接受  java.util.Date 作为参数
                return !(clazz.isAssignableFrom(Time.class));

            case Types.TIMESTAMP:
                // 接受  java.util.Date 作为参数
                return !(clazz.isAssignableFrom(Timestamp.class));

            default:
                // 可以用任意类型作为参数, 只要能够转换
                return false;
        }
    }

    /**
     * 安全的关闭数据库连接。
     * 
     * @param conn - 数据库连接
     */
    public static void safeClose(Connection conn) {

        if (conn != null) {

            try {
                // 关闭连接
                conn.close();

            } catch (SQLException e) {

                // 输出警告日志
                if (logger.isWarnEnabled()) {
                    logger.warn("Could not close JDBC Connection", e);
                }

            } catch (Throwable e) {

                // 输出警告日志
                if (logger.isWarnEnabled()) {
                    logger.warn("Unexpected exception on closing JDBC Connection", e);
                }
            }
        }
    }

    /**
     * 安全的关闭查询语句。
     * 
     * @param stmt - 查询语句
     */
    public static void safeClose(Statement stmt) {

        if (stmt != null) {

            try {
                // 关闭查询语句
                stmt.close();

            } catch (SQLException e) {

                // 输出警告日志
                if (logger.isWarnEnabled()) {
                    logger.warn("Could not close JDBC Statement", e);
                }

            } catch (Throwable e) {

                // 输出警告日志
                if (logger.isWarnEnabled()) {
                    logger.warn("Unexpected exception on closing JDBC Statement", e);
                }
            }
        }
    }

    /**
     * 安全的关闭查询结果集。
     * 
     * @param rs - 查询结果集
     */
    public static void safeClose(ResultSet rs) {

        if (rs != null) {

            try {
                // 关闭查询语句
                rs.close();

            } catch (SQLException e) {

                // 输出警告日志
                if (logger.isWarnEnabled()) {
                    logger.warn("Could not close JDBC ResultSet", e);
                }

            } catch (Throwable e) {

                // 输出警告日志
                if (logger.isWarnEnabled()) {
                    logger.warn("Unexpected exception on closing JDBC ResultSet", e);
                }
            }
        }
    }
}
