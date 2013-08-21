package com.xiaonei.reg.register.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.register.model.RegExtraMsg;


public class RegExtraMsgDAO extends XnDAO {
    
    public static RegExtraMsgDAO getInstance() {
        return instance;
    }

    private RegExtraMsgDAO() {
        super();
    }

    private static RegExtraMsgDAO instance = new RegExtraMsgDAO();
    
    private final static String sqlInsert = "insert into `reg_extra_msg` (`uid`, `input_key`, `input_value`) values (?, ?, ?)";

    public int save(final RegExtraMsg msg) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsert, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, msg.getUid());
                ps.setString(2, msg.getInput_key());
                ps.setString(3, msg.getInput_value());
            }
        };
        return DataAccessMgr.getInstance().update(op);
    }
    
    private final static String sqlUpdateMsgFlagByUidAndInpuKey = "UPDATE `reg_extra_msg` SET `reg_flag`=? WHERE `uid`=? and `input_key`=?";

    public int updateMsgFlagByUidAndInpuKey(final RegExtraMsg msg) throws SQLException {

        OpUpdate op = new OpUpdate(sqlUpdateMsgFlagByUidAndInpuKey,
                DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, msg.getReg_flag());
                ps.setInt(2, msg.getUid());
                ps.setString(3, msg.getInput_key());
            }
        };

        return DataAccessMgr.getInstance().update(op);
    }

    private final static String sqlUpdateFlag = "UPDATE `reg_extra_msg` SET `reg_flag`=? WHERE `uid`=? ";

    public int updateInviteStatus(final RegExtraMsg msg)
            throws SQLException {
        OpUpdate op = new OpUpdate(sqlUpdateFlag, DataAccessMgr.BIZ_STAT) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, msg.getReg_flag());
                ps.setInt(2, msg.getUid());
            }
        };

        return DataAccessMgr.getInstance().update(op);
    }
    
    
    private final static String sqlMsgSelect = "SELECT * FROM `reg_extra_msg` WHERE `uid`=?";

    @SuppressWarnings("unchecked")
    public List<RegExtraMsg> getMsgInfoByUid(final RegExtraMsg msg)
            throws SQLException {
         OpList op = new OpList(sqlMsgSelect, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, msg.getUid());
            }

            @Override
            public RegExtraMsg parse(ResultSet rs) throws SQLException {
                return parseRegExtraMsg(rs);
            }
        };
        return (List<RegExtraMsg>) DataAccessMgr.getInstance().queryList(op);
    }

    private final static String sqlMsgGetByUidAndInputkey = "SELECT * FROM `reg_extra_msg` WHERE `uid`=? AND `input_key`=?";

    @SuppressWarnings("unchecked")
    public List<RegExtraMsg> getMsgInfoByUidAndInputkey(final RegExtraMsg msg) throws SQLException {
        OpList op = new OpList(sqlMsgGetByUidAndInputkey, DataAccessMgr.BIZ_STAT) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, msg.getUid());
                ps.setString(2, msg.getInput_key());
            }

            @Override
            public RegExtraMsg parse(ResultSet rs) throws SQLException {
                return parseRegExtraMsg(rs);
            }
        };
        return (List<RegExtraMsg>) DataAccessMgr.getInstance().queryList(op);
    }
    
    
    private RegExtraMsg parseRegExtraMsg(ResultSet rs) throws SQLException {
        RegExtraMsg msg = new RegExtraMsg();
        msg.setId(rs.getInt("id"));
        msg.setUid(rs.getInt("uid"));
        msg.setInput_key(rs.getString("input_key"));
        msg.setInput_value(rs.getString("input_value"));
        msg.setReg_time(rs.getDate("reg_time"));
        msg.setReg_flag(rs.getInt("reg_flag"));
        return msg;
    }
}
