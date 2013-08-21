package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.RegAppInviteRelation;

/**
 * @author wei.cheng@opi-corp.com
 * @version 1.0
 * @date Mar 22, 2010 1:43:05 PM
 * 类说明
 */
public class RegAppInviteRelationDAO extends XnDAO {
    private static RegAppInviteRelationDAO instance = new RegAppInviteRelationDAO();

    public static RegAppInviteRelationDAO getInstance() {
        return instance;
    }

    private RegAppInviteRelationDAO() {
        super();
    }

    /**
     * 插入被邀请注册的人和邀请人的映射关系表
     *
     * @param regInviteMapTable
     * @return
     * @throws SQLException
     */
    public int save(final RegAppInviteRelation regInviteRelation) throws SQLException {
        String sqlInsert = "insert into app_invite_relation (inviter_id, invitee_id, app_id) VALUES (?, ?, ?)";
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, regInviteRelation.getInviterId());
                ps.setInt(2, regInviteRelation.getInviteeId());
                ps.setInt(3, regInviteRelation.getAppId());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
}
