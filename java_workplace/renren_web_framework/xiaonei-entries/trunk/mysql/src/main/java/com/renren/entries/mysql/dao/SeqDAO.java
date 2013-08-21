package com.renren.entries.mysql.dao;

/**
 * 注意：seqDAO不是一个Jade DAO，必须明确实现
 * 
 * @author zhiliang.wang@renren-inc.com
 * 
 */

public interface SeqDAO {

    /**
     * 
     * 返回一个
     * 
     * @return
     */
    public int nextSequnenceSuffix();

}
