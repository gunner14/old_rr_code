package com.renren.entrybase.mysql.model;

import java.util.ArrayList;
import java.util.List;

import javax.annotation.PostConstruct;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.renren.entrybase.model.Field;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class FieldConstraints {

    private static Log logger = LogFactory.getLog(FieldConstraints.class);

    /**
     * 主散字段名称，默认为null表示按照entry的id散
     * <p>
     * 如果非null，在新增entry时侯必须提供该字段的值进来
     */
    private String shardField;

    /** 可以索引的字段名字，需要通过外部配置进来 */
    private String[] indexFields;

    /** 必须索引的字段名字，默认无 */
    private String[] forceIndexFields = new String[0];

    /**
     * 设置entry的散表字段名，即依据entry的哪个field来做散表。
     * <p>
     * 如果没有设置散表字段名，则默认就是根据id来散。
     * 
     * @param shardField
     */
    public void setShardField(String shardField) {
        logger.info("set shard field as " + shardField);
        if ("id".equals(shardField) || "entryId".equals(shardField)) {
            shardField = null;
        }
        this.shardField = shardField;
    }

    public String getShardField() {
        return shardField;
    }

    public String[] getIndexFields() {
        return indexFields;
    }

    public String[] getForceIndexFields() {
        return forceIndexFields;
    }

    // 将shardField调整到最前面－－这样是否好的还有待证明
    @PostConstruct
    public void postConstruct() {
        if (shardField != null) {
            boolean index = supportsIndex(shardField);
            if (!index) {
                String[] indexFields = new String[this.indexFields.length + 1];
                indexFields[0] = shardField;
                System.arraycopy(this.indexFields, 0, indexFields, 1, this.indexFields.length);
                this.indexFields = indexFields;
            } else {
                for (int i = 0; i < this.indexFields.length; i++) {
                    if (this.indexFields[i].equals(shardField)) {
                        this.indexFields[i] = this.indexFields[0];
                        this.indexFields[0] = shardField;
                        break;
                    }
                }
            }
        }
    }

    /**
     * 设置可支持索引的字段名称
     * 
     * @param indexFields
     */
    public void setIndexFields(String[] indexFields) {
        List<String> indexes = new ArrayList<String>(indexFields.length);
        List<String> forces = new ArrayList<String>(indexFields.length);
        for (int i = 0; i < indexFields.length; i++) {
            String field = indexFields[i].trim();
            // 两个减号之后的表示忽略
            if (field.equals("--")) {
                break;
            }
            // 加!后缀表示强制索引
            if (field.endsWith("!")) {
                field = field.substring(0, field.length() - 1);
                forces.add(field);
            }
            indexes.add(field);
        }
        this.indexFields = indexes.toArray(new String[0]);
        this.forceIndexFields = forces.toArray(new String[0]);
    }

    /**
     * 是否支持对给定的字段进行索引查找？
     * 
     * @param field
     * @return
     */
    public boolean supportsIndex(String field) {
        return ArrayUtils.contains(indexFields, field);
    }

    /**
     * 是否支持对给定的字段进行索引查找？
     * 
     * @param field
     * @return
     */
    public boolean forceIndex(String field) {
        return ArrayUtils.contains(forceIndexFields, field);
    }

    /**
     * 
     * @param field
     * @return
     */
    public boolean getIndex(Field field) {
        String name = field.getName();
        if (forceIndex(name)) {
            return true;
        }
        if (field.getIndex() == Field.INDEX_DEFAULT) {
            return supportsIndex(name);
        }
        return field.getIndex(false);
    }
}
