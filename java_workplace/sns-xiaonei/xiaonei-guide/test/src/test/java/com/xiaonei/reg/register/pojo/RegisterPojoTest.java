/**
 * xiaonei-reg
 * com.xiaonei.reg.register.pojo
 * RegisterPojoTest.java
 * Sep 7, 2009 - 9:17:42 PM
 */
package com.xiaonei.reg.register.pojo;

import static org.junit.Assert.*;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.Collection;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;

import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.form.RegEduForm;
import com.xiaonei.reg.register.form.RegForm;
import com.xiaonei.reg.register.form.RegIDCommonForm;
import com.xiaonei.reg.register.form.RegProfilePageForm;
import com.xiaonei.reg.register.form.RegPureIDCommonForm;
import com.xiaonei.reg.register.form.RegShortCommonForm;
import com.xiaonei.reg.register.form.RegShortInviteForm;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;
import com.xiaonei.reg.register.form.base.userganerate.IAudit;
import com.xiaonei.reg.register.form.base.userganerate.IBirthday;
import com.xiaonei.reg.register.form.base.userganerate.IGender;
import com.xiaonei.reg.register.form.base.userganerate.IHeadupload;
import com.xiaonei.reg.register.form.base.userganerate.IHomecity;
import com.xiaonei.reg.register.form.base.userganerate.IInvite;
import com.xiaonei.reg.register.form.base.userganerate.IMiddleschool;
import com.xiaonei.reg.register.form.base.userganerate.IName;
import com.xiaonei.reg.register.form.base.userganerate.IPwd;
import com.xiaonei.reg.register.form.base.userganerate.IStage;
import com.xiaonei.reg.register.form.base.userganerate.IUniversity;
import com.xiaonei.reg.register.form.base.userganerate.IWorkplace;

/**
 * RegisterPojoTest
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 9:17:42 PM
 * 
 */
@RunWith(Parameterized.class)
public class RegisterPojoTest {

	RegisterPojo test;
	IAccType accType;
	IBirthday birthday;
	IGender gender;
	IName name;
	IPwd pwd;
	IStage stage;
	IUniversity university;
	IWorkplace workplace;
	IMiddleschool middleschool;
	IHomecity homecity;
	IInvite invite;
	IAudit audit;
	IHeadupload headupload;

	public RegisterPojoTest(IAccType accType, IBirthday birthday,
			IGender gender, IName name, IPwd pwd, IStage stage,
			IUniversity university, IWorkplace workpalce,
			IMiddleschool middleschool, IHomecity homecity, IInvite invite,
			IAudit audit, IHeadupload headupload) {
		super();
		this.accType = accType;
		this.birthday = birthday;
		this.gender = gender;
		this.name = name;
		this.pwd = pwd;
		this.stage = stage;
		this.university = university;
		this.workplace = workpalce;
		this.middleschool = middleschool;
		this.homecity = homecity;
		this.invite = invite;
		this.audit = audit;
		this.headupload = headupload;
	}

	@Parameters
	public static Collection<Object[]> initParam() {
		IAccType accType = new IAccType() {
			@Override
			public void setRegEmail(String regEmail) {
			}

			@Override
			public void setNicknameId(String nicknameId) {
			}

			@Override
			public void setAccType(int accType) {
			}

			@Override
			public String getRegEmail() {
				return "hello@junitwt.com";
			}

			@Override
			public String getNicknameId() {
				return "hellojunitwt";
			}

			@Override
			public int getAccType() {
				return IAccountType.EMAIL;
			}
		};
		IBirthday birthday = new IBirthday() {
			@Override
			public void setBirth_year(String birthYear) {
			}

			@Override
			public void setBirth_month(String birthMonth) {
			}

			@Override
			public void setBirth_day(String birthDay) {
			}

			@Override
			public String getBirth_year() {
				return "1984";
			}

			@Override
			public String getBirth_month() {
				return "2";
			}

			@Override
			public String getBirth_day() {
				return "25";
			}
		};
		IGender gender = new IGender() {
			@Override
			public void setGender(String gender) {
			}

			@Override
			public String getGender() {
				return "男生";
			}
		};
		IName name = new IName() {
			@Override
			public void setName(String name) {
			}

			@Override
			public String getName() {
				return "马槽";
			}
		};
		IPwd pwd = new IPwd() {
			@Override
			public void setRepwd(String repwd) {
			}

			@Override
			public void setPwd(String pwd) {
			}

			@Override
			public String getRepwd() {
				return "xiaonei";
			}

			@Override
			public String getPwd() {
				return "renren";
			}
		};
		IStage stage = new IStage() {
			@Override
			public void setStage(int stage) {
			}

			@Override
			public int getStage() {
				return 0;
			}
		};
		IUniversity university = new IUniversity() {
			@Override
			public void setUniversityname(String universityname) {
			}

			@Override
			public void setUniversityid(int universityid) {
			}

			@Override
			public void setTypeOfCourse(String typeOfCourse) {
			}

			@Override
			public String getUniversityname() {
				return "沈阳工业大学";
			}

			@Override
			public int getUniversityid() {
				return 1000001;
			}

			@Override
			public String getTypeOfCourse() {
				return "本科";
			}
		};
		IWorkplace workpalce = new IWorkplace() {
			@Override
			public void setWorkplacename(String workplacename) {
			}

			@Override
			public void setWorkplaceid(String workplaceid) {
			}

			@Override
			public String getWorkplacename() {
				return "千橡";
			}

			@Override
			public String getWorkplaceid() {
				return "1000001";
			}
		};
		IMiddleschool middleschool = new IMiddleschool() {

			@Override
			public void setTechHighSchoolName(String techHighSchoolName) {
			}

			@Override
			public void setTechHighSchoolId(String techHighSchoolId) {
			}

			@Override
			public void setSchoolType(String schoolType) {
			}

			@Override
			public void setJuniorHighSchoolName(String juniorHighSchoolName) {
			}

			@Override
			public void setJuniorHighSchoolId(String juniorHighSchoolId) {
			}

			@Override
			public void setHighSchoolName(String highSchoolName) {
			}

			@Override
			public void setHighSchoolCode(String highSchoolCode) {
			}

			@Override
			public void setHighSchoolClass(String highSchoolClass) {
			}

			@Override
			public void setEnrollYear(int enrollYear) {
			}

			@Override
			public String getTechHighSchoolName() {
				return null;
			}

			@Override
			public String getTechHighSchoolId() {
				return null;
			}

			@Override
			public String getSchoolType() {
				return null;
			}

			@Override
			public String getJuniorHighSchoolName() {
				return null;
			}

			@Override
			public String getJuniorHighSchoolId() {
				return null;
			}

			@Override
			public String getHighSchoolName() {
				return null;
			}

			@Override
			public String getHighSchoolCode() {
				return null;
			}

			@Override
			public String getHighSchoolClass() {
				return null;
			}

			@Override
			public int getEnrollYear() {
				return 0;
			}
		};
		IHomecity homecity = new IHomecity() {
			
			@Override
			public void setHomeprovince(String homeprovince) {
				
			}
			
			@Override
			public void setHomecityname(String homecityname) {
				
			}
			
			@Override
			public void setHomecitycode(String homecitycode) {
				
			}
			
			@Override
			public String getHomeprovince() {
				return "ln";
			}
			
			@Override
			public String getHomecityname() {
				return "01";
			}
			
			@Override
			public String getHomecitycode() {
				return "sy";
			}
		};
		IInvite invite = new IInvite() {

			@Override
			public void setInviterId(int inviterId) {
			}

			@Override
			public void setInviteeEmail(String inviteeEmail) {
			}

			@Override
			public void setInviteType(int inviteType) {
			}

			@Override
			public void setGroupName(String groupName) {
			}

			@Override
			public void setAppToken(String appToken) {
			}

			@Override
			public void setAppId(int appId) {
			}

			@Override
			public int getInviterId() {
				return 888001;
			}

			@Override
			public String getInviteeEmail() {
				return "hello_wang@1444.com";
			}

			@Override
			public int getInviteType() {
				return 0;
			}

			@Override
			public String getGroupName() {
				return "大学同学";
			}

			@Override
			public String getAppToken() {
				return "8801";
			}

			@Override
			public int getAppId() {
				return 21;
			}

            @Override
            public String getAppName() {
                return "开心农场";
            }

            @Override
            public void setAppName(String appName) {                
            }
		};
		IAudit audit = null;
		IHeadupload headupload = null;
		Object[][] objArray = new Object[][] { { accType, birthday, gender,
				name, pwd, stage, university, workpalce, middleschool,
				homecity, invite, audit, headupload }, };
		return Arrays.asList(objArray);
	}

	/**
	 * setUpBeforeClass
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 9:17:42 PM
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
	}

	/**
	 * tearDownAfterClass
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 9:17:42 PM
	 */
	@AfterClass
	public static void tearDownAfterClass() throws Exception {
	}

	/**
	 * setUp
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 9:17:42 PM
	 */
	@Before
	public void setUp() throws Exception {
	}

	/**
	 * tearDown
	 * 
	 * @throws java.lang.Exception
	 * @author wangtai(tai.wang@opi-corp.com) Sep 7, 2009 - 9:17:42 PM
	 */
	@After
	public void tearDown() throws Exception {
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.base.BaseRegForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoBaseRegFormString() {
		BaseRegForm form = new BaseRegForm() {
			private static final long serialVersionUID = 1L;
		} ;
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		test = new RegisterPojo(form, "255.255.255.255");
		assertNotNull(test);
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoRegFormString() {
		RegForm form =  new RegForm();
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		form.setBasicControl(0);
		form.setDepartment("软件学院");
		form.setRegEmail("hell@email.com");
		form.setNicknameId("nicknameid");
		String ip = "255.255.255.255";
		
		form.setAccType(IAccountType.EMAIL);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(0);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(IAccountType.XID);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getNicknameId());
		
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegInviteForm, java.lang.String)}
	 * .
	 */
	@Ignore
	@Test
	public final void testRegisterPojoRegInviteFormString() {
		fail("Not yet implemented"); // 
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegShortInviteForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoRegShortInviteFormString() {
		RegShortInviteForm form =  new RegShortInviteForm();
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		form.setGender("男生");
		form.setIc("icode");
		form.setName("马槽");
		form.setInviteeEmail("inviteeEmail");
		form.setInviterId(9732222);
		form.setInviteType(2);
		form.setAppId(21);
		form.setAppToken("sdfsd-civx-wer");
		form.setGroupName("group");
		form.setRegEmail("hell@email.com");
		form.setNicknameId("nicknameid");
		String ip = "255.255.255.255";
		
		form.setAccType(IAccountType.EMAIL);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(0);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(IAccountType.XID);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getNicknameId());
		
		assertEquals(test.getInviteeEmail(), form.getInviteeEmail());
		assertEquals(test.getInviterId(), form.getInviterId());
		assertEquals(test.getAppId(), form.getAppId());
		assertEquals(test.getAppToken(), form.getAppToken());
		assertEquals(test.getGender(), form.getGender());
		assertEquals(test.getGroupName(),form.getGroupName());
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegIDCommonForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoRegIDCommonFormString() {
		RegIDCommonForm form =  new RegIDCommonForm();
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		form.setGender("男生");
		form.setIc("icode");
		form.setName("马槽");
		form.setRegEmail("hell@email.com");
		form.setNicknameId("nicknameid");
		String ip = "255.255.255.255";
		
		form.setAccType(IAccountType.EMAIL);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(0);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(IAccountType.XID);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getNicknameId());
		
		assertEquals(test.getGender(), form.getGender());
		assertEquals(test.getName(), form.getName());
		assertEquals(test.getPwd(), form.getPwd());
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegPureIDCommonForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoRegPureIDCommonFormString() {
		RegPureIDCommonForm form =  new RegPureIDCommonForm();
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		form.setGender("男生");
		form.setIc("icode");
		form.setName("马槽");
		form.setRegEmail("hell@email.com");
		form.setNicknameId("nicknameid");
		String ip = "255.255.255.255";
		
		form.setAccType(IAccountType.EMAIL);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(0);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(IAccountType.XID);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getNicknameId());
		
		assertEquals(test.getGender(), form.getGender());
		assertEquals(test.getName(), form.getName());
		assertEquals(test.getPwd(), form.getPwd());
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegShortCommonForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoRegShortCommonFormString() {
		RegShortCommonForm form =  new RegShortCommonForm();
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		form.setGender("男生");
		form.setIc("icode");
		form.setName("马槽");
		form.setRegEmail("hell@email.com");
		form.setNicknameId("nicknameid");
		String ip = "255.255.255.255";
		
		form.setAccType(IAccountType.EMAIL);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(0);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(IAccountType.XID);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getNicknameId());
		
		assertEquals(test.getGender(), form.getGender());
		assertEquals(test.getName(), form.getName());
		assertEquals(test.getPwd(), form.getPwd());
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegCommForm, java.lang.String)}
	 * .
	 */
	@Ignore
	@Test
	public final void testRegisterPojoRegCommFormString() {
		fail("Not yet implemented"); // 
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegEduForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoRegEduFormString() {
		RegEduForm form =  new RegEduForm();
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		form.setGender("男生");
		form.setIc("icode");
		form.setName("马槽");
		form.setRegEmail("hell@email.com");
		form.setNicknameId("nicknameid");
		String ip = "255.255.255.255";
		
		form.setAccType(IAccountType.EMAIL);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(0);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(IAccountType.XID);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getNicknameId());
		
		assertEquals(test.getGender(), form.getGender());
		assertEquals(test.getName(), form.getName());
		assertEquals(test.getPwd(), form.getPwd());
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.model.RegAuditHighSchool, com.xiaonei.reg.register.model.RegAppAuditHighSchool)}
	 * .
	 */
	@Ignore
	@Test
	public final void testRegisterPojoRegAuditHighSchoolRegAppAuditHighSchool() {
		fail("Not yet implemented"); // 
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.model.RegMobileWebInfoMapHS)}
	 * .
	 */
	@Ignore
	@Test
	public final void testRegisterPojoRegMobileWebInfoMapHS() {
		fail("Not yet implemented"); // 
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.model.RegAudit, com.xiaonei.reg.register.model.RegAppAudit)}
	 * .
	 */
	@Ignore
	@Test
	public final void testRegisterPojoRegAuditRegAppAudit() {
		fail("Not yet implemented"); // 
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.model.UserAuditMajor)}
	 * .
	 */
	@Ignore
	@Test
	public final void testRegisterPojoUserAuditMajor() {
		fail("Not yet implemented"); // 
	}

	/**
	 * Test method for
	 * {@link com.xiaonei.reg.register.pojo.RegisterPojo#RegisterPojo(com.xiaonei.reg.register.form.RegProfilePageForm, java.lang.String)}
	 * .
	 */
	@Test
	public final void testRegisterPojoRegProfilePageFormString() {
		RegProfilePageForm form =  new RegProfilePageForm();
		form.setAction_id("205205");
		form.setB("b");
		form.setG("g");
		form.setChannel("chanel");
		form.setCode("code");
		form.setFrom("from");
		form.setFromUrl("fromurl");
		form.setIc("ic");
		form.setIu("iu");
		form.setSs("ss");
		form.setUuid("uuid");
		form.setGender("男生");
		form.setIc("icode");
		form.setName("马槽");
		form.setRegEmail("hell@email.com");
		form.setNicknameId("nicknameid");
		form.setInviterId(39423408);
		String ip = "255.255.255.255";
		
		form.setAccType(IAccountType.EMAIL);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(0);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getRegEmail());
		
		form.setAccType(IAccountType.XID);
		test = new RegisterPojo(form, ip);
		assertEquals(test.getRegEmail(), form.getNicknameId());
		
		assertEquals(test.getGender(), form.getGender());
		assertEquals(test.getName(), form.getName());
		assertEquals(test.getPwd(), form.getPwd());
		assertEquals(test.getInviterId(), form.getInviterId());
	}

	@Test
	public final void constructor() throws SecurityException,
			NoSuchMethodException, IllegalArgumentException,
			IllegalAccessException, InvocationTargetException {
		RegisterPojo rp = new RegisterPojo();
		Method constructor = rp.getClass().getDeclaredMethod("constructor",
				IAccType.class, IBirthday.class, IGender.class, IName.class,
				IPwd.class, IStage.class, IUniversity.class, IWorkplace.class,
				IMiddleschool.class, IHomecity.class, IInvite.class,
				IAudit.class, IHeadupload.class, String.class);
		
		constructor.setAccessible(true);

		constructor.invoke(rp, accType, birthday, gender, name, pwd, stage,
				university, workplace, middleschool, homecity, invite, audit,
				headupload, "255,255,255,255");
		
		assertEquals(rp.getRegEmail(), accType.getRegEmail());
		assertEquals(rp.getAppId(),invite.getAppId() );
		assertEquals(rp.getAppToken(),invite.getAppToken());
		assertEquals(rp.getBirth_day(),birthday.getBirth_day());
		assertEquals(rp.getBirth_year(),birthday.getBirth_year());
		assertEquals(rp.getBirth_month(),birthday.getBirth_month());
		assertEquals(rp.getHomecitycode(),homecity.getHomecitycode());
		assertEquals(rp.getLargeurl(),null);

	}
}
