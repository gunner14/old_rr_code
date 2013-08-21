package com.xiaonei.commons.jade.impl.xceadapter.datasource.junit;

import junit.framework.Test;
import junit.framework.TestSuite;

/**
 * 运行所有的测试用例。
 * 
 * @author han.liao
 */
public class AllTests extends TestSuite {

    /**
     * 创建相应的测试。
     * 
     * @return 相应的测试
     */
    public static Test suite() {

        TestSuite suite = new TestSuite(
                "Test for com.xiaonei.commons.jade.impl.xceadapter.datasource");

        //$JUnit-BEGIN$
        suite.addTestSuite(MysqlParserTests.class);
        suite.addTestSuite(SqlRewriterTests.class);
        suite.addTestSuite(SqlTokenizerTests.class);
        //$JUnit-END$

        return suite;
    }
}
