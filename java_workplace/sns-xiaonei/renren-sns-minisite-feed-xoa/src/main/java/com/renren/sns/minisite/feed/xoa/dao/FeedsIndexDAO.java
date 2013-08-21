package com.renren.sns.minisite.feed.xoa.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

@DAO(catalog = "minisite")
public interface FeedsIndexDAO {

    @SQL("select feed_index from feeds_index where user_id=:user_id")
    public Long getIndex(@SQLParam("user_id") int userid);

    @SQL("insert into feeds_index (user_id, feed_index) values(:user_id, :inc) on duplicate key update feed_index=:inc")
    public void incrIndex(@SQLParam("user_id") int userid, @SQLParam("inc") long inc);
    
    @SQL("update feeds_index set feed_index = :inc where user_id = :user_id")
    public int updateIndex(@SQLParam("user_id") int userid, @SQLParam("inc") long inc);
    
    @SQL("insert into feeds_index (user_id, feed_index) values(:user_id, :inc)")
    public void insertIndex(@SQLParam("user_id") int userid, @SQLParam("inc") long inc);

    @SQL("insert into feeds_index (user_id, feed_index) values(:user_id, :feed_index) on duplicate key update feed_index=:feed_index")
    public void initIndex(@SQLParam("user_id") int userid, @SQLParam("feed_index") long index);
}
