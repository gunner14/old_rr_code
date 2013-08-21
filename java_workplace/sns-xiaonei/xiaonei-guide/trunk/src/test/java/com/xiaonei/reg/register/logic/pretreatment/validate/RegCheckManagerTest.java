package com.xiaonei.reg.register.logic.pretreatment.validate;

import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import com.xiaonei.reg.register.constants.IRegCheckCodeDef;
import com.xiaonei.reg.register.form.RegForm;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.RegShortInviteForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;

@RunWith(Parameterized.class)
public class RegCheckManagerTest {
	RegCheckManager test;
	Class<? extends BaseRegForm> form;
	List<BaseRegCheckLogic> expecteds;

	public RegCheckManagerTest(Class<? extends BaseRegForm> form,
			List<BaseRegCheckLogic> expecteds) {
		super();
		this.form = form;
		this.expecteds = expecteds;
	}

	@Parameters
	public static Collection<Object[]> initParam() {
		List<BaseRegCheckLogic> l = new ArrayList<BaseRegCheckLogic>();
		l.add(new CheckAccType());
		l.add(new CheckGender());
		l.add(new CheckName());
		l.add(new CheckPwd());
		l.add(new CheckStage());
		l.add(new CheckUniversity());
		l.add(new CheckWorkplace());
		l.add(new CheckMiddleschool());
		l.add(new CheckHomecity());
		l.add(new CheckInvite());

		List<BaseRegCheckLogic> lRegShortCommonForm = new ArrayList<BaseRegCheckLogic>();
		lRegShortCommonForm.add(new CheckAccType());
		lRegShortCommonForm.add(new CheckPwd());
		lRegShortCommonForm.add(new CheckName());
		lRegShortCommonForm.add(new CheckGender());
		lRegShortCommonForm.add(new CheckBirthday());
		
		List<BaseRegCheckLogic> lRegShortInviteForm = new ArrayList<BaseRegCheckLogic>();
		lRegShortInviteForm.add(new CheckAccType());
		lRegShortInviteForm.add(new CheckPwd());
		lRegShortInviteForm.add(new CheckName());
		lRegShortInviteForm.add(new CheckGender());
		lRegShortInviteForm.add(new CheckInvite());
		lRegShortInviteForm.add(new CheckBirthday());
		
		
		Object[][] objArray = new Object[][] { 
				{ RegForm.class, l },
				{ RegShortCommonForm.class,lRegShortCommonForm},
				{ RegShortInviteForm.class,lRegShortInviteForm},
				};
		return Arrays.asList(objArray);
	}

	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception {
	}

	@Before
	public void setUp() throws Exception {
		test = RegCheckManager.getInstance();
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	@Ignore
	public final void testGetChecker() {
		List<BaseRegCheckLogic> checker = test.getChecker(form);
		assertSame(expecteds.toArray().length, checker.toArray().length);
		for (int i = 0; i < checker.size(); i++) {
			assertSame(expecteds.get(i).getClass(), checker.get(i).getClass());
		}
	}

	@Test
	public final void testGetErrorValue() {
		assertEquals("abcdefg",test.getErrorValue("abcdefg"));
	}

	@Test
	public final void testIsOK() {
		assertTrue(test.isOK(IRegCheckCodeDef.OK));
	}

}
