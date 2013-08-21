package com.renren.entries.core.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

import com.renren.entries.model.Tag;

/**
 * tag表数据访问
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-15 下午05:12:04
 */
@DAO(catalog = "tag")
public interface TagDAO {

    /**
     * 
     * @param value
     * @return 该tag的id
     */
    @SQL("INSERT INTO tag (namespace, value) VALUES (:1, :2)")
    public void save(String namespace, String value);

    @SQL("SELECT id, namespace, value FROM tag WHERE namespace = :1 AND value = :2")
    public Tag getByValue(String namespace, String value);

    @SQL("SELECT id, namespace, value FROM tag WHERE id = :1")
    public Tag getById(int id);

}
