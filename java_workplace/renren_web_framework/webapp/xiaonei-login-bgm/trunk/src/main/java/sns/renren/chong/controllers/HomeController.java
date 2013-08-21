package sns.renren.chong.controllers;

import org.springframework.beans.factory.annotation.Autowired;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import sns.renren.chong.annotations.LoginRequired;
import sns.renren.chong.beans.UserBean;
import sns.renren.chong.services.HomeService;

/**
 *主页，登陆成功后默认重定向到该controller
 *
 *@author wenhong.mo@renren-inc.com
 */
@LoginRequired
@Path("")
public class HomeController {
	
	@Autowired
	private HomeService homeService;
	
	@Get("")
	@Post("")
	public String showPage(Invocation inv) {
		UserBean user = (UserBean)inv.getRequest().getSession().getAttribute("user");
		String identity = user.getIdentity();
		//if (identity.equals("S") || identity.equals("s")) {
		if ("S".equals(identity) || "s".equals(identity)) {
			int applicantNum = homeService.getApplicantNum();
			int userNum = homeService.getUserNum();
			inv.addModel("applicantNum", applicantNum);
			inv.addModel("userNum", userNum);
			return "home-super-showpage";
		}
		return "home-normal-showpage";
	}
	
	public String loginOut(Invocation inv) {
		inv.getRequest().getSession().removeAttribute("user");
		return "r:/login/showPage";
	}

}
