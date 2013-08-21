package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Mar 9, 2010 9:49:57 AM
 * 保存免激活信息
 */
public class RegNoActivateRecordDAO {
    public static RegNoActivateRecordDAO getInstance() {
        return instance;
    }

    private RegNoActivateRecordDAO() {
        super();
    }

    private static RegNoActivateRecordDAO instance = new RegNoActivateRecordDAO();
    
    private final static String sqlInsert = "insert into `reg_no_activated_info` (`uid`) values (?)";

    /**
     * 
     * @param record
     * @return
     * @throws SQLException
     * @author wei.cheng@opi-corp.com
     * @version 1.0
     * @date 创建时间：Mar 9, 2010 9:51:29 AM
     */
    public int save(final int uid) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, uid);
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}
