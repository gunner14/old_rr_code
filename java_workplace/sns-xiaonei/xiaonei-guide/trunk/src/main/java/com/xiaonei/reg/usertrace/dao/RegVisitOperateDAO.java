package com.xiaonei.reg.usertrace.dao;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.usertrace.model.RegVisitOperate;
import com.xiaonei.xce.XceAdapter;

public class RegVisitOperateDAO extends XnDAO {
    private static RegVisitOperateDAO instance = new RegVisitOperateDAO();

    public static RegVisitOperateDAO getInstance() {
        return instance;
    }

    private RegVisitOperateDAO() {
        super();
    }

    /**
     * 插入访问的信息
     *
     * @param RegVisitOperate
     * @return
     * @throws SQLException
     */
    public int insertOprLog(final RegVisitOperate logOpr) throws SQLException {
        String sqlInsert = "insert into act2 (action_id, user_id, user_email, stage, ip, xn_sess_id, time, s1, s2, s3, s4,domain_name,inviter_id) values (?, ?, ?, ?, ?, ?, now(), ?, ?, ?, ?, ?, ?)";
        String domain = logOpr.getDomainName();
        String bizName = XceAdapter.DB_REG_INVITE_ACT;
        if(domain!= null && domain.indexOf("kaixin.com") > -1){
            bizName = DataAccessMgr.BIZ_KX_REG_INVITE;
        }
        OpUpdate op = new OpUpdate(sqlInsert, bizName) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                if (logOpr.getIp() != null && logOpr.getIp().length() > 20) {
                    logOpr.setIp(logOpr.getIp().substring(0, 20));
                }
                ps.setInt(1, logOpr.getActionid());
                ps.setInt(2, logOpr.getUserid());
                ps.setString(3, logOpr.getEmail());
                ps.setInt(4, logOpr.getStage());
                ps.setString(5, logOpr.getIp());
                ps.setString(6, logOpr.getXnSessId());
                ps.setString(7, logOpr.getS1());
                ps.setString(8, logOpr.getS2());
                ps.setString(9, logOpr.getS3());
                ps.setString(10, logOpr.getS4());
                ps.setString(11, logOpr.getDomainName());
                ps.setInt(12, logOpr.getInviterId());
            }
        };
        return DataAccessMgr.getInstance().update(op);
	}
    
    public int[] batchSave(final HashMap<Integer, RegVisitOperate> infos) throws SQLException {
        String sqlInsert = "insert into act2 (action_id, user_id, user_email, stage, ip, xn_sess_id, time, s1, s2, s3, s4,domain_name,inviter_id) values (?, ?, ?, ?, ?, ?, now(), ?, ?, ?, ?, ?, ?)";
        String domain = infos!=null && infos.get(0)!= null ? infos.get(0).getDomainName() : "";
        String bizName = XceAdapter.DB_REG_INVITE_ACT;
        if(domain!= null && domain.indexOf("kaixin.com") > -1){
            bizName = DataAccessMgr.BIZ_KX_REG_INVITE;
        }
        OpUpdate op = new OpUpdate(sqlInsert, bizName) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                Iterator<Integer> iterator = infos.keySet().iterator();
                while(iterator.hasNext()){
                    Object oj=iterator.next();
                    RegVisitOperate logOpr = (RegVisitOperate)infos.get(oj);
                    if (logOpr.getIp() != null && logOpr.getIp().length() > 20) {
                        logOpr.setIp(logOpr.getIp().substring(0, 20));
                    }
                    ps.setInt(1, logOpr.getActionid());
                    ps.setInt(2, logOpr.getUserid());
                    ps.setString(3, logOpr.getEmail());
                    ps.setInt(4, logOpr.getStage());
                    ps.setString(5, logOpr.getIp());
                    ps.setString(6, logOpr.getXnSessId());
                    ps.setString(7, logOpr.getS1());
                    ps.setString(8, logOpr.getS2());
                    ps.setString(9, logOpr.getS3());
                    ps.setString(10, logOpr.getS4());
                    ps.setString(11, logOpr.getDomainName());
                    ps.setInt(12, logOpr.getInviterId());
                    ps.addBatch();
                }
            }
        };
        return DataAccessMgr.getInstance().batchUpdate(op);
    }
}
