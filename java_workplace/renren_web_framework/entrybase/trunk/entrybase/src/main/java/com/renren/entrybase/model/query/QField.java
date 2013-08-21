package com.renren.entrybase.model.query;

import java.util.ArrayList;
import java.util.List;

public class QField {

    private String name;

    private List<QValue> values = new ArrayList<QValue>(4);

    public QField() {
    }

    public QField(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void addValue(QValue orNode) {
        values.add(orNode);
    }

    public List<QValue> getValues() {
        return values;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(name);
        sb.append(":(");
        int orcount = 0;
        for (QValue orNode : getValues()) {
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
