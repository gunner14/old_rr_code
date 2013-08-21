package com.renren.entries.core.query;

import java.util.ArrayList;
import java.util.List;

public class Query {

    private ArrayList<Field> fields = new ArrayList<Field>();

    private Field typeField;

    public void addField(Field field) {
        if (field.getName().equals("type")) {
            typeField = field;
        } else {
            fields.add(field);
        }
    }

    /**
     * 获取除type之外的，被加入的fields
     * 
     * @return
     */
    public List<Field> getFields() {
        return fields;
    }

    public Field getTypeField() {
        return typeField;
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (Field field : fields) {
            appendField(sb, field);
        }
        if (typeField != null) {
            appendField(sb, typeField);
        }
        sb.append(";");
        return sb.toString();
    }

    private void appendField(StringBuilder sb, Field andNode) {
        if (sb.length() > 0) {
            sb.append(" and ");
        }
        sb.append(andNode.getName());
        sb.append(":(");
        int orcount = 0;
        for (Value v : andNode.getValues()) {
            if (orcount++ > 0) {
                sb.append(" or ");
            }
            if (v.isString()) {
                sb.append("\"").append(v.getValue1()).append("\"");// v.getValue1可能含有引号，不做特别处理!
            } else {
                sb.append(v.getValue1());
                if (v.getValue2() != null) {
                    sb.append("-");
                    sb.append(v.getValue2());
                }
            }
        }
        sb.append(")");
    }

}
