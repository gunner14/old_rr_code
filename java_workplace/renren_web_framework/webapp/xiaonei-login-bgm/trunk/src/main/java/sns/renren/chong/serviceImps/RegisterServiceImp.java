package sns.renren.chong.serviceImps;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.ApplicantBean;
import sns.renren.chong.dao.ApplicantsDAO;
import sns.renren.chong.dao.UsersDAO;
import sns.renren.chong.services.RegisterService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class RegisterServiceImp implements RegisterService {
	
	@Autowired
	private ApplicantsDAO applicantDAO;
	
	@Autowired
	private UsersDAO userDAO;
	
	@Override
	public int commitApplicant(ApplicantBean appBean) {
		try {
			if (!(userDAO.isExist(appBean.getName()) == null))
				return 1;
			if (!(applicantDAO.isExist(appBean.getName()) == null))
				return 2;
			applicantDAO.addRecord(appBean);
		
		} catch (Exception e) {
			
		}
		return 0;
	}

}
