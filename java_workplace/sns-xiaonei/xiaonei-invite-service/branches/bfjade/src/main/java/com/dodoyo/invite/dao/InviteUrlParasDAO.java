package com.dodoyo.invite.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.dodoyo.invite.core.InviteDatasourceMgr;
import com.dodoyo.invite.core.entity.InviteUrlParas;
import com.dodoyo.invite.core.logic.LogUtils;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;

/**
 * 邀请连接参数dao
 *
 * @author dream
 */
public class InviteUrlParasDAO extends XnDAO {

    static class SingletonHolder {
        static InviteUrlParasDAO instance = new InviteUrlParasDAO();
    }

    public static InviteUrlParasDAO getInstance() {
        return SingletonHolder.instance;
    }

    private int getNext() throws SQLException 
    {
    	final String sqlGetNext = "select nextval('"+InviteDatasourceMgr.getInstance().getSequenceName(this.getClass())+"')";
        OpUniq op = new OpUniq(sqlGetNext, DataAccessMgr.BIZ_IDSEQUENCE);
        return DataAccessMgr.getInstance().queryId(op);
    }
    
    private static final String sqlInsert = "insert ignore into invite_url_paras (id, uuid, user_id, email_from, email_to, ss, app_id, app_token, md5, extar_paras, buddy_group, code, rt, time) values(?,?,?,?,?,?,?,?,?,?,?,?,?,now())";

    //	private static final String sqlInsert = "insert ignore into invite_url_paras (uuid, userid, email_from, email_to, ss, app_id, app_token, md5, date) values(?,?,?,?,?,?,?,?,now())";
    public int InsertUser(final InviteUrlParas u) throws SQLException {
        if (true)//cancel the following codes from line 48 to 69
        {
            com.dodoyo.invite.core.logic.BatchInsertUtils.getInstance().addIup(u);
            return 0;
        }
    	final int nextId = getNext();
    	LogUtils.logInfo("getnext id for invite_url_paras id = " + nextId);
        OpUpdate op = new OpUpdate(sqlInsert, InviteDatasourceMgr.getInstance().getBizStat(this.getClass())) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, nextId);
                ps.setString(2, u.getUuid());
                ps.setInt(3, u.getUser_id());
                ps.setString(4, u.getEmail_from());
                ps.setString(5, u.getEmail_to());
                ps.setString(6, u.getSs());
                ps.setInt(7, u.getApp_id());
                ps.setString(8, u.getApp_token());
                ps.setString(9, u.getMd5());
                ps.setString(10, u.getExtar_paras());
                ps.setString(11, u.getBuddy_group());
                ps.setString(12, u.getCode());
                ps.setString(13, u.getRt());

            }
        };
        return DataAccessMgr.getInstance().update(op);

    }

    private static final String sqlSelectAll = "select * from invite_url_paras where md5 = ?";

    @SuppressWarnings("unchecked")
    public List<InviteUrlParas> getInviteUrlParasByMd5(final String md5) throws SQLException {
        OpList op = new OpList(sqlSelectAll, InviteDatasourceMgr.getInstance().getBizStat(this.getClass())) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setString(1, md5);
            }

            @Override
            public InviteUrlParas parse(ResultSet rs) throws SQLException {
                return parseUserCheckMailModel(rs);
            }
        };
        return (List<InviteUrlParas>) DataAccessMgr.getInstance().queryList(op);
    }

    private static final String sqlGetUserByUserId = "selelct * from invite_url_paras where user_id = ?";

    @SuppressWarnings("unchecked")
    public List<InviteUrlParas> selectUserByUserId(final int userId) throws SQLException {
        OpList op = new OpList(sqlGetUserByUserId, InviteDatasourceMgr.getInstance().getBizStat(this.getClass())) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, userId);
            }

            @Override
            public InviteUrlParas parse(ResultSet rs) throws SQLException {
                return parseUserCheckMailModel(rs);
            }
        };
        return (List<InviteUrlParas>) DataAccessMgr.getInstance().queryList(op);
    }

    private InviteUrlParas parseUserCheckMailModel(ResultSet rs) throws SQLException {
        InviteUrlParas u = new InviteUrlParas();
//        u.setId(rs.getInt("id"));
        u.setApp_id(rs.getInt("app_id"));
        u.setApp_token(rs.getString("app_token"));
        u.setTime(rs.getTimestamp("time"));
        u.setEmail_from(rs.getString("email_from"));
        u.setEmail_to(rs.getString("email_to"));
//        u.setId(rs.getInt("id"));
        u.setMd5(rs.getString("md5"));
        u.setSs(rs.getString("ss"));
        u.setUser_id(rs.getInt("user_id"));
        u.setUuid(rs.getString("uuid"));
        u.setExtar_paras(rs.getString("extar_paras"));
        return u;
    }
}
