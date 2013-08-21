/**
 *
 */
package com.xiaonei.reg.register.logic;

import java.lang.reflect.Constructor;
import java.util.HashMap;
import java.util.Map;

import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.register.logic.impl.RegisterDirectHSCommonLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectHSEducationLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectHSInviteLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectHSMobileCommonLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectHSQuickLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectMajorCommonLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectMajorInviteLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectMajorQuickLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectOthersCommonLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectOthersInviteLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectOthersQuickLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectProfileLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectShortCommonLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectShortInviteLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectUnivCommonLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectUnivInviteLogic;
import com.xiaonei.reg.register.logic.impl.RegisterDirectUnivQuickLogic;

/**
 * RegisterLogicFactory.java
 * 
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-27 下午05:55:08
 */
public class RegisterLogicFactory {
	static private Map<String, IRegisterLogic> m_registry = new HashMap<String, IRegisterLogic>();

	static final private RegLogger logger = RegLogger
			.getLoggerWithType(RegisterLogicFactory.class);

	/** * 静态工厂方法,返还此类惟一的实例 */
	static private IRegisterLogic getInstance(String regClazzName) {
		if (regClazzName == null) {
			return null;
		}
		if (m_registry.get(regClazzName) == null) {
			try {
				Constructor<?> cxtor = Class.forName(regClazzName)
						.getDeclaredConstructor();
				cxtor.setAccessible(true);
				m_registry.put(regClazzName, (IRegisterLogic) cxtor
						.newInstance());
			} catch (Exception e) {
				logger.error(e);
			}
		}
		return (IRegisterLogic) (m_registry.get(regClazzName));
	}

	public static IRegisterLogic getRegEduHs() {
		return getInstance(RegisterDirectHSEducationLogic.class.getName());
	}

	public static IRegisterLogic getRegShortInvite() {
		return getInstance(RegisterDirectShortInviteLogic.class.getName());
	}

	public static IRegisterLogic getRegProfile() {
		return getInstance(RegisterDirectProfileLogic.class.getName());
	}

	public static IRegisterLogic getRegShortCommon() {
		return getInstance(RegisterDirectShortCommonLogic.class.getName());
	}

	public static IRegisterLogic getMajorCommon() {
		return getInstance(RegisterDirectMajorCommonLogic.class.getName());
	}

	public static IRegisterLogic getHSCommon() {
		return getInstance(RegisterDirectHSCommonLogic.class.getName());
	}

	public static IRegisterLogic getUnivCommon() {
		return getInstance(RegisterDirectUnivCommonLogic.class.getName());
	}

	public static IRegisterLogic getOthersCommon() {
		return getInstance(RegisterDirectOthersCommonLogic.class.getName());
	}

	public static IRegisterLogic getMajorInvite() {
		return getInstance(RegisterDirectMajorInviteLogic.class.getName());
	}

	public static IRegisterLogic getHSInvite() {
		return getInstance(RegisterDirectHSInviteLogic.class.getName());
	}

	public static IRegisterLogic getUnivInvite() {
		return getInstance(RegisterDirectUnivInviteLogic.class.getName());
	}

	public static IRegisterLogic getOthersInvite() {
		return getInstance(RegisterDirectOthersInviteLogic.class.getName());
	}

	public static IRegisterLogic getHSMobileCommon() {
		return getInstance(RegisterDirectHSMobileCommonLogic.class.getName());
	}

	public static IRegisterLogic getMajorQuick() {
		return getInstance(RegisterDirectMajorQuickLogic.class.getName());
	}

	public static IRegisterLogic getHSQuick() {
		return getInstance(RegisterDirectHSQuickLogic.class.getName());
	}

	public static IRegisterLogic getOthersQuick() {
		return getInstance(RegisterDirectOthersQuickLogic.class.getName());
	}

	public static IRegisterLogic getUnivQuick() {
		return getInstance(RegisterDirectUnivQuickLogic.class.getName());
	}

	static public void main(String [] aa) throws Exception{
		System.out.print(getUnivQuick().register(null));
	}
}
