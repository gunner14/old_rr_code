package sns.renren.chong.serviceImps;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.LoginConfigBean;
import sns.renren.chong.dao.LoginConfigDAO;
import sns.renren.chong.services.LoginConfigService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class LoginConfigServiceImp implements LoginConfigService {
	@Autowired
	private LoginConfigDAO loginConfigDAO;
	
	@Override
	public int recordsSum() {
		int result = 0;
		try {
			result = this.loginConfigDAO.recordsSum();
		} catch (Exception e) {
			
		}
		return result;
	}

	@Override
	public List<LoginConfigBean> getSpecifiedPageContent(int specifiedPage,
			int pageSize) {
		List<LoginConfigBean> content = null;
		try {
			content = this.loginConfigDAO.getSpecifiedRegionRecords((specifiedPage-1)*pageSize, pageSize);
		} catch (Exception e) {
			
		}
		return content;
	}

	@Override
	public int mathRecordsSum(String pattern) {
		int sum = 0;
		try {
			sum = this.loginConfigDAO.matchRecordsSum("%" + pattern + "%");
		} catch (Exception e) {
			
		}
		return sum;
	}

	@Override
	public List<LoginConfigBean> getSpecifiedPageContentFromMatchSet(String pattern,
																	 int specifiedPage, int pageSize) {
		List<LoginConfigBean> content = null;
		try {
			content = this.loginConfigDAO.getSpecifiedRegionRecordsFromMatchSet(
								"%" + pattern + "%", 
								(specifiedPage-1)*pageSize, pageSize
								);
		} catch (Exception e) {
			
		}
		return content;
	}

	@Override
	public boolean deleteRecord(LoginConfigBean loginConfigBean) {
		int result = 0;
		try {
			result = this.loginConfigDAO.deleteRecord(loginConfigBean);
		} catch (Exception e) {
			
		}
		if (result > 0)
			return true;
		return false;
	}

	@Override
	public boolean addRecord(LoginConfigBean loginConfigBean) {
		int result = 0;
		try {
			result = this.loginConfigDAO.addRecord(loginConfigBean);
		} catch (Exception e) {
			
		}
		if (result > 0)
			return true;
		return false;
	}

	@Override
	public boolean modifyRecord(LoginConfigBean loginConfigBean, String newValue) {
		int result = 0;
		try {
			result = this.loginConfigDAO.modifyRecord(loginConfigBean, newValue);
		} catch (Exception e) {
			
		}
		if (result > 0)
			return true;
		
		return false;
	}

}
