package com.renren.ad.sns.services.antispam.ahocorasick;

import junit.framework.*;

public class TestAll extends TestCase {
    public static Test suite() {
	TestSuite suite = new TestSuite();
	suite.addTestSuite(TestState.class);
	suite.addTestSuite(TestAhoCorasick.class);
	suite.addTestSuite(TestQueue.class);
	return suite;
    }
}
