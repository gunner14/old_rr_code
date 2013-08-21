package com.xiaonei.passport.dbwrite;

public interface AccountService {
	
	String sql_select_user_accounts="select user_id from user_accounts where user_id=? and account=?";
	String sql_insert_user_accounts="insert into user_accounts(user_id, account, account_type, last_login_time) values(? ,? ,?, 0)";
	String sql_select_transfer_accounts="select user_id from user_transfers where user_id=? and transfer_id=?";
	String sql_insert_user_transfers="insert into user_transfers(user_id, transfer_id, last_login_time) values(? ,? ,0)";
	String sql_update_transfer_last_login_time="update user_transfers set last_login_time=? where user_id=? and transfer_id=?";
	String sql_update_account_last_login_time="update user_accounts set last_login_time=? where user_id=? and account=?";
	String sql_update_account_account="update user_accounts set account=? , account_type=? where user_id=? and account=?";
	String sql_delete_transfer="delete from user_transfers  where user_id=? and transfer_id=?";
	String sql_delete_account="delete from user_accounts  where user_id=? and account=?";
	public enum AccountType{
		EMAIL(1), TELEPHONE(2), ID(3);
		private int type;
		private AccountType(int type){
			this.type=type;
		}
		public int getValue(){
			return type;
		}
	}
	/**
	 * 增加帐户信息
	 * @param userId 用户id
	 * @param account 帐号
	 * @param accountType 帐号类型
	 * @return
	 */
	public boolean addUserAccount(int userId, String account, AccountType accountType);
	
	/**
	 * 对于page增加管理员和page对应关系
	 * @param userId 管理员id
	 * @param transferId page id
	 * @return
	 */
	public boolean addUserTransfer(int userId, int transferId);
	
	/**
	 * 更新user_transfers表,更新最后登陆时间字段
	 * @param userId
	 * @return
	 */
	public boolean updateUserTransfersLastLoginTime(int userId, int transferId);
	

	/**
	 * 更新user_accounts表,更新最后登陆时间字段
	 * @param userId
	 * @return
	 */
	public boolean updateUserAccountsLastLoginTime(int userId, String account);
	

	/**
	 * 更新user_accounts表,更新帐号
	 * @param userId
	 * @return
	 */
	public boolean updateUserAccounts(int userId, String origAccount, String newAccount, AccountType newAccountType);
	
	/**
	 * 更新user_accounts表,更新帐号
	 * @param userId
	 * @return
	 */
	public boolean deleteTransferAccount(int userId, int transferId);
	
	
	/**
	 * 更新user_accounts表,更新帐号
	 * @param userId
	 * @return
	 */
	public boolean deleteUserAccounts(int userId, String account);
}
