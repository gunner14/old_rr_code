package com.xiaonei.passport.logic.adim_groupmember;

import java.util.Map;

/**
 * 
 * 
 * @author Wang Tai (tai.wang@opi-corp.com)
 */
public interface IAdminGroupmember {

	/**
	 * 获得对应ip数组
	 * 
	 * @param userId
	 * @return 没有数据返回长度0
	 */
	public String[] getIpsArarry(int userId);

	/**
	 * 检查用户id和ip是否匹配
	 * 
	 * @param userId
	 * @param ip
	 * @return boolean
	 */
	public boolean isCouple(int userId, String ip);


	/**
	 * 更新内存中的adminGroups
	 * 
	 * @param adminGroups
	 */
	public void update(final Map<Integer,String[]> adminGroups);

	/**
	 * 判断用户ID是否在列表中
	 * 
	 * @param userId
	 * @return boolean
	 */
	public boolean isDefinedUserId(int userId);
}
