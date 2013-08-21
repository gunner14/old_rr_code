package com.xiaonei.commons.gid.util;

import junit.framework.Assert;

import org.junit.Test;

/**
 * @see Type
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class TypeTest {

    /**
     * 类型名
     */
    String[] names = { "person", "page", "xiaozhan", "xiaozu", "group", "mini_group", "event", "application", "org",
            "comment", "status", "blog", "photo", "video", "album", "share", "vote", "gossip",
            "message", "request", "tag", "place" };

    /**
     * 类型编号
     */
    long[] typeValues = { 0x100, 0x110, 0x120, 0x130, 0x140, 0x150, 0x160, 0x170, 0x180, 0x300, 0x310, 0x320, 0x330, 0x340,
            0x350, 0x360, 0x370, 0x380, 0x390, 0x3A0, 0x3B0, 0x3C0 };

    /**
     * 通过值定义获取Type是否正确？
     */
    @Test
    public void testToTypeByValue() {
        for (int i = 0; i < typeValues.length; i++) {
            Type type1 = Type.toType(typeValues[i]);
            Type type2 = Type.toType("0x" + typeValues[i]);
            Assert.assertEquals(typeValues[i], type1.value);
            Assert.assertEquals(typeValues[i], type2.value);
            Assert.assertSame(type1, type2);
        }
    }

    /**
     * 通过名称获取Type是否正确？
     */
    @Test
    public void testToTypeByName() {
        for (int i = 0; i < names.length; i++) {
            Type type = Type.toType(names[i]);
            Assert.assertEquals(typeValues[i], type.value);
        }
    }

    /**
     * toString是否正确？
     */
    @Test
    public void testToString() {
        for (int i = 0; i < typeValues.length; i++) {
            Type type = Type.toType(names[i]);
            Assert.assertEquals(names[i].toUpperCase(), type.toString());
        }
    }

    /**
     * hashCode是否正确
     */
    @Test
    public void testHashCode() {
        for (int i = 0; i < typeValues.length; i++) {
            Type type = Type.toType(names[i]);
            Assert.assertEquals(typeValues[i], type.hashCode());
        }
    }

    /**
     * 是否保持唯一实例，且正确实现equals方法
     */
    @Test
    public void testEquals() {
        for (int i = 0; i < typeValues.length; i++) {
            Type type1 = Type.toType(names[i]);
            Type type2 = Type.toType(names[i]);
            Assert.assertEquals(type1, type2);
            Assert.assertSame(type1, type2);
        }
    }
}
