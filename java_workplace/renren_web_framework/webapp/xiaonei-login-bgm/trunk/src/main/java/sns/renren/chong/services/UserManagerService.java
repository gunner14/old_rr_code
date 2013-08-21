package sns.renren.chong.services;

import java.util.List;

import sns.renren.chong.beans.UserBean;

/**
 * 该接口的实现类将提供用户管理相关服务
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface UserManagerService {
	
	List<UserBean> getAllUsers(UserBean host);
	
	List<UserBean> getMatchUsers(String pattern);
	
	int deleteUser(String name);
}
