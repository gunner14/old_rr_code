package com.xiaonei.reg.usertrace.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.framework.dao.XnDAO;
import com.xiaonei.reg.usertrace.model.RegVisitLogView;
import com.xiaonei.xce.XceAdapter;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;

public class RegVisitLogViewDAO extends XnDAO {

    private static RegVisitLogViewDAO instance = new RegVisitLogViewDAO();

    public static RegVisitLogViewDAO getInstance() {
        return instance;
    }

    private RegVisitLogViewDAO() {
        super();
    }

    /**
     * 插入访问的信息
     *
     * @param RegVisitLogView
     * @return
     * @throws SQLException
     */
    public int insertLogView(final RegVisitLogView logView) throws SQLException {
        String sqlInsert = "insert into act1 (ip, xn_sess_id, referer, time, s1, s2, s3, s4, action_id, domain_name, inviter_id) values (?, ?, ?, now(), ?, ?, ?, ?, ?, ?, ?)";
        String domain = logView.getDomainName();
        String bizName = XceAdapter.DB_REG_INVITE_ACT;
        if(domain!= null && domain.indexOf("kaixin.com") > -1){
            bizName = DataAccessMgr.BIZ_KX_REG_INVITE;
        }
        OpUpdate op = new OpUpdate(sqlInsert, bizName) {

            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                if (logView.getReferer() != null && logView.getReferer().length() > 255) {
                    logView.setReferer(logView.getReferer().substring(0, 255));
                }
                if (logView.getIp() != null && logView.getIp().length() > 20) {
                    logView.setIp(logView.getIp().substring(0, 20));
                }
                ps.setString(1, logView.getIp());
                ps.setString(2, logView.getXnSessId());
                ps.setString(3, logView.getReferer());
                ps.setString(4, logView.getS1());
                ps.setString(5, logView.getS2());
                ps.setString(6, logView.getS3());
                ps.setString(7, logView.getS4());
                ps.setInt(8, logView.getActionid());
                ps.setString(9, logView.getDomainName());
                ps.setInt(10, logView.getInviterId());
            }
        };
        return DataAccessMgr.getInstance().update(op);
	}
    
    public int[] batchSave(final HashMap<Integer, RegVisitLogView> infos) throws SQLException {
        String sqlInsert = "insert into act1 (ip, xn_sess_id, referer, time, s1, s2, s3, s4, action_id, domain_name, inviter_id) values (?, ?, ?, now(), ?, ?, ?, ?, ?, ?, ?)";
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
                    RegVisitLogView logView = (RegVisitLogView)infos.get(oj);
                    if (logView.getReferer() != null && logView.getReferer().length() > 255) {
                        logView.setReferer(logView.getReferer().substring(0, 255));
                    }
                    if (logView.getIp() != null && logView.getIp().length() > 20) {
                        logView.setIp(logView.getIp().substring(0, 20));
                    }
                    ps.setString(1, logView.getIp());
                    ps.setString(2, logView.getXnSessId());
                    ps.setString(3, logView.getReferer());
                    ps.setString(4, logView.getS1());
                    ps.setString(5, logView.getS2());
                    ps.setString(6, logView.getS3());
                    ps.setString(7, logView.getS4());
                    ps.setInt(8, logView.getActionid());
                    ps.setString(9, logView.getDomainName());
                    ps.setInt(10, logView.getInviterId());
                    ps.addBatch();
                }
            }
        };
        return DataAccessMgr.getInstance().batchUpdate(op);
    }
}
