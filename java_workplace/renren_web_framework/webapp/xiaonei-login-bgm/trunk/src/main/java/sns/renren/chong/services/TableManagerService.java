package sns.renren.chong.services;

import java.util.List;

import sns.renren.chong.beans.LoginConfigBean;
import sns.renren.chong.beans.TableEditedBean;

/**
 * 该接口的实现类将提供操纵login_config表的相关服务
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface TableManagerService {

	/**
	 * 返回LoginConfig表的记录数
	 * */
	int getLoginConfigRecordsSum();

	/**
	 * 返回LoginConfig表指定的记录页
	 * */
	List<LoginConfigBean> getSpecifiedPageContentFromLoginConfig(int specifiedPage,
			int pageSize);

	/**
	 * 返回loginConfig表中满足匹配的记录数
	 * */
	int getLoginConfigMatchRecordsSum(String pattern);

	/**
	 * 返回loginConfig表匹配记录中指定的记录页
	 * */
	List<LoginConfigBean> getSpecifiedPageContentFromLoginConfigMatch(String pattern,
			int specifiedPage, int pageSize);
	/**
	 * 将记录从LoginConfig表中删除，如果成功将操作结果添加到TableEditedHistory
	 * */
	void deleteLoginConfigRecord(LoginConfigBean loginConfigBean, String name);

	/**
	 * 将记录添加LoginConfig表中，如果成功将操作结果添加到TableEditedHistory
	 * */
	boolean addLoginConfigRecord(LoginConfigBean loginConfigBean, String name);

	/**
	 * 修改LoginConfig表的特定记录，如果成功将操作结果添加到TableEditedHistory
	 * */
	void modifyLoginConfigRecord(LoginConfigBean loginConfigBean, String newValue, String name);

	/**
	 * 获取特定key的历史编辑记录
	 * */
	List<TableEditedBean> getSpecifiedKeyHistory(String configKey);

	/**
	 * 获取所有历史记录，按时间降序排序
	 * */
	List<TableEditedBean> getTimeSortHistory();

	/**
	 * 获取所有历史记录，按key升序排列
	 * */
	List<TableEditedBean> getKeySortHistory();

	/**
	 *获取匹配key模式的所有configKey的编辑历史，并以key排序 
	 * */
	List<TableEditedBean> getHistoryMatchKeyPattern(String keyPattern);

}
