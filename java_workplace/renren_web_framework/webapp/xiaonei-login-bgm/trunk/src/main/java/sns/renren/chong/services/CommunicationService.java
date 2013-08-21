package sns.renren.chong.services;

import sns.renren.chong.beans.ApplicantBean;
import sns.renren.chong.beans.UserBean;

/**
 * 该接口的实现类将提供通信相关服务，目前主要是发送邮件
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface CommunicationService {
	int sendNewRegisterMailToSuperUser(UserBean registerUser);

	int sendAgreeMailToUser(ApplicantBean applicant);

	int sendRefuseMailToUser(ApplicantBean applicant, String reason);
	
}
