package sns.renren.chong.services;

import java.util.List;

import sns.renren.chong.beans.ApplicantBean;

/**
 * 该接口的实现类将提供申请管理相关服务
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface ApplicantManagerService {

	boolean addUser(String name);

	boolean deleteApplicant(String name);

	List<ApplicantBean> getAllApplicants();

	List<ApplicantBean> getMatchApplicants(String pattern);

}
