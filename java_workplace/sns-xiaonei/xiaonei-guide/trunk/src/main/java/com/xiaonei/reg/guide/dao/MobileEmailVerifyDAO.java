/**
 *
 */
package com.xiaonei.reg.guide.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.MobileEmailVerify;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * MobileEmailVerifyDAO
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2008-12-5 - 下午03:39:59
 */
public class MobileEmailVerifyDAO extends XnDAO {

    private static MobileEmailVerifyDAO instance = new MobileEmailVerifyDAO();

    public static MobileEmailVerifyDAO getInstance() {
        return instance;
    }

    private MobileEmailVerifyDAO() {
        super();
    }

    public String getEmailByMobile(final String mobile) throws SQLException {
        final String sqlGetRegMobileInfoByMobileHs = "select email from mobile_email_verify where mobile=?";
        OpUniq op = new OpUniq(sqlGetRegMobileInfoByMobileHs, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, mobile);
            }

            @Override
            public String parse(ResultSet rs) throws SQLException {
                return rs.getString("email");
            }

        };
        return (String) DataAccessMgr.getInstance().queryUnique(op);
    }

    public int updateEmailOK(final String mobile) throws SQLException {
        final String sqlUpdateEmail = "UPDATE mobile_email_verify set status=" + MobileEmailVerify.VERIFY_OK + " where mobile=?";
        OpUpdate op = new OpUpdate(sqlUpdateEmail, DataAccessMgr.BIZ_MAIL_MMS) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, mobile);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }
}
