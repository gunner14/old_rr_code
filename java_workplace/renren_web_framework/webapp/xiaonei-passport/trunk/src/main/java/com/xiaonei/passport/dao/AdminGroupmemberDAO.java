package com.xiaonei.passport.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import org.springframework.dao.DataAccessException;

import com.xiaonei.passport.model.AdminGroupmember;
import com.xiaonei.platform.core.opt.DataAccessMgr;

@DAO(catalog = DataAccessMgr.BIZ_COMMON)
public interface AdminGroupmemberDAO {
	String getAdminGroupListSql="select adminid,restrict_ip from admin_groupmember where groupid=25 and restrict_ip!=''";
	String addSql="insert into admin_groupmember (adminid,restrict_ip,groupid) values (?,?,26)";
	String delSql="delete from admin_groupmember where adminid=?";
	
	@SQL("select adminid,restrict_ip from admin_groupmember where groupid=25 and restrict_ip!=''")
	public List<AdminGroupmember> getAdminGroupList() throws DataAccessException;

	@SQL("insert into admin_groupmember (adminid,restrict_ip,groupid) values (:1,:2,26)")
	public void add(final int userid, final String ip) throws DataAccessException;

	@SQL("delete from admin_groupmember where adminid=:1")
	public void del(final int userid) throws DataAccessException;

}
