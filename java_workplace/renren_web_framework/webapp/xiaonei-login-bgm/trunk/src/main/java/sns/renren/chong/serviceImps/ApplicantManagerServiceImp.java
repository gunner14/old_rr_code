package sns.renren.chong.serviceImps;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.ApplicantBean;
import sns.renren.chong.beans.UserBean;
import sns.renren.chong.dao.ApplicantsDAO;
import sns.renren.chong.dao.UsersDAO;
import sns.renren.chong.services.ApplicantManagerService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class ApplicantManagerServiceImp implements ApplicantManagerService {
	@Autowired
	private UsersDAO userDAO;
	@Autowired
	private ApplicantsDAO applicantDAO;
	@Override
	
	/**
	 * delete from applicants, then add user to users;
	 * */
	public boolean addUser(String name) {
		
		ApplicantBean applicant = null;
		try {
			applicant = applicantDAO.getRecord(name);
			if (applicant == null) { 
				return false;
			}
			UserBean user = new UserBean();
			user.setName(applicant.getName());
			user.setMail(applicant.getMail());
			user.setPassword(applicant.getPassword());
			user.setIdentity("N");
			
			if(userDAO.addRecord(user) > 0){
				applicantDAO.deleteRecord(name);
				return true;
			}
			
		} catch (Exception  e) {
			
			return false;
		}
	
		return false;
	}

	@Override
	public boolean deleteApplicant(String name) {
		// TODO Auto-generated method stub
		int result = 0;
		try { 
			result = applicantDAO.deleteRecord(name);
		} catch (Exception e) {
			
		}
		if (result > 0)
			return true;
		return false;
	}

	@Override
	public List<ApplicantBean> getAllApplicants() {
		// TODO Auto-generated method stub
		List<ApplicantBean> applicants = null;
		try {
			applicants = applicantDAO.getAllRecords();
		} catch (Exception e) {
			
		}
		return applicants;
	}

	@Override
	public List<ApplicantBean> getMatchApplicants(String pattern) {
		// TODO Auto-generated method stub
		List<ApplicantBean> applicants = null;
		try {
			applicants = applicantDAO.getMatchRecords("%" + pattern + "%");
		} catch (Exception e) {
			
		}
		return applicants;
	}

}
