package com.renren.entries.model;

import org.codehaus.jackson.annotate.JsonIgnore;

/**
 * {@link Field} 代表一个作为 {@link Entry} 部分的字段
 * <p>
 * 
 * @author qieqie.wang@gmail.com
 * @author yanghe.liang@renren-inc.com
 * @author weibo.li@renren-inc.com
 */
public class Field implements Cloneable {

    public static final int INDEX_DEFAULT = 0;

    public static final int INDEX_TRUE = 1;

    public static final int INDEX_FALSE = 2;

    /** 字段名称 */
    private String name;

    /** 字段值 */
    private String value;

    /** 是否要索引,大部分情况下请保持默认，让服务器来决定 */
    private int index = INDEX_DEFAULT;

    //-----------------------------

    /**
     * 构建一个还未设置名称及其值的字段
     * 
     * @see #setName(String)
     * @see #setValue(String)
     */
    public Field() {
    }

    /**
     * 构建一个设置了名称及其值的字段
     * 
     * @param name
     * @param value
     */
    public Field(String name, Object value) {
        this(name, value, INDEX_DEFAULT);
    }

    /**
     * 
     * @param name
     * @param value
     * @param index
     */
    public Field(String name, Object value, Boolean index) {
        this.setName(name);
        this.setValue(value);
        if (index != null) {
            this.setIndex(index ? INDEX_TRUE : INDEX_FALSE);
        }
    }

    /**
     * 
     * @param name
     * @param value
     * @param index
     */
    public Field(String name, Object value, int index) {
        this.setName(name);
        this.setValue(value);
        this.setIndex(index);
    }

    /**
     * 返回字段的名称
     * 
     * @return
     */
    public String getName() {
        return name;
    }

    /**
     * 设置字段的名称
     * 
     * @param name
     * @return
     * @throws IllegalArgumentException 如果字段名称非法时
     */
    public Field setName(String name) {
        if (name == null || name.length() == 0) {
            throw new IllegalArgumentException(//
                    "Field name can not be null or empty.");
        }
        for (int i = 0; i < name.length(); i++) {
            char c = name.charAt(i);
            if (!(c == '_' || c == '.' || (c >= 'a' && c <= 'z'))) {
                // 非法字符
                throw new IllegalArgumentException(//
                        "Field name contains illegal charactor: '" //
                                + c + "' in \"" + name + "\"");
            }
        }
        //
        this.name = name;
        return this;
    }

    /**
     * 判断字段是否是以这个给定的名称命名
     * 
     * @param name
     * @return
     */
    public boolean isNamed(String name) {
        return this.name != null && this.name.equals(name);
    }

    /**
     * 返回设置的索引标志
     * 
     * @return 返回null代表还未设置或由服务器来决定是否索引
     */
    public int getIndex() {
        return index;
    }

    /**
     * 返回设置的索引标志
     * 
     * @param valueIfDefault 如果INDEX标识为INDEX_DEFAULT返回该值
     * @return
     */
    public boolean getIndex(boolean valueIfDefault) {
        return index == INDEX_DEFAULT ? valueIfDefault : index == INDEX_TRUE;
    }

    /**
     * 设置是否索引标志
     * <p>
     * 
     * @param index 设置null代表还未设置或由服务器来决定是否索引
     * @return
     */
    public Field setIndex(int index) {
        if (index != INDEX_DEFAULT && index != INDEX_TRUE && index != INDEX_FALSE) {
            throw new IllegalArgumentException("Invalid index value '" + index + "'");
        }
        this.index = index;
        return this;
    }

    /**
     * 返回该字段的值
     * 
     * @return
     */
    public String getValue() {
        return value;
    }

    /**
     * 设置该字段的值
     * <p>
     * 当您更新一个条目 {@link Entry} 时，如果设置一个字段的值为null，表示要删除该字段
     * 
     * @param value
     * @return
     */
    public Field setValue(String value) {
        this.value = value;
        return this;
    }

    /**
     * 调用给用参数(非空时)的toString方法，并设置为该字段的值
     * <p>
     * 当您更新一个条目 {@link Entry} 时，如果设置一个字段的值为null，表示要删除该字段
     * 
     * @param value
     * @return
     */
    public Field setValue(Object value) {
        this.setValue(value == null ? null : value.toString());
        return this;
    }

    @JsonIgnore
    public boolean hasValue() {
        return value != null;
    }

    @Override
    public Field clone() {
        return new Field(name, value, index);
    }

    /**
     * 将字段值转化为 {@link Long} 类型，并返回
     * 
     * @return 如果字段值为null，将返回null
     */
    @JsonIgnore
    public Long getLongValue() {
        if (value == null || value.length() == 0) {
            return null;
        }
        return Long.valueOf(value);
    }

    /**
     * 将字段值转化为 long 类型，并返回
     * 
     * @param defIfNull 如果字段值为null，返回此值
     * @return
     */
    public long getLongValue(long defIfNull) {
        Long value = getLongValue();
        return value == null ? defIfNull : value.longValue();
    }

    @JsonIgnore
    public Integer getIntValue() {
        if (value == null || value.length() == 0) {
            return null;
        }
        return Integer.valueOf(value);
    }

    public int getIntValue(int defIfNull) {
        Integer value = getIntValue();
        return value == null ? defIfNull : value.intValue();
    }

    @JsonIgnore
    public Character getCharValue() {
        if (value == null || value.length() == 0) {
            return null;
        }
        if (value.length() == 1) {
            return value.charAt(0);
        }
        throw new IllegalArgumentException("Unable to convert '" + value + "' to Character.");
    }

    public char getCharValue(char defIfNull) {
        Character value = getCharValue();
        return value == null ? defIfNull : value.charValue();
    }

    @JsonIgnore
    public Boolean getBooleanValue() {
        if (value == null || value.length() == 0) {
            return null;
        }
        return Boolean.valueOf(value);
    }

    public boolean getBooleanValue(boolean defIfNull) {
        Boolean value = getBooleanValue();
        return value == null ? defIfNull : value.booleanValue();
    }

    @JsonIgnore
    public Double getDoubleValue() {
        if (value == null || value.length() == 0) {
            return null;
        }
        return Double.valueOf(value);
    }

    public double getDoubleValue(double defIfNull) {
        Double value = getDoubleValue();
        return value == null ? defIfNull : value.doubleValue();
    }

    @JsonIgnore
    public Float getFloatValue() {
        if (value == null || value.length() == 0) {
            return null;
        }
        return Float.valueOf(value);
    }

    public float getFloatValue(float defIfNull) {
        Float value = getFloatValue();
        return value == null ? defIfNull : value.floatValue();
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Field)) {
            return false;
        }
        Field o = (Field) obj;
        return this.name.equals(o.name) //
                && this.value.equals(o.value);
    }

    @Override
    public int hashCode() {
        return name.hashCode() * 31 + value.hashCode();
    }

    @Override
    public String toString() {
        return name + "='" + value + "'";
    }

}
