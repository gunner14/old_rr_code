package sns.renren.chong.dao;

import java.util.List;

import sns.renren.chong.beans.TableEditedBean;
import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

/**
 * 对应数据库tableEditedHistory表
 * 
 * @author wenhong.mo@renren-inc.com
 */
@DAO
public interface TableEditedHistoryDAO {
	@SQL("create table tableEditedHistory " +
		 "(" +
		 "time datetime not null," +
		 "opType varchar(10) not null," +
		 "configKey varchar(50) not null," +
		 "oldValue varchar(50) not null," +
		 "newValue varchar(50) not null," +
		 "editor varchar(50) not null," +
		 "comment varchar(50) not null" +
		 ")"
		 )
	void createTable();
	
	@SQL("insert into tableEditedHistory (time, opType, configKey, oldValue, newValue, editor, comment)" +
		 "values (now(), :1.opType, :1.configKey, :1.oldValue, :1.newValue, :1.editor, :1.comment)")
	int addRecord(TableEditedBean bean);

	@SQL("select time, opType, configKey, oldValue, newValue, editor, comment from tableEditedHistory")
	List<TableEditedBean> getAllRecords();

	@SQL("select time, opType, configKey, oldValue, newValue, editor, comment from tableEditedHistory where configKey = :1")
	List<TableEditedBean> getSpecifiedKeyHistories(String configKey);

	@SQL("select time, opType, configKey, oldValue, newValue, editor, comment from tableEditedHistory where configKey like :1")
	List<TableEditedBean> getMatchKeyPatternHistories(String keyPattern);
}
