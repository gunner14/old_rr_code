package com.xiaonei.platform.component.gossip.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

import com.xiaonei.jade.datasource.Catalogs;
import com.xiaonei.platform.component.gossip.model.GossipCount;
/**
 * 留言count DAO<br>
 * 操作leaveword_count表
 * 
 * @author 吕恩乐 [enle.lv@opi-corp.com]
 */

@DAO(catalog=Catalogs.GOSSIP_INFO_NEW)
//@DAO
public interface GossipCountDAO {

	final static String sqlInsertGossipCount = "insert into leaveword_count (id, leaveword_count,chat_count) values (:id, 0, 0)";
	@SQL(sqlInsertGossipCount)
	public int save(@SQLParam("id")final int id);
	
	final static String sqlIncreaseGossipCount = "update leaveword_count set leaveword_count=leaveword_count+1 where id=:id";
	@SQL(sqlIncreaseGossipCount)
	public int increaseRecentCount(@SQLParam("id")final int id);
	
	final static String sqlDecreaseGossipCount = "update leaveword_count set leaveword_count=leaveword_count-1 where id=:id";
	@SQL(sqlDecreaseGossipCount)
	public int decreaseRecentCount(@SQLParam("id")final int id) ;
	

	final static String sqlUpdateGossipCount = "update leaveword_count set leaveword_count=:count where id=:id";
	@SQL(sqlUpdateGossipCount)
	public int updateRecentCount(@SQLParam("count")final int count,@SQLParam("id")final int id) ;

	final static String sqlGetGossipCount = "select leaveword_count,chat_count from leaveword_count where id = :id";
	@SQL(sqlGetGossipCount)
	public GossipCount getGossipCount(@SQLParam("id")final int hostId) ;

	
	final static String sqlIncreaseChatCount = "update leaveword_count set chat_count=chat_count+1 where id=:id";
	@SQL(sqlIncreaseChatCount)
	public int increaseChatCount(@SQLParam("id")final int id);


	final static String sqlDecreaseChatCount = "update leaveword_count set chat_count=chat_count-1 where id=:id";
	@SQL(sqlDecreaseChatCount)
	public int decreaseChatCount(@SQLParam("id")final int id) ;
	
	final static String sqlUpdateLevelCount = "update leaveword_count set leaveword_count=:count where id=:id";
	@SQL(sqlUpdateLevelCount)
	public int updateLevelCount(@SQLParam("id")final int id ,@SQLParam("count") final int count);
	
}