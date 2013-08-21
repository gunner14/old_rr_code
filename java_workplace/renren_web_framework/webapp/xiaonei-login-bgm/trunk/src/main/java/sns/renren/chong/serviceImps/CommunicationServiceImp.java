package sns.renren.chong.serviceImps;

import java.io.StringWriter;
import java.util.Date;
import java.util.List;
import java.util.ListIterator;
import java.util.Properties;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;
import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.services.CommunicationService;

import sns.renren.chong.beans.ApplicantBean;
import sns.renren.chong.beans.UserBean;
import sns.renren.chong.communicateModule.EmailInformation;
import sns.renren.chong.communicateModule.ServerDnsNameSet;
import sns.renren.chong.communicateModule.SimpleEmailWrapper;
import sns.renren.chong.dao.UsersDAO;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class CommunicationServiceImp implements CommunicationService {
	
	private static String newRegisterVM = "email/template/register-mail-to-super.vm";
	private static String agreeVM = "email/template/register-agree-mail-to-applicant.vm";
	private static String refuseVM = "email/template/register-refuse-to-applicant.vm";
	
	private static String urlToAppManager = "http://adminlogin.renren.com/applicantManager/showPage";
	
	private static String urlToLogin = "http://adminlogin.renren.com/login/showPage";
	
	private static VelocityEngine velocityEngine = null;
	static {
		Properties p = new Properties();
		p.setProperty(VelocityEngine.RESOURCE_LOADER, "class");
		p.setProperty("class.resource.loader.class", "org.apache.velocity.runtime.resource.loader.ClasspathResourceLoader");
		p.setProperty(VelocityEngine.INPUT_ENCODING, "UTF-8");
		p.setProperty(VelocityEngine.OUTPUT_ENCODING, "UTF-8");
	
	    velocityEngine = new VelocityEngine();
	    try {
			velocityEngine.init(p);
		} catch (Exception e) {
			
		}	
	}
	
	@Autowired
	private UsersDAO userDAO;
	private ServerDnsNameSet dnsNameSet = new ServerDnsNameSet();
	
	/** 注：邮件发送功能被关闭，若想用，请自行在下面两项填写好邮件名+密码*/
	private String sysMailName = "";
	private String sysMailPsw = "";
	
	/**
	 * 邮件通知超级用户有新的注册申请
	 */
	@Override
	public int sendNewRegisterMailToSuperUser(UserBean registerUser) {
		
		EmailInformation info = new EmailInformation();
		this.initNewRegisterEmailInformation(info, registerUser);
		SimpleEmailWrapper email = new SimpleEmailWrapper(info);
		return email.sendMail(info);
	}
	
	/**
	 * 邮件通知注册申请用户，注册成功 
	 */
	@Override
	public int sendAgreeMailToUser(ApplicantBean applicant) {
		
		EmailInformation info = new EmailInformation();
		this.initAgreeMailInformaiton(info, applicant);
		SimpleEmailWrapper email = new SimpleEmailWrapper(info);
		return email.sendMail(info);
		
	}

	/**
	 *邮件通知注册申请用户，注册失败 
	 * 
	 */
	@Override
	public int sendRefuseMailToUser(ApplicantBean applicant, String reason) {
		
		EmailInformation info = new EmailInformation();
		this.initRefuseMailInformaiton(info, applicant, reason);
		SimpleEmailWrapper email = new SimpleEmailWrapper(info);
		return email.sendMail(info);
	}
	
	private void initNewRegisterEmailInformation(EmailInformation info, UserBean registerUser) {
		info.setSender(this.sysMailName);
		info.setPassword(this.sysMailPsw);
		info.setCharset("UTF-8");
		info.setHostName(dnsNameSet.getServerDnsName(this.sysMailName));
		info.setSubject("新的注册请求");
		
		List<UserBean> users = userDAO.getSuperUsers();
		ListIterator<UserBean> ito = users.listIterator();
		while (ito.hasNext()) {
			UserBean user = ito.next();
			info.addReciver(user.getMail());
		}
	
		String msg = this.getNewRegisterMsg(registerUser, users.size());
		info.setMsg(msg);
	}
	
	private String getNewRegisterMsg(UserBean registerUser, int count) {
		String content = "";
		
		VelocityContext context = new VelocityContext();		
		context.put("registerUser", registerUser);
		context.put("count", count);
		context.put("url", urlToAppManager);
		context.put("date", new Date());
		
		StringWriter writer = new StringWriter();
		Template template = null;
		try {
			template = velocityEngine.getTemplate(CommunicationServiceImp.newRegisterVM, "utf-8");
			template.merge(context, writer);
		} catch (Exception e) {
			
		}
		
		content = writer.toString();
		return content;
	}
	
	private void initAgreeMailInformaiton(EmailInformation info, ApplicantBean applicant) {
		info.setSender(this.sysMailName);
		info.setPassword(this.sysMailPsw);
		info.setCharset("UTF-8");
		info.setHostName(dnsNameSet.getServerDnsName(this.sysMailName));
		info.setSubject("申请通过");
		info.addReciver(applicant.getMail());
		String msg = this.getAgreeMailInfomation(applicant);
		info.setMsg(msg);
	}
	
	private String getAgreeMailInfomation(ApplicantBean applicant) {
		String content = "";
		
		VelocityContext context = new VelocityContext();		
		context.put("name", applicant.getName());
		context.put("date", new Date());
		context.put("url", urlToLogin);
		
		StringWriter writer = new StringWriter();
		Template template = null;
		try {
			template = velocityEngine.getTemplate(CommunicationServiceImp.agreeVM, "utf-8");
			template.merge(context, writer);
		} catch (Exception e) {
			
		}
		
		content = writer.toString();
		return content;
	}
	
	private void initRefuseMailInformaiton(EmailInformation info, ApplicantBean applicant, String reason) {
		info.setSender(this.sysMailName);
		info.setPassword(this.sysMailPsw);
		info.setCharset("UTF-8");
		info.setHostName(dnsNameSet.getServerDnsName(this.sysMailName));
		info.setSubject("申请被拒绝");
		info.addReciver(applicant.getMail());
		String msg = this.getRefuseMailInfomation(applicant, reason);
		info.setMsg(msg);
	}
	
	private String getRefuseMailInfomation(ApplicantBean applicant, String reason) {
		String content = "";
		
		VelocityContext context = new VelocityContext();		
		context.put("name", applicant.getName());
		context.put("date", new Date());
		context.put("reason", reason);
		context.put("url", urlToLogin);
		
		StringWriter writer = new StringWriter();
		Template template = null;
		try {
			template = velocityEngine.getTemplate(CommunicationServiceImp.refuseVM, "utf-8");
			template.merge(context, writer);
		} catch (Exception e) {
			
		}
		
		content = writer.toString();
		return content;
	}
	
	
}
