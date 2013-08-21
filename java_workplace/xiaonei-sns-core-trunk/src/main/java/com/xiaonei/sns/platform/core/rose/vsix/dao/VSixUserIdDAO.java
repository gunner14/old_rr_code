package com.xiaonei.sns.platform.core.rose.vsix.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Collections;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

//@DAO(catalog = "reg_invite")
public class VSixUserIdDAO extends XnDAO {

    private static final VSixUserIdDAO instance = new VSixUserIdDAO();

    public static final VSixUserIdDAO getInstance() {
        return instance;
    }

    private VSixUserIdDAO() {

    }

    //    @SQL("SELECT userid FROM v6_user_id")
    @SuppressWarnings("unchecked")
    public List<Integer> getAllV6UserIds() {
        final OpList op = new OpList("SELECT userid FROM v6_user_id", DataAccessMgr.BIZ_REG_INVITE) {

            @Override
            public Object parse(final ResultSet rs) throws SQLException {
                return rs.getInt("userid");
            }
        };
        try {
            return DataAccessMgr.getInstance().queryList(op);
        } catch (final SQLException e) {
            return Collections.emptyList();
        }

    }

    //    @SQL("INSERT INTO v6_user_id SET userid=:userid ")
    public void insertUserId(/*@SQLParam("userid") */final int userid) {
        final OpUpdate op = new OpUpdate("INSERT INTO v6_user_id SET userid=?", "reg_invite") {

            @Override
            public void setParam(final PreparedStatement ps) throws SQLException {
                ps.setInt(1, userid);
            }
        };
        try {
            DataAccessMgr.getInstance().update(op);
        } catch (final SQLException e) {
            e.printStackTrace();
        }
    }
}
