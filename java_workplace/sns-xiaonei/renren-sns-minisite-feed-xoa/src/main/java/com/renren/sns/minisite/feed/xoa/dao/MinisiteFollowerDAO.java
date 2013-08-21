package com.renren.sns.minisite.feed.xoa.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

@DAO(catalog = "minisite")
public interface MinisiteFollowerDAO {

    @SQL("select follower_id from minisite_follower where site_id=:site_id")
    public List<Integer> getFollowerIdsInSite(@SQLParam("site_id") int siteId);

    @SQL("select site_id from minisite_follower where follower_id=:follower_id")
    public List<Integer> getSiteIdsFromFollower(@SQLParam("follower_id") int userid);

    @SQL("select owner_id FROM minisite_owner WHERE site_id=:site_id")
    public List<Integer> getSiteOwnerIdsBySiteId(@SQLParam("site_id") int siteId);
}
