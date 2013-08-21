package com.dodoyo.invite.dao;

import com.dodoyo.invite.model.UserInviteCodeView;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.framework.dao.XnDAO;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class UserInviteCodeViewDAO extends XnDAO {

    public UserInviteCodeViewDAO() {
        super();
    }

    private final static String sqlGetByUser = "select id, invitecode from user_personalinfo where id = ?";

    public UserInviteCodeView getByUser(final int usrId) throws SQLException {
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
        return (UserInviteCodeView) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetByUserNCode = "select id, invitecode from user_personalinfo where id = ? and invitecode = ?";

    public UserInviteCodeView getByUserNCode(final int usrId, final String code)
            throws SQLException {
        OpUniq op = new OpUniq(sqlGetByUserNCode, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, usrId);
                ps.setString(2, code);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUserInviteCodeView(rs);
            }

        };
        return (UserInviteCodeView) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlGetByCode = "select id, invitecode from user_personalinfo where invitecode = ?";

    public UserInviteCodeView getByCode(final String code) throws SQLException {
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
        return (UserInviteCodeView) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static UserInviteCodeView parseUserInviteCodeView(ResultSet rs)
            throws SQLException {
        UserInviteCodeView uic = new UserInviteCodeView();
        uic.setId(rs.getInt("id"));
        uic.setInviteCode(rs.getString("invitecode"));
        return uic;
    }
}
