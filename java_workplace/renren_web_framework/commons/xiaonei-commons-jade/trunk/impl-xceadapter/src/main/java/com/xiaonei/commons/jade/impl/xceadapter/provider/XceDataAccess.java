package com.xiaonei.commons.jade.impl.xceadapter.provider;

import java.util.List;
import java.util.Map;

import net.paoding.rose.jade.jadeinterface.annotation.ShardParam;
import net.paoding.rose.jade.jadeinterface.annotation.UseMaster;
import net.paoding.rose.jade.jadeinterface.exql.ExprResolver;
import net.paoding.rose.jade.jadeinterface.exql.impl.ExprResolverImpl;
import net.paoding.rose.jade.jadeinterface.provider.DataAccess;
import net.paoding.rose.jade.jadeinterface.provider.Modifier;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.dao.InvalidDataAccessApiUsageException;
import org.springframework.jdbc.core.RowMapper;

import com.meidusa.amoeba.parser.dbobject.Column;
import com.xiaonei.commons.jade.impl.util.ThreadUtils;
import com.xiaonei.commons.jade.impl.xceadapter.datasource.XceParseInfo;

/**
 * 提供 xceadapter 版本的
 * {@link net.paoding.rose.jade.jadeinterface.provider.DataAccess} 实现。
 * 
 * @author han.liao
 */
public class XceDataAccess implements DataAccess {

    private static final Log logger = LogFactory.getLog(XceDataAccess.class);

    private final DataAccess dataAccess;

    public XceDataAccess(DataAccess dataAccess) {
        this.dataAccess = dataAccess;
    }

    @Override
    public List<?> select(String sql, Modifier modifier, Map<String, ?> parameters,
            RowMapper rowMapper) {

        try {
            processAnnotations(modifier, parameters);

            return dataAccess.select(sql, modifier, parameters, rowMapper);

        } finally {
            ThreadUtils.clearAll();
        }
    }

    @Override
    public int update(String sql, Modifier modifier, Map<String, ?> parameters) {

        try {
            processAnnotations(modifier, parameters);

            return dataAccess.update(sql, modifier, parameters);

        } finally {
            ThreadUtils.clearAll();
        }
    }

    @Override
    public Number insertReturnId(String sql, Modifier modifier, Map<String, ?> parameters) {

        try {
            processAnnotations(modifier, parameters);

            return dataAccess.insertReturnId(sql, modifier, parameters);

        } finally {
            ThreadUtils.clearAll();
        }
    }

    protected void processAnnotations(Modifier modifier, Map<String, ?> parameters) {

        UseMaster masterIndicator = modifier.getAnnotation(UseMaster.class);
        if (masterIndicator != null) {
            ThreadUtils.setForceWrite(masterIndicator.value());
        }

        ShardParam shardParam = modifier.getAnnotation(ShardParam.class);
        if (shardParam != null) {

            Column column = XceParseInfo.newColumn(shardParam.name());
            Object value = null;

            try {
                ExprResolver exprResolver = new ExprResolverImpl(parameters);
                value = exprResolver.executeExpr(shardParam.value());

            } catch (Exception e) {

                if (logger.isWarnEnabled()) {
                    logger.warn("Can't execute @ShardParam expr: " + shardParam.value(), e);
                }

                throw new InvalidDataAccessApiUsageException("Can't execute @ShardParam expr: "
                        + shardParam.value(), e);
            }

            ThreadUtils.setShardColumn(column);
            ThreadUtils.setShardParam(value);
        }
    }
}
