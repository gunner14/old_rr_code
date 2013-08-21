package com.renren.sns.wiki.dao;

import com.xiaonei.platform.core.opt.DataAccessMgr;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

/**
 * wiki 序列的服务
 * 
 * @author yi.li@renren-inc.com since 2012-3-26
 * 
 */
@DAO(catalog = DataAccessMgr.BIZ_IDSEQUENCE)
public interface SequenceDAO {

    /**
     * 获取wiki下一个可用的序列
     */
    @SQL("select nextval('social_wiki_id_seq')")
    public int getWikiNextId();
}
