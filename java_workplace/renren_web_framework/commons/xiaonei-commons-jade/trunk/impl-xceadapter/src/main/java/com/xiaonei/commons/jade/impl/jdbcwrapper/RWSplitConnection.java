package com.xiaonei.commons.jade.impl.jdbcwrapper;

import java.sql.Connection;
import java.sql.SQLException;

/**
 * 实现简单读写分离的数据库连接，用于功能验证。
 * 
 * @author han.liao
 */
public class RWSplitConnection extends HAConnection {

    // 读写数据库状态
    protected boolean desireRead = true;

    // 使用的数据源。
    protected RWSplitDataSource ds;

    /**
     * 创建: RWSplitConnection 实例。
     * 
     * @param ds - 指定的数据源。
     */
    public RWSplitConnection(RWSplitDataSource ds) {

        this.ds = ds;
    }

    @Override
    public void setAutoCommit(boolean autoCommit) throws SQLException {

        if (autoCommit != this.autoCommit) {

            freeConnection(); // 切换使用的连接

            this.autoCommit = autoCommit;
        }
    }

    @Override
    public void setReadOnly(boolean readOnly) throws SQLException {

        if (readOnly != this.readOnly) {

            freeConnection(); // 切换使用的连接

            this.readOnly = readOnly;
        }
    }

    @Override
    protected boolean analyzeSql(String sql) throws SQLException {

        // 输出信息。
        super.analyzeSql(sql);

        // 拆分查询语句。
        String[] tokens = sql.split("\\s+");

        if (tokens.length >= 2) {

            // 只有非事务的  SELECT 查询采用只读数据库连接。
            boolean desireRead = readOnly || (autoCommit && "SELECT".equalsIgnoreCase(tokens[0]));

            if (desireRead != this.desireRead) {

                if (desireRead) {

                    // 如果是下列查询，防止切换连接。
                    if ("@@LAST_INSERT_ID".equalsIgnoreCase(tokens[1])
                            || "LAST_INSERT_ID()".equalsIgnoreCase(tokens[1])
                            || "FOUND_ROWS()".equalsIgnoreCase(tokens[1])) {

                        // 查询上一次插入的自动生成  ID 不能切换连接
                        // SELECT @@LAST_INSERT_ID
                        // SELECT LAST_INSERT_ID()

                        // 查询上一次返回的记录行数 不能切换连接
                        // SELECT FOUND_ROWS()

                        return false;
                    }
                }

                // 切换使用的连接。
                freeConnection();

                this.desireRead = desireRead;

                return true;
            }
        }

        return false;
    }

    @Override
    protected Connection getConnection() throws SQLException {

        if (!desireRead) {

            // 获取可写数据库连接。
            return ds.getWriteConnection();

        } else {

            // 获取只读数据库连接。
            return ds.getReadConnection();
        }
    }
}
