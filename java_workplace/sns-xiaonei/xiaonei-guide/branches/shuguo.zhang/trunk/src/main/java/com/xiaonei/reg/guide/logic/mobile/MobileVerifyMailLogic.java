/**
 *
 */
package com.xiaonei.reg.guide.logic.mobile;

import com.xiaonei.reg.guide.dao.MobileEmailVerifyDAO;

import java.sql.SQLException;

/**
 * MobileVerifyMailLogic
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-11 - 下午02:52:59
 */
public class MobileVerifyMailLogic {
    private static MobileVerifyMailLogic instance = new MobileVerifyMailLogic();

    public static MobileVerifyMailLogic getInstance() {
        return instance;
    }

    private MobileVerifyMailLogic() {
        super();
    }

    /**
     * 获得激活信地址
     *
     * @param mobile
     * @return
     * @throws SQLException
     * @author wang-tai(tai.wang@opi-corp.com) 2008-12-11 - 下午02:53:54
     */
    public String getEmailByMobile(String mobile) throws SQLException {
        String email = MobileEmailVerifyDAO.getInstance().getEmailByMobile(mobile);
        MobileEmailVerifyDAO.getInstance().updateEmailOK(mobile);
        return email;
	}
}
