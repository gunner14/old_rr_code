package com.renren.entries.model.query;

public class QValue {

    public static final int STRING = 0;

    public static final int INTEGER = 1;

    public static final int DOUBLE = 2;

    private String field;

    private String value1;

    private String value2;

    private int type = STRING;

    public QValue() {
    }

    public QValue(String field, Object value1) {
        this.setField(field);
        this.setValue1(value1.toString());
        if (value1 instanceof Double || value1 instanceof Float) {
            this.setType(DOUBLE);
        } else if (value1 instanceof Number) {
            this.setType(INTEGER);
        }
    }

    public QValue(String field, Object value1, Object value2) {
        this.setField(field);
        if (value1.getClass() != value2.getClass()) {
            throw new IllegalArgumentException(value1.getClass().getName() + ", "
                    + value2.getClass().getName());
        }
        this.setValue1(value1.toString());
        this.setValue2(value2.toString());
        if (value1 instanceof Double || value1 instanceof Float) {
            this.setType(DOUBLE);
        } else if (value1 instanceof Number) {
            this.setType(INTEGER);
        }
    }

    public String getField() {
        return field;
    }

    public void setField(String field) {
        this.field = field;
    }

    public String getValue1() {
        return value1;
    }

    public void setValue1(String value1) {
        this.value1 = value1;
    }

    public String getValue2() {
        return value2;
    }

    public void setValue2(String value2) {
        this.value2 = value2;
    }

    public void setType(int type) {
        this.type = type;
    }

    public boolean isNumber() {
        return type != STRING;
    }

    public boolean isInteger() {
        return type == INTEGER;
    }

    public boolean isDouble() {
        return type == DOUBLE;
    }

    public boolean isString() {
        return type == STRING;
    }

    public Object convertValue1() {
        switch (type) {
            case INTEGER:
                return Long.parseLong(value1);
            case DOUBLE:
                return Double.parseDouble(value1);
            default:
                return value1;
        }
    }

    public Object convertValue2() {
        switch (type) {
            case INTEGER:
                return Long.parseLong(value2);
            case DOUBLE:
                return Double.parseDouble(value2);
            default:
                return value2;
        }
    }

    @Override
    public String toString() {
        if (value2 == null) {
            return field + ":" + value1.toString();
        } else {
            return field + ":" + value1 + "-" + value2;
        }
    }
}
