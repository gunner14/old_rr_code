package com.xiaonei.passport.event.before;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.dao.BanIPDAO;
import com.xiaonei.passport.event.ILogicBeforeLogin;
@Service
public class IpBindBeforeLogin implements ILogicBeforeLogin {
	@Autowired
	private BanIPDAO banIPDAO ;
	private static List<String> banIpList;
	private static long timeInterval = 0;
	@Override
	public LoginStatusCode process(ILoginParameter loginParameter) {
		ILoginContext context =loginParameter.getLogincontext();
		   /* 地址封禁过滤 */
        boolean isBan=false;
		try {
			isBan = isBanIp(context.getIp());
		} catch (Exception e) {
			e.printStackTrace();
			return LoginStatusCode.LOGIN_OK;
			
		}
        if (isBan) {
            return LoginStatusCode.FAILBANIP;
        }
        return LoginStatusCode.LOGIN_OK;
	}
	

	private boolean isBanIp(String ip) throws Exception {
		// OPTIMIZE:登录量极大 这个数据已经较少维护
		if ((banIpList == null)
				|| ((System.currentTimeMillis() - timeInterval) > 600000l)) {// 1h
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
}
