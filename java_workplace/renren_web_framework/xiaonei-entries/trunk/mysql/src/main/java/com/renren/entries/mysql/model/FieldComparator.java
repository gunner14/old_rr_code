package com.renren.entries.mysql.model;

import java.util.Comparator;

import com.renren.entries.model.query.QField;

/**
 * 用于对Field进行排序
 * 
 * @author qieqie.wang
 * 
 */
public class FieldComparator implements Comparator<QField> {

    private String[] fields;

    public FieldComparator(String... fields) {
        this.fields = fields;
    }

    @Override
    public int compare(QField o1, QField o2) {
        if (o1 == o2) {
            return 0;
        }
        String name1 = o1.getName();
        String name2 = o2.getName();
        int index1 = indexOf(name1);
        int index2 = indexOf(name2);
        if (index1 == -1) {
            if (index2 != -1) {
                index1 = Integer.MAX_VALUE;
            } else {
                return name1.compareTo(name2);
            }
        } else if (index2 == -1) {
            index2 = Integer.MAX_VALUE;
        }
        // index大的后面，所以返回1
        if (index1 > index2) {
            return 1;
        } else if (index1 < index2) {
            return -1;
        }
        return 0;
    }

    private int indexOf(String name) {
        for (int i = 0; i < fields.length; i++) {
            if (name.equals(fields[i])) {
                return i;
            }
        }
        return -1;
    }

}