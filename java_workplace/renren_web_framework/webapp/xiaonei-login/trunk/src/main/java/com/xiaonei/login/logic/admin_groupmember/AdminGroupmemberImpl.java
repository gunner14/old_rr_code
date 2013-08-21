/**
 * 
 */
package com.xiaonei.login.logic.admin_groupmember;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.ArrayUtils;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Service;

import com.xiaonei.login.dao.AdminGroupmemberDAO;
import com.xiaonei.login.model.AdminGroupmember;

/**
 * AdminGroupmemberImpl <br>
 *
 * @author tai.wang@opi-corp.com Jun 18, 2010 - 10:21:08 AM
 */
@Service
public class AdminGroupmemberImpl implements IAdminGroupmember {

	protected Logger logger = LogManager.getLogger(this.getClass());

	@Autowired
	private AdminGroupmemberDAO dao;
	
	private Map<Integer,String[]> adminGroups = AdminGroupmemberDataDef.ADMIN_GROUPS;

	private volatile long updateTime = 0l;

	public  static long intervalTime = 1000l * 60l * 5l;// 5m

	/**
	 * Constructs a new instance.
	 */
	public AdminGroupmemberImpl()	{
	}

	@Override
	public String[] getIpsArarry(int userId) {
		if(logger.isDebugEnabled()){
			logger.debug("userId: "+userId);
		}
		update(this.adminGroups);
		String[] groups = adminGroups.get(Integer.valueOf(userId));
		if(groups == null ){
			String[] nu = {};
			return nu;
		}
		return groups;
	}

	@Override
	public boolean isCouple(int userId, String ip) {
		if(logger.isDebugEnabled()){
			logger.debug("userId: "+userId+", ip: "+ip);
		}
		if(null == ip){
			return false;
		}
		update(this.adminGroups);
		String[] g = getIpsArarry(userId);
		for(String iip : g){
			if(ip.equals(iip)){
				return true;
			}
		}
		return false;
	}

	/**
	 * {@inheritDoc}
	 * @see IAdminGroupmember#update(Map<Integer,String[]>)
	 */
	public void update(final Map<Integer,String[]> adminGroups){
		if ((System.currentTimeMillis() - updateTime) < intervalTime){
			if (logger.isDebugEnabled()) {
				StringBuilder buffer = new StringBuilder();
				buffer.append("updateTime: " + this.updateTime
						+ ", System.currentTimeMillis(): "
						+ System.currentTimeMillis() );
				logger.debug(buffer.toString());
			}
			return;
		}
		if (logger.isDebugEnabled()) {
			StringBuilder buffer = new StringBuilder();
			buffer.append("update start, updateTime: "+this.updateTime+", System.currentTimeMillis(): "+System.currentTimeMillis()+", ips:|");
			for(Map.Entry<Integer,String[]> e : adminGroups.entrySet()){
				buffer.append(e.getKey()).append("-");
				for(String s:  e.getValue()){
					buffer.append(s+",");
				}
				buffer.append("|==|");
			}
			logger.debug(buffer.toString());

		}
		updateTime = System.currentTimeMillis();
		synchronized(adminGroups){
			List<AdminGroupmember> list;
			try {
				//AdminGroupmemberDAO dao = Globals.getRoseAppContext().getBean(AdminGroupmemberDAO.class);
				list = dao .getAdminGroupList();
			} catch (DataAccessException e) {
				e.printStackTrace();
				return;
			}
			Map<Integer,String[]> gT = new HashMap<Integer,String[]>();
			for(AdminGroupmember member : list){
				String[] ips = member.getRestrictIp().split("\\|");
				gT.put(member.getAdminid(),ips);
			}
			adminGroups.clear();
			adminGroups.putAll(gT);
		}
		if (logger.isDebugEnabled()) {
			StringBuilder buffer = new StringBuilder();
			buffer.append("update end, updateTime: "+this.updateTime+", System.currentTimeMillis()"+System.currentTimeMillis()+", ips:|");
			for(Map.Entry<Integer,String[]> e : adminGroups.entrySet()){
				buffer.append(e.getKey()).append("-");
				for(String s:  e.getValue()){
					buffer.append(s+",");
				}
				buffer.append("|==|");
			}
			logger.debug(buffer.toString());
		}
	}

	/**
	 * {@inheritDoc}
	 * @see IAdminGroupmember#isDefinedIp(String)
	 */
	@Override
	public boolean isDefinedUserId(int userId) {
		if (logger.isDebugEnabled()){
			logger.debug(userId +" isDefiendUserId: "+(!ArrayUtils.isEmpty(getIpsArarry(userId))));
		}
		return !ArrayUtils.isEmpty(getIpsArarry(userId));
	}
}
