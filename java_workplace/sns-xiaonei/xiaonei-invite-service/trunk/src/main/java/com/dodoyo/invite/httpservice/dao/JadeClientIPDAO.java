package com.dodoyo.invite.httpservice.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.dodoyo.invite.httpservice.entity.ClientInfo;
import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.framework.exception.DataAccessException;

@DAO(catalog = Catalogs.STAT)
public interface JadeClientIPDAO
{
    @SQL("insert ignore into invite_service_ip_list(client_name,client_ip,client_key," +
    		"type,expire_time,create_time) values(:ci.clientName,:ci.clientIp,:ci.clentKey,:ci.expireTime,:ci.createTime,now())")
    public int addClientInfo(@SQLParam("ci") ClientInfo ci) 
    throws DataAccessException;
    
    
    @SQL("select * from invite_service_ip_list where client_ip=:ip")
    public ClientInfo getClientInfo(@SQLParam("ip") String clientIp) 
    throws DataAccessException;
    
    
    @SQL("select * from `invite_service_ip_list` limit 200")
    public List<ClientInfo> getClientInfoList() 
    throws DataAccessException;
    
    @SQL("update invite_service_ip_list set client_name=:ci.clientName, client_ip=:ci.clientIp, " +
    		"type=:ci.type, client_key=:ci.clientKey,expire_time=:ci.expireTime where id=:ci.id")
    public int updateClientInfo(@SQLParam("ci") ClientInfo ci) throws DataAccessException;
    
}