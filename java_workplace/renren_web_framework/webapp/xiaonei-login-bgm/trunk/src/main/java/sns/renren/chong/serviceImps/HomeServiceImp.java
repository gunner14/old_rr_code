package sns.renren.chong.serviceImps;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.dao.ApplicantsDAO;
import sns.renren.chong.dao.UsersDAO;
import sns.renren.chong.services.HomeService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class HomeServiceImp implements HomeService {
	@Autowired
	private UsersDAO userDAO;
	@Autowired
	private ApplicantsDAO appDAO;
	
	@Override
	public int getApplicantNum() {
		int result = 0;
		try {
			result = appDAO.recordsNum();
		} catch (Exception e) {
			
		}
		return result;
	}

	@Override
	public int getUserNum() {
		int result = 0;
		try {
			result = userDAO.recordsNum();
		} catch (Exception e) {
			
		}
		return result;
	}

}
