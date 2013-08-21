package com.dodoyo.invite.dao;


import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.dodoyo.invite.model.Email;
import com.xiaonei.platform.framework.exception.DataAccessException;

public interface JadeEmailDAO
{
    @SQL("insert into email (email_to, email_from, body, title, " +
    		"addtime, disc) values (:email.emailTo, :email.emailFrom, " +
    		":email.body, :email.title, :email.addtime, :email.disc)")
    public int save(@SQLParam("email") Email email) throws DataAccessException;
}
