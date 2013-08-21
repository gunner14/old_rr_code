package com.xiaonei.commons.gid.util;

import junit.framework.Assert;

import org.junit.Test;

/**
 * @see GID
 * @author 王志亮 zhiliang.wang@opi-corp.com
 * 
 */
public class GTest {

    /**
     * 类型编号
     */
    long[] typeValues = { 0x100, 0x110, 0x120, 0x130, 0x140, 0x150, 0x160, 0x170, 0x180, 0x300,
            0x310, 0x320, 0x330, 0x340, 0x350, 0x360, 0x370, 0x380, 0x390, 0x3A0, 0x3B0, 0x3C0 };

    /**
     * 将整型转化为统一ID
     */
    @Test
    public void testGid() {
        // PERSON
        Assert.assertSame(Type.PERSON, G.extractType(1001));
        Assert.assertTrue(G.isTypeOf(1001, Type.PERSON));
        Assert.assertEquals(1001, G.extractX(1001));

        // !=PERSON的
        long[] _values = { 1001, (1L << 44) - 1 }; // (1L << 44) - 1= 8796093022208L 八千亿
        for (int i = 0; i < _values.length; i++) {
            long x = _values[i];
            for (int j = 0; j < this.typeValues.length; j++) {
                if (typeValues[j] != Type.PERSON.value && typeValues[j] != Type.PAGE.value
                        && typeValues[j] != Type.ORG.value
                        && typeValues[j] != Type.MINI_GROUP.value
                        && typeValues[j] != Type.XIAOZHAN.value) {
                    Type type = Type.toType(this.typeValues[j]);
                    long gid = G.id(x, type); // 八千亿
                    Assert.assertSame(type, G.extractType(gid));
                    Assert.assertTrue(G.isTypeOf(gid, type));
                    Assert.assertEquals(_values[i], G.extractX(gid));
                }
            }
        }
    }

    /**
     * 非法的转化
     */
    @Test
    public void testWrongXid() {
        long[] _values = { -1, 1L << 44 };
        for (int i = 0; i < _values.length; i++) {
            long x = _values[i];
            try {
                G.id(x, Type.ALBUM);
                Assert.assertNull("程序不应该执行到此的", false);
            } catch (Exception e) {
                Assert.assertEquals(e.getClass(), IllegalArgumentException.class);
            }
        }
    }
}
