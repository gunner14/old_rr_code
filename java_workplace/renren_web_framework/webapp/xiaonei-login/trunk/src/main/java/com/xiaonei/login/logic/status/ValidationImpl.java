package com.xiaonei.login.logic.status;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Service;


import com.xiaonei.helper.password.PwdUtil;
import com.xiaonei.login.constants.Globals;
import com.xiaonei.login.constants.LoginLoggerDef;
import com.xiaonei.login.constants.LoginMemcachedKeyDef;
import com.xiaonei.login.constants.LoginParamDef;
import com.xiaonei.login.constants.LoginStatusCode;
import com.xiaonei.login.dao.BanIPDAO;
import com.xiaonei.login.logic.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.base.UserConfirmLogic;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.xce.passport.Account;

@Service
public class ValidationImpl implements Validation,InitializingBean {

	private Map<Integer, String> accountBindedIp = new HashMap<Integer, String>();
	
	private MemCacheFactory memCacheFactory = MemCacheFactory.getInstance();

	public ValidationImpl bindMemCacheFactory(MemCacheFactory memCacheFactory) {
		this.memCacheFactory = memCacheFactory;
		return this;
	}

	@Override
	public boolean checkUserSafeStatus(User user) {
		return user.getSafeStatus() > 0;
	}

	@Override
	public LoginStatusCode checkUserStatus(Account account) {
		switch (account.getStatus()) {
		case User.statusActivateVerified:
			if (!UserConfirmLogic.getInstance().isConfirmed(account)) {
				return LoginStatusCode.LOGIN_OK;
			}
			return LoginStatusCode.FAILUSERACTIVE;
		case User.statusActivateVerify:
			if (!UserConfirmLogic.getInstance().isConfirmed(account)) {
				return LoginStatusCode.LOGIN_OK;
			}
			return LoginStatusCode.FAILUSERACTIVE;
		case User.statusBanned:
			return LoginStatusCode.FAILUSERBAN;
		case User.statusSuicide:
			return LoginStatusCode.FAILUSERSUICIDE;
		default:
			return LoginStatusCode.LOGIN_OK;
		}

	}

	public int endTime(long time) {
		long hour = 1000 * 3600;
		long day = 24 * hour;
		long cacheTime = day - (time % day + 8 * hour);// 夏令时
		return (int) ((cacheTime >= 0 ? cacheTime : day + cacheTime) / 1000);
	}

	@Override
	public Boolean getWarningTagOfWeakPwd(HttpServletRequest request) {
		if (null == request
				.getAttribute(LoginParamDef.ATTRIBUTE_WEAK_PASSWORD_WARNING)) {
			return false;
		}
		return (Boolean) request
				.getAttribute(LoginParamDef.ATTRIBUTE_WEAK_PASSWORD_WARNING);
	}

	@Override
	public boolean isBanIP(String ip) {
		try {
			return isBanIp(ip);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}

	@Override
	public boolean isBindedIPDoNotBind(String ip, int id) {
		LoginLoggerDef.login.trace("accountBindedIp.get(id):"
				+ accountBindedIp.get(id));
		if (accountBindedIp.get(id) == null) {
			return false;
		}
		ip = StringUtils.isEmpty(ip) ? "" : ip;
		return !ip.equals(accountBindedIp.get(id));
	}

	@Override
	public boolean isValidEmailAddr(String mailAddr) {
		return StringFormat.isValidEmailAddr(mailAddr);
	}

	@Override
	public boolean isValidMobile(String mobile) {
		return StringFormat.isValidMobile(mobile);
	}

	@Override
	public boolean isWarningUser(String username, String password) {
		if (Globals.WARNING_USERNAME == null) {
			return false;
		}
		Boolean obj = Globals.WARNING_USERNAME.get((username + "\t" + password)
				.trim().toLowerCase());
		return obj != null;
	}

	@Override
	public boolean isWeekPasswordInMd5Format(String md5Password) {
		return PwdUtil.isWeakPassWordCipher(md5Password);
	}

	@Override
	public void setWarningTagOfWeakPasswordInAttribut(HttpServletRequest request) {
		try {
			MemCacheManager mem = memCacheFactory
					.getManagerByPoolName(MemCacheKeys.pool_default);
			long cnt = mem.incr(LoginMemcachedKeyDef.WEAK_PASSWORD_WARNING);
			if (cnt < 10000) {
				request.setAttribute(
						LoginParamDef.ATTRIBUTE_WEAK_PASSWORD_WARNING, true);
				if (cnt <= 0) {
					// System.err.println("Add WeakPassword Counter!!!");
					mem.add(LoginMemcachedKeyDef.WEAK_PASSWORD_WARNING, 0,
							endTime(System.currentTimeMillis()));
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static List<String> banIpList;

	private static long timeInterval = 0;
	
	@Autowired
	private BanIPDAO banIPDAO ;

	boolean isBanIp(String ip) throws Exception {
		// OPTIMIZE:登录量极大 这个数据已经较少维护
		if ((banIpList == null)
				|| ((System.currentTimeMillis() - timeInterval) > 3600000l)) {// 1h
			timeInterval = System.currentTimeMillis();
			Runnable getip = new Runnable() {
				@Override
				public void run() {
					try {
						banIpList = banIPDAO.getBanedIpList();
					} catch (DataAccessException ex) {
						ex.printStackTrace();
					}
				}
			};
			new Thread(getip).start();
		}
		if (banIpList == null) {
            return false;
        }
		return banIpList.contains(ip);
	}

    @Override
    public void afterPropertiesSet() throws Exception {
        accountBindedIp.put(315659715, "10.2.19.131");        
    }
}
