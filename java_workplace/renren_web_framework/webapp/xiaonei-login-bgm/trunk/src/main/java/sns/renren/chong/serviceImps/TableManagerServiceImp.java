package sns.renren.chong.serviceImps;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.LoginConfigBean;
import sns.renren.chong.beans.TableEditedBean;
import sns.renren.chong.services.LoginConfigService;
import sns.renren.chong.services.TableEditedHistoryService;
import sns.renren.chong.services.TableManagerService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class TableManagerServiceImp implements TableManagerService {
	@Autowired 
	private LoginConfigService loginConfigServiceForTableManager;
	@Autowired
	private TableEditedHistoryService tableEditedHistoryServiceForTableManager;
	
	
	@Override
	public int getLoginConfigRecordsSum() {
		int sum = 0;
		try {
			sum = this.loginConfigServiceForTableManager.recordsSum();
		} catch (Exception e) {
			
		}
		return sum;
	}

	@Override
	public List<LoginConfigBean> getSpecifiedPageContentFromLoginConfig(
			int specifiedPage, int pageSize) {
		List<LoginConfigBean> content = null;
		try {
			content = this.loginConfigServiceForTableManager.getSpecifiedPageContent(specifiedPage, pageSize);
		} catch (Exception e) {
			
		}
		return content;
	}

	@Override
	public int getLoginConfigMatchRecordsSum(String pattern) {
	
		int sum = 0;
		try {
			sum = this.loginConfigServiceForTableManager.mathRecordsSum(pattern);
		} catch (Exception e) {
			
		}
		return sum;
	}

	@Override
	public List<LoginConfigBean> getSpecifiedPageContentFromLoginConfigMatch(
			String pattern, int specifiedPage, int pageSize) {
	
		List<LoginConfigBean> content = null;
		try {
			content = this.loginConfigServiceForTableManager.getSpecifiedPageContentFromMatchSet(
															pattern, specifiedPage, pageSize);
		} catch (Exception e) {
			
		}
		return content;
	}

	@Override
	public void deleteLoginConfigRecord(LoginConfigBean loginConfigBean,
			String name) {
		try {
			if (this.loginConfigServiceForTableManager.deleteRecord(loginConfigBean)) {
				TableEditedBean bean = new TableEditedBean(loginConfigBean);
				bean.setEditor(name);
				bean.setOpType("delete");
				this.tableEditedHistoryServiceForTableManager.addRecord(bean);
			}
		} catch (Exception e) {
			
		}
	}

	@Override
	public boolean addLoginConfigRecord(LoginConfigBean loginConfigBean,
			String name) {
	
		try {
			if (this.loginConfigServiceForTableManager.addRecord(loginConfigBean)) {
				TableEditedBean bean = new TableEditedBean(loginConfigBean);
				bean.setEditor(name);
				bean.setOpType("add");
				this.tableEditedHistoryServiceForTableManager.addRecord(bean);
				return true;
			}
		} catch (Exception e) {
			
		}
		return false;
	}

	@Override
	public void modifyLoginConfigRecord(LoginConfigBean loginConfigBean,
			String newValue, String name) {
		try {
			if (this.loginConfigServiceForTableManager.modifyRecord(loginConfigBean, newValue)) {
				TableEditedBean bean = new TableEditedBean();
				bean.setConfigKey(loginConfigBean.getConfigKey());
				bean.setOldValue(loginConfigBean.getConfigValue());
				bean.setNewValue(newValue);
				bean.setEditor(name);
				bean.setOpType("modify");
				this.tableEditedHistoryServiceForTableManager.addRecord(bean);
			}
		} catch (Exception e) {
			
		}
	}

	@Override
	public List<TableEditedBean> getSpecifiedKeyHistory(String configKey) {
		List<TableEditedBean> histories = null;
		try {
			histories = this.tableEditedHistoryServiceForTableManager.getSpecifiedKeyHistories(configKey);
		} catch (Exception e) {
			
		}
		return histories;
	}

	@Override
	public List<TableEditedBean> getTimeSortHistory() {
		List<TableEditedBean> histories = null;
		try {
			histories = this.tableEditedHistoryServiceForTableManager.getAllRecords();
		} catch (Exception e) {
			
		}
		
		if (histories != null) {
			sortHistories(histories, new ComparatorTimeAsc());
		}	
		return histories;
	}

	@Override
	public List<TableEditedBean> getKeySortHistory() {
		List<TableEditedBean> histories = null;
		try {
			histories = this.tableEditedHistoryServiceForTableManager.getAllRecords();
		} catch (Exception e) {
			
		}
		if (histories != null) {
			sortHistories(histories, new ComparatorKeyAsc());
		}
		return histories;
	}
	
	@Override
	public List<TableEditedBean> getHistoryMatchKeyPattern(String keyPattern) {
		List<TableEditedBean> histories = null;
		try {
			histories = this.tableEditedHistoryServiceForTableManager.getHistoryMatchKeyPattern(keyPattern);
		} catch (Exception e) {
			//e.printStackTrace();
		}
		if (histories != null) {
			sortHistories(histories, new ComparatorKeyAsc());
		}
		return histories;
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	private void sortHistories(List<TableEditedBean> histories, Comparator cmp) {
		Collections.sort(histories, cmp);
	}
	
	@SuppressWarnings("rawtypes")
	class ComparatorTimeAsc implements Comparator {

		@Override
		public int compare(Object o1, Object o2) {
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			Date date1 = null;
			Date date2 = null;
			try {
				date1 = sdf.parse(((TableEditedBean)o1).getTime());
				date2 = sdf.parse(((TableEditedBean)o2).getTime());
				return date1.compareTo(date2);
			} catch (ParseException e) {
				//e.printStackTrace();
			}
			return 0;
		}
	}
	
	@SuppressWarnings("rawtypes")
	class ComparatorTimeDesc implements Comparator {

		@Override
		public int compare(Object o1, Object o2) {
			
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			Date date1 = null;
			Date date2 = null;
			try {
				date1 = sdf.parse(((TableEditedBean)o1).getTime());
				date2 = sdf.parse(((TableEditedBean)o2).getTime());
				return date2.compareTo(date1);
			} catch (ParseException e) {
				//e.printStackTrace();
			}
			return 0;
		}
		
	}
	
	@SuppressWarnings("rawtypes")
	class ComparatorKeyAsc implements Comparator {

		@Override
		public int compare(Object o1, Object o2) {
			TableEditedBean bean1 = (TableEditedBean) o1;
			TableEditedBean bean2 = (TableEditedBean) o2;
			if (! bean1.getConfigKey().equals(bean2.getConfigKey()))
				return bean1.getConfigKey().compareTo(bean2.getConfigKey());
			
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			Date date1 = null;
			Date date2 = null;
			try {
				date1 = sdf.parse(((TableEditedBean)o1).getTime());
				date2 = sdf.parse(((TableEditedBean)o2).getTime());
				return date1.compareTo(date2);
			} catch (ParseException e) {
				//e.printStackTrace();
			}
			return 0;
		}
		
	}
	
}
