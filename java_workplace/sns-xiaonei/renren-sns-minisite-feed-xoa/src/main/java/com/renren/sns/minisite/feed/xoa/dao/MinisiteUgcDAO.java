package com.renren.sns.minisite.feed.xoa.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;
import net.paoding.rose.jade.annotation.SQLParam;

@DAO(catalog = "minisite")
public interface MinisiteUgcDAO {

    @SQL("select ugc_id from minisite_ugc where site_id in ( :site_ids ) order by time desc limit :offset,:limit")
    public List<Long> getFeedUgcIds(@SQLParam("site_ids") List<Integer> siteIds,
            @SQLParam("offset") int offset, @SQLParam("limit") int limit);

}
