package com.renren.entrybase.mysql.dao.supports;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import net.paoding.rose.jade.statement.Interpreter;
import net.paoding.rose.jade.statement.StatementRuntime;

import org.springframework.core.annotation.Order;

import com.renren.entrybase.mysql.dao.EntryDAO;
import com.renren.entrybase.mysql.dao.IndexDAO;
import com.renren.entrybase.mysql.dao.ShardDAO;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
@Order(-100)
public class PartitionSQLInterpreter implements Interpreter {

    public static final String TABLE = PartitionSQLInterpreter.class + "#table";

    public static final String SHARD_VALUE = PartitionSQLInterpreter.class + "#shard_value";

    private String entryShardField;

    private int entryPartitions;

    private String indexCreateTimePattern;

    private Map<String, Integer> indexPartitions = new HashMap<String, Integer>();

    public void setEntryShardField(String entryShardField) {
        this.entryShardField = entryShardField;
    }

    public void setEntryPartitions(int entryPatitions) {
        this.entryPartitions = entryPatitions;
    }

    public void setIndexPartitions(Map<String, Integer> indexPartitions) {
        this.indexPartitions = indexPartitions;
    }

    public void setIndexPartition(String indexName, int size) {
        if (this.indexPartitions == null) {
            this.indexPartitions = new HashMap<String, Integer>();
        }
        this.indexPartitions.put(indexName, size);
    }

    public void setIndexCreateTimePattern(String indexCreateTimePattern) {
        this.indexCreateTimePattern = indexCreateTimePattern;
    }

    @Override
    public void interpret(StatementRuntime runtime) {
        Class<?> daoClass = runtime.getMetaData().getDAOMetaData().getDAOClass();

        // 实体表
        if (EntryDAO.class == daoClass) {
            interpretEntryDAO(runtime);
        }

        // shard字典表(保存entry->shardValue关系)
        else if (ShardDAO.class == daoClass) {
            interpretShardDAO(runtime);
        }

        // 索引表：此时包含了entry表（从某种角度主体表也是一索引表，一种特殊的索引表)
        else if (IndexDAO.class == daoClass) {
            String field = (String) runtime.getParameters().get("field");
            Object fieldValue = runtime.getParameters().get("fieldValue");
            if (fieldValue == null) {
                throw new NullPointerException("fieldValue is required");
            }
            // entry实体表
            if (field == null || (entryShardField != null && entryShardField.equals(field))) {
                interpretIndexDAOAsEntryTable(runtime, fieldValue);
                return;
            }
            // create_time
            else if ("create_time".equals(field)) {
                interpretIndexDAOAsIndexCreateTimeTable(runtime, field, fieldValue);
            }
            // long
            else if (fieldValue instanceof Long || fieldValue instanceof String) {
                fieldValue = interpretGenericIndexDAO(runtime, field, fieldValue);
            } else {
                throw new IllegalArgumentException("wrong " + field + " type "
                        + fieldValue.getClass().getName());
            }
        } // end of IndexDAO

    }

    private Object interpretGenericIndexDAO(StatementRuntime runtime, String field,
            Object fieldValue) {
        if (fieldValue instanceof String) {
            fieldValue = Long.parseLong(fieldValue.toString());
        }
        String table;
        Integer p = indexPartitions.get(field);
        if (p == null) {
            p = indexPartitions.get("default");
        }
        if (p == null || p.intValue() <= 0) {
            table = "index_" + field;
        } else {
            table = "index_" + field + "_" + ((Long) fieldValue).longValue() % p.intValue();
            runtime.setProperty(SHARD_VALUE, fieldValue);
        }
        String sql = runtime.getSQL();
        sql = sql.replace("${INDEX_TABLE}", table);
        sql = sql.replace("${INDEX_NAME}", field);
        sql = sql.replace("${INDEX_SELECT_COLUMNS}", "entry_id, entry_seq");
        runtime.setSQL(sql);
        runtime.setProperty(TABLE, "index_" + field);
        return fieldValue;
    }

    private void interpretIndexDAOAsIndexCreateTimeTable(StatementRuntime runtime, String field,
            Object fieldValue) {
        String table = "index_create_time";
        if (indexCreateTimePattern != null) {
            SimpleDateFormat sdf = new SimpleDateFormat(indexCreateTimePattern);
            Date createTime;
            if (fieldValue instanceof Date) {
                createTime = (Date) fieldValue;
            } else {
                createTime = new Date((Long) fieldValue);
            }
            String suffix = sdf.format(createTime);
            table = "index_create_time_" + suffix;
            runtime.setProperty(SHARD_VALUE, createTime.getTime());
        } else {
            table = "index_create_time";
        }
        String sql = runtime.getSQL();
        sql = sql.replace("${INDEX_TABLE}", table);
        sql = sql.replace("${INDEX_NAME}", field);
        sql = sql.replace("${INDEX_SELECT_COLUMNS}", "DISTINCT entry_id, entry_seq");
        runtime.setSQL(sql);
        runtime.setProperty(TABLE, "index_create_time");
    }

    private void interpretIndexDAOAsEntryTable(StatementRuntime runtime, Object fieldValue) {
        String table;
        if (entryPartitions > 0) {
            table = "entry_" + ((Long) fieldValue).longValue() % entryPartitions;
        } else {
            table = "entry";
        }
        String sql = runtime.getSQL();
        sql = sql.replace("${INDEX_TABLE}", table);
        sql = sql.replace("${INDEX_NAME}", "shard_value");
        sql = sql.replace("${INDEX_SELECT_COLUMNS}",
                "entry_seq, entry_id, shard_value, create_time, info, body, optimism_lock");
        runtime.setSQL(sql);
        runtime.setProperty(TABLE, "entry");
        runtime.setProperty(SHARD_VALUE, fieldValue);
    }

    private void interpretShardDAO(StatementRuntime runtime) {
        if (entryPartitions > 0) {
            Long shardValue = (Long) runtime.getParameters().get("entryId");
            long partition = shardValue.longValue() % entryPartitions;
            String table = "index_shard_" + partition;
            runtime.setSQL(runtime.getSQL().replace("${INDEX_SHARD_TABLE}", table));
            runtime.setProperty(SHARD_VALUE, shardValue);
        } else {
            runtime.setSQL(runtime.getSQL().replace("${INDEX_SHARD_TABLE}", "index_shard"));
        }
        runtime.setProperty(TABLE, "index_shard");
    }

    private void interpretEntryDAO(StatementRuntime runtime) {
        if (entryPartitions > 0) {
            Long shardValue = (Long) runtime.getParameters().get("shardValue");
            long partition = shardValue.longValue() % entryPartitions;
            String table = "entry_" + partition;
            runtime.setSQL(runtime.getSQL().replace("${ENTRY_TABLE}", table));
            runtime.setProperty(SHARD_VALUE, shardValue);
        } else {
            runtime.setSQL(runtime.getSQL().replace("${ENTRY_TABLE}", "entry"));
        }
        runtime.setProperty(TABLE, "entry");
    }

}
