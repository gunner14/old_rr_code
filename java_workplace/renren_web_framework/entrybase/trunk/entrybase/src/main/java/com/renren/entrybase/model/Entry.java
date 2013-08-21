package com.renren.entrybase.model;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Pattern;

/**
 * {@link Entry} 代表一个可被本系统存储的对象，它由一个全局唯一的 ID 以及若干字段 {@link Field} 组成。
 * <p>
 * {@link Entry}是可扩展字段的，我们制定了一个“字段命名规范”，您在使用前一定要阅读这个Wiki： <a href=
 * "http://project.d.xiaonei.com/projects/infrastructure-entries/wiki/%E5%AD%97%E6%AE%B5%E5%91%BD%E5%90%8D%E8%A7%84%E8%8C%83"
 * >字段命名规范</a>
 * 
 * @author qieqie.wang@gmail.com
 * @author yanghe.liang@renren-inc.com
 * @author weibo.leo@gmail.com
 */

public class Entry implements Cloneable, Comparable<Entry> {

    /** 全局唯一ID，由 entrybase 的使用者提供，但必须保证唯一 */
    private long id;

    /** 全局唯一的序列，每个entry的sequence都不一样，由entrybase系统内部自动生成 */
    private long sequence;

    /** 条目的组成字段 */
    private List<Field> fields;

    public Entry() {
    }

    public Entry(long id, Field... fields) {
        setId(id);
        ArrayList<Field> fs = new ArrayList<Field>(fields.length + 4);
        fs.addAll(Arrays.asList(fields));
        setFields(fs);
    }

    public Entry(long id, List<Field> fields) {
        setId(id);
        setFields(fields);
    }

    /**
     * 返回条目的ID
     * 
     * @return
     */
    public long getId() {
        return id;
    }

    /**
     * 设置条目的ID
     * 
     * @param id
     */
    public void setId(long id) {
        this.id = id;
    }

    /**
     * 返回条目在信息聚合系统内部的序列号
     * 
     * @return
     */
    public long getSequence() {
        return sequence;
    }

    /**
     * 设置条目在信息聚合系统中的序列号（由信息聚合系统自己创建）
     * 
     * @param sequence
     */
    public void setSequence(long sequence) {
        this.sequence = sequence;
    }

    /**
     * 返回条目的所有字段，如果该条目没有任何字段，则将返回一个长度为0的数组
     * <p>
     * 
     * @return
     */
    @SuppressWarnings("unchecked")
    public List<Field> getFields() {
        return (List<Field>) (fields == null ? Collections.emptyList() : fields);
    }

    /**
     * 设置该条目的字段，如果原先已有设置，则将进行替换
     * <p>
     * 可以为一个 {@link Entry}对象设置多个相同名称的字段以表示可拥有多个值
     * 
     * @param fields
     */
    public void setFields(List<Field> fields) {
        this.fields = fields;
    }

    /**
     * 返回给定名称的第一个字段，如果无则返回null
     * 
     * @param name
     * @return
     */
    public Field getField(String name) {
        if (name == null) {
            throw new IllegalArgumentException("name");
        }
        for (Field f : getFields()) {
            if (f == null) {
                continue;
            }
            if (name.equals(f.getName())) {
                return f;
            }
        }
        return null;
    }

    /**
     * 返回给定名称并且其值与传入参数(toString后的值)相同的字段，如果无则返回null
     * 
     * @param name
     * @param value 可以为null，表示返回给定名称的字段并且其值为null的
     * @return
     */
    public Field getField(String name, Object value) {
        if (name == null) {
            throw new IllegalArgumentException("name");
        }
        String valueString = value == null ? null : value.toString();
        for (Field f : getFields()) {
            if (f == null) {
                continue;
            }
            if (name.equals(f.getName())) {
                if (valueString == null) {
                    if (f.getValue() == null) {
                        return f;
                    }
                } else if (valueString.equals(f.getValue())) {
                    return f;
                }
            }
        }
        return null;
    }

    public <T> T getFieldValue(String name, Class<T> clazz) {
        return getFieldValue(name, clazz, null);
    }

    @SuppressWarnings("unchecked")
    public <T> T getFieldValue(String name, Class<T> clazz, T valueIfNull) {
        String v = getFieldValue(name);
        if (v == null) {
            return valueIfNull;
        }
        if (clazz == String.class) {
            return (T) v;
        }
        if (clazz == Long.class || clazz == long.class) {
            return (T) Long.valueOf(v);
        }
        if (clazz == Integer.class || clazz == int.class) {
            return (T) Integer.valueOf(v);
        }
        if (clazz == Double.class || clazz == double.class) {
            return (T) Double.valueOf(v);
        }
        if (clazz == Float.class || clazz == float.class) {
            return (T) Float.valueOf(v);
        }
        if (clazz == Boolean.class || clazz == boolean.class) {
            return (T) Boolean.valueOf(v);
        }
        if (clazz == Byte.class || clazz == byte.class) {
            return (T) Byte.valueOf(v);
        }
        if (clazz == Character.class || clazz == char.class) {
            return (T) (v.length() == 0 ? "" : v.charAt(0));
        }
        throw new IllegalArgumentException("wrong type: " + clazz.getName());
    }

    public String getFieldValue(String name) {
        Field f = getField(name);
        return f == null ? null : f.getValue();
    }

    /**
     * 返回给定名称的所有字段，如无则返回一个长度为0的列表
     * 
     * @param name
     * @return
     */
    @SuppressWarnings("unchecked")
    public List<Field> getFields(String name) {
        if (name == null) {
            throw new IllegalArgumentException("name");
        }
        ArrayList<Field> fields = null;
        for (Field f : getFields()) {
            if (name.equals(f.getName())) {
                if (fields == null) {
                    fields = new ArrayList<Field>(8);
                }
                fields.add(f);
            }
        }
        return (List<Field>) (fields == null ? Collections.emptyList() : fields);
    }

    @SuppressWarnings("unchecked")
    public List<String> getFieldValues(String name) {
        ArrayList<String> fieldValues = null;
        for (Field f : getFields(name)) {
            if (f.hasValue()) {
                if (fieldValues == null) {
                    fieldValues = new ArrayList<String>(8);
                }
                fieldValues.add(f.getValue());
            }
        }
        return (List<String>) (fieldValues == null ? Collections.emptyList() : fieldValues);
    }

    public boolean hasFieldValue(String name, Object value) {
        value = String.valueOf(value);
        for (Field f : fields) {
            if (f.hasValue() && f.getName().equals(name) && f.getValue().equals(value)) {
                return true;
            }
        }
        return false;
    }

    /**
     * 返回给定名称的字段的个数
     * 
     * @param name
     * @return
     */
    public int countFields(String name) {
        int count = 0;
        for (Field f : getFields()) {
            if (name == null) {
                if (f.getName() == null) {
                    count++;
                }
            } else if (name.equals(f.getName())) {
                count++;
            }
        }
        return count;
    }

    /**
     * 替换掉给定名称的字段，替换的字段没有要求其名称和被替换的名称一样
     * 
     * @param name
     * @param newFiled
     * @return 被替换的
     */
    public Field replaceField(String name, Field newFiled) {
        List<Field> fields = getFields();
        for (int i = 0; i < fields.size(); i++) {
            Field f = fields.get(i);
            if (f.getName().equals(name)) {
                fields.set(i, newFiled);
                return f;
            }
        }
        return null;
    }

    public int removeFields(String name) {
        int deleted = 0;
        if (fields != null) {
            for (Iterator<Field> iter = fields.iterator(); iter.hasNext();) {
                Field f = iter.next();
                if (f != null && name.equals(f.getName())) {
                    iter.remove();
                    deleted++;
                }
            }
        }
        return deleted;
    }

    /**
     * 
     * @param field
     */
    public synchronized void addField(Field field) {
        if (field == null) {
            throw new NullPointerException("field");
        }
        if (fields == null) {
            synchronized (this) {
                if (fields == null) {
                    fields = new LinkedList<Field>();
                }
            }
        }
        this.fields.add(field);
    }

    /**
     * 
     * @param name
     * @param value
     * @return
     */
    public Field addField(String name, Object value) {
        Field field = new Field(name, value);
        if (fields == null) {
            synchronized (this) {
                if (fields == null) {
                    fields = new LinkedList<Field>();
                }
            }
        }
        this.fields.add(field);
        return field;
    }

    /**
     * 
     * @param name
     * @param value
     */
    public void setField(String name, Object value) {
        removeFields(name);
        addField(name, value);
    }

    @Override
    public int compareTo(Entry o) {
        if (sequence < o.sequence) {
            return 1;
        }
        if (sequence == o.sequence) {
            return 0;
        }
        return -1;
    }

    @Override
    public Entry clone() {
        List<Field> fs = new ArrayList<Field>(this.getFields().size());
        for (Field field : getFields()) {
            if (field != null) {
                fs.add(field.clone());
            }
        }
        return new Entry(id, fs);
    }

    @Override
    public String toString() {
        return this.id + "[" + toInfoString(fields) + "]";
    }

    @Override
    public int hashCode() {
        return new Long(id).hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Entry)) {
            return false;
        }
        Entry o = (Entry) obj;
        return this.id == o.id;
    }

    /**
     * 
     * @return
     */
    public String toInfoString() {
        return toInfoString(getFields());
    }

    private static final Pattern PATTERN_AND = Pattern.compile("&");

    private static final Pattern PATTERN_EQUALS = Pattern.compile("=");

    /**
     * 将info信息串(key1=value1&key2=value2)解析为 {@link Field} 数组
     * 
     * @param info
     * @return
     */
    public static final List<Field> toFields(String info) {
        if (info == null || info.length() == 0) {
            return Collections.emptyList();
        }
        String[] fieldTokens = PATTERN_AND.split(info);
        List<Field> fields = new ArrayList<Field>(fieldTokens.length + 4);
        for (int i = 0; i < fieldTokens.length; i++) {
            fields.add(toField(fieldTokens[i]));
        }
        return fields;
    }

    public static String toInfoString(Field... fields) {
        return toInfoString(Arrays.asList(fields));
    }

    /**
     * 将多个Field拼接成一个info串(key1=value1&key2=value2)
     * <p>
     * 
     * @param fields
     * @return
     */
    public static String toInfoString(List<Field> fields) {
        if (fields == null) {
            return "";
        }
        int length = fields.size() * 3 + 20;
        for (Field field : fields) {
            if (field == null || field.getName() == null || field.getValue() == null) {
                continue;
            }
            length += field.getName().length();
            length += field.getValue().length();
        }
        //
        StringBuilder info = new StringBuilder(length);
        for (Field field : fields) {
            if (field == null || field.getName() == null || field.getValue() == null
                    || field.getName().equals("body") || field.getName().equals("optimism_lock")) {
                continue;
            }
            String name = field.getName();
            String value = field.getValue();
            //
            if (info.length() > 0) {
                info.append("&"); //field之间的分隔符
            }
            info.append(name);
            info.append("="); //name=value之间的分隔符
            info.append(FieldEncoder.encodeField(value));
            if (field.getIndex(false)) { //要求被索引
                info.append("$");
            }
        }
        return info.toString();
    }

    /**
     * 
     * @param fieldToken
     * @return
     */
    private static Field toField(String fieldToken) {
        String name;
        String value;
        String[] nameAndValue = PATTERN_EQUALS.split(fieldToken);
        if (nameAndValue.length == 2) {
            name = nameAndValue[0];
            value = nameAndValue[1];
        } else {
            name = nameAndValue[0];
            value = "";
        }
        int index = Field.INDEX_FALSE;
        if (value.endsWith("$")) {
            index = Field.INDEX_TRUE;
            value = value.substring(0, value.length() - "$".length());
        }
        if (!"body".equals(name)) {
            // 解码
            value = FieldEncoder.decodeField(value);
        }
        return new Field(name, value, index);
    }

}
