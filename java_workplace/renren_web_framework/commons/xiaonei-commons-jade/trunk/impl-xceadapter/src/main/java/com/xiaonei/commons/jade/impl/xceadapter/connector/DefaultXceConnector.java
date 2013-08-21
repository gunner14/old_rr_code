package com.xiaonei.commons.jade.impl.xceadapter.connector;

import java.sql.Connection;
import java.sql.SQLException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceConnector;
import com.xiaonei.xce.XceAdapter;

/**
 * 实现使用: XceAdapter 获取数据库连接的连接器，这也是默认的连接器。
 * 
 * @author han.liao
 */
public class DefaultXceConnector implements XceConnector {

    // 输出日志
    protected static final Log logger = LogFactory.getLog(DefaultXceConnector.class);

    // 使用单例
    protected static DefaultXceConnector instance = new DefaultXceConnector();

    @Override
    public Connection getWriteConnection(String bizName, String bizRoute) throws SQLException {

        if (bizRoute == null) {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting write connection [" + bizName + ']');
            }

            return XceAdapter.getInstance().getWriteConnection(bizName);

        } else {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting write connection [" + bizName + '.' + bizRoute + ']');
            }

            return XceAdapter.getInstance().getWriteConnection(bizName, bizRoute);
        }
    }

    @Override
    public Connection getReadConnection(String bizName, String bizRoute) throws SQLException {

        if (bizRoute == null) {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting read connection [" + bizName + ']');
            }

            return XceAdapter.getInstance().getReadConnection(bizName);

        } else {

            if (logger.isDebugEnabled()) {
                logger.debug("Getting read connection [" + bizName + '.' + bizRoute + ']');
            }

            return XceAdapter.getInstance().getReadConnection(bizName, bizRoute);
        }
    }

    /**
     * 返回: DefaultXceConnector 的实例。
     * 
     * @return DefaultXceConnector 的实例
     */
    public static DefaultXceConnector getInstance() {
        return instance;
    }
}
