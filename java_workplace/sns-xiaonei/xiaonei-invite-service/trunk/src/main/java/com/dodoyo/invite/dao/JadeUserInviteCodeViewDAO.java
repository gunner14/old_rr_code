package com.dodoyo.invite.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.dodoyo.invite.model.UserInviteCodeView;
import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.framework.exception.DataAccessException;

@DAO (catalog = Catalogs.COMMON)
public interface JadeUserInviteCodeViewDAO
{
    
    @SQL("select id, invitecode from user_personalinfo where id = :id")
    public UserInviteCodeView getByUser(@SQLParam("id") int usrId) 
    throws DataAccessException;

    
    @SQL("select id, invitecode from user_personalinfo where id = :id and invitecode = :code")
    public UserInviteCodeView getByUserNCode(@SQLParam("id") int usrId, @SQLParam("code") String code)
    throws DataAccessException;
    
    
    @SQL("select id, invitecode from user_personalinfo where invitecode = :code")
    public UserInviteCodeView getByCode(@SQLParam("code") String code) 
    throws DataAccessException;
}
