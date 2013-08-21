package sns.renren.chong.services;

import java.util.List;

import sns.renren.chong.beans.LoginConfigBean;

/**
 * 该接口的实现类将提供与数据库login_config表相关的服务
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface LoginConfigService {

	int recordsSum();

	List<LoginConfigBean> getSpecifiedPageContent(int specifiedPage, int pageSize);

	int mathRecordsSum(String pattern);

	List<LoginConfigBean> getSpecifiedPageContentFromMatchSet(String pattern, int specifiedPage, int pageSize);

	boolean deleteRecord(LoginConfigBean loginConfigBean);

	boolean addRecord(LoginConfigBean loginConfigBean);

	boolean modifyRecord(LoginConfigBean loginConfigBean, String newValue);

}
