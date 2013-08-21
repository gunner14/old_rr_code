package sns.renren.chong.serviceImps;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;

import sns.renren.chong.beans.TableEditedBean;
import sns.renren.chong.dao.TableEditedHistoryDAO;
import sns.renren.chong.services.TableEditedHistoryService;

/**
 * @author wenhong.mo@renren-inc.com
 */
public class TableEditedHistoryServiceImp implements TableEditedHistoryService {
	@Autowired
	private TableEditedHistoryDAO tableEditedDAO;
	
	@Override
	public boolean addRecord(TableEditedBean bean) {
		int result = 0;
		try {
			result = this.tableEditedDAO.addRecord(bean);
		} catch (Exception e) {
			
		}
		if (result > 0)
			return true;
		return false;
	}

	@Override
	public List<TableEditedBean> getAllRecords() {
		List<TableEditedBean> histories = null;
		try {
			histories = this.tableEditedDAO.getAllRecords();
		} catch (Exception e) {
		
		}
		return histories;
	}

	@Override
	public List<TableEditedBean> getSpecifiedKeyHistories(String configKey) {
		List<TableEditedBean> histories = null;
		try {
			histories = this.tableEditedDAO.getSpecifiedKeyHistories(configKey);
		} catch (Exception e) {
			
		}
		return histories;
	}

	@Override
	public List<TableEditedBean> getHistoryMatchKeyPattern(String keyPattern) {
		List<TableEditedBean> histories = null;
		try {
			histories = this.tableEditedDAO.getMatchKeyPatternHistories("%" + keyPattern + "%");
		} catch (Exception e) {
			
		}
		return histories;
	}
}
