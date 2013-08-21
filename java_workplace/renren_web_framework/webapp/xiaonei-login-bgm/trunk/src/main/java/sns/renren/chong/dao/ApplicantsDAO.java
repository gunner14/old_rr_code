package sns.renren.chong.dao;

import java.util.List;

import sns.renren.chong.beans.ApplicantBean;
import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

/**
 * 对应数据库applicants表
 * 
 * @author wenhong.mo@renren-inc.com
 */
@DAO
public interface ApplicantsDAO {
    String TABLE_NAME = "applicants";
	@SQL("create table applicants " +
		"(" +
		"name varchar(50) not null," +
		"mail varchar(50) not null," +
		"password varchar(50) not null," +
		"remark varchar(50)," +
		"primary key (name)" +
		")"
		)
	int createTable();
	
	@SQL("select name from applicants where name = :1")
	String isExist(String name);
	
	@SQL("insert into applicants (name, mail, password , remark) values (:1.name, :1.mail, :1.password, :1.remark)")
	int addRecord(ApplicantBean app);
	
	@SQL("delete from applicants where name = :1")
	int deleteRecord(String name);
	
	@SQL("select count(*) from applicants")
	int recordsNum();
	
	@SQL("select name, mail, password, remark from applicants where name = :1")
	ApplicantBean getRecord(String name);
	
	@SQL("select name, mail, remark from applicants")
	List<ApplicantBean> getAllRecords();
	
	@SQL("select name, mail, remark from applicants where name like :1 OR mail like :1")
	List<ApplicantBean> getMatchRecords(String pattern);
}
