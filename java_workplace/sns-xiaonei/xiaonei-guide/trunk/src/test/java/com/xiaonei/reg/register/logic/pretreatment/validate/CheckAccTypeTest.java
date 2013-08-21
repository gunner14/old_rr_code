/**
 * xiaonei-reg
 * com.xiaonei.reg.register.logic.pretreatment.validate
 * CheckAccTypeTest.java
 * Sep 7, 2009 - 3:56:05 PM
 */
package com.xiaonei.reg.register.logic.pretreatment.validate;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.fail;

import java.util.Arrays;
import java.util.Collection;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

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
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegFormLogic;

/**
 * CheckAccTypeTest
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 3:56:05 PM
 * 
 */
@RunWith(Parameterized.class)
public class CheckAccTypeTest {

	CheckAccType test;
	BaseRegForm regForm;
	Integer accType;

	public CheckAccTypeTest(BaseRegForm regForm, Integer accType) {
		this.regForm = regForm;
		this.accType = accType;
	}

	@Parameters
	public static Collection<Object[]> initParam() {
		Object[][] objArray = new Object[][] {
				{ new RegCommForm(), IAccountType.EMAIL },
				{ new RegDogForm(), IAccountType.EMAIL },
				{ new RegEduForm(), IAccountType.EMAIL },
				{ new RegIDCommonForm(), IAccountType.EMAIL },
				{ new RegInviteForm(), IAccountType.EMAIL },
				{ new RegProfilePageForm(), IAccountType.EMAIL },
				{ new RegPureIDCommonForm(), IAccountType.EMAIL },
				{ new RegShortCommonForm(), IAccountType.EMAIL },
				{ new RegShortInviteForm(), IAccountType.EMAIL },
				{ new RegCommForm(), IAccountType.XID },
				{ new RegDogForm(), IAccountType.XID },
				{ new RegEduForm(), IAccountType.XID },
				{ new RegIDCommonForm(), IAccountType.XID },
				{ new RegInviteForm(), IAccountType.XID },
				{ new RegProfilePageForm(), IAccountType.XID },
				{ new RegPureIDCommonForm(), IAccountType.XID },
				{ new RegShortCommonForm(), IAccountType.XID },
				{ new RegShortInviteForm(), IAccountType.XID } };

		return Arrays.asList(objArray);
	}

	/**
	 * setUp
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 3:56:05 PM
	 */
	@Before
	public void setUp() throws Exception {
		test = new CheckAccType();
		regForm = new RegShortCommonForm();
	}

	/**
	 * tearDown
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 3:56:05 PM
	 */
	@After
	public void tearDown() throws Exception {
		test = null;
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.logic.pretreatment.validate.CheckAccType#check(com.xiaonei.reg.register.form.base.BaseRegForm)}
	 * .
	 */
	@Test
	public final void testCheck() {
		IAccType iac;
		try {
			iac = (IAccType) BaseRegFormLogic.getInstance().coverInterface(
					regForm, IAccType.class);
		} catch (Exception e) {
			e.printStackTrace();
			fail();
			return;
		}
		iac.setAccType(accType);
		iac.setRegEmail("hafsljfsf@163.com");
		iac.setNicknameId("lkajsfsfs");
		assertNotNull(test.check(regForm));
	}

}
