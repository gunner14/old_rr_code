package sns.renren.chong.dao;

import java.util.List;

import sns.renren.chong.beans.LoginConfigBean;
import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

/**
 * 对应数据库表login_config
 * 
 * @author wenhong.mo@renren-inc.com
 */
@DAO
public interface LoginConfigDAO {
	@SQL("create table login_config" +
		 "(" +
		 "id int not null auto_increment," +
		 "configKey varchar(50) not null," +
		 "configValue varchar(50) not null," +
		 "primary key (id)," +
		 "unique (configKey)" +
		 ")"
		 )
	String createTable();
	
	@SQL("select count(*) from login_config")
	int recordsSum();

	@SQL("select id, configKey, configValue from login_config limit :1, :2")
	List<LoginConfigBean> getSpecifiedRegionRecords(int i, int pageSize);

	@SQL("select count(*) from login_config where configKey like :1 or configValue like :1")
	int matchRecordsSum(String pattern);

	@SQL("select id, configKey, configValue from login_config where configKey like :1 or configValue like :1 limit :2, :3")
	List<LoginConfigBean> getSpecifiedRegionRecordsFromMatchSet(String pattern, int i, int pageSize);

	@SQL("delete from login_config where configKey = :1.configKey")
	int deleteRecord(LoginConfigBean loginConfigBean);

	@SQL("insert into login_config (configKey, configValue) values (:1.configKey, :1.configValue)")
	int addRecord(LoginConfigBean loginConfigBean);

	@SQL("update login_config set configValue = :2 where configKey = :1.configKey")
	int modifyRecord(LoginConfigBean loginConfigBean, String newValue);
	
}
