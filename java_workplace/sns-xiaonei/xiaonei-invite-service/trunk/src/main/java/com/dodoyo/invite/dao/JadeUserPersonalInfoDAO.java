package com.dodoyo.invite.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.framework.exception.DataAccessException;

@DAO (catalog = Catalogs.COMMON)
public interface JadeUserPersonalInfoDAO
{
    @SQL("insert into user_personalinfo (id, invitecode, source) values (:id, :code, 0)")
    public int insertInviteCode(@SQLParam("id") int id, @SQLParam("code") String invitecode)
            throws DataAccessException;
    
    @SQL("update user_personalinfo set invitecode = :code where id = :id")
    public int updateInviteCode(@SQLParam("id") int id, @SQLParam("code") String invitecode)
            throws DataAccessException;
    
}
