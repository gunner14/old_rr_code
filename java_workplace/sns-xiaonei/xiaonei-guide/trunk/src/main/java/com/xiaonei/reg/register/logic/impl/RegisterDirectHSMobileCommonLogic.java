package com.xiaonei.reg.register.logic.impl;

import com.xiaonei.platform.core.model.User;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.RegMobileWebInfoMapDAO;
import com.xiaonei.reg.register.logic.IRegMobileLogic;
import com.xiaonei.reg.register.logic.additional.ActivateLogic;
import com.xiaonei.reg.register.logic.pretreatment.RegMobileLogic;
import com.xiaonei.reg.register.model.RegMobileWebInfoMapHS;
import com.xiaonei.reg.register.pojo.RegisterPojo;
import org.apache.commons.lang.StringUtils;

import java.sql.SQLException;

public class RegisterDirectHSMobileCommonLogic extends
		RegisterDirectHSCommonLogic implements IRegMobileLogic {

	protected RegisterDirectHSMobileCommonLogic() {
		super();
	}

	public int regAfterCheckUpCode(String mobileNum) {
		if (null == StringUtils.trimToNull(mobileNum)) {
			return sys_err;
		}
		try {
			if (true == UserAdapter.hasUser(mobileNum)) {
				return user_exist;
			}
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		RegMobileWebInfoMapHS rm = null;
		try {
			rm = RegMobileWebInfoMapDAO.getInstance()
					.getRegMobileInfoLatest_hs(mobileNum);
		} catch (SQLException e) {
			e.printStackTrace();
			return sys_err;
		}
		if (null == rm) {
			return no_user;
		} else {
			RegisterPojo rp = new RegisterPojo(rm);
			int rt;
			try {
				rt = this.register(rp);
			} catch (Exception e1) {
				e1.printStackTrace();
				return sys_err;
			}
			if (rt > 0) {
				return sys_err;
			} else {
				int userId = -rt;
				// 激活
				User user = UserAdapter.getUser(userId);
				try {
					ActivateLogic.getInstance().activateUser(user);
				} catch (Exception e) {
					e.printStackTrace();
				}
				return userId;
			}
		}
	}

	public int checkDownCodeAndReg(String mobileNum, String downCode) {
		try {
			if (UserAdapter.hasUser(mobileNum)) {
				return user_exist;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return sys_err;
		}
		// 验证码错误
		try {
			if (null == RegMobileLogic.getInstance().getRegMobileInfoByCode_hs(
					mobileNum, downCode)) {
				return check_down_code_false;
			}
			// 验证成功
			else {
				int rt = 0;
				// 注册
				try {
					RegMobileWebInfoMapHS mo = RegMobileWebInfoMapDAO
							.getInstance().getRegMobileInfoByMobileAndCode_hs(
									mobileNum, downCode);
					RegisterPojo rp = new RegisterPojo(mo);
					rt = this.register(rp);
				} catch (Exception e) {
					e.printStackTrace();
					return sys_err;
				}
				if (rt < 0) {
					int userId = -rt;
					// 激活
					User user = UserAdapter.getUser(userId);
					try {
						ActivateLogic.getInstance().activateUser(user);
					} catch (Exception e) {
						e.printStackTrace();
					}
					return userId;
				} else {
					return rt;
				}
			}
		} catch (SQLException e) {
			e.printStackTrace();
			return check_down_code_false;
		}
	}

	@Override
	protected int additional(User user, RegisterPojo regPojo) {
		int rt = super.additional(user, regPojo);
		try {
			bindAndCreateMMS(user);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return rt;
	}

	private void bindAndCreateMMS(User user) {
		RegMobileLogic.getInstance().bindMobileAndCreateMMSAlbum(user);
	}

}
