package sns.renren.chong.serviceImps;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.UserBean;
import sns.renren.chong.dao.UsersDAO;
import sns.renren.chong.services.LoginService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class LoginServiceImp implements LoginService {
	@Autowired
	private UsersDAO userDAO;
	
	@Override
	public boolean isMatch(UserBean user) {
		UserBean rel = null;
		try {
			rel = userDAO.isMatch(user.getName(), user.getPassword());
		} catch (Exception e) {
			
		}
		if (rel == null)
			return false;
		
		user.setIdentity(rel.getIdentity());
		user.setMail(rel.getMail());
		return true;
	}

}
