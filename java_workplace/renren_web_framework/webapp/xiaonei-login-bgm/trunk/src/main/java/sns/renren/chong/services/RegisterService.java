package sns.renren.chong.services;

import sns.renren.chong.beans.ApplicantBean;

/**
 * 该接口的实现类将为注册页面提供提交申请服务
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface RegisterService {
	/**
	 * @return  0 成功；1 用户名已存在； 2 重复的申请
	 * */
	int commitApplicant(ApplicantBean appBean);
}
