package com.dodoyo.invite.dao;

import com.xiaonei.platform.framework.dao.XnDAO;

/**
 * @author dream
 *         邀请邮件统计dao,产品张魏
 */
public class InviteMailStatDAO extends XnDAO {/*

    static class SingltonHolder {
        static InviteMailStatDAO instance = new InviteMailStatDAO();
    }

    private static InviteMailStatDAO getInstance() {
        return SingltonHolder.instance;
    }

    private static final String sqlInsert = "insert ignore into invite_detail_stat (userid, email_from, email_to, app_id, source_id, action_type, mail_suffix, mail_suffix_from, mail_suffix_to, enter_type,invite_type,s1,s2,s3,s4,time) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,now())";

    *//**
     * 插入一条记录，保存发送过的Email
     *
     * @param i
     * @return
     * @throws SQLException
     *//*
    public int insertRecorder(final InviteMailStat i) throws SQLException {
        if (true)
        {
            //com.dodoyo.invite.core.logic.BatchInsertUtils.getInstance().addIms(i);
            return 0;
        }
        OpUpdate op = new OpUpdate(sqlInsert, InviteDatasourceMgr.getInstance().getBizStat(this.getClass())) {
            public void setParam(PreparedStatement ps) throws SQLException {
                ps.setInt(1, i.getUserid());
                ps.setString(2, i.getEmail_from());
                ps.setString(3, i.getEmail_to());
                ps.setInt(4, i.getApp_id());
                ps.setInt(5, i.getSource_id());
                ps.setInt(6, i.getAction_type());
                ps.setString(7, i.getMail_suffix());
                ps.setString(8, i.getMailSuffixFrom());
                ps.setString(9, i.getMailSuffixTo());
                ps.setInt(10, i.getEnter_type());
                ps.setInt(11, i.getInvite_type());
                ps.setString(12, i.getS1());
                ps.setString(13, i.getS2());
                ps.setString(14, i.getS3());
                ps.setString(15, i.getS4());
            }
        };
        return DataAccessMgr.getInstance().update(op);
	}
*/}
