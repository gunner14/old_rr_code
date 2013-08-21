package com.xiaonei.reg.register.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.RegProcessAppdatas;

import java.sql.PreparedStatement;
import java.sql.SQLException;

public class RegProcessAppdatasDAO extends XnDAO {

    private static RegProcessAppdatasDAO instance = new RegProcessAppdatasDAO();

    public static RegProcessAppdatasDAO getInstance() {
        return instance;
    }

    private RegProcessAppdatasDAO() {
        super();
    }

    /**
     * 插入待处理的app的信息，例如：注册进来给邀请者加钱，具体实现是张洁增值组做
     *
     * @param RegVisitHis
     * @return
     * @throws SQLException
     */
    public int save(final RegProcessAppdatas regProcessAppdatas) throws SQLException {
        String sqlInsert = "insert into reg_process_appdatas (app_id, inviter_id, invitee_id, create_time) values (?, ?, ?, now())";
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, regProcessAppdatas.getAppId());
                ps.setInt(2, regProcessAppdatas.getInviterId());
                ps.setInt(3, regProcessAppdatas.getInviteeId());
            }
        };
        return DataAccessMgr.getInstance().update(op);
	}
}
