/**
 *
 */
package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.guide.model.UserPersonalInfoModel;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

/**
 * UserPersonalInfoDAO.java
 *
 * @author (wang-tai)tai.wang@opi-corp.com
 * @date 2008-9-28 下午05:39:20
 */
public class UserPersonalInfoDAO extends XnDAO {
    private static UserPersonalInfoDAO instance = new UserPersonalInfoDAO();

    public static UserPersonalInfoDAO getInstance() {
        return instance;
    }

    private UserPersonalInfoDAO() {
        super();
    }

    private final static String sqlInsertInviteCode = "insert into user_personalinfo (id, invitecode, source) values (?, ?, 0)";

    public int insertInviteCode(final int id, final String invitecode) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertInviteCode, DataAccessMgr.BIZ_COMMON) {

            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, id);
                ps.setString(2, invitecode);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlUpdateInviteCode = "update user_personalinfo set invitecode = ? where id = ?";

    public int updateInviteCode(final int id, final String invitecode) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateInviteCode, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, invitecode);
                ps.setInt(2, id);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlGetUser = "SELECT * FROM user_personalinfo WHERE lastip_long = ?";

    @SuppressWarnings("unchecked")
    public List<UserPersonalInfoModel> getUserList(final long ip) throws SQLException {

        OpList op = new OpList(sqlGetUser, DataAccessMgr.BIZ_COMMON) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setLong(1, ip);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseUserPersonalInfoModel(rs);
            }
        };

        return (List<UserPersonalInfoModel>) DataAccessMgr.getInstance().queryList(op);
    }

    private UserPersonalInfoModel parseUserPersonalInfoModel(ResultSet rs) throws SQLException {
        UserPersonalInfoModel model = new UserPersonalInfoModel();
        model.setId(rs.getInt("id"));
        model.setInviteCode(rs.getString("invitecode"));
        model.setLastIp(rs.getLong("lastip_long"));
        model.setLink(rs.getString("link"));
        model.setLinkStatus(rs.getInt("linkstatus"));
        model.setSource(rs.getInt("source"));
        model.setSpaceAll(rs.getInt("space_all"));
        return model;
    }
}
