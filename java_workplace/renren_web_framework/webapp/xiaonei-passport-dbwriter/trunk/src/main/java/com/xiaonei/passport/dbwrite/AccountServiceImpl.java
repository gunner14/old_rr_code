package com.xiaonei.passport.dbwrite;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Date;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.dbwrite.AccountService.AccountType;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;

public class AccountServiceImpl implements AccountService {
	private Log logger = LogFactory.getLog(AccountServiceImpl.class);
	private String DATABASE_SOURCE=DataAccessMgr.BIZ_USER_COMMON;
	private static AccountService _instance=new AccountServiceImpl();
	private AccountServiceImpl(){}
	public static AccountService getInstance(){
		return _instance;
	}
	@Override
	public boolean addUserAccount(final int userId, final String account,
			final AccountType accountType) {
		try {
			if (existAccount(userId, account)) {
				return true;
			}
			OpUpdate op = new OpUpdate(AccountService.sql_insert_user_accounts,
					DATABASE_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, userId);
					ps.setString(2, account);
					ps.setInt(3, accountType.getValue());
				}

			};
			return DataAccessMgr.getInstance().insert(op);
		} catch (Exception e) {
			logger.error("addUserAccount error", e);
			return false;
		}

	}

	@Override
	public boolean addUserTransfer(final int userId, final int transferId) {
		try {
			if (existTransfer(userId, transferId)) {
				return true;
			}
			OpUpdate op = new OpUpdate(AccountService.sql_insert_user_transfers,
					DATABASE_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, userId);
					ps.setInt(2, transferId);
				}

			};
			return DataAccessMgr.getInstance().insert(op);
		} catch (Exception e) {
			logger.error("addUserTransfer error", e);
			return false;
		}
	}
	@Override
	public boolean updateUserAccountsLastLoginTime(final int userId, final String account) {
		try {
			if (!existAccount(userId, account)) {
				return false;
			}
			final long now=new Date().getTime();
			OpUpdate op = new OpUpdate(AccountService.sql_update_account_last_login_time,
					DATABASE_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setLong(1, now);
					ps.setInt(2, userId);
					ps.setString(3, account);
				}

			};
			DataAccessMgr.getInstance().update(op);
			return true;
		} catch (SQLException e) {
			logger.error("updateUserAccountsLastLoginTime execute  error", e);
			return false;
		}
	}

	@Override
	public boolean updateUserAccounts(final int userId, final String origAccount,
			final String newAccount, final AccountType newAccountType) {
		try {
			if (!existAccount(userId, origAccount)) {
				return false;
			}
			OpUpdate op = new OpUpdate(AccountService.sql_update_account_account,
					DATABASE_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setString(1, newAccount);
					ps.setInt(2, newAccountType.getValue());
					ps.setInt(3, userId);
					ps.setString(4, origAccount);
				}

			};
			DataAccessMgr.getInstance().update(op);
			return true;
		} catch (SQLException e) {
			logger.error("updateUserAccounts execute  error", e);
			return false;
		}
	}
	@Override
	public boolean updateUserTransfersLastLoginTime(final int userId, final int transferId) {
		try {
			if(!existTransfer(userId, transferId)){
				return false;
			}
			final long now=new Date().getTime();
			OpUpdate op = new OpUpdate(AccountService.sql_update_transfer_last_login_time,
					DATABASE_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setLong(1, now);
					ps.setInt(2, userId);
					ps.setInt(3, transferId);
				}

			};
			DataAccessMgr.getInstance().update(op);
			return true;
		} catch (SQLException e) {
			logger.error("updateUserTransfersLastLoginTime execute  error", e);
			e.printStackTrace();
			return false;
		}
	}
	@Override
	public boolean deleteTransferAccount(final int userId, final int transferId) {
		try {
			if(!existTransfer(userId, transferId)){
				return true;
			}
			OpUpdate op = new OpUpdate(AccountService.sql_delete_transfer,
					DATABASE_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, userId);
					ps.setInt(2, transferId);
				}

			};
			DataAccessMgr.getInstance().update(op);
			return true;
		} catch (SQLException e) {
			logger.error("deleteTransferAccount execute  error", e);
			e.printStackTrace();
			return false;
		}
	}
	/**
	 * 某用户是否已经存在对应的帐号
	 * @param userId 用户id
	 * @param account 帐号
	 * @return
	 */
	private boolean existAccount(final int userId, final String account){
		OpList op =new OpList(AccountService.sql_select_user_accounts, DATABASE_SOURCE){

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				int userId=rs.getInt("user_id");
				return userId;
			}
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setString(2, account);
			}
			
		};
		try {
			List<?> list=DataAccessMgr.getInstance().queryList(op);
			if(list==null||list.size()==0){
				return false;
			}
			return true;
		} catch (SQLException e) {
			logger.error("existAccount execute  error", e);
			return false;
		}
	}
	
	/**
	 * 是否存在管理员和transfer的对应关系
	 * @param userId 用户id
	 * @param transferId transfer id
	 * @return 
	 */
	private boolean existTransfer(final int userId, final int transferId){
		OpList op =new OpList(AccountService.sql_select_transfer_accounts, DATABASE_SOURCE){

			@Override
			public Object parse(ResultSet rs) throws SQLException {
				int userId=rs.getInt("user_id");
				return userId;
			}
			@Override
			public void setParam(PreparedStatement ps) throws SQLException {
				ps.setInt(1, userId);
				ps.setInt(2, transferId);
			}
			
		};
		try {
			List<?> list=DataAccessMgr.getInstance().queryList(op);
			if(list==null||list.size()==0){
				return false;
			}
			return true;
		} catch (SQLException e) {
			logger.error("existAccount execute  error", e);
			return false;
		}
	}
	public static void main(String[] args) {
		AccountService service=new AccountServiceImpl();
		boolean success=service.deleteTransferAccount(1, 2);
		System.out.println(success);
		//success=service.addUserTransfer(1, 2);
		//System.out.println(success);
	}
	@Override
	public boolean deleteUserAccounts(final int userId ,final String account){// TODO Auto-generated method stub
			try {
				if(!existAccount(userId, account)){
					return true;
				}
				OpUpdate op = new OpUpdate(AccountService.sql_delete_account,
						DATABASE_SOURCE) {

					@Override
					public void setParam(PreparedStatement ps) throws SQLException {
						ps.setInt(1, userId);
						ps.setString(2, account);
					}

				};
				DataAccessMgr.getInstance().update(op);
				return true;
			} catch (SQLException e) {
				logger.error("deleteUserAccounts execute  error", e);
				e.printStackTrace();
				return false;
			}
	}
	
}
