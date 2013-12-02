package org.apache.mahout.fpm.pfpgrowth.extra;

import org.apache.hadoop.io.Writable;

public class ArrayWritable<E> extends org.apache.hadoop.io.ArrayWritable {

    public ArrayWritable(Class<? extends Writable> valueClass) {
        super(valueClass);
    }

    public ArrayWritable(Class<? extends Writable> valueClass, Writable[] values) {
        super(valueClass, values);
    }

    @Override
    public String toString() {
        StringBuffer result = new StringBuffer("[");
        String sep = "";
        Writable[] values = get();
        for (int i = 0; i < values.length; i++) {
            result.append(sep + values[i].toString());
            sep = ", ";
        }
        result.append("]");
        return result.toString();
    }
}
