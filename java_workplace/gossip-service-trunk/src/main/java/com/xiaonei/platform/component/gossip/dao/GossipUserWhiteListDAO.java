package com.xiaonei.platform.component.gossip.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.xiaonei.jade.datasource.Catalogs;

@DAO(catalog = Catalogs.GOSSIP_INFO_NEW)
public interface GossipUserWhiteListDAO {

    static final String sqlInsert = "insert into gossip_user_white_list(user_id) values (:owner)";

    @SQL(sqlInsert)
    public int save(@SQLParam("owner") int owner);

    static final String sqlUpdate = "update gossip_user_white_list set is_enable=:enable where user_id=:owner";

    @SQL(sqlUpdate)
    public int update(@SQLParam("owner") int owner, @SQLParam("enable") int enable);
    
    static final String sqlSelect = "select user_id from gossip_user_white_list where is_enable = 1";
    
    @SQL(sqlSelect)
    public List<Integer> findWhiteList();
}
