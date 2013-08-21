package sns.renren.chong.controllers.applicantManager;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.annotations.LoginRequired;
import sns.renren.chong.beans.ApplicantBean;
import sns.renren.chong.beans.UserBean;
import sns.renren.chong.services.ApplicantManagerService;
import sns.renren.chong.services.CommunicationService;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.Param;
import net.paoding.rose.web.annotation.Path;
import net.paoding.rose.web.annotation.rest.Post;


/**
 *该controller对应 -- 超级用户处理注册申请
 *
 *@author wenhong.mo@renren-inc.com
 */
@LoginRequired
@Path("")
public class ApplicantManagerController {
	
	@Autowired
	private ApplicantManagerService applicantManagerService;
	
	@Autowired
	private CommunicationService comServiceApplicant;
	
	
	/**
	 * 处理url：/applicantManager/showPage
	 * 返回相应注册申请页面
	 */
	public String showPage(Invocation inv, @Param("pattern")String pattern) {
		UserBean host = (UserBean)inv.getRequest().getSession().getAttribute("user");
		List<ApplicantBean> users = null;
		if(pattern == null) {
			users = applicantManagerService.getAllApplicants();
		} else {
			users = applicantManagerService.getMatchApplicants(pattern);
		}
		inv.addModel("host", host);
		inv.addModel("users", users);
		return "applicantManager-showpage";
	}
	
	/**
	 * 处理url: /applicantMangager/actionAgree
	 * 添加用户，并给用户发送注册成功邮件，重定向到showPage
	 */
	@Post("actionAgree")
	public String actionAgree(Invocation inv, ApplicantBean applicant) {
		
		if (applicantManagerService.addUser(applicant.getName())){
			comServiceApplicant.sendAgreeMailToUser(applicant);
		}
		return "r:/applicantManager/showPage";
	}
	
	/**
	 * 处理url:BackroundManager/applicantManager/actionRefuse
	 * 不同意用户注册申请，并给用户发送拒绝邮件，重定向到showPage
	 */
	@Post("actionRefuse")
	public String actionRefuse(Invocation inv, ApplicantBean applicant,
			                   @Param("reason")String reason) {
		if (applicantManagerService.deleteApplicant(applicant.getName())) {
			comServiceApplicant.sendRefuseMailToUser(applicant, reason);
		}
		return "r:/applicantManager/showPage";
	}


}

