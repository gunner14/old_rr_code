package com.dodoyo.invite.httpservice.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.dodoyo.invite.httpservice.entity.Token;
import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.framework.exception.DataAccessException;

@DAO(catalog = Catalogs.STAT)
public interface JadeTokenDAO
{
    @SQL("insert ignore into invite_service_token_history (token, client_ip, action, params, create_time) " +
    		"values(:token.token,:token.clientIp,:token.action,:token.params,now())")
    public int addToken(@SQLParam("token") Token token) throws DataAccessException;
    
    
    @SQL("select * from invite_service_token_history where token=:token")
    public Token getToken(@SQLParam("token") String token)
            throws DataAccessException;
    
}