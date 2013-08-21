package sns.renren.chong.controllers.register;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.ApplicantBean;
import sns.renren.chong.beans.UserBean;
import sns.renren.chong.services.CommunicationService;
import sns.renren.chong.services.RegisterService;


/**
 * 该controller --对应-- 注册页面
 * 
 * @author wenhong.mo@renren-inc.com
 */
@Path("")
public class RegisterController {
	@Autowired
	private RegisterService registerService;
	
	@Autowired
	private CommunicationService comServiceRegister;
	
	public String showPage(Invocation inv) {
		return "register-showpage";
	}
	
	/**
	 * 处理url: post - /register/actionRegister
	 * 提交注册申请，若成功，邮件通知超级用户有新的注册请求 
	 */
	@Post("actionRegister")
	public String actionRegister(Invocation inv, ApplicantBean appBean) {
		// 提交申请，提交成功则向super user发送邮件
		int result = registerService.commitApplicant(appBean);
		if (result == 0) {
			UserBean user = this.getUserBean(appBean);
			comServiceRegister.sendNewRegisterMailToSuperUser(user);
			return "register-sucess";
		}
		return "register-fail";
	}
	
	private UserBean getUserBean(ApplicantBean appBean) {
		UserBean user =  new UserBean();
		user.setName(appBean.getName());
		user.setMail(appBean.getMail());
		user.setIdentity("N");
		return user;
	}
	
}
