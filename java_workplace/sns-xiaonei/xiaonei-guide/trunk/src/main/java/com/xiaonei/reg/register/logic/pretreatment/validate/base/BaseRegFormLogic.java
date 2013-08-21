/**
 *
 */
package com.xiaonei.reg.register.logic.pretreatment.validate.base;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.reg.register.constants.IAccountType;
import com.xiaonei.reg.register.form.base.BaseRegForm;
import com.xiaonei.reg.register.form.base.userganerate.IAccType;

/**
 * BaseRegFromLogic
 * 
 * @author wangtai(tai.wang@opi-corp.com) Sep 5, 2009 - 1:25:13 PM
 * 
 */
public class BaseRegFormLogic {

	private static final BaseRegFormLogic instance = new BaseRegFormLogic();

	public static BaseRegFormLogic getInstance() {
		return instance;
	}

	protected BaseRegFormLogic() {
	}

	final public boolean asImplements(Object obj, Class<?> iface) {
		if (null == obj || null == iface) {
			return false;
		}
		Class<?>[] ifs = obj.getClass().getInterfaces();
		for (Class<?> i : ifs) {
			if (iface.equals(i)) {
				return true;
			}
		}
		return false;
	}

	final public Object coverInterface(Object obj, Class<?> iface)
			throws Exception {
		if (asImplements(obj, iface)) {
			try {
				return iface.cast(obj);
			} catch (Exception e) {
				throw e;
			}
		}
		throw new Exception();
	}

	/**
	 * getRegAccount
	 * 
	 * 从regform里面取得用户注册时候，填写的accout
	 * 
	 * @param form
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 4, 2009 - 11:45:13 PM
	 */
	protected final String getRegAccount(BaseRegForm form) {
		try {
			IAccType ia = (IAccType) coverInterface(form, IAccType.class);
			return getRegAccount(ia);
		} catch (Exception e) {
			return null;
		}

	}

	/**
	 * getRegAccount
	 * 
	 * 从IAccType-里面取得用户注册时候，填写的accout
	 * 
	 * @param accType
	 * @return
	 * @author wangtai(tai.wang@opi-corp.com) Sep 4, 2009 - 11:53:00 PM
	 */
	protected final String getRegAccount(IAccType accType) {
		// account type存在
		if (accType.getAccType() == IAccountType.XID) {
			return accType.getNicknameId();
		} else if (accType.getAccType() == IAccountType.EMAIL) {
			return accType.getRegEmail();
		} else {
			// account type 不存在
			if (StringUtils.isNotEmpty(accType.getRegEmail())) {
				// 只有邮件存在
				return accType.getRegEmail();
			} else if (StringUtils.isNotEmpty(accType.getNicknameId())) {
				// 只有xid存在
				return accType.getNicknameId();
			} else {
				return null;
			}
		}
	}
}
