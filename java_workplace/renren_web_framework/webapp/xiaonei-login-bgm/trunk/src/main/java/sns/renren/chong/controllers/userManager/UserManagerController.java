package sns.renren.chong.controllers.userManager;

import java.util.List;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.annotations.LoginRequired;
import sns.renren.chong.beans.UserBean;
import sns.renren.chong.services.UserManagerService;

/**
 * 该controller --对应-- 管理用户页面（超级用户独有）
 * 
 * @author wenhong.mo@renren-inc.com
 */
@LoginRequired
@Path("")
public class UserManagerController {
	@Autowired
	private UserManagerService userManagerService;
	

	/**
	 * 处理url: /userManager/showPage
	 */
	public String showPage(Invocation inv, @Param("pattern")String pattern) {
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		List<UserBean> users = null;
		if (pattern == null || "".equals(pattern)) {
			users = userManagerService.getAllUsers(host);
		} else {
			users = userManagerService.getMatchUsers(pattern);
		}
		
		inv.addModel("users", users);
		inv.addModel("host", host);
		
		return "userManager-showpage";
	}
	
	/**
	 * 处理url: post - /userManager/actionDelete 
	 * 删除用户
	 * 重定向：./showPage
	 */
	@Post("actionDelete")
	public String actionDelete(Invocation inv, @Param("name")String userName) {
		userManagerService.deleteUser(userName);
		return "r:/userManager/showPage";
	}
	
}
