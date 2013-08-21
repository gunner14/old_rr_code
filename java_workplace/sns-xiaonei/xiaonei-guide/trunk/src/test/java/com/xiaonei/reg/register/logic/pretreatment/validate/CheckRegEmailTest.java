/**
 * xiaonei-reg
 * com.xiaonei.reg.register.logic.pretreatment.validate
 * CheckRegEmailTest.java
 * Sep 7, 2009 - 4:43:22 PM
 */
package com.xiaonei.reg.register.logic.pretreatment.validate;

import static org.junit.Assert.*;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;


import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

/**
 * CheckRegEmailTest
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 4:43:22 PM
 * 
 */
public class CheckRegEmailTest {
	CheckRegEmail test;

	/**
	 * setUp
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 4:43:22 PM
	 */
	@Before
	public void setUp() throws Exception {
		test = new CheckRegEmail();
	}

	/**
	 * tearDown
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 4:43:22 PM
	 */
	@After
	public void tearDown() throws Exception {
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.CheckRegEmail#check(com.xiaonei.reg.register.form.base.BaseRegForm)}
	 * .
	 */
	@Test
	public final void testCheckBaseRegForm() {
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.CheckRegEmail#check(java.lang.String, int)}
	 * .
	 */
	@Test
	public final void testCheckStringInt() {
	}

	@Test
	public final void teatCheckEmailDomainIsValid() throws SecurityException,
			NoSuchMethodException, IllegalArgumentException,
			IllegalAccessException, InvocationTargetException {
		Method m = CheckRegEmail.class.getDeclaredMethod(
				"checkEmailDomainIsValid", String.class);
		m.setAccessible(true);
		assertTrue((Boolean) m.invoke(test, "hello@163.com"));
	}
	
	@Ignore
	@Test
	public final void teatCheckEmailDomainIsValid1() throws SecurityException,
	NoSuchMethodException, IllegalArgumentException,
	IllegalAccessException, InvocationTargetException {
		Method m = CheckRegEmail.class.getDeclaredMethod(
				"checkEmailDomainIsValid", String.class);
		m.setAccessible(true);
		assertFalse((Boolean) m.invoke(test, "hello@163.co0m"));
	}
}
