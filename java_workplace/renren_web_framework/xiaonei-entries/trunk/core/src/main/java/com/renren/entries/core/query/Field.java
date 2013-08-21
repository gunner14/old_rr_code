package com.renren.entries.core.query;

import java.util.ArrayList;
import java.util.List;

public class Field {

    private String name;

    private List<Value> values = new ArrayList<Value>(4);

    public Field() {
    }

    public Field(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void addValue(Value orNode) {
        values.add(orNode);
    }

    public List<Value> getValues() {
        return values;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(name);
        sb.append(":(");
        int orcount = 0;
        for (Value orNode : getValues()) {
            if (orcount++ > 0) {
                sb.append(" OR ");
            }
            sb.append(orNode.getValue1());
            if (orNode.getValue2() != null) {
                sb.append("-");
                sb.append(orNode.getValue2());
            }
        }
        sb.append(")");
        return sb.toString();
    }

}
