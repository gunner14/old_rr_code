package com.renren.entries.core.query;

import java.util.ArrayList;
import java.util.List;
import java.util.RandomAccess;

public class Query {

    private ArrayList<Field> fields = new ArrayList<Field>();

    private Field typeField;

    private Value createTime;

    public void addField(Field field) {
        if (field.getName().equals("type")) {
            typeField = field;
        } else if (field.getName().equals("create_time")) {
            if (field.getValues().size() != 1) {
                throw new IllegalArgumentException(
                        "the create_time field do not suports 'or', or size is "
                                + field.getValues().size());
            }
            Value createTime = field.getValues().get(0);
            // TODO： 应该要支持单边的比如“－123456”， “567890－”这样的
            if (createTime.getValue2() == null) {
                throw new IllegalArgumentException("illegal create_time field query: " + field);
            }
            this.createTime = createTime;
        } else {
            fields.add(field);
        }
    }

    /**
     * 获取除type之外的，被加入的fields
     * 
     * @return shold be a {@link RandomAccess} list
     */
    public List<Field> getFields() {
        return fields;
    }

    public Field getTypeField() {
        return typeField;
    }

    public Value getCreateTime() {
        return createTime;
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
