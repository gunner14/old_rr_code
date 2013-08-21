package com.renren.entrybase.mysql;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Map;

import javax.sql.DataSource;

import net.paoding.rose.jade.dataaccess.DataSourceFactory;
import net.paoding.rose.jade.statement.StatementMetaData;

import org.springframework.jdbc.datasource.SingleConnectionDataSource;

import com.renren.entrybase.mysql.dao.supports.PartitionSQLInterpreter;

public class CommentCenterDataSourceFactory implements DataSourceFactory {

    SingleConnectionDataSource[] entryDataSources;

    public void destroy() {
        for (SingleConnectionDataSource dataSource : entryDataSources) {
            if (dataSource != null) dataSource.destroy();
        }
    }

    public CommentCenterDataSourceFactory() {
        try {
            entryDataSources = new SingleConnectionDataSource[10];
            for (int i = 0; i < entryDataSources.length; i++) {
                SingleConnectionDataSource dataSource = new SingleConnectionDataSource();
                dataSource.setUrl("jdbc:hsqldb:mem:comment_center_node_" + Math.random());
                dataSource.setDriverClassName("org.hsqldb.jdbc.JDBCDriver");
                Connection conn = dataSource.getConnection();
                Statement st = conn.createStatement();
                entryDataSources[i] = dataSource;
                int from = i;
                int to = (i + 1);
                createEntryNode(st, from, to);
                createIndexAuthorNode(st, from, to);
                createIndexShardNode(st, from, to);
                st.close();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    @Override
    public DataSource getDataSource(StatementMetaData metaData,
            Map<String, Object> runtimeProperties) {
        long shardValue = (Long) runtimeProperties.get(PartitionSQLInterpreter.SHARD_VALUE);
        int shard = (int) ((shardValue % 10));
        return entryDataSources[shard];
    }

    protected void createEntryNode(Statement st, int from, int to) throws SQLException {
        for (int i = from; i < to; i++) {
            // create table entry
            String ddl = "CREATE TABLE entry_" + i + " (entry_seq bigint NOT NULL PRIMARY KEY ,"
                    + "entry_id bigint NOT NULL," + "shard_value bigint NOT NULL,"
                    + "create_time datetime NOT NULL," + "optimism_lock int,"
                    + "info varchar(10000) NOT NULL," + "body varchar(10000)" + ") ";
            st.execute(ddl);
        }
    }

    protected void createIndexShardNode(Statement st, int from, int to) throws SQLException {
        for (int i = from; i < to; i++) {
            // create table index_shard
            String ddl = "CREATE TABLE index_shard_" + i + " ("
                    + "entry_seq bigint NOT NULL PRIMARY KEY," + "entry_id bigint NOT NULL,"
                    + "entry_shard_value bigint NOT NULL" + ");";
            st.execute(ddl);
        }
    }

    protected void createIndexAuthorNode(Statement st, int from, int to) throws SQLException {
        for (int i = from; i < to; i++) {
            // create table index_shard
            String ddl = "CREATE TABLE index_author_" + i + " (" + " author bigint NOT NULL,"
                    + " entry_id bigint NOT NULL," + " entry_seq bigint NOT NULL" + " );";
            st.execute(ddl);
        }
    }

}
