package sns.renren.chong.dao;

import java.util.List;

import sns.renren.chong.beans.UserBean;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

/**
 * 对应数据库users表
 * 
 * @author wenhong.mo@renren-inc.com
 */
@DAO
public interface UsersDAO {
	@SQL("create table users " +
		 "(" +
		 "name varchar(50) not null," +
		 "mail varchar(50) not null," +
		 "password varchar(50) not null," +
		 "identity char(2) not null," +
		 "primary key (name)" +
		  ")"
		 )
	String createTable();
	
	@SQL("drop table :1")
	int deleteTable(String tableName);
	
	@SQL("delete from users where name = :1")
	int deleteRecord(String name);
	
	@SQL("select name, mail, identity from users")
	List<UserBean> getAllUsers();
	
	@SQL("select name, mail, password, identity from users where identity = \'s\' or identity = \'S\'")
	List<UserBean> getSuperUsers();
	
	@SQL("select name, mail, password, identity from users where identity = \'n\' or identity = \'N\'")
	List<UserBean> getNormalUsers();
	
	@SQL("select name, mail, password, identity from users where name like :1 or mail like :1")
	List<UserBean> getMatchUsers(String pattern);
	
	@SQL("insert into users (name, mail, password, identity) value (:1.name, :1.mail, :1.password, :1.identity)")
	int addRecord(UserBean user);
	
	@SQL("select name from users where name = :1")
	String isExist(String name);
	
	@SQL("select name, mail, identity from users where name=:1 and password=:2")
	UserBean isMatch(String name, String psw);
	
	@SQL("select count(*) from users")
	int recordsNum();
}
