package com.xiaonei.reg.guide.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.guide.model.UserBootStatus;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class UserBootStatusDAO extends XnDAO {


    private static UserBootStatusDAO instance = new UserBootStatusDAO();

    public static UserBootStatusDAO getInstance() {
        return instance;
    }

    private UserBootStatusDAO() {
        super();
    }

    private final static String sqlInsert = "INSERT INTO user_boot_status (id, invite_status) values (?, ?)";

    public int save(final UserBootStatus ubs) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, ubs.getId());
                // ps.setInt(2, ubs.getPassedAudit());
                ps.setInt(2, ubs.getInviteStatus());
            }
        };
        return DataAccessMgr.getInstance().insertReturnId(op);
    }

    private final static String sqlGetAuditStatus = "SELECT passed_audit FROM user_boot_status WHERE id = ?";

    public int getAuditStatus(final int userId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetAuditStatus, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }
        };

        return DataAccessMgr.getInstance().queryInt(op);
    }

    private final static String sqlGetInviteStatus = "SELECT invite_status FROM user_boot_status WHERE id = ?";

    public int getInviteStatus(final int userId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetInviteStatus, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }
        };

        return DataAccessMgr.getInstance().queryInt(op);//ForStat(op);
    }

    private final static String sqlUpdateAuditStatus = "UPDATE user_boot_status SET passed_audit = ? WHERE id = ?";

    public int updateAuditStatus(final int userId, final int status) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateAuditStatus, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, status);
                ps.setInt(2, userId);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlGetUserBootStatus = "SELECT id, invite_status, passed_audit FROM user_boot_status WHERE id = ?";

    public UserBootStatus getUserBootStatus(final int userId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetUserBootStatus, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUserBootStatus(rs);
            }

        };
        return (UserBootStatus) DataAccessMgr.getInstance().queryUnique(op);
    }

    private static UserBootStatus parseUserBootStatus(ResultSet rs) throws SQLException {
        UserBootStatus ubs = new UserBootStatus();
        ubs.setId(rs.getInt(1));
        ubs.setInviteStatus(rs.getInt(2));
        ubs.setPassedAudit(rs.getInt(3));
        return ubs;
    }

}
