/**
 * xiaonei-reg
 * com.xiaonei.reg.register.logic.pretreatment.validate.base
 * BaseRegFormLogicTest.java
 * Sep 7, 2009 - 3:09:29 PM
 */
package com.xiaonei.reg.register.logic.pretreatment.validate.base;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.form.RegCommForm;
import com.xiaonei.reg.register.form.RegDogForm;
import com.xiaonei.reg.register.form.RegEduForm;
import com.xiaonei.reg.register.form.RegIDCommonForm;
import com.xiaonei.reg.register.form.RegInviteForm;
import com.xiaonei.reg.register.form.RegProfilePageForm;
import com.xiaonei.reg.register.form.RegPureIDCommonForm;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.RegShortInviteForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;

/**
 * BaseRegFormLogicTest
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 3:09:29 PM
 * 
 */
public class BaseRegFormLogicTest {

	BaseRegFormLogic test;
//	BaseRegForm regForm;
	IAccType accType;
	Object[] o = { new RegCommForm(), new RegDogForm(), new RegEduForm(),
			new RegIDCommonForm(), new RegInviteForm(),
			new RegProfilePageForm(), new RegPureIDCommonForm(),
			new RegShortCommonForm(), new RegShortInviteForm() };

	/**
	 * setUp
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 3:09:29 PM
	 */
	@Before
	public void setUp() throws Exception {
		test = BaseRegFormLogic.getInstance();
//		regForm = new RegProfilePageForm();
		accType = new RegProfilePageForm();
	}

	/**
	 * tearDown
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 3:09:29 PM
	 */
	@After
	public void tearDown() throws Exception {
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegFormLogic#asImplements(java.lang.Object, java.lang.Class)}
	 * .
	 */
	@Test
	public void testAsImplements() {
		for (Object regForm : o) {
			System.out.println(regForm.getClass().getName());
			assertTrue(test.asImplements(regForm, IAccType.class));
			assertFalse(test.asImplements(regForm, Integer.class));
		}
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegFormLogic#coverInterface(java.lang.Object, java.lang.Class)}
	 * .
	 */
	@Test
	public void testCoverInterface() {
		for (Object regForm : o) {
			try {
				assertTrue(test.coverInterface(regForm, IAccType.class) instanceof IAccType);
			} catch (Exception e) {
				e.printStackTrace();
				fail();
			}
			try {
				assertFalse(test.coverInterface(regForm, Integer.class) instanceof IAccType);
			} catch (Exception e) {
				assertTrue(true);
			}
		}
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegFormLogic#getRegAccount(com.xiaonei.reg.register.form.base.BaseRegForm)}
	 * .
	 */
	@Test
	public void testGetRegAccountBaseRegForm() {
		// fail("Not yet implemented");
	}

	@Test
	public void testGetRegAccountIAccType() {
		accType.setAccType(IAccountType.EMAIL);
		assertNull(test.getRegAccount(accType));
	}

	@Test
	public void testGetRegAccountIAccType2() {
		accType.setAccType(IAccountType.XID);
		assertNull(test.getRegAccount(accType));
	}

	@Test
	public void testGetRegAccountIAccType1() {
		accType.setAccType(IAccountType.EMAIL);
		accType.setRegEmail("hello@hello.test");
		accType.setNicknameId("hello");
		assertEquals("hello@hello.test", test.getRegAccount(accType));
	}

	@Test
	public void testGetRegAccountIAccType3() {
		accType.setAccType(IAccountType.XID);
		accType.setRegEmail("hello@hello.test");
		accType.setNicknameId("hello");
		assertEquals("hello", test.getRegAccount(accType));
	}

}
