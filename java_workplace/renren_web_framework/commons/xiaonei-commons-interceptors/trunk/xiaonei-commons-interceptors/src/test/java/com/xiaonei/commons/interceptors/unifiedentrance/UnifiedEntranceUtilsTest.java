package com.xiaonei.commons.interceptors.unifiedentrance;

import org.junit.Assert;
import org.junit.Test;

public class UnifiedEntranceUtilsTest {

    @Test
    public void test1() throws Exception {
        Assert.assertEquals("https://blog.d:8080/hello/java?foo=bar", UnifiedEntranceUtils
                .urlhash("https://blog.d.renren.com:8080/hello/java?foo=bar"));
    }

    @Test
    public void test2() throws Exception {
        Assert.assertEquals("//blog/home?foo=bar", UnifiedEntranceUtils
                .urlhash("http://blog.renren.com/home?foo=bar"));
    }
}
