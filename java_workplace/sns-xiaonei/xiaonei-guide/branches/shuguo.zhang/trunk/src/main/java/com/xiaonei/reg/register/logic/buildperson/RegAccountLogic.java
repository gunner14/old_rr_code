package com.xiaonei.reg.register.logic.buildperson;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.apache.commons.lang.StringUtils;

import com.xiaonei.platform.core.cache.MemCacheFactory;
import com.xiaonei.platform.core.cache.MemCacheKeys;
import com.xiaonei.platform.core.cache.MemCacheManager;
import com.xiaonei.platform.core.model.User;
import com.xiaonei.platform.core.opt.ice.WUserAdapter;
import com.xiaonei.platform.core.opt.ice.WUserDescAdapter;
import com.xiaonei.platform.core.utility.Encoder;
import com.xiaonei.platform.core.utility.StringFormat;
import com.xiaonei.platform.core.utility.VerifyCode;
import com.xiaonei.reg.common.RegLogger;
import com.xiaonei.reg.common.constants.Globals;
import com.xiaonei.reg.common.users.UserAdapter;
import com.xiaonei.reg.register.dao.UserDAO;
import com.xiaonei.reg.register.dao.UserMailDAO;
import com.xiaonei.reg.register.dao.UserRegisterIpDAO;
import com.xiaonei.reg.register.logic.TimeCost;
import com.xiaonei.reg.register.logic.additional.MultiAccountsMapLogic;
import com.xiaonei.reg.register.logic.additional.RegUserConfigLogic;
import com.xiaonei.reg.register.model.UserMail;
import com.xiaonei.reg.register.model.UserRegisterIp;
import com.xiaonei.reg.register.pojo.RegAccountPojo;
import com.xiaonei.sns.platform.core.opt.ice.impl.SnsAdapterFactory;
import com.xiaonei.sns.platform.core.opt.ice.model.UserBorn;
import com.xiaonei.sns.platform.core.opt.ice.model.UserDoing;
import com.xiaonei.sns.platform.core.opt.ice.model.UserStage;
import com.xiaonei.xce.usercontact.UserContactInfo;
import com.xiaonei.xce.userfav.UserFavInfo;
import com.xiaonei.xce.username.UserNameInfo;
import com.xiaonei.xce.userpassport.UserPassportInfo;
import com.xiaonei.xce.userright.UserRightInfo;
import com.xiaonei.xce.userstate.UserStateInfo;
import com.xiaonei.xce.usertime.UserTimeInfo;
import com.xiaonei.xce.userurl.UserUrlInfo;

/**
 * 注册时用户帐户管理 <br>
 * 负责创建一个信息账户
 * 
 * @author wang-tai
 */
public class RegAccountLogic {

	private static RegAccountLogic _instance = new RegAccountLogic();
	public static final String MEM_CACHE_NEW_INVITEE_KEY = "mem_cache_rr_reg_vc_";
	
	private final RegLogger logger = RegLogger.getLogger(RegAccountLogic.class
			.getName(), this.getClass().getName());
	private final RegLogger xcelogger = RegLogger.getLogger("xceStat", this.getClass().getName());
	public static RegAccountLogic getInstance() {
		return _instance;
	}

	private RegAccountLogic() {
		super();
	}

	/**
	 * 创建一个帐号<br>
	 * 包括创建用户信息，用户基本资料，用户的描述资料
	 * 
	 * @param account
	 * @return User
	 * @throws Exception
	 */
	public User create(final RegAccountPojo account) throws Exception,
			Ice.LocalException {
		final List<Thread> threadList = new ArrayList<Thread>();
		final TimeCost cost = TimeCost.begin();
		final int userId = getNewUserId();
		cost.endStep();

		// blow methods are async
		final Thread tLC = new CreateOceObjectThread("incLoginCount", userId,
				logger) {
			@Override
			protected void work() {
				incLoginCount(userId);
			}
		};
		threadList.add(tLC);

		final Thread tVC = new CreateOceObjectThread("setVerifyCode", userId,
				logger) {
			@Override
			protected void work() {
				setVerifyCode(userId);
			}
		};
		threadList.add(tVC);

		final Thread tUN = new CreateOceObjectThread("UserName", userId, logger) {
			@Override
			protected void work() {
				createUserName(account, userId);
			}
		};
		threadList.add(tUN);

		final Thread tUR = new CreateOceObjectThread("UserRight", userId,
				logger) {
			@Override
			protected void work() {
				createUserRight(account, userId);
			}
		};
		threadList.add(tUR);

		final Thread tUS = new CreateOceObjectThread("UserState", userId,
				logger) {
			@Override
			protected void work() {
				createUserState(account, userId);
			}
		};
		threadList.add(tUS);

		//2010-01-20 UserBasic注释掉，不再使用了
		/*final Thread tUB = new CreateOceObjectThread("UserBasic", userId,
				logger) {
			@Override
			protected void work() {
				createUserBasic(account, userId);
			}
		};
		threadList.add(tUB);*/
		

		final Thread tUUI = new CreateOceObjectThread("UserUrlInfo", userId,
				logger) {
			@Override
			protected void work() {
				createUserUrlInfo(account, userId);
			}
		};
		threadList.add(tUUI);

		final Thread tUT = new CreateOceObjectThread("UserTime", userId, logger) {
			@Override
			protected void work() {
				createUserTime(account, userId);
			}
		};
		threadList.add(tUT);

		final Thread tUC = new CreateOceObjectThread("UserContact", userId,
				logger) {
			@Override
			protected void work() {
				createUserContact(account, userId);
			}
		};
		threadList.add(tUC);

		//2010.01.26 应赵宁需要，注册的时候可以去掉UserFav的操作
		/*final Thread tUF = new CreateOceObjectThread("UserFav", userId, logger) {
			@Override
			protected void work() {
				createUserFav(account, userId);
			}
		};
		threadList.add(tUF);
		*/

		final Thread tUM = new CreateOceObjectThread("UserMail", userId, logger) {
			@Override
			protected void work() {
				setUserMail(account, userId);
			}
		};
		threadList.add(tUM);

		final Thread tURI = new CreateOceObjectThread("UserRegisterIp", userId,
				logger) {
			@Override
			protected void work() {
				setUserRegisterIp(account, userId);
			}
		};
		threadList.add(tURI);

		final Thread tMA = new CreateOceObjectThread("MultiAccounts", userId,
				logger) {
			@Override
			protected void work() {
				setMultiAccounts(account, userId);
			}
		};
		threadList.add(tMA);

		final Thread tUCf = new CreateOceObjectThread("UserConfig", userId,
				logger) {
			@Override
			protected void work() {
				setUserConfig(userId);
			}
		};
		threadList.add(tUCf);

		final Thread tUBf = new CreateOceObjectThread("setUserBorn", userId,
                logger) {
            @Override
            protected void work() {
                setUserBorn(account, userId);
            }
        };
        threadList.add(tUBf);
        
        final Thread tUDf = new CreateOceObjectThread("UserDoing", userId,
                logger) {
            @Override
            protected void work() {
                setUserDoing(account, userId);
            }
        };
        threadList.add(tUDf);
        
        final Thread tUSf = new CreateOceObjectThread("UserStage", userId,
                logger) {
            @Override
            protected void work() {
                setUserStage(account, userId);
            }
        };
        threadList.add(tUSf);
        
        
		for (final Thread t : threadList) {
			t.start();
		}
		for (final Thread t : threadList) {
			try {
				t.join();
			} catch (final InterruptedException e) {
				e.printStackTrace();
				logger.error(((CreateOceObjectThread) t).getOceObjectName()
						+ " " + ((CreateOceObjectThread) t).getId()
						+ " InterruptedException", e);
			}
		}
		
		// sync create passport
		logger.info("create UserPassport: " + userId + " start");
		long timeCostL = -1l;
		try {
			createUserPassport(account, userId);
			timeCostL = cost.endStep();
		} catch (final Ice.LocalException e) {
			logger.error("create UserPassport: " + userId + " exception", e);
			xcelogger.error("create UserPassport: " + userId + " exception", e);
			throw e;
		} catch (final Exception e) {
			logger.error("create UserPassport: " + userId + " exception", e);
			xcelogger.error("create UserPassport: " + userId + " exception", e);
			throw e;
		} finally {
			logger.info("create UserPassport: " + userId + " over, time cost: "
					+ timeCostL);
			cost.endFinally();
		}
		
		Ice.UnknownUserException ex = null;
		for (int i = 0; i < 30; i++) {
			try {
				return UserAdapter.getUser(userId);
			} catch (final Ice.UnknownUserException e) {
				ex = e;
				try {
					Thread.sleep(30);
				} catch (Exception e1) {
				}
			}
		}
		if (null != ex) {
			throw ex;
		} else {
			throw new RuntimeException("NoSuchUserException");
		}
	}

	private void setVerifyCode(final int userId) {
	    String vc = VerifyCode.getVerifyCode(10);
        MemCacheManager mem = null;
        try {
            mem = MemCacheFactory.getInstance().getManagerByPoolName(MemCacheKeys.pool_default);
            mem.set(MEM_CACHE_NEW_INVITEE_KEY + userId, vc, (int) new Date(MemCacheKeys.hour * 2)
                    .getTime());
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
		UserAdapter.get(userId).setVerifyCode(userId,
		        vc);
	}

	private void createUserState(final RegAccountPojo account, final int userId) {
		final UserStateInfo userStateInfo = new UserStateInfo();
		userStateInfo.setId(userId);
		userStateInfo.setStar(User.usrNotSelected);
		try {
			WUserAdapter.getInstance().getStateAdapter().createUserState(
					userId, userStateInfo);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("create UserStateInfo: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void incLoginCount(final int userId) {
		try {
			WUserAdapter.getInstance().getLoginCountAdapter()
					.createUserLoginCount(userId);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("incLoginCount: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void createUserRight(final RegAccountPojo ac, final int userId) {
		final UserRightInfo userRightInfo = new UserRightInfo();
		userRightInfo.setId(userId);
		try {
			WUserAdapter.getInstance().getRightAdapter().createUserRight(
					userId, userRightInfo);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("create UserRightInfo: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void createUserName(final RegAccountPojo ac, final int userId) {
		final UserNameInfo userNameInfo = new UserNameInfo();
		userNameInfo.setId(userId);
		userNameInfo.setName(ac.getName());
		try {
			WUserAdapter.getInstance().getNameAdapter().createUserName(userId,
					userNameInfo);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("create UserNameInfo: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void createUserPassport(final RegAccountPojo ac, final int userId)
			throws Ice.UnknownException {
		final UserPassportInfo userPassportInfo = new UserPassportInfo();
		userPassportInfo.setId(userId);
		userPassportInfo.setAccount(StringUtils.trimToEmpty(ac.getAccount())
				.toLowerCase());
		userPassportInfo.setDomain(Globals.domain.toString());
		userPassportInfo.setPasswordMd5(Encoder.encode(ac.getPwd().trim()));
		userPassportInfo.setStatus(User.statusActivateVerified);

		try {
			WUserAdapter.getInstance().getPassportAdapter().createUserPassport(
					userId, userPassportInfo);
		} catch (final Ice.TimeoutException e) {
			e.printStackTrace();
		}
	}

	private int getNewUserId() throws SQLException {
		final int userId = UserDAO.getInstance().getNext();
		return userId;
	}


	@SuppressWarnings("unused")
    private void createUserFav(final RegAccountPojo ac, final int userId) {
		final UserFavInfo userFavInfo = new UserFavInfo();
		userFavInfo.setId(userId);
		try {
			WUserDescAdapter.getInstance().getFavAdapter().createUserFav(
					userId, userFavInfo);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("create UserFavInfo: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void createUserContact(final RegAccountPojo ac, final int userId) {
		final UserContactInfo userContactInfo = new UserContactInfo();
		userContactInfo.setId(userId);
		userContactInfo.setMsn(ac.getMsn());
		try {
			WUserDescAdapter.getInstance().getContactAdapter()
					.createUserContact(userId, userContactInfo);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("create UserContactInfo: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void createUserTime(final RegAccountPojo ac, final int userId) {
		final UserTimeInfo userTimeInfo = new UserTimeInfo();
		final Date dt = new Date();
		userTimeInfo.setId(userId);
		userTimeInfo.setLastLoginTime(dt);
		userTimeInfo.setLastLogoutTime(dt);
		userTimeInfo.setRegisterTime(dt);
		try {
			UserAdapter.getTimeAdapter().createUserTime(userId, userTimeInfo);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("create UserTimeInfo: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void createUserUrlInfo(final RegAccountPojo ac, final int userId) {
		final UserUrlInfo userUrlInfo = new UserUrlInfo();
		userUrlInfo.setId(userId);
		// 根据性别设置头像
		if ("女生".equals(ac.getGender())) {
			userUrlInfo.setHeadUrl(Globals.userWomenHeadUrl);
			userUrlInfo.setLargeUrl(Globals.userWomenMainUrl);
			userUrlInfo.setMainUrl(Globals.userWomenMainUrl);
			userUrlInfo.setTinyUrl(Globals.userWomenTinyUrl);
		} else {
			userUrlInfo.setHeadUrl(Globals.userMenHeadUrl);
			userUrlInfo.setLargeUrl(Globals.userMenMainUrl);
			userUrlInfo.setMainUrl(Globals.userMenMainUrl);
			userUrlInfo.setTinyUrl(Globals.userMenTinyUrl);
		}
		if (StringUtils.isNotEmpty(ac.getMainUrl())
				&& StringUtils.isNotEmpty(ac.getHeadUrl())
				&& StringUtils.isNotEmpty(ac.getLargeUrl())
				&& StringUtils.isNotEmpty(ac.getTinyUrl())) {
			userUrlInfo.setHeadUrl(ac.getHeadUrl());
			userUrlInfo.setLargeUrl(ac.getLargeUrl());
			userUrlInfo.setMainUrl(ac.getMainUrl());
			userUrlInfo.setTinyUrl(ac.getTinyUrl());
		}
		try {
			UserAdapter.getUrlAdapter().createUserUrl(userId, userUrlInfo);
		} catch (final Ice.TimeoutException e) {
		    xcelogger.error("create UserUrlInfo: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void setUserMail(final RegAccountPojo ac, final int userId) {
		UserMail userMail = new UserMail();
		userMail.setId(userId);
		if (StringFormat.isValidEmailAddr(ac.getAccount())) {
			userMail.setCorrespond_mail(ac.getAccount());
		}
		try {
			UserMailDAO.getInstance().setUserMail(userMail);
		} catch (SQLException e) {
		    xcelogger.error("create UserMail: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void setUserRegisterIp(final RegAccountPojo ac, final int userId) {
		UserRegisterIp userRegisterIp = new UserRegisterIp();
		userRegisterIp.setId(userId);
		userRegisterIp.setRegister_ip(ac.getIp());
		try {
			UserRegisterIpDAO.getInstance().setRegisterIp(userRegisterIp);
		} catch (SQLException e) {
		    xcelogger.error("create UserRegisterIp: " + userId + " exception", e);
			e.printStackTrace();
		}
	}

	private void setMultiAccounts(final RegAccountPojo ac, final int userId) {
		MultiAccountsMapLogic.getInstance().setAccount(ac.getAccount(), userId);
	}

	private void setUserConfig(final int userId) {
		RegUserConfigLogic.getInstance().doConfig(userId);
	}
	
	private void setUserBorn(final RegAccountPojo ac, final int userId){
	    UserBorn ub = new UserBorn();
	    ub.setId(userId);
	    ub.setBirthYear(ac.getBirthYear());
	    ub.setBirthMonth(ac.getBirthMonth());
	    ub.setBirthDay(ac.getBirthDay());
	    
	    ub.setGender(ac.getGender());
	    
	    try {
            SnsAdapterFactory.getUserBornAdapter().createUserBorn(userId, ub);
        } catch (RuntimeException e) {
            xcelogger.error("create UserBorn: " + userId + " exception", e);
            e.printStackTrace();
        }
	}
	
	private void setUserDoing(final RegAccountPojo ac, final int userId){
	    UserDoing ud = new UserDoing();
	    ud.setId(userId);
	    try {
            SnsAdapterFactory.getUserDoingAdapter().createUserDoing(userId, ud);
        } catch (RuntimeException e) {
            xcelogger.error("create UserDoing: " + userId + " exception", e);
            e.printStackTrace();
        }
	}
	
	private void setUserStage(final RegAccountPojo ac, final int userId){
	    UserStage us = new UserStage();
        us.setId(userId);
        us.setStage(ac.getStage());
        try {
            SnsAdapterFactory.getUserStageAdapter().createUserStage(userId, us);
        } catch (RuntimeException e) {
            xcelogger.error("create UserStage: " + userId + " exception", e);
            e.printStackTrace();
        }
	}
}
