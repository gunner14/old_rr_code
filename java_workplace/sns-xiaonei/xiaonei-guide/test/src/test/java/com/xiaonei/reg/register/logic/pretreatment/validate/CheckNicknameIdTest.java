/**
 * xiaonei-reg
 * com.xiaonei.reg.register.logic.pretreatment.validate
 * CheckNicknameIdTest.java
 * Sep 7, 2009 - 8:13:05 PM
 */
package com.xiaonei.reg.register.logic.pretreatment.validate;


import static org.junit.Assert.assertNotNull;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

/**
 * CheckNicknameIdTest
 *
 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 8:13:05 PM
 *
 */
public class CheckNicknameIdTest {
	
	CheckNicknameId test;

	/**
	 * setUp 
	 *
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 8:13:05 PM
	 */
	@Before
	public void setUp() throws Exception {
		test = new CheckNicknameId();
	}

	/**
	 * tearDown 
	 *
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 8:13:05 PM
	 */
	@After
	public void tearDown() throws Exception {
	}

	/**
	 * testCheckXId 
	 *
	 * @param xId
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 9:12:04 PM
	 */
	@Test
	public void testCheckXId() {
		assertNotNull(test.checkXId("asdfasf"));
	}
}
