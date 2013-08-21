package com.renren.passport.service;

import java.util.List;

/**
 * 这个接口是为了实现一种兼容模式（xoaService和普通service）而做的一个顶层接口
 * 其接口定义必须与LoginXoaService完全一致
 * @author shuguo.zhang
 * 
 *
 */

public interface ILoginService {
	/**
	 * 提供登陆服务,登陆成功后返回登陆结果
	 * @param request 登陆需要的参数.主要包含如下内容
	 * <p>request</p>
	 * 		<li>userName:登陆用户户名</li>
	 * 		<li>md5Password:登陆的md5密码</li>
	 * 		<li>autoLogin:是否自动登陆</li>
	 * 		<li>icode:验证码，不需要验证码时，值可以为空</li>
	 * 		<li>captchaType:验证类型，一般有安全中心分配</li>
	 *  	<li>ick:安全中心在创建验证码时产生的票，在web端会种在cookie里</li>
	 *   	<li>expirePeriod:p票的保存时间</li>
	 *    	<li>loginRegistrationId:非常重要，必须填写。否则不予登陆。此值有Passport中心分配</li>
	 *     	<li>createRenRenTicket:如果是登陆人人产品可以同时产生t票。</li>
	 *   <p>request</p>
	 * @param context 主要是和连接请求相关的参数。建议把request中的各个数据都放在context中，如果以后需要某个特殊元素，不需要修改接口
	 * @return 登陆结果 public int userId; // required
	 * <p>LoginResponse</p>
	 * 		<li>userId:登陆用户的id</li>
	 * 		<li>realUserId:如果登陆用户是page用户，则此id是page管理员id，否则和userId相同</li>
	 * 		<li>loginStatusCode:登陆结果代码，具体见枚举类型LoginStatusCode</li>
	 * 		<li>loginStatusdescription:登陆结果描述,具体见枚举类型LoginStatusCode</li>
	 * 		<li>passportTicket:返回登陆的p票</li>
	 *  	<li>ticket:返回登陆的t票，此值只有在request中的createRenRenTicket=true时才会有值，否则值为null</li>
	 *   	<li>account:登陆帐号</li>
	 *    	<li>status:用户的状态</li>
	 *     	<li>domain:用户登陆的应用属于哪个域名。如登陆renren，则域名为renren.com.登陆56，域名为56.com</li>
	 *      <li>catchaCount:此值表明用户输入密码的错误次数。有些前端会根据此值做展示</li>
	 *      <li>weakPassword:</li>	
	 *      <li>gotoSafeCenter:是否需要跳转到安全中心，如果需要可以根据此值判断。</li>
	 *      <li>sendMail:后台会根据用户密码错误次数发送邮件，此值表明后台是否有发邮件的策略</li>
	 *      <li>oldTicket:为了和老登陆兼容使用，产生老登陆的t票</li>
	 *      <li>oldPassport:为了和老登陆兼容使用，产生老登陆的p票</li>
	 *   <p>LoginResponse</p>
	 */
	LoginResponse login(LoginRequest request, LoginContext context);
	/**
	 * 注销服务.主要清空服务器票的存储
	 * @param passportTicket 登陆p票
	 * @param ticket 登陆t票, 可以为null,如果为null,则不会清除
	 * @param context 主要是和连接请求相关的参数。建议把request中的各个数据都放在context中，如果以后需要某个特殊元素，不需要修改接口
	 * @return 返回注销结果
	 */
	LogoutResponse logout(String passportTicket, String ticket, LoginContext context);
	/**
	 * 换票服务.
	 * @param request 换票服务请求
	 *  <p>request</p>
	 * 		<li>passport:p票</li>
	 * 		<li>origUrl:登陆成功后跳转地址</li>
	 * 		<li>loginType:换哪种登陆的t票，具体见LoginType枚举类型</li>
	 * 		<li>loginRegistrationId:passport中心分配的代码，如果此值小于0，默认换web的t票</li>
	 * 		<li>userName:登陆的用户名</li>
	 *   <p>request</p>
	 * @param context 主要是和连接请求相关的参数。建议把request中的各个数据都放在context中，如果以后需要某个特殊元素，不需要修改接口
	 * @return 见login服务的描述
	 */
	LoginResponse transfer(TransferRequest request, LoginContext context);
	

	/**
	 * 切换身份服务.
	 * @param request 切换身份请求
	 * 	 <p>request</p>
	 * 		<li>passport:p票</li>
	 * 		<li>ticket:t票</li>
	 * 		<li>destId:需要切换到的用户id</li>
	 * 		<li>realUserId:如果要切换的是page用户，则此字段指明切换到哪个管理员的id。</li>
	 * 		<li>md5Password:md5密码</li>
	 * 		<li>loginRegistrationId：非常重要，必须填写。否则不予登陆。此值有Passport中心分配</li>
	 * 		<li>icode:验证码，不需要验证码时，值可以为空</li>
	 * 		<li>captchaType:验证类型，一般有安全中心分配</li>
	 *  	<li>ick:安全中心在创建验证码时产生的票，在web端会种在cookie里</li>
	 *  	<li>createRenRenTicket:如果是登陆人人产品可以同时产生t票</li>
	 *   <p>request</p>
	 * @param context 主要是和连接请求相关的参数。建议把request中的各个数据都放在context中，如果以后需要某个特殊元素，不需要修改接口
	 * @return 见login服务的描述
	 */
	LoginResponse switchAccount(SwitchRequest request, LoginContext context);
	/**
	 * 获取验证码级别，如果值为0，表示不显示验证码
	 * @param account 登陆用户名
	 * @param ip  客户端ip
	 * @return 验证码级别
	 */
	int getCaptchaGrade( String account, final String ip);
	/**
	 * 根据t票获取对应的用户id。且此用户登陆的登陆类型和loginLoginType相同
	 * @param passport p票
	 * @param loginLoginType 登陆类型
	 * @return 用户id，如果没有则返回-1
	 */
	long verifyPassport(String passport, LoginType loginLoginType);
	/**
	 * 根据t票获取对应的用户id。且此用户登陆的登陆类型和loginLoginType相同
	 * @param ticket t票
	 * @param loginLoginType 登陆类型
	 * @return 用户id，如果没有则返回-1
	 */
	long verifyTicket(String ticket, LoginType loginLoginType);
	/**
	 * 根据t票获取对应的用户id。且此用户登陆的登陆类型在loginLoginType中存在
	 * @param passport p票
	 * @param loginLoginType 登陆类型
	 * @return 用户id，如果没有则返回-1
	 */
	long verifyPassportForTypes(String ticket, List<LoginType> loginLoginType);
	/**
	 * 根据t票获取对应的用户id。且此用户登陆的登陆类型在loginLoginType中存在
	 * @param ticket t票
	 * @param loginLoginType 登陆类型
	 * @return 用户id，如果没有则返回-1
	 */
	long verifyTicketForTypes(String ticket, List<LoginType> loginLoginType);
	/**
	 * 根据t票获取用户的登陆信息
	 * @param ticket t票
	 * @return TicketInfo
	 * <p>TicketInfo</p>
	 * 		<<li>userId:登陆用户的id</li>
	 * 		<li>realUserId:如果登陆用户是page用户，则此id是page管理员id，否则和userId相同</li>
	 * 		<li>loginType:用户的登陆类型</li>
	 * 		<li>ip:用户ip</li>
	 *   <p>request</p>
	 */
	TicketInfo getTicketInfo(String ticket);
	/**
	 * 根据p票获取用户的登陆信息
	 * @param ticket t票
	 * @return TicketInfo
	 * <p>TicketInfo</p>
	 * 		<<li>userId:登陆用户的id</li>
	 * 		<li>realUserId:如果登陆用户是page用户，则此id是page管理员id，否则和userId相同</li>
	 * 		<li>loginType:用户的登陆类型</li>
	 * 		<li>ip:用户ip</li>
	 *   <p>request</p>
	 */
	TicketInfo getPassportInfo(String ticket);
}
