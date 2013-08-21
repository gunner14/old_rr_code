package com.xiaonei.reg.register.logic.pretreatment.validate;

import org.apache.commons.lang.StringUtils;

import com.dodoyo.opt.DodoyoResource;
import com.xiaonei.antispam.AntiSpamHome;
import com.xiaonei.antispam.model.CheckResult;
import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.utility.CharCode;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IName;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.pretreatment.validate.base.BaseRegCheckLogic;
import com.xiaonei.reg.register.model.AntispamCheckResult;

public class CheckName extends BaseRegCheckLogic {

	@Override
	public String check(final BaseRegForm regForm) {
		if (!asImplements(regForm, IName.class)) {
			return OK;
		}
		final IName p = (IName) regForm;
		return this.checkName(p.getName());
	}

	/**
	 * 用户输入的真实姓名检测
	 * 
	 * @param name
	 * @return
	 */
	public String checkName(String name) {
		final TimeCost cost = TimeCost.begin();
		if (name == null || name.length() == 0) {
			return ERR_NAME_NOT_EMPTY;
		}
		cost.endStep();
		if (containsIllegalChars(name)) {
			return ERR_NAME_NOT_REAL;
		}
		cost.endStep();
		final AntiSpamHome ash = AntiSpamHome.getInstance();
		AntispamCheckResult cr = null;
		try {
			cr = checkRealName(name, ash);
		} catch (final Throwable e1) {
			return OK;
		}
		if(null == cr){
			return OK;
		}
		cost.endStep();
		if (cr.getM_type() == CheckResult.PROHIBITED) {
			return ERR_NAME_NOT_SAFE;
		} else if (cr.getM_type() == CheckResult.SYSTEM_ERROR) {
			return OK;
		} else if (cr.getM_type() == CheckResult.NEED_REAL_NAME) {
			return ERR_NAME_NOT_REAL;
		}
		cost.endStep();
		name = cr.getM_content();
		cost.endStep();
		try {
			if (CharCode.isLengthExeed(name, 12)) {
				return ERR_NAME_TOO_LONG;
			}
		} catch (final Exception e) {
			e.printStackTrace();
			return ERR_NAME_NOT_SAFE;
		}
		cost.endStep();

		return OK;
	}

	private static final MemCacheManager mem = MemCacheFactory.getInstance()
			.getManagerByPoolName(MemCacheKeys.pool_default);

	private AntispamCheckResult checkRealName(String name,
			final AntiSpamHome ash) {
		AntispamCheckResult cr;
		String key = "check_real_name_" + name;
		Object oCr = mem.get(key);
		if (oCr != null && (oCr instanceof AntispamCheckResult)) {
			return (AntispamCheckResult) oCr;
		}
		cr = AntispamCheckResult.cover(ash.checkRealName(0, name));
		if (cr != null) {
			mem.set(key, cr, (int) MemCacheKeys.minute * 30);
		}
		return cr;
	}

	/**
	 * -r 包含不合理字符
	 * 
	 * @param name
	 * @return
	 */
	public static boolean containsIllegalChars(final String name) {
		final char[] illegalChars = DodoyoResource.getString(
				"name.illegal.chars", "check-rules").toCharArray();
		for (final char iChar : illegalChars) {
			if (StringUtils.contains(name, iChar))
				return true;
		}
		return false;
	}

}
