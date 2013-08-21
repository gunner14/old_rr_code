package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.UserInviteCode;

public class UserInviteCodeViewDAO extends XnDAO {

    private static UserInviteCodeViewDAO instance = new UserInviteCodeViewDAO();

    public static UserInviteCodeViewDAO getInstance() {
        return instance;
    }
    
    public UserInviteCodeViewDAO() {
        super();
    }

    private final static String sqlGetByUser = "select id, invitecode from user_personalinfo where id = ?";

    public UserInviteCode getByUser(final int usrId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetByUser, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, usrId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUserInviteCodeView(rs);
            }

        };
        return (UserInviteCode) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetByCode = "select id, invitecode from user_personalinfo where invitecode = ?";

    public UserInviteCode getByCode(final String code) throws SQLException {
        OpUniq op = new OpUniq(sqlGetByCode, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, code);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUserInviteCodeView(rs);
            }

        };
        return (UserInviteCode) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static UserInviteCode parseUserInviteCodeView(ResultSet rs) throws SQLException {
        UserInviteCode uic = new UserInviteCode();
        uic.setId(rs.getInt("id"));
        uic.setInviteCode(rs.getString("invitecode"));
        return uic;
    }
}