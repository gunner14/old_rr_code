package sns.renren.chong.controllers.login;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.UserBean;
import sns.renren.chong.services.LoginService;


/**
 * 该controller --对应-- 登陆页面
 * 
 * @author wenhong.mo@renren-inc.com
 */
@Path("")
public class LoginController {
	@Autowired
	private LoginService loginService;
	
	/**
	 * 处理url：/login/showPage
	 */
	public String showPage(Invocation inv, @Param("href")String href) {
		if (href != null) {
			inv.addModel("href", href);
		}
		return "login-showpage";
	}
	
	/**
	 * 处理url: Post - /login/actionLogin
	 * 往session里添加user对象，代表已经登陆，重定向到Home
	 */
	@Post("actionLogin")
	public String actionLogin(Invocation inv, UserBean user, @Param("href")String href) {
		// 判断的同时将user消息补充
		boolean result = loginService.isMatch(user);
		if (!result) {
			return "login-fail";
		}
		inv.getRequest().getSession().setAttribute("user", user);
		if (href == null || "".equals(href))
			return "r:/";
		return "r:" + href;
	}
	
	
}
