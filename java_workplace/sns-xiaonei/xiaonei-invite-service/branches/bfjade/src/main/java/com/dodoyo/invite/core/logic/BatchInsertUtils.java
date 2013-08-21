package com.dodoyo.invite.core.logic;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.HashSet;
import java.util.Set;

import com.dodoyo.invite.core.InviteDatasourceMgr;
import com.dodoyo.invite.core.entity.InviteMailStat;
import com.dodoyo.invite.core.entity.InviteUrlParas;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpUniq;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.platform.core.opt.OpiConstants;

public class BatchInsertUtils
{
    private static Set<InviteUrlParas> datasIUP = new HashSet<InviteUrlParas>();

    private static Set<InviteMailStat> datasIMS = new HashSet<InviteMailStat>();
    
    private static BatchInsertUtils biu = new BatchInsertUtils();
    
    private static final int MAX_BATCH_COUNT = 10;
    
    private static long LAST_BATCH_OP_TIME_IUP = 0L; 

    private static long LAST_BATCH_OP_TIME_IMS = 0L; 
    
    private static final long MAX_BATCH_OP_INTERVAL = 5000L;
    
    /** IUP insert related */
    private static final String sqlInsert = "insert ignore into invite_url_paras (id, uuid, user_id, email_from, email_to, ss, app_id, app_token, md5, extar_paras, buddy_group, code, rt, time) values(?,?,?,?,?,?,?,?,?,?,?,?,?,now())";
    
    private int getNext() throws SQLException 
    {
        final String sqlGetNext = "select nextval('"+InviteDatasourceMgr.getInstance().getSequenceName(this.getClass())+"')";
        OpUniq op = new OpUniq(sqlGetNext, DataAccessMgr.BIZ_IDSEQUENCE);
        return DataAccessMgr.getInstance().queryId(op);
    }
    
    private int [] insertIups(final Set<InviteUrlParas> midDatas) throws Exception
    {
        OpUpdate op = new OpUpdate(sqlInsert, InviteDatasourceMgr.getInstance().getBizStat(this.getClass())) {
            @Override
            public void setParam(PreparedStatement ps) throws SQLException {
                for (InviteUrlParas u : midDatas)
                {
                    int nextId = getNext();
                    LogUtils.logInfo("insertIups getnext id for invite_url_paras id = " + nextId);
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
                    ps.addBatch();
                }

            }
        };
        return DataAccessMgr.getInstance().batchUpdate(op);
    }
    
    /** Send mail record stat related */
    private static final String sqlInsertStat = "insert ignore into invite_detail_stat (userid, email_from, email_to, app_id, source_id, action_type, mail_suffix, mail_suffix_from, mail_suffix_to, enter_type,invite_type,s1,s2,s3,s4,time) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,now())";

    private int [] insertImses(final Set<InviteMailStat> midDatas) throws SQLException {
        OpUpdate op = new OpUpdate(sqlInsertStat, InviteDatasourceMgr.getInstance().getBizStat(this.getClass())) {
            public void setParam(PreparedStatement ps) throws SQLException {
                for (InviteMailStat i : midDatas)
                {
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
                    ps.addBatch();
                }
            }
        };
        return DataAccessMgr.getInstance().batchUpdate(op);
    }
    
    public static BatchInsertUtils getInstance()
    {
        return biu;
    }
    
    /**
     * Add iup insert task to task collection
     * @param iup
     */
    public synchronized void addIup(InviteUrlParas iup)
    {
        if (iup != null)
            datasIUP.add(iup);
        int currentDataSize = datasIUP.size();
        LogUtils.logInfo("current datasIUP size = " + currentDataSize);
        if (datasIUP.size() >= MAX_BATCH_COUNT || System.currentTimeMillis() - LAST_BATCH_OP_TIME_IUP > MAX_BATCH_OP_INTERVAL)
        {
            final Set<InviteUrlParas> midDatas = new HashSet<InviteUrlParas>();
            midDatas.addAll(datasIUP);
            datasIUP.clear();
            final String domain = OpiConstants.getCurrentDomain();
            Runnable task = new Runnable()
            {
                @Override
                public void run() 
                {
                    OpiConstants.setCurrentDomain(domain);
                    try 
                    {
                        insertIups(midDatas);
                        midDatas.clear();
                    } 
                    catch (Exception e) 
                    {
                        e.printStackTrace(System.err);
                    }
                }
            };
            LogUtils.logInfo("Launch a task for batch add iup ...., task count = " + currentDataSize);
            if (midDatas.size() > 0)
                RunnableTaskExecutor.getInstance().runTask(task);
            LAST_BATCH_OP_TIME_IUP = System.currentTimeMillis();
        }
        
    }
    
    /**
     * Add ims insert task to task collection
     * @param ims
     */
    public synchronized void addIms(InviteMailStat ims)
    {
        if (ims != null)
            datasIMS.add(ims);
        int currentDataSize = datasIMS.size();
        LogUtils.logInfo("current datasIMS size = " + currentDataSize);
        if (datasIMS.size() >= MAX_BATCH_COUNT  || System.currentTimeMillis() - LAST_BATCH_OP_TIME_IMS > MAX_BATCH_OP_INTERVAL)
        {
            final Set<InviteMailStat> midDatas = new HashSet<InviteMailStat>();
            midDatas.addAll(datasIMS);
            datasIMS.clear();
            final String domain = OpiConstants.getCurrentDomain();
            Runnable task = new Runnable()
            {
                @Override
                public void run() 
                {
                    OpiConstants.setCurrentDomain(domain);
                    try 
                    {
                        insertImses(midDatas);
                        midDatas.clear();
                    } 
                    catch (Exception e) 
                    {
                        e.printStackTrace(System.err);
                    }
                }
            };
            LogUtils.logInfo("Launch a task for batch add ims ...., task count = " + currentDataSize);
            if (midDatas.size() > 0)
                RunnableTaskExecutor.getInstance().runTask(task);
            LAST_BATCH_OP_TIME_IMS = System.currentTimeMillis();
        }
        
    }
}