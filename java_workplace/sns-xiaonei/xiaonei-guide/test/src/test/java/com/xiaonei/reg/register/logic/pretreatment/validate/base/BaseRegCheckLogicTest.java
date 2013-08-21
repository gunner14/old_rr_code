/**
 * xiaonei-reg
 * com.xiaonei.reg.register.logic.pretreatment.validate.base
 * BaseRegCheckLogicTest.java
 * Sep 7, 2009 - 8:36:50 PM
 */
package com.xiaonei.reg.register.logic.pretreatment.validate.base;

import static org.junit.Assert.*;

import java.lang.reflect.InvocationTargetException;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.form.base.BaseRegForm;

/**
 * BaseRegCheckLogicTest
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 8:36:50 PM
 * 
 */
public class BaseRegCheckLogicTest {

	BaseRegCheckLogic test;

	@BeforeClass
	public static void setUpBeforeClass() throws Exception {

	}

	/**
	 * setUp
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 8:36:51 PM
	 */
	@Before
	public void setUp() throws Exception {
		test = new BaseRegCheckLogic() {
			@Override
			public String check(BaseRegForm regForm) {
				return null;
			}
		};
	}

	/**
	 * tearDown
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 8:36:51 PM
	 */
	@After
	public void tearDown() throws Exception {
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic#asImplements(java.lang.Object, java.lang.Class)}
	 * .
	 */
	@Ignore
	@Test
	public final void testAsImplements() {
		fail("Not yet implemented");
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic#coverInterface(java.lang.Object, java.lang.Class)}
	 * .
	 */
	@Ignore
	@Test
	public final void testCoverInterface() {
		fail("Not yet implemented");
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic#isOk(java.lang.String)}
	 * .
	 * 
	 * @throws NoSuchMethodException
	 * @throws SecurityException
	 * @throws InvocationTargetException
	 * @throws IllegalAccessException
	 * @throws InstantiationException
	 * @throws IllegalArgumentException
	 */
	@Ignore
	@Test
	public final void testIsOk() throws SecurityException, NoSuchMethodException, IllegalArgumentException, InstantiationException, IllegalAccessException, InvocationTargetException {
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic#isNotOk(java.lang.String)}
	 * .
	 */
	@Test
	public final void testIsNotOk() {
		assertFalse(test.isNotOk(IRegCheckCodeDef.OK));
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic#getRegAccount(com.xiaonei.reg.register.form.base.BaseRegForm)}
	 * .
	 */
	@Ignore
	@Test
	public final void testGetRegAccountBaseRegForm() {
		fail("Not yet implemented"); // 
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic#getRegAccount(com.xiaonei.reg.register.form.base.userganerate.IAccType)}
	 * .
	 */
	@Ignore
	@Test
	public final void testGetRegAccountIAccType() {
		fail("Not yet implemented");
	}

}
