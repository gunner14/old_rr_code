package com.renren.entries.model;

import java.util.Comparator;

/**
 * 用于对Field安名称进行排序
 * 
 * @author qieqie.wang
 * 
 */
public class FieldComparator implements Comparator<Field> {

    private String[] fields;

    public FieldComparator(String... fields) {
        this.fields = fields;
    }

    @Override
    public int compare(Field o1, Field o2) {
        if (o1 == o2) {
            return 0;
        }
        String name1 = o1.getName();
        String name2 = o2.getName();
        // "."的靠前返回-1
        if (name1.startsWith(".")) {
            if (name2.startsWith(".")) {
                return name1.compareTo(name2);
            } else {
                return -1;
            }
        } else if (name2.startsWith(".")) {
            return 1;
        }
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
