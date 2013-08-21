package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.component.exp.model.DailyExp;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;

public class DailyExpDAO {
    
    private static DailyExpDAO instance = new DailyExpDAO();

    public static DailyExpDAO getInstance() {
        return instance;
    }
    
    private final static String sqlInsert = "insert into daily_exp (id, daily_exp, time) values (?, ?, ?)";

    public int save(final DailyExp exp) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_COMMON) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, exp.getId());
                ps.setDouble(2, exp.getDailyExp());
                ps.setTimestamp(3, new Timestamp(exp.getTime().getTime()));
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}