package com.renren.entries.core;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.ArrayUtils;

import com.renren.entries.model.Field;

/**
 * 
 * @author qieqie.wang@gmail.com
 * 
 */
public class FieldConstraints {

    /** 可以索引的字段名字，需要通过外部配置进来 */
    private String[] indexFields;

    /** 必须索引的字段名字，默认无 */
    private String[] forceIndexFields = new String[0];

    public String[] getIndexFields() {
        return indexFields;
    }

    public String[] getForceIndexFields() {
        return forceIndexFields;
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
