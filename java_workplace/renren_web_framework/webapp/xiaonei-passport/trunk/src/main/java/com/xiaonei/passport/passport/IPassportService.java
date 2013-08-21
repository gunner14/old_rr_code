package com.xiaonei.passport.passport;


import java.util.List;

import com.xiaonei.passport.model.Account;

public interface IPassportService {
	
	String sql_find_ids="select user_id,last_login_time from user_accounts where account=?";
	
	String sql_transfer_check="select transfer_id, last_login_time from user_transfers where user_id =? ";
	
	
	String sql_update_account_last_login_time="update user_accounts set last_login_time=? where user_id=? and account=?";
	
	String sql_update_transfer_last_login_time="update user_transfers set last_login_time=? where user_id=? and transfer_id=?";
	
	String sql_insert_user_accounts="insert into user_accounts(user_id, account, account_type, last_login_time) values(? ,? ,?, 0)";
	
	String sql_insert_user_transfers="insert into user_transfers(user_id, transfer_id, last_login_time) values(? ,? ,0)";
	
	/**
	 * 获取和此account匹配的所有记录
	 * @param account
	 * @param password
	 * @return
	 */
	public List<Account> getMatchedAccount(String account, String password);
	
	/**
	 * 获取account对应的所有id,并一一验证密码,从密码正确的id中获取最近登录的那个返回.如果
	 * 上次登陆的是page用户,则返回pageId
	 * @param account 用户名
	 * @param password 密码
	 * @return
	 */
	public Account login(String account, String password);
	
	/**
	 * 根据id登陆，如果是以管理员身份登陆。则需要管理员id。
	 * @param userId      登陆用户id。
	 * @param transferId 如过普通用户登陆，其值=userId。如果以page管理员身份登陆，则需要管理员id
	 * @param password
	 * @return
	 */
	public Account login(int userId, int realUserId ,String password);
	
	/**
	 * 保存账户信息
	 * @param userId 用户id
	 * @param account 帐号
	 * @param accountType 帐号类型
	 */
	public void saveAccount(int userId, String account, int accountType);
	

	/**
	 * 保存账户信息
	 * @param userId 用户id
	 * @param account 帐号
	 * @param accountType 帐号类型
	 */
	public void saveTransferAccount(int userId, int transferId);
	
	/**
	 * 根据帐号，获取同名帐号信息
	 * @param email
	 * @return
	 */
	public java.util.List<Account> getAccount(String email);
	
	/**
	 * 根据id获取帐户信息.不能够区分realId和userId
	 * @param id
	 * @return
	 */
	public Account getAccount(long id);
	
	/**
	 * 更新user_accounts表,更新最后登陆时间字段
	 * @param userId
	 * @return
	 */
	public boolean updateUserAccounts(int userId, String account);
	
	/**
	 * 更新user_transfers表,更新最后登陆时间字段
	 * @param userId
	 * @return
	 */
	public boolean updateUserTransfers(int userId, int transferId);
}
