package com.renren.entries.xoa.api;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.xiaonei.commons.gid.util.SubType;
import com.xiaonei.commons.gid.util.Type;

/**
 * 
 * @author qieqie.wang
 * 
 */
public class Query {

    public static void main(String[] args) {
        // author:2342 and master:2434245;
        System.out.println(new Query().add(Name.AUTHOR, 2342).add(Name.MASTER, 2434245));

        // author:(2342 or 4234234) and master:2434245;
        System.out.println(new Query().add(Name.AUTHOR, 2342, 4234234).add("master", 2434245));

        int[] uids = new int[] { 1, 2, 3 };
        System.out.println(new Query().add(Name.AUTHOR, uids).add(Name.TYPE, Type.VIDEO));

        double[] uids2 = new double[] { 1.2, 2.44, 3.14 };
        System.out.println(new Query().add(Name.AUTHOR, uids2).add(Name.TYPE, Type.VIDEO));

    }

    private List<String> fieldNames = new ArrayList<String>(4);

    private Map<String, Object[]> fields = new HashMap<String, Object[]>();

    private String toString;

    public Query add(String name, Collection<?> values) {
        Object[] array = values.toArray();
        return add(name, array);
    }

    public Query add(String name, Object... values) {
        if (values.length == 0) {
            throw new IllegalArgumentException("empty values");
        }
        if (values[0].getClass().isArray()) { // 如果传入的是java基本类型的数组，如int[],long[]等
            if (values.length > 1) {
                throw new IllegalArgumentException("it's not allowed more than one array here");
            }
            int len = Array.getLength(values[0]);
            Object[] objects = new Object[len];
            for (int i = 0; i < objects.length; i++) {
                // 这里会把基本数据类型，自动装箱为相应的对象
                objects[i] = Array.get(values[0], i);
            }
            values = objects;
        }
        if (name.equals("type")) {
            if (values.length != 1) {
                throw new IllegalArgumentException(//
                        "wrong type value length: " + values.length + " " + Arrays.toString(values));
            }
            if (values[0] != null && !(values[0] instanceof Type)) {
                throw new IllegalArgumentException("wrong class for 'type' query: "
                        + values[0].getClass());
            }
        } else if (name.equals("subtype")) {
            if (values.length != 1) {
                throw new IllegalArgumentException(//
                        "wrong subtype value length: " + values.length + " "
                                + Arrays.toString(values));
            }
            if (!fields.containsKey("type")) {
                throw new IllegalArgumentException("use type before subtype");
            }
            if (values[0] != null && !(values[0] instanceof SubType)) {
                throw new IllegalArgumentException("wrong class for 'subtype' query: "
                        + values[0].getClass());
            }
        }
        if (fields.containsKey(name)) {
            throw new IllegalArgumentException("replicated field: " + name);
        }
        this.toString = null;
        this.fields.put(name, values);
        this.fieldNames.add(name);
        return this;
    }

    public String build() {
        return toString();
    }

    public String toString() {
        if (toString == null) {
            StringBuilder sb = new StringBuilder();
            for (String andNode : fieldNames) {
                if (sb.length() > 0) {
                    sb.append(" and ");
                }
                sb.append(andNode);
                Object[] values = fields.get(andNode);
                if (values.length > 1) {
                    sb.append(":(");
                } else {
                    sb.append(":");
                }
                int orcount = 0;
                for (Object value : values) {
                    if (value == null) {
                        // 不支持is null操作
                        continue;
                    }
                    if (orcount++ > 0) {
                        sb.append(" or ");
                    }
                    sb.append(getFileValueString(value));
                }
                if (values.length > 1) {
                    sb.append(")");
                }
            }
            sb.append(";");
            return toString = sb.toString();
        }
        return toString;
    }

    private String getFileValueString(Object value) {
        if (value instanceof Number) {
            return String.valueOf(value);
        } else {
            return "\"" + value + "\""; // value可能也含有引号，不做特殊处理！
        }
    }
}
