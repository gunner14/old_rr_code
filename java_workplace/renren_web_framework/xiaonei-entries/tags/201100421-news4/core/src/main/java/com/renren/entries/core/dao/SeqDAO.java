package com.renren.entries.core.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

/**
 * 
 * @author zhiliang.wang@opi-corp.com
 * 
 */
@DAO(catalog = "idseq")
public interface SeqDAO {

    /**
     * 返回DB同学定义的给entry_index_xxx表使用的序列，从0开始
     * 
     * @return
     */
    @SQL("select nextval('entry_index_id_seq')")
    public long nextIndexId();

    /**
     * 返回DB同学定义的，给各种对象(entry_info以及entry_body表)使用的作为其ID重要组成部分的序列，从0开始
     * 
     * @return
     */
    @SQL("select nextval('entry_id_seq')")
    public long nextEntryId();

}
