package com.renren.entries.mysql;

import net.paoding.rose.jade.context.application.JadeFactory;
import net.paoding.rose.jade.dataaccess.DataSourceFactory;
import net.paoding.rose.jade.statement.Interpreter;

import org.springframework.util.Assert;

import com.renren.entries.mysql.dao.EntryDAO;
import com.renren.entries.mysql.dao.IndexDAO;
import com.renren.entries.mysql.dao.ShardDAO;
import com.renren.entries.mysql.dao.supports.DefaultSeqDAO;
import com.renren.entries.mysql.dao.supports.PartitionSQLInterpreter;
import com.renren.entries.mysql.model.FieldConstraints;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class EntryBaseBuilder {

    private DataSourceFactory dataSourceFactory;

    private FieldConstraints fieldConstraints;

    private PartitionSQLInterpreter partitionInterpreter = new PartitionSQLInterpreter();

    public EntryBaseBuilder setDataSource(DataSourceFactory dataSourceFactory) {
        this.dataSourceFactory = dataSourceFactory;
        return this;
    }

    /**
     * 设置主表散列数量
     * 
     * @param size
     * @return
     */
    public EntryBaseBuilder setEntryPartitions(int size) {
        partitionInterpreter.setEntryPartitions(size);
        return this;
    }

    /**
     * 设置各个索引的散表数量
     * 
     * @param indexName
     * @param size
     * @return
     */
    public EntryBaseBuilder setIndexPartitions(String indexName, int size) {
        partitionInterpreter.setIndexPartition(indexName, size);
        return this;
    }

    /**
     * 设置主表被散列的字段以及其它被索引的字段
     * 
     * @param entryShardField
     * @param indexFields
     * @return
     */
    public EntryBaseBuilder setIndexFields(String entryShardField, String[] indexFields) {
        this.fieldConstraints = new FieldConstraints();
        this.fieldConstraints.setShardField(entryShardField);
        this.fieldConstraints.setIndexFields(indexFields);
        this.fieldConstraints.postConstruct();
        return this;
    }

    public MySQLEntryBase build() {
        // validation
        Assert.notNull(fieldConstraints);
        Assert.notNull(dataSourceFactory);

        // create the dao
        JadeFactory jade = new JadeFactory();
        jade.addInterpreter(new Interpreter[] { partitionInterpreter });
        jade.setDataSourceFactory(dataSourceFactory);
        EntryDAO entryDAO = jade.create(EntryDAO.class);
        IndexDAO indexDAO = jade.create(IndexDAO.class);
        ShardDAO shardDAO = jade.create(ShardDAO.class);

        // build 
        MySQLEntryBase base = new MySQLEntryBase();
        base.setEntryDAO(entryDAO);
        base.setIndexDAO(indexDAO);
        base.setShardDAO(shardDAO);
        base.setSeqDAO(new DefaultSeqDAO());
        base.setFieldConstraints(fieldConstraints);
        base.postConstruct();
        return base;
    }

}
