package sns.renren.chong.services;

import java.util.List;

import sns.renren.chong.beans.TableEditedBean;

/**
 * 该接口的实现类将提供操纵编辑历史表tableEditedHistory相关服务
 *
 * @author wenhong.mo@renren-inc.com
 */
public interface TableEditedHistoryService {

	boolean addRecord(TableEditedBean bean);

	List<TableEditedBean> getAllRecords();

	List<TableEditedBean> getSpecifiedKeyHistories(String configKey);

	List<TableEditedBean> getHistoryMatchKeyPattern(String keyPattern);

}
