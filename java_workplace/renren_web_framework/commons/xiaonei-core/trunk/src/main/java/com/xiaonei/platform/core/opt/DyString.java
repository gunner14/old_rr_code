/*
 * OPI All Rights Reselved.
 */
package com.xiaonei.platform.core.opt;

/**
 * 用于表示可以在多域名环境下使用的地址字符串。
 * <p>
 * 用于集合的要求<br>
 * 不可用用于Set， HashMap， Hashtable等环境中，建议只用在LinkedList, ArrayList存放
 * 
 * @author zhiliang.wang
 * 
 */
public abstract class DyString implements CharSequence {

    /**
     * 返回调用时刻，当前线程的地址或域名字符串
     */
    public abstract String toString();

    @Override
    public int hashCode() {
        return toString().hashCode();
    }

    public boolean equalsIgnoreCase(String str) {
        return toString().equalsIgnoreCase(str);
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (!(obj instanceof String) && !(obj instanceof DyString)) {
            return false;
        }
        return toString().equals(obj.toString());
    }

    @Override
    public char charAt(int index) {
        return toString().charAt(index);
    }

    @Override
    public int length() {
        return toString().length();
    }

    @Override
    public CharSequence subSequence(int start, int end) {
        return toString().subSequence(start, end);
    }
}
