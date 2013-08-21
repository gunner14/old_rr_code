package com.xiaonei.passport.login;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.model.Account;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.passport.model.LogoutResult;

/**
 * 登陆提供最外层接口
 * @author xiaodong.yuan
 * @author shuguo.zhang
 * 
 * 最底层的登录实现，以jar包的形式提供给外面调用，而不关心调用者是以何种方式（普通依赖、XOA调用）来使用这个接口
 */
public interface ICommonLoginService {
	/**
	 * 
	 * @param email
	 *            用户名
	 * @param md5Password
	 *            md5后的密码
	 * @param expirePeriod
	 *            票的有效时间，如果-1表示session有效
	 * @param reuseTicket
	 * @param site
	 *            域名，如renren.com/kaixin.com
	 * @param loginType
	 *            每个使用登陆的系统都对应一种type
	 * @param ip
	 *            最外端ip。
	 * @param context
	 *            和连接相关的参数
	 * @return
	 */
	LoginResult login(String userName, String md5Password, boolean isAutoLogin,
			String origUrl, String icode, String captchaType, String ick,
			long expirePeriod, int loginRegistrationId,
			boolean createRenRenTicket, final ILoginContext context);

	/**
	 * 登出
	 * 
	 * @param passportTicket
	 *            p票
	 * @param ticket
	 *            t票,如果存在则清除票
	 * @param loginRegistration
	 *            登陆注册号
	 * @param context
	 * @return
	 */
	LogoutResult logout(String passportTicket, String ticket,  final ILoginContext context);

	/**
	 * 根据p票换人人t票
	 * 
	 * @param userName
	 *            用户名
	 * @param passport
	 *            p票
	 * @param origUrl
	 *            origUrl
	 * @param loginType
	 *            要换票的类型
	 * @param loginRegistrationId
	 *            登陆注册id
	 * @param createRenRenTicket
	 *            是否创建人人T票
	 * @param context
	 *            登陆上下文环境
	 * @return
	 */
	LoginResult transfer(String userName, String passport, String origUrl,
			int loginType, int loginRegistrationId,
			final ILoginContext context);


	/** 根据pageId切换到相应的帐号上, 第一个参数为从哪个id切到page，第二个为要切到的id */
	public LoginResult switchAccount(String passport, String ticket, int desId, int realUserId,
			String md5Password, int loginRegistrationId, String icode, String captchaType, String ick,
			boolean createRenRenTicket, ILoginContext context);
	
	
	  /**
     * 是否显示验证码
     * 
     * @param account 帐号
     * @return 验证码级别 0为不显示,如果不是renren.com不显示
     */
    public int getCaptchaGrade( String account, final String ip);
    
    
    public LoginResult createTicketAndExecuteLogic(final Account account,
			final ILoginParameter loginParameter, int loginRegistrationId,
			long expirePeriod, boolean createRenRenTicket,
			ILoginContext context, String afterLoginKey, String failLoginKey);
}
