package sns.renren.chong.services;

import sns.renren.chong.beans.UserBean;

/**
 * 该接口的实现类将提供登陆相关服务
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface LoginService {
	/**
	 * @return true 代表匹配；false 代表匹配失败
	 * */
	boolean isMatch(UserBean user);
}
