package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Date;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.IntroducerLog;

public class IntroducerLogDAO extends XnDAO {

    private static IntroducerLogDAO instance = new IntroducerLogDAO();

    public static IntroducerLogDAO getInstance() {
        return instance;
    }
    
    public IntroducerLogDAO() {
        super();
    }


    private final static String sqlGetByNewbie = "select id, introducer, newbie, time from introducer_log where newbie = ?";

    public IntroducerLog getByNewbie(final int newbieId) throws SQLException {
        OpUniq op = new OpUniq(sqlGetByNewbie, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, newbieId);
            }

            @Override
            public Object parse(ResultSet rs) throws SQLException {
                return parseIntroducerLog(rs);
            }

        };
        return (IntroducerLog) DataAccessMgr.getInstance().queryUnique(op);
    }

    private final static String sqlInsert = "insert into introducer_log (introducer, newbie, time) values (?, ?, ?)";

    public int insert(final IntroducerLog inlog) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, inlog.getIntroducer());
                ps.setInt(2, inlog.getNewbie());
                ps.setTimestamp(3, new Timestamp(inlog.getTime().getTime()));
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlUpdateTime = "update introducer_log set time = ? where id = ?";

    public int updateTime(final int id, final Date time) throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateTime, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setTimestamp(1, new Timestamp(time.getTime()));
                ps.setInt(2, id);
            }

        };
        return DataAccessMgr.getInstance().update(op);
    }

    private static IntroducerLog parseIntroducerLog(ResultSet rs) throws SQLException {
        IntroducerLog inl = new IntroducerLog();
        inl.setId(rs.getInt("id"));
        inl.setIntroducer(rs.getInt("introducer"));
        inl.setNewbie(rs.getInt("newbie"));
        inl.setTime(rs.getTimestamp("time"));
        return inl;
    }
}