package com.xiaonei.passport.passport;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.model.Account;
import com.xiaonei.platform.core.opt.DataAccessMgr;
import com.xiaonei.platform.core.opt.OpList;
import com.xiaonei.platform.core.opt.OpUpdate;
import com.xiaonei.sns.platform.core.opt.ice.IUserPassportAdapter;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.xce.idcache.IdInfoAdapter;
import com.xiaonei.xce.userpassport.UserPassportInfo;

public class PassportServiceImpl implements IPassportService {
	private final static Log logger = LogFactory
			.getLog(PassportServiceImpl.class);
	private final static String DATA_SOURCE = "user_accounts";
	private IUserPassportAdapter passportAdapter = SnsAdapterFactory
			.getUserPassportAdapter();
	private static IPassportService _instance = new PassportServiceImpl();

	public static IPassportService getInstance() {
		return _instance;
	}

	private PassportServiceImpl() {
	}

	private class AccountInfo {
		private int userId = -1;
		private int transferId = -1;
		private long lastLoginTime = 0;
		private String domain;
		private int status = -1;
		private int safeStaus = -1;
	}
	
	@Override
	public List<Account> getMatchedAccount(String account, String password){
		List<Account> allAccounts = new ArrayList<Account>();
		try {
			List<AccountInfo> accounts = findAllMatchedAccount(account);
			if (accounts == null || accounts.size() == 0) {
				return allAccounts;
			}
			List<AccountInfo> matchPasswordAccounts = findMatchPasswordAccount(
					password, accounts);
			if (matchPasswordAccounts == null || matchPasswordAccounts.size() == 0) {
				return allAccounts;
			}
			for(AccountInfo item : matchPasswordAccounts){
				int transferId = getTransferId(item.userId, item.lastLoginTime);
				if (transferId != -1) {
					item.transferId = transferId;
				} else {
					item.transferId = item.userId;
				}
				Account accountResult = new Account(item.transferId, account, item.domain, item.status, item.userId, item.safeStaus);
				allAccounts.add(accountResult);
			}
		} catch (Exception e) {
			
		}
		return allAccounts; 
	}

	@Override
	public Account login(String account, String password) {
		try{
			long readAccountDb = System.nanoTime();
			List<AccountInfo> accounts = findAllMatchedAccount(account);
			long estimatedTime = System.nanoTime() - readAccountDb;
			if(estimatedTime > 100000000 && logger.isWarnEnabled()) {
			    logger.warn("WARN! read Account Db use time " + estimatedTime);
			}
			if (accounts == null || accounts.size() == 0) {
				return new Account(LoginStatusCode.FAILUSERNOTEXIST, account);
			}
			List<AccountInfo> matchPasswordAccounts = findMatchPasswordAccount(
					password, accounts);
			if (matchPasswordAccounts == null || matchPasswordAccounts.size() == 0) {
				return new Account(LoginStatusCode.FAILUSER, account);
			}
			AccountInfo accountInfo = getLoginUserId(matchPasswordAccounts);
			int transferId = getTransferId(accountInfo.userId,
					accountInfo.lastLoginTime);
			if (transferId != -1) {
				accountInfo.transferId = transferId;
			} else {
				accountInfo.transferId = accountInfo.userId;
			}
			Account accountResult = new Account(accountInfo.transferId, account,
					accountInfo.domain, accountInfo.status, accountInfo.userId,
					accountInfo.safeStaus);
			return accountResult;
		}catch(Exception e){
			logger.error("login fail, the user name is "+account, e);
			return new Account(LoginStatusCode.FAILUNKNOWN, account);
		}
		
	}

	@Override
	public Account login(int userId, int realUserId ,String password) {
		try{
		    Integer realId = getTransUserId(userId);
			UserPassportInfo user=passportAdapter.getUserPassport(realId);
			
			if(user==null){
				return new Account(LoginStatusCode.FAILUSERNOTEXIST, "");
			}
			//在外部已经验证过一次，这里不再验证
			if(!user.getPasswordMd5().equals(password)){
				return new Account(LoginStatusCode.FAILUSER, user.getAccount());
			}
			Account account=new Account(user.getId(), user.getAccount(), user.getDomain(), user.getStatus(), realUserId, user.getSafeStatus());
			return account;
		}catch(Exception e){
			logger.error("login fail, the user id is "+userId, e);
			return new Account(LoginStatusCode.FAILUNKNOWN, "");
		}
	
	}

	@Override
	public void saveAccount(final int userId, final String account,
			final int accountType) {
		
		try {
			OpUpdate op = new OpUpdate(IPassportService.sql_insert_user_accounts,
					DATA_SOURCE) {
				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, userId);
					ps.setString(2, account);
					ps.setInt(3, accountType);
				}

			};
			DataAccessMgr.getInstance().insert(op);
		} catch (SQLException e) {
			logger.error("saveAccount execute  error", e);
		}
	}

	@Override
	public void saveTransferAccount(final int userId, final int transferId) {
		
		try {
			OpUpdate op = new OpUpdate(IPassportService.sql_insert_user_transfers,
					DATA_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, userId);
					ps.setInt(2, transferId);
				}

			};
			DataAccessMgr.getInstance().insert(op);
		} catch (SQLException e) {
			logger.error("AdminGroupmemberDAOImpl execute add error", e);
		}
	}

	@Override
	public List<Account> getAccount(String account) {
		List<Account> result = new ArrayList<Account>();
		try{
			List<AccountInfo> accounts = this.findAllMatchedAccount(account);
			if (accounts == null || accounts.size() == 0) {
				return new ArrayList<Account>();
			}
			for (AccountInfo info : accounts) {
				Account accountInfo = getAccount(info.userId);
				result.add(accountInfo);
			}
			return result;
		}catch(Exception e){
			logger.error("getAccount error. the account is "+ account, e);
			return result;
		}
		
	}

	@Override
	public Account getAccount(long id) {
		if (id <= 0) {
			return null;
		}
		try{
			int uid = Long.valueOf(id).intValue();
			UserPassportInfo user = passportAdapter.getUserPassport(uid);
			int userId = user.getId();
			String domain = user.getDomain();
			int status = user.getStatus();
			int realId = userId;
			int safeStatus = user.getSafeStatus();
			Account accountInfo = new Account(userId, user.getAccount(), domain,
					status, realId, safeStatus);
			return accountInfo;
		}catch(Exception e){
			logger.error("getAccount method error, the id is "+id, e);
			return null;
		}
		
	}

	@Override
	public boolean updateUserAccounts(final int userId, final String account) {
		try {
			final long now=new Date().getTime();
			OpUpdate op = new OpUpdate(IPassportService.sql_update_account_last_login_time,
					DATA_SOURCE) {

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
			logger.error("updateUserAccounts execute  error", e);
			return false;
		}
	}

	@Override
	public boolean updateUserTransfers(final int userId, final int transferId) {
		try {
			final long now=new Date().getTime();
			OpUpdate op = new OpUpdate(IPassportService.sql_update_transfer_last_login_time,
					DATA_SOURCE) {

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
			logger.error("updateUserAccounts execute  error", e);
			return false;
		}
	}
	/**
	 * 根据帐号对应的所有合法的账户.以及根据这些帐号中符合密码的帐号中找到最终登陆的账户
	 * 
	 * @param accounts
	 *            所有和输入帐号相等的帐号信息
	 * @param matchPasswordAccounts
	 *            所有和帐号相同,且密码相等的帐号信息
	 * @return 最终决定可以登陆的账户
	 */
	private AccountInfo getLoginUserId(List<AccountInfo> matchPasswordAccounts) {
		try{
			if (matchPasswordAccounts == null || matchPasswordAccounts.size() == 0) {
				return null;
			}
			Collections.sort(matchPasswordAccounts, new Comparator<AccountInfo>() {

				@Override
				public int compare(AccountInfo o1, AccountInfo o2) {
					if (o1.lastLoginTime == o2.lastLoginTime) {
						return o1.userId - o2.userId;
					} else {
						if(o2.lastLoginTime-o1.lastLoginTime>0){
							return 1;
						} else{
							return -1;
						}
					}
				}

			});
			return matchPasswordAccounts.get(0);
		}catch(Exception e){
			logger.error("getLoginUserId method error", e);
			return matchPasswordAccounts.get(0);
		}
		
	}

	/***
	 * 查找所有符合此账户的id
	 * 
	 * @param account
	 *            帐号
	 * @return
	 */

	@SuppressWarnings("unchecked")
	private List<AccountInfo> findAllMatchedAccount(final String account) {
	
		try {
			OpList op = new OpList(IPassportService.sql_find_ids, DATA_SOURCE) {
				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setString(1, account);
				}

				@Override
				public Object parse(ResultSet rs) throws SQLException {
					AccountInfo info = new AccountInfo();
					info.userId = rs.getInt("user_id");
					info.lastLoginTime = rs.getLong("last_login_time");
					return info;
				}

			};
			List<AccountInfo> queryList = (List<AccountInfo>) DataAccessMgr
					.getInstance().queryList(op);
			return queryList;
		} catch (SQLException e) {
			logger.error("findAllIdOfAccount execute add error", e);
			return new ArrayList<AccountInfo>();
		}
	}

	/**
	 * 查询所有符合密码的账户信息
	 * 
	 * @param password
	 *            密码
	 * @param ids
	 *            需要查询的id串，如id1，id2，id3
	 * @return 所有符合条件的账户信息
	 */
	private List<AccountInfo> findMatchPasswordAccount(final String password,
			final List<AccountInfo> accounts) {
		List<AccountInfo> matchAccounts = new ArrayList<AccountInfo>();
		for (AccountInfo info : accounts) {
			if (fillAccountInfo(info.userId, password, info)) {
				matchAccounts.add(info);
			}
		}
		return matchAccounts;
	}
	
	private static IdInfoAdapter idInfoAdapter = new IdInfoAdapter();
	
	private int getTransUserId(int accountId){
        if (idInfoAdapter.isVirtualId(accountId)) {
            return idInfoAdapter.getRelatedTransId(accountId);
        }else{
            return accountId;
        }
    }

	private boolean fillAccountInfo(int userId, String password,
			AccountInfo info) {
		try{
			UserPassportInfo user = passportAdapter.getUserPassport(userId);
			if (user != null && user.getPasswordMd5() != null
					&& user.getPasswordMd5().equals(password)) {
				info.domain = user.getDomain();
				info.status = user.getStatus();
				info.safeStaus = user.getSafeStatus();
				info.userId = userId;
				return true;
			}
			return false;
		}catch(Exception e){
			logger.error("fillAccountInfo method error", e);
			return false;
		}
	
	}

	/**
	 * 检查是否有page切换,如果有返回page_id,否则返回-1
	 * 
	 * @param id
	 *            管理员id
	 * @param lastLoginTime
	 *            切换时间
	 * @return pageId
	 */
	@SuppressWarnings("unchecked")
	private int getTransferId(final int id, final long lastLoginTime) {
		
		try {
			OpList op = new OpList(IPassportService.sql_transfer_check, DATA_SOURCE) {

				@Override
				public void setParam(PreparedStatement ps) throws SQLException {
					ps.setInt(1, id);
				}

				@Override
				public Object parse(ResultSet rs) throws SQLException {
					long [] row=new long[2];
					row[0]=rs.getLong("transfer_id");
					row[1]=rs.getLong("last_login_time");
					return row;
				}
				

			};
			List<long[]> result = (List<long[]>) DataAccessMgr.getInstance().queryList(op);
			if (result==null||result.size() == 0) {
				return -1;
			}
			long lastLoginTransferId=-1;
			long maxLastLoginTime=-1;
			for(int i=0 ;i<result.size();i++){
				long[] transferInfo=result.get(i);
				long transferId=transferInfo[0];
				long lastLogin=transferInfo[1];
				if(lastLogin>lastLoginTime&&lastLogin>maxLastLoginTime){
					maxLastLoginTime=lastLogin;
					lastLoginTransferId=transferId;
				}
			}
			return Long.valueOf(lastLoginTransferId).intValue();
		} catch (SQLException e) {
			logger.error("getTransferId execute  error", e);
			return -1;
		}
	}

}
