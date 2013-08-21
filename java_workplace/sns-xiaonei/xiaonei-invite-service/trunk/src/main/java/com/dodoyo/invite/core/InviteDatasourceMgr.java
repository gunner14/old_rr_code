package com.dodoyo.invite.core;

import com.dodoyo.invite.core.logic.LogUtils;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpiConstants;

public class InviteDatasourceMgr
{
	private InviteDatasourceMgr(){}
	
	private static InviteDatasourceMgr ds = new InviteDatasourceMgr();
	
	private static final String RENREN = "renren.com";
	
	public static InviteDatasourceMgr getInstance()
	{
		return ds;
	}
	
	/**
	 * 根据domain来选择数据源
	 * @param currentDomain
	 * @return
	 */
	public String getBizStat(Class<?> dao)
	{
		LogUtils.logDebug(dao.getName() + " getBizStat() current domain is: " 
				+ OpiConstants.domain.toString());
		if (OpiConstants.getCurrentDomain().toString().equals(RENREN))
			return DataAccessMgr.BIZ_STAT;
		
		return DataAccessMgr.BIZ_KX_STAT;//invite1.db
	}
	
	/**
	 * 取sequence的名字
	 * @param dao
	 * @return
	 */
	public String getSequenceName(Class<?> dao)
	{
		LogUtils.logDebug(dao.getName() + " getSequenceName() current domain is: " 
				+ OpiConstants.domain.toString());
		if (OpiConstants.getCurrentDomain().toString().equals(RENREN))
			return "invite_url_paras_id_seq";
		
		return "kx_invite_url_paras_id_seq";
	}
	
/*    private String getBizRegInvite(String currentDomain)
	{
		if (OpiConstants.domain.toString().equals(RENREN))
			return DataAccessMgr.BIZ_REG_INVITE;
		
		return DataAccessMgr.BIZ_KX_REG_INVITE;
	}
*/	
	/**
	 * 根据domain选择email数据源
	 * @param dao
	 * @return
	 */
	public String getBizInviteMail(Class<?> dao)
	{
		LogUtils.logDebug(dao.getName() + " getBizInviteMail() current domain is: " 
				+ OpiConstants.domain.toString());
		if (OpiConstants.domain.toString().equals(RENREN))
			return DataAccessMgr.BIZ_IMMEDIATE_EMAIL;
		
		return DataAccessMgr.BIZ_INVITE_LETTER;
	}
}